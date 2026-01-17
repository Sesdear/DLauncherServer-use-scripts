import git
import os

def clone_repo(repo_url: str, local_dir: str = ".") -> None:
    try:
        repo = git.Repo.clone_from(repo_url, local_dir)
        print(f"Successfully cloned {repo_url} into {os.path.abspath(local_dir)}")
    except git.exc.GitCommandError as e:
        print(f"Error cloning repository: {e}")

def pull_repo(local_dir: str):
    if os.path.isdir(local_dir):
        try:
            # Create a Repo object
            repo = git.Repo(local_dir)

            # Get the 'origin' remote (the default name for the remote repository)
            origin = repo.remotes.origin

            # Pull the latest changes from the remote
            # This is equivalent to `git pull origin` in the command line
            # The pull() method returns a list of FetchInfo objects
            fetch_info_list = origin.pull()

            print(f"Successfully pulled changes for repository: {local_dir}")
            for info in fetch_info_list:
                print(f"  Ref: {info.ref.name}, Flags: {info.flags}")

        except Exception as e:
            print(f"An error occurred during git pull: {e}")
    else:
        print(f"Error: Directory not found or not a git repository at {local_dir}")

if __name__ == "__main__":
    usr_inpt: int = int(input("1. Clone repo\n2. Pull repo"))
    
    url: str = "https://github.com/Sesdear/DLauncherServer-use-scripts.git"
    local_dir: str = "./DLauncherServer-use-scripts"
    
    match usr_inpt:
        case 1:
            clone_repo(repo_url=url, local_dir=local_dir)
        case 2:
            pull_repo(local_dir=local_dir)
            