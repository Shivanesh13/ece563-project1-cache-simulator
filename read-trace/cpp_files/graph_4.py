import subprocess
import os

# Configuration
l1_assoc = 4
l2_size = 0  # No L2 cache
l2_assoc = 0
replacement = 0
inclusion = 0
trace_file = "../../traces/gcc_trace.txt"
sim_command = "./sim"
results_dir = "simulation_data_graph4"

# Create results directory if it doesn't exist
if not os.path.exists(results_dir):
    os.makedirs(results_dir)

# Sizes: 1KB to 32KB in powers of two
sizes = [2**i for i in range(10, 16)]  # 1024 to 32768

# Block sizes: 16 to 128 in powers of two
block_sizes = [2**i for i in range(4, 8)]  # 16, 32, 64, 128

# Function to extract L1 miss rate from output
def extract_l1_miss_rate(output):
    for line in output.splitlines():
        if line.strip().startswith("e. L1 miss rate:"):
            parts = line.split()
            return float(parts[-1])
    raise ValueError("L1 miss rate not found in output")

# Run simulations and store results
for size in sizes:
    for block_size in block_sizes:
        output_file = os.path.join(results_dir, f"size_{size}_blocksize_{block_size}.txt")
        
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
            # Use Popen with PIPE for Python 3.6 compatibility
            process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
            stdout, stderr = process.communicate()
            if process.returncode != 0:
                raise subprocess.CalledProcessError(process.returncode, cmd, stdout, stderr)
            
            l1_miss_rate = extract_l1_miss_rate(stdout)
            with open(output_file, 'w') as f:
                f.write(f"Size: {size}B, Block Size: {block_size}B, L1 Miss Rate: {l1_miss_rate}\n")
            print(f"Results saved to {output_file}")
        except subprocess.CalledProcessError as e:
            print(f"Error running sim for size {size}, block size {block_size}: {e.stderr}")
        except ValueError as e:
            print(f"Error parsing output for size {size}, block size {block_size}: {e}")