using System;
using LibGit2Sharp;

namespace GitProvider
{
    class Program
    {

        static bool clone_repo(string repo_url, string local_dir)
        {
            try{
                var cloneOptions = new CloneOptions();
                string clonedPath = Repository.Clone(repo_url, local_dir, cloneOptions);

                using (var repo = new Repository(clonedPath))
                {
                    Console.WriteLine("Cloned to: " + repo.Info.WorkingDirectory);
                }
                return true;
                
            }
            catch (Exception)
            {
                return false;
            }
        }

        static bool pull_repo(string repoPath)
        {
            using (var repo = new Repository(repoPath))
            {
                var pullOptions = new PullOptions()
                {
                    FetchOptions = new FetchOptions(){}
                };

                var signature = new Signature("User", "user@example.com", DateTimeOffset.Now);
                MergeResult result = Commands.Pull(repo, signature, pullOptions);
                
                return result.Status == MergeStatus.UpToDate
                    || result.Status == MergeStatus.FastForward
                    || result.Status == MergeStatus.NonFastForward;
            }
        }

        static void Main(string[] args)
        {
            Console.Write("1. Clone repo\n2. Pull repo\nChoose option: ");
            int user_input = Convert.ToInt32(Console.ReadLine());

            string repo_url = "https://github.com/Sesdear/DLauncherServer-use-scripts.git";
            string local_dir = "./DLauncherServer-use-scripts";

            switch (user_input)
            {
                case 1:
                    if (clone_repo(repo_url, local_dir))
                    {
                        Console.WriteLine("Repository cloned successfully.");
                    }
                    else
                    {
                        Console.WriteLine("Failed to clone repository.");
                    }
                    break;
                case 2:
                    if (pull_repo(local_dir))
                    {
                        Console.WriteLine("Repository pulled successfully.");
                    }
                    else
                    {
                        Console.WriteLine("Failed to pull repository.");
                    }
                    break;
                default:
                    break;
            }

        }

        

    }
}
