import vitis
import os
import sys

# ======= USER CONFIG ==========================================
WORKSPACE_DIR = "/home/t1_2/lazy-sherlock/Project_kernels_HLS/_x.xilinx_u55c_gen3x16_xdma_3_202210_1"
DEFAULT_COMPONENT_NAME = "krnl_proj"   # change if needed
# ===============================================================

OP_MAP = {
    "csim":   ["C_SIMULATION"],
    "sim":    ["C_SIMULATION"],
    "c-sim":  ["C_SIMULATION"],

    "synth":  ["SYNTHESIS"],
    "csynth": ["SYNTHESIS"],

    "cosim":  ["CO_SIMULATION"],
    "co":     ["CO_SIMULATION"],

    "all":    ["C_SIMULATION", "SYNTHESIS", "CO_SIMULATION"],
}

def main():
    # Optional: component name from command line
    if len(sys.argv) >= 2:
        component_name = sys.argv[1]
    else:
        component_name = DEFAULT_COMPONENT_NAME

    print(f"[INFO] Workspace : {WORKSPACE_DIR}")
    print(f"[INFO] Component : {component_name}")

    client = vitis.create_client()
    client.set_workspace(path=WORKSPACE_DIR)
    comp = client.get_component(name=component_name)

    print("[INFO] Connected. Interactive HLS shell.")
    print("       Commands: csim, synth, cosim, all, help, quit/exit")
    print("")

    while True:
        try:
            cmd = input("hls> ").strip().lower()
        except EOFError:
            # Ctrl+D
            cmd = "exit"

        if cmd in ("quit", "exit", "q"):
            print("[INFO] Exiting.")
            break

        if cmd in ("help", "h", "?"):
            print("Commands:")
            print("  csim      - C simulation")
            print("  synth     - C synthesis")
            print("  cosim     - C/RTL co-simulation")
            print("  all       - csim + synth + cosim")
            print("  help      - show this message")
            print("  quit/exit - leave this shell")
            continue

        if cmd not in OP_MAP:
            print(f"[WARN] Unknown command: {cmd}")
            print("       Use: csim, synth, cosim, all, help, exit")
            continue

        steps = OP_MAP[cmd]
        for step in steps:
            print(f"\n[INFO] Running {step} for {component_name} ...")
            try:
                # If execute() ever fails in your version, switch to:
                # comp.run(operation=step)
                comp.execute(step)
                print(f"[INFO] Done: {step}")
            except Exception as e:
                print(f"[ERROR] Failed to run {step}: {e}")
                break

    client.close()
    print("[INFO] Client closed.")

if __name__ == "__main__":
    main()

