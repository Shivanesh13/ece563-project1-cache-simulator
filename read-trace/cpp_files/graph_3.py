import subprocess
import os

# Configuration
block_size = 32
l2_size = 16384  # 16KB L2 cache
l2_assoc = 8     # 8-way set-associative L2
replacement = 0
inclusion = 0    # Adjust if needed to enable L2
trace_file = "../../traces/gcc_trace.txt"
sim_command = "./sim"
results_dir = "simulation_data_graph3"

# Create results directory if it doesn't exist
if not os.path.exists(results_dir):
    os.makedirs(results_dir)

# Sizes: 1KB to 8KB in powers of two
sizes = [2**i for i in range(10, 14)]  # 1024 to 8192

# Associativities: 1,2,4,8
assocs = [1, 2, 4, 8]

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
for size in sizes:
    for assoc in assocs:
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
            # Use Popen with PIPE for Python 3.6 compatibility
            process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
            stdout, stderr = process.communicate()
            if process.returncode != 0:
                raise subprocess.CalledProcessError(process.returncode, cmd, stdout, stderr)
            
            l1_miss_rate = extract_l1_miss_rate(stdout)
            l2_miss_rate = extract_l2_miss_rate(stdout)
            
            with open(output_file, 'w') as f:
                f.write(f"Size: {size}B, Assoc: {assoc}, L1 Miss Rate: {l1_miss_rate}")
                if l2_miss_rate is not None:
                    f.write(f", L2 Miss Rate: {l2_miss_rate}\n")
                else:
                    f.write(f", L2 Miss Rate: Not Available (L2 data not found in output)\n")
            print(f"Results saved to {output_file}")
        except subprocess.CalledProcessError as e:
            print(f"Error running sim for size {size}, assoc {assoc}: {e.stderr}")
        except ValueError as e:
            print(f"Error parsing output for size {size}, assoc {assoc}: {e}")