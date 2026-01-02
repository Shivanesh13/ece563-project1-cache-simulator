import subprocess
import os

# Configuration
l1_size = 1024  # 1KB
l1_block_size = 16
l1_assoc = 1
l2_size = 0  # No L2 cache
l2_assoc = 0
trace_file = "streams_trace.txt"  # Local directory
sim_command = "./sim"
results_dir = "simulation_data_table1"

# Create results directory if it doesn't exist
if not os.path.exists(results_dir):
    os.makedirs(results_dir)

# Prefetching configurations: PREF_N values (replacing replacement)
pref_n_values = [0, 1, 2, 3, 4]

# Function to extract L1 miss rate from output
def extract_l1_miss_rate(output):
    for line in output.splitlines():
        if line.strip().startswith("e. L1 miss rate:"):
            parts = line.split()
            return float(parts[-1])
    raise ValueError("L1 miss rate not found in output")

# Run simulations and store results
for pref_n in pref_n_values:
    output_file = os.path.join(results_dir, f"pref_n_{pref_n}.txt")
    
    # Set pref_m to 0 if pref_n is 0, otherwise 4
    pref_m = 0 if pref_n == 0 else 4
    
    # cmd list with pref_n and pref_m in place of replacement and inclusion
    cmd = [
        sim_command,
        str(l1_block_size),  # Block size
        str(l1_size),        # Cache size
        str(l1_assoc),       # Associativity
        str(l2_size),        # L2 size
        str(l2_assoc),       # L2 associativity
        str(pref_n),         # PREF_N (replaces replacement)
        str(pref_m),         # PREF_M (replaces inclusion)
        trace_file           # Trace file
    ]
    
    try:
        # Use Popen with PIPE for Python 3.6 compatibility
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
        stdout, stderr = process.communicate()
        if process.returncode != 0:
            raise subprocess.CalledProcessError(process.returncode, cmd, stdout, stderr)
        
        l1_miss_rate = extract_l1_miss_rate(stdout)
        with open(output_file, 'w') as f:
            f.write(f"PREF_N: {pref_n}, PREF_M: {pref_m}, L1 Miss Rate: {l1_miss_rate}\n")
        print(f"Results saved to {output_file}")
    except subprocess.CalledProcessError as e:
        print(f"Error running sim for PREF_N={pref_n}: {e.stderr}")
    except ValueError as e:
        print(f"Error parsing output for PREF_N={pref_n}: {e}")