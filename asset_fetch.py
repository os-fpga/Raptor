import os
import json
import shutil
import subprocess

# get the repo root
root_dir = os.path.dirname(os.path.abspath(__file__))
# set the Json file path
home_dir = os.path.expanduser("~")
json_file_path = os.path.join(home_dir, '.local', 'raptor-dev.json')


def validate_json_keys (json_data, nested_keys):
    """
    Validate given keys are present in the JSON data.

    Args:
    - json_data (dict): The loaded JSON data.
    - keys (list): List of tuples indicating key, e.g., ('GITHUB', 'TOKEN').

    Returns:
    - bool: True if all nested keys are present, False otherwise.
    """
    for keys in nested_keys:
        temp_data = json_data
        for key in keys:
            if key not in temp_data:
                return False
            temp_data = temp_data[key]
    return True

def run_command(cmd, cwd=None):
    """
    Runs a shell command and returns result if it succeeds, exit otherwise.

    Args:
        cmd (list or str): The command to run.
        cwd (str, optional): The directory to run the command in. Defaults to None.
    """
    try:
        result = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error during command\n{cmd}\n {e.stderr}")
        print("Please contact maintainer.")
        os._exit(0)
    return result

def move_item(src_path, dest_path):
    """Move a file or directory from src_path to dest_path."""
    #print(f'source\n\t{src_path}\ndestination\n\t{dest_path}')
    try:
        if os.path.isdir(src_path):
            if os.path.exists(dest_path):
                shutil.rmtree(dest_path)
            shutil.copytree(src_path, dest_path)  # Copy the directory
        else:
            if os.path.exists(dest_path):
                os.remove(dest_path)
            shutil.copy(src_path, dest_path)  # Copy the file

        print(f"Moved {src_path} to {dest_path}")

    except FileNotFoundError:
        print(f"Error: '{src_path}' was not found.")

    except Exception as e:
        print(f"Failed to move {src_path}: {e}")

def bringing_commercial_devices(repo):
    """
    Download and populate the commercial devices

    Need From Json
    - src repo
    - destination repo
    - src path
    - destination path
    """
    required_keys = [
    ('GITHUB', 'TOKEN'), 
    ('REPO', 'REPO_SRC'), 
    ('REPO', 'REPO_DEST'),
    ('REPO', 'PATH_SRC'),
    ('REPO', 'PATH_DEST')
    ]

    try:
        with open(json_file_path, 'r') as f:
            data = json.load(f)
            
        if not validate_json_keys(data,required_keys):
            print('Missing required key in json')
            os._exit(0)
        
        token = data.get("GITHUB", {}).get("TOKEN", None)
        repo_src = data['REPO']['REPO_SRC']
        repo_dest = data['REPO']['REPO_DEST']
        path_src = data['REPO']['PATH_SRC']
        path_dest = data['REPO']['PATH_DEST']
        iterations = len(path_src)
        for i in range(iterations):
            src_repo = repo_src[i]
            dest_repo = repo_dest[i]
            paths_src = path_src[i]
            paths_dest = path_dest[i]

            if dest_repo == repo:
                print(f"Processing Repo Source: {src_repo}")
                print(f"Processing Repo Destination: {dest_repo}")
                print(f"Paths Source: {paths_src}")
                print(f"Paths Destination: {paths_dest}")

                git_clone_cmd = ['git', 'clone', '--filter=blob:none', '--no-checkout',
                f'https://{token}@github.com/{src_repo}.git', 'fetch_temp'
                ]
                run_command(git_clone_cmd,root_dir)
                # Iterate over the PATH_SRC and PATH_DEST arrays
                for j, src_path in enumerate(paths_src):
                    dest_path = paths_dest[j] if j < len(paths_dest) else ''
                    if len(dest_path) < 1:
                      dest_path = src_path
                    if not os.path.exists(os.path.join(root_dir,'fetch_temp',src_path)):
                        sparse_checkout_cmd = ['git', 'sparse-checkout', 'set', src_path]
                        run_command(sparse_checkout_cmd, cwd=os.path.join(root_dir, 'fetch_temp'))
                        sparse_bringin_cmd = ['git', 'checkout', '@']
                        run_command(sparse_bringin_cmd, cwd=os.path.join(root_dir, 'fetch_temp'))
                    files_to_move = os.path.join(root_dir, 'fetch_temp', src_path)
                    dest_item = os.path.join(root_dir, dest_path)
                    if os.path.exists(files_to_move):
                        if os.path.isdir(files_to_move):
                            for file_name in os.listdir(files_to_move):
                                src_file_path = os.path.join(files_to_move, file_name)
                                dest_file_path = os.path.join(dest_item, file_name)
                                move_item(src_file_path, dest_file_path)
                        else:
                            move_item(files_to_move, dest_item)
                    else:
                        print("No files found in asset fetch folder")   
    except json.JSONDecodeError:
        print(f"Error: Failed to decode JSON from file {json_file_path}")
        os._exit(0)
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
        os._exit(0)

def should_fetch():
    """
    Check
    - Type of Repo (Raptor, RPE, ..)
    - Make sure assests/work is not done before
    - Confirm the presence of Json file

    Returns:
    - repo type: destination repo name in which assets will be fetched
    """

    if not os.path.exists(json_file_path):
        print(f"No such file as\n\t{json_file_path}")
        os._exit(0)
    if os.path.exists(os.path.join(root_dir, 'fetch_temp')):
        print('Assests are already present so removing')
        shutil.rmtree(os.path.join(root_dir, 'fetch_temp'))

    detect_repo_cmd = ['git', 'remote', 'get-url', '--push', 'origin']
    repo_type = run_command(detect_repo_cmd, root_dir)
    
    if 'Raptor' in repo_type.stdout:
        return 'os-fpga/Raptor'
    elif 'rapid_power_estimator' in repo_type.stdout:
        return 'os-fpga/rapid_power_estimator'
    else:
        print("Not recognisable repo, if it is a mistake, disable the call of function 'should_fetch' and contact maintainer")
        os._exit(0)
    
if __name__ == "__main__":
    repo = should_fetch()
    bringing_commercial_devices(repo)



