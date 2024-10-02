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
    Runs a shell command and returns True if it succeeds, False otherwise.

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
        repo_src = data.get("REPO", {}).get("REPO_SRC", None)
        repo_dest = data.get("REPO", {}).get("REPO_DEST", None)
        path_src = data.get("REPO", {}).get("PATH_SRC", None)
        path_dest = data.get("REPO", {}).get("PATH_DEST", None)

        if repo == 'Raptor':
            repo_src = repo_src [0]
            repo_dest = repo_dest[0]
            path_src = path_src[0]
            path_dest = path_dest[0]
        if repo == 'rapid_power_estimator':
            repo_src = repo_src [1]
            repo_dest = repo_dest[1]
            path_src = path_src[1]
            path_dest = path_dest[1]
        #print(f'the toke is\n\t{token}\nand repos src is\n\t{repo_src}, {repo_dest}, {path_src}, {path_dest}')
        for srcpath in path_src:
            print(f'Currently doing for {srcpath}')  
            # Define the devices directory path
            devices_dir = os.path.join(root_dir, srcpath)
            # Check if the devices directory exists and rename it
            if os.path.exists(devices_dir):
                new_devices_dir = os.path.join(root_dir, srcpath +'_osfpga')
                shutil.move(devices_dir, new_devices_dir)
                print(f"Renamed previous devices directory to {new_devices_dir}")
            else:
                raise FileNotFoundError(f"Devices directory not found or failed to renamed: {devices_dir}")
            # Fetch assets from the repository
            print(f"Fetching Assets from {repo_src}")

            git_clone_cmd = [ 'git', 'clone', '--filter=blob:none', '--no-checkout',
                f'https://{token}@github.com/{repo_src}.git', srcpath
            ]
            run_command(git_clone_cmd,root_dir)
            # Perform sparse checkout
            sparse_checkout_cmd = ['git', 'sparse-checkout', 'set', srcpath]
            run_command(sparse_checkout_cmd, cwd=os.path.join(root_dir, srcpath))
            files_to_move = os.path.join(root_dir, srcpath, srcpath)
            dest_item = os.path.join(root_dir, srcpath)
            if os.path.exists(files_to_move):
                for file_name in os.listdir(files_to_move):
                    file_path = os.path.join(files_to_move, file_name)
                    try:
                        shutil.move(file_path, dest_item)
                        print(f"Moved {file_path} to {dest_item}")
                    except Exception as e:
                        print(f"Failed to move {file_path}: {e}")
            else:
                print("No files found to setup")
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
    - Make sure assests/work is not done before and it is first time
    - Confirm the presence of Json file

    Returns:
    - bool: should fetch assest or not
    """

    if not os.path.exists(json_file_path):
        print(f"No such file as\n\t{json_file_path}")
        os._exit(0)

    git_cmd = [
        'git', 'remote', 'get-url', '--push', 'origin'
    ]
    
    try:
        repo_type = subprocess.run(git_cmd, cwd=root_dir, 
                                      capture_output=True, text=True, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error during detecting Repo type: {e.stderr}")
    
    if 'Raptor' in repo_type.stdout:
        if os.path.exists(os.path.join(root_dir, 'etc/devices/etc/devices')):
            print('Assests are already present')
            os._exit(0)
        else:
            return 'Raptor'
    else:
        print("Not recognisable repo, if it is a mistake, disable the call of function 'should_fetch'.")
        os._exit(0)
    
if __name__ == "__main__":
    repo = should_fetch()
    bringing_commercial_devices(repo)



