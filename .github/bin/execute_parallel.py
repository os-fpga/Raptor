import argparse
import concurrent.futures
import subprocess
import ray

# Define function to run a command
def run_command(command):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)
    for line in process.stdout.readlines():
        print(line.decode("utf-8").strip())
    return process.returncode

# Function to dynamically adjust process pool size based on available resources
def get_optimal_pool_size():
    # Use ray library to get resource availability
    cpu_resources = ray.cluster_resources().get("CPU", 0)
    memory_resources = ray.cluster_resources().get("Memory", 0)

    # Calculate available CPUs and memory
    available_cpus = max(1, int(cpu_resources))
    available_memory = int(memory_resources / 1024**3)  # Convert memory to GB

    # Define a minimum and maximum pool size
    min_pool_size = 1
    max_pool_size = available_cpus

    # Adjust pool size based on available memory
    if available_memory < 16:  # 16GB minimum for comfortable execution
        max_pool_size = min(max_pool_size, available_memory // 2)

    # Choose optimal pool size within limits
    return min(max(min_pool_size, available_cpus // 2), max_pool_size)

def main():
    parser = argparse.ArgumentParser(description="Run commands in parallel")
    parser.add_argument("--filename", help="The file containing the commands")
    args = parser.parse_args()

    with open(args.filename) as f:
        commands = f.readlines()

    ray.init()  # Initialize Ray

    # Get optimal pool size based on resources
    num_processes = get_optimal_pool_size()

    # Create a Ray ProcessPoolExecutor
    executor = concurrent.futures.ProcessPoolExecutor(max_workers=num_processes)

    # Submit commands to the pool and capture their results
    results = {executor.submit(run_command, command): command for command in commands}

    # Wait for all processes to finish and print results
    for future, command in results.items():
        returncode = future.result()
        if returncode != 0:
            print(f"Command '{command}' failed with return code {returncode}")

    ray.shutdown()
    print("All simulations completed!")

if __name__ == "__main__":
    main()
