import subprocess
import os

# Configuration
l1_block_size = 32
l2_block_size = 32
replacement = 0
inclusion = 0
trace_file = "../../traces/gcc_trace.txt"
sim_command = "./sim"
results_dir = "simulation_data_graph5"

# Create results directory if it doesn't exist
if not os.path.exists(results_dir):
    os.makedirs(results_dir)

# L1 Sizes: 1KB to 8KB in powers of two
l1_sizes = [2**i for i in range(10, 14)]  # 1024 to 8192

# L2 Sizes: 16KB to 64KB in powers of two
l2_sizes = [2**i for i in range(14, 17)]  # 16384 to 65536

# Function to extract L1 miss rate from output
def extract_l1_miss_rate(output):
    for line in output.splitlines():
        if line.strip().startswith("e. L1 miss rate:"):
            parts = line.split()
            return float(parts[-1])
    raise ValueError("L1 miss rate not found in output")

# Function to extract L2 miss rate from output (returns None if not found)
def extract_l2_miss_rate(output):
    for line in output.splitlines():
        if line.strip().startswith("n. L2 miss rate:"):
            parts = line.split()
            return float(parts[-1])
    return None  # Return None if L2 miss rate is not found

# Run simulations and store results
for l1_size in l1_sizes:
    for l2_size in l2_sizes:
        l1_assoc = 4
        l2_assoc = 8
        output_file = os.path.join(results_dir, f"l1_size_{l1_size}_l2_size_{l2_size}.txt")
        
        cmd = [
            sim_command,
            str(l1_block_size),
            str(l1_size),
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
            l2_miss_rate = extract_l2_miss_rate(stdout)
            
            with open(output_file, 'w') as f:
                f.write(f"L1 Size: {l1_size}B, L2 Size: {l2_size}B, L1 Miss Rate: {l1_miss_rate}")
                if l2_miss_rate is not None:
                    f.write(f", L2 Miss Rate: {l2_miss_rate}\n")
                else:
                    f.write(f", L2 Miss Rate: Not Available\n")
            print(f"Results saved to {output_file}")
        except subprocess.CalledProcessError as e:
            print(f"Error running sim for L1 size {l1_size}, L2 size {l2_size}: {e.stderr}")
        except ValueError as e:
            print(f"Error parsing output for L1 size {l1_size}, L2 size {l2_size}: {e}")