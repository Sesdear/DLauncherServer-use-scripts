#include <git2.h>
#include <iostream>
#include <string>

bool clone_repo(const char *repo_url, const char *local_dir);
bool pull_repo(const char *local_dir);


struct fetch_cb_payload {
    git_repository *repo;
    git_annotated_commit **out;
};

static int fetchhead_cb(const char *ref_name, const char *remote_url, 
                       const git_oid *oid, unsigned int is_merge, void *payload) {
    if (is_merge) {
        struct fetch_cb_payload *p = (struct fetch_cb_payload *)payload;
        git_annotated_commit **target = p->out;
        git_repository *repo = p->repo;
        int err = git_annotated_commit_from_fetchhead(target, repo, ref_name, remote_url, oid);
        return err;
    }
    return GIT_ITEROVER;
}




int main() {
    git_libgit2_init();

    short int user_input;
    std::cout << "1. Clone repo\n2. Pull repo\nChoose an option: ";
    std::cin >> user_input;

    const char* repo_url = "https://github.com/Sesdear/DLauncherServer-use-scripts.git";
    const char* local_dir = "./DLauncherServer-use-scripts";

    switch (user_input) {
    case 1:
        if (clone_repo(repo_url, local_dir)) {
            std::cout << "\nClone True\n";
        } else {
            std::cout << "\nClone False\n";
        }
        break;
    case 2:
        if (pull_repo(local_dir)) {
            std::cout << "\nPulled True\n";
        } else {
            std::cout << "\nPulled False\n";
        }
        break;
    default:
        std::cout << "\nInvalid option\n";
        break;
    }
    return 0;
}

bool clone_repo(const char *repo_url, const char *local_dir) {
    git_repository *repo = NULL;
    git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
    
    int error = git_clone(&repo, repo_url, local_dir, &clone_opts);
    
    if (error != 0) {
        const git_error *e = git_error_last();
        std::cerr << "Clone failed: " << error << " (" 
                  << (e ? e->message : "unknown") << ")\n";
    }
    
    if (repo) git_repository_free(repo);
    return error == 0;
}

bool pull_repo(const char *repo_path) {
    git_repository *repo = NULL;
    git_remote *remote = NULL;
    git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;
    git_signature *sig = NULL;
    git_annotated_commit *fetch_head = NULL;
    git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;
    int error = 0;
    
    error = git_repository_open(&repo, repo_path);
    if (error == 0) {
        error = git_remote_lookup(&remote, repo, "origin");
    }
    
    // 1. Fetch
    if (error == 0) {
        error = git_remote_fetch(remote, NULL, &fetch_opts, NULL);
    }
    
    // 2. Load FETCH_HEAD
    if (error == 0) {
        struct fetch_cb_payload payload = { repo, &fetch_head };
        error = git_repository_fetchhead_foreach(repo, fetchhead_cb, &payload);
    }
    
    // 3. Merge (fast-forward or create merge commit)
    if (error == 0) {
        if (fetch_head) {
            const git_annotated_commit *heads[] = { fetch_head };
            error = git_merge(repo, heads, 1, NULL, NULL);  // NULL opts = defaults
        } else {
            error = 0;
        }
    }

    if (fetch_head) git_annotated_commit_free(fetch_head);
    if (sig) git_signature_free(sig);
    if (remote) git_remote_free(remote);
    if (repo) git_repository_free(repo);
    return error == 0;
}



