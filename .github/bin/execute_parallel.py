import argparse
import concurrent.futures
import subprocess
import ray

# Define function to run a command
def run_command(command):
    try:
        process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        returncode = process.returncode

        # Print stdout and stderr for better logging
        print(f"Command: {command}")
        print(f"Return Code: {returncode}")
        print(f"stdout:\n{stdout.decode('utf-8')}")
        print(f"stderr:\n{stderr.decode('utf-8')}")

        return returncode
    except Exception as e:
        print(f"Error running command: {e}")
        return 1  # Return a non-zero code for failure

# Function to dynamically adjust process pool size based on available resources
def get_optimal_pool_size():
    # Use ray library to get resource availability
    cpu_resources = ray.cluster_resources().get("CPU", 0)
    memory_resources = ray.cluster_resources().get("memory", 0)

    print(f"CPU Resources: {cpu_resources}")
    print(f"Memory Resources: {memory_resources}")

    # Calculate available CPUs and memory
    available_cpus = max(1, int(cpu_resources))
    available_memory = int(memory_resources / 1024**3)  # Convert memory to GB

    print(f"Available CPUs: {available_cpus}")
    print(f"Available Memory (GB): {available_memory}")

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

    ray.init()
    print(ray.cluster_resources())

    num_processes = get_optimal_pool_size()

    executor = concurrent.futures.ProcessPoolExecutor(max_workers=num_processes)

    results = {executor.submit(run_command, command.strip()): command for command in commands}

    for future, command in results.items():
        returncode = future.result()
        if returncode != 0:
            print(f"Command '{command}' failed with return code {returncode}")

    ray.shutdown()
    print("All simulations completed!")

if __name__ == "__main__":
    main()

