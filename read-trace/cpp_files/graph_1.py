import subprocess
import os

# Configuration
block_size = 32
l2_size = 0
l2_assoc = 0
replacement = 0
inclusion = 0
trace_file = "../../traces/gcc_trace.txt"
sim_command = "./sim"
results_dir = "simulation_data"

# Create results directory if it doesn't exist
if not os.path.exists(results_dir):
    os.makedirs(results_dir)

# Sizes: 1KB to 1MB in powers of two
sizes = [2**i for i in range(10, 21)]  # 1024 to 1048576

# Associativities: 1,2,4,8,full
assocs = [1, 2, 4, 8, 'full']

# Function to extract L1 miss rate from output
def extract_miss_rate(output):
    for line in output.splitlines():
        if line.strip().startswith("e. L1 miss rate:"):
            parts = line.split()
            return float(parts[-1])
    raise ValueError("L1 miss rate not found in output")

# Run simulations and store results
for size in sizes:
    for assoc in assocs:
        if assoc == 'full':
            l1_assoc = size // block_size
        else:
            l1_assoc = assoc
        
        output_file = os.path.join(results_dir, f"size_{size}_assoc_{assoc}.txt")
        
        cmd = [
            sim_command,
            str(block_size),
            str(size),
            str(l1_assoc),
            str(l2_size),
            str(l2_assoc),
            str(replacement),
            str(inclusion),
            trace_file
        ]
        
        try:
            # Use stdout=subprocess.PIPE instead of capture_output
            process = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True, universal_newlines=True)
            miss_rate = extract_miss_rate(process.stdout)
            with open(output_file, 'w') as f:
                f.write(f"Size: {size}B, Assoc: {assoc}, Miss Rate: {miss_rate}\n")
            print(f"Results saved to {output_file}")
        except subprocess.CalledProcessError as e:
            print(f"Error running sim for size {size}, assoc {assoc}: {e.stderr.decode()}")
        except ValueError as e:
            print(f"Error parsing output for size {size}, assoc {assoc}: {e}")

