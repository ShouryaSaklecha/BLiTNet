#!/usr/bin/env python3
"""Vitis HLS 2023.2 steps with compact output, modelled on the ~/fpga engine.

Raw logs and reports stay on disk under build/hls; only a short digest is printed.
Identical inputs (chip sources, testbench, config, library and tool version) return
the cached digest without rerunning the tool.

  hls.py run  csim|synth|cosim|export [--force]   run a step, print its digest
  hls.py log  csim|synth|cosim|export             tool messages, grouped by id
  hls.py rpt  synth|cosim [--grep PATTERN]        slice of one raw report, capped
"""
import argparse
import hashlib
import os
import re
import subprocess
import sys
import time
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CFG = ROOT / "hls" / "hls_config.cfg"
WORK = ROOT / "build" / "hls"
COMP = WORK / "blitnet"
LOGS = WORK / "logs"
CACHE = WORK / "cache"
VITIS = Path(os.environ.get("XILINX_VITIS", "/tools/Xilinx/Vitis/2023.2"))
CMD = {
    "csim":   ["vitis-run", "--mode", "hls", "--csim"],
    "synth":  ["v++", "-c", "--mode", "hls"],
    "cosim":  ["vitis-run", "--mode", "hls", "--cosim"],
    "export": ["vitis-run", "--mode", "hls", "--package"],
}
MSG = re.compile(r"^(ERROR|CRITICAL WARNING|WARNING): \[([A-Za-z]+ [\d-]+)\]\s*(.*)")
SEVERITY = ["ERROR", "CRITICAL WARNING", "WARNING"]
REPORT_CAP = 60


def cfg_value(key):
    m = re.search(rf"^\s*{re.escape(key)}\s*=\s*(\S+)", CFG.read_text(), re.M)
    return m.group(1) if m else None


def inputs_hash():
    files = sorted(p for p in (ROOT / "src" / "chip").rglob("*") if p.is_file())
    files += sorted((ROOT / "tests").glob("*.hpp")) + [CFG]
    files += [(CFG.parent / f).resolve() for f in re.findall(r"^\s*tb\.file\s*=\s*(\S+)", CFG.read_text(), re.M)]
    files.append(ROOT / "vendor" / "Vitis_Libraries" / ".git" / "HEAD")
    h = hashlib.sha256(str(VITIS).encode())
    for f in files:
        if f.is_file():
            h.update(str(f).encode())
            h.update(f.read_bytes())
    return h.hexdigest()[:12]


def grouped_messages(text, cap):
    groups = {}
    for line in text.splitlines():
        m = MSG.match(line)
        if m:
            g = groups.setdefault((m.group(1), m.group(2)), [0, m.group(3)])
            g[0] += 1
    rows = sorted(groups.items(), key=lambda kv: SEVERITY.index(kv[0][0]))
    label = {"ERROR": "error", "CRITICAL WARNING": "crit ", "WARNING": "warn "}
    out = [f"{label[sev]} x{n} [{mid}] {ex[:110]}" for (sev, mid), (n, ex) in rows[:cap]]
    if len(rows) > cap:
        out.append(f"... {len(rows) - cap} more message ids")
    return out


def pct(used, avail):
    return f"{used} ({100 * used / avail:.1f}%)" if avail else str(used)


def synth_digest():
    top = cfg_value("syn.top")
    rep = COMP / "hls" / "syn" / "report"
    r = ET.parse(rep / "csynth.xml").getroot()
    tgt = float(r.findtext(".//TargetClockPeriod"))
    est = float(r.findtext(".//EstimatedClockPeriod"))
    best, worst = r.findtext(".//Best-caseLatency"), r.findtext(".//Worst-caseLatency")
    imin, imax = r.findtext(".//Interval-min"), r.findtext(".//Interval-max")
    lines = [f"clock   {tgt:.2f} ns target, {est:.3f} est, slack {tgt - est:+.3f} ns",
             f"latency {best}" + (f"-{worst}" if worst != best else "") + f" cycles, interval {imin}"
             + (f"-{imax}" if imax != imin else "")]
    advice = []
    if est > tgt:
        advice.append(f"timing: estimate exceeds target by {est - tgt:.3f} ns")
    tr = ET.parse(rep / f"{top}_csynth.xml").getroot()
    loops = tr.find(".//SummaryOfLoopLatency")
    for loop in (list(loops) if loops is not None else [])[:8]:
        ii = loop.findtext("PipelineII")
        lines.append(f"loop    {loop.tag}: II {ii or '-'}, depth {loop.findtext('PipelineDepth') or '-'}, "
                     f"trip {loop.findtext('TripCount')}")
        if ii and ii.isdigit() and int(ii) > 1:
            advice.append(f"{loop.tag} pipelined at II={ii}: look for a memory-port or loop-carried dependency")
    used, avail = r.find(".//AreaEstimates/Resources"), r.find(".//AvailableResources")
    area = []
    for k in ["LUT", "FF", "DSP", "BRAM_18K", "URAM"]:
        u, a = int(used.findtext(k) or 0), int(avail.findtext(k) or 0)
        area.append(f"{k} {pct(u, a)}")
        if a and u / a > 0.8:
            advice.append(f"{k} at {100 * u / a:.0f}%: placement risk")
    lines.append("area    " + "  ".join(area))
    return lines + [f"advice  {a}" for a in advice]


def cosim_digest(text):
    rpt = COMP / "reports" / "hls_cosim.rpt"
    lines = []
    for line in rpt.read_text().splitlines() if rpt.is_file() else []:
        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        if cells and cells[0] in ("Verilog", "VHDL") and cells[1] != "NA":
            lines.append(f"{cells[0].lower():7} {cells[1]}, latency min/avg/max {cells[2]}/{cells[3]}/{cells[4]}")
    m = re.search(r"co-simulation finished: (\w+)", text)
    return [f"result  {m.group(1) if m else 'unknown'}"] + lines


def csim_digest(text):
    m = re.search(r"CSim done with (\d+) errors", text)
    fails = {}
    for line in text.splitlines():
        f = re.match(r"^(\S+:\d+): (CHECK\w* failed.*)", line)
        if f:
            loc = re.sub(r"^(\.\./)+", "", f.group(1))
            fails.setdefault(loc, [0, f.group(2)])[0] += 1
    out = [f"result  {m.group(1) + ' errors' if m else 'did not finish'}"]
    return out + [f"fail x{n} {loc} {msg[:100]}" for loc, (n, msg) in list(fails.items())[:5]]


def export_digest():
    zips = sorted((COMP / "hls" / "impl" / "ip").glob("*.zip"))
    return [f"ip      {z.relative_to(ROOT)} ({z.stat().st_size // 1024} KB)" for z in zips] or ["ip      none written"]


def run_step(step, force):
    key = inputs_hash()
    cached = CACHE / key / f"{step}.txt"
    if cached.is_file() and not force:
        print(f"[cached] {cached.read_text()}", end="")
        return 0
    if step in ("cosim", "export"):
        marker = COMP / ".synth_hash"
        if not marker.is_file() or marker.read_text().strip() != key:
            print("synth needed first:")
            if run_step("synth", True):
                return 1
    LOGS.mkdir(parents=True, exist_ok=True)
    log = LOGS / f"{step}.log"
    cmd = [str(VITIS / "bin" / CMD[step][0])] + CMD[step][1:] + \
          ["--config", os.path.relpath(CFG, WORK), "--work_dir", COMP.name]
    t0 = time.time()
    with open(log, "w") as fh:
        rc = subprocess.call(cmd, cwd=WORK, stdout=fh, stderr=subprocess.STDOUT)
    text = log.read_text(errors="replace")
    body = []
    try:
        body = {"csim": lambda: csim_digest(text), "synth": synth_digest,
                "cosim": lambda: cosim_digest(text), "export": export_digest}[step]()
    except (OSError, ET.ParseError, AttributeError) as e:
        body = [f"no report to digest ({type(e).__name__})"]
    if step == "synth" and rc == 0:
        (COMP / ".synth_hash").write_text(key)
    ok = rc == 0 and not any(l.startswith(("result  FAIL", "result  did not", "fail ", "no report")) for l in body)
    if step == "csim":
        ok = ok and "result  0 errors" in body
    msgs = grouped_messages(text, 6)
    digest = [f"{step} {'OK' if ok else 'FAILED'} {time.time() - t0:.0f}s [{key}]"] + body + msgs
    digest.append(f"log     {log.relative_to(ROOT)}")
    text_out = "\n".join(digest) + "\n"
    if ok:
        cached.parent.mkdir(parents=True, exist_ok=True)
        cached.write_text(text_out)
    print(text_out, end="")
    return 0 if ok else 1


def show_log(step):
    log = LOGS / f"{step}.log"
    if not log.is_file():
        sys.exit(f"no {step} log yet; run: make {step}")
    print("\n".join(grouped_messages(log.read_text(errors="replace"), 25)) or "no warnings or errors")


def show_rpt(step, grep):
    path = {"synth": COMP / "hls" / "syn" / "report" / f"{cfg_value('syn.top')}_csynth.rpt",
            "cosim": COMP / "reports" / "hls_cosim.rpt"}[step]
    lines = [l for l in path.read_text().splitlines()
             if l.strip() and "Copyright" not in l and not l.startswith("=")]
    if grep:
        lines = [l for l in lines if re.search(grep, l)]
    print("\n".join(lines[:REPORT_CAP]))
    if len(lines) > REPORT_CAP:
        print(f"... {len(lines) - REPORT_CAP} more lines; narrow with --grep")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("action", choices=["run", "log", "rpt"])
    ap.add_argument("step", choices=list(CMD))
    ap.add_argument("--force", action="store_true")
    ap.add_argument("--grep")
    a = ap.parse_args()
    WORK.mkdir(parents=True, exist_ok=True)
    if a.action == "run":
        sys.exit(run_step(a.step, a.force))
    show_log(a.step) if a.action == "log" else show_rpt(a.step, a.grep)


if __name__ == "__main__":
    main()
