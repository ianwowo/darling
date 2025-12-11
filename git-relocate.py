import subprocess

class SubModule:
    def __init__(self, url_key: str, url: str):
        self.url_key = url_key
        self.url = url

CLONED_SUBMODULES = [
    # Put your cloned submodule urls here
    '../darling.git',
    '../darling-xnu.git',
    '../darling-perl.git',
    '../darling-ruby.git',
    '../darling-metal.git',
    '../darling-foundation.git',
    '../darling-corefoundation.git',
    '../darling-iokituser.git',
    '../darlingserver.git',
    '../darling-libpthread.git',
    '../darling-libmalloc.git',
    '../darling-liblzma.git',
    '../darling-Libc.git',
    '../darling-bash.git',
    '../darling-cocotron.git',
    '../darling-security.git',
    '../darling-cfnetwork.git',
    '../darling-bzip2.git',
    '../darling-objc4.git',
]

UPSTREAM_GITHUB_USER = "darlinghq"

def get_my_repo_link():
    """Retrieve the URL of the main Git repository."""
    try:
        result = subprocess.run(
            ['git', 'remote', 'get-url', 'origin'],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            check=True
        )
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        print("Error retrieving main repository URL:", e.stderr)
        return None


def get_submodules():
    """Retrieve a list of submodule URLs from the current Git repository."""
    try:
        result = subprocess.run(
            ['git', 'config', '--file', '.gitmodules', '--get-regexp', 'url'],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            check=True
        )
        mods = [SubModule(line.split(' ')[0], line.split(' ')[1]) for line in result.stdout.strip().split('\n') if line]
        return mods
    except subprocess.CalledProcessError as e:
        print("Error retrieving submodule URLs:", e.stderr)
        return []
    
def replace_submodule_link(mod: SubModule, new_link):
    """Replace a submodule URL in the .gitmodules file."""
    try:
        subprocess.run(
            ['git', 'config', '--file', '.gitmodules', '--replace-all', mod.url_key, new_link],
            check=True
        )
        print(f"Replaced submodule link: {mod.url} -> {new_link}")
    except subprocess.CalledProcessError as e:
        print("Error replacing submodule URL:", e.stderr)
    

if __name__ == "__main__":
    my_repo_link = get_my_repo_link()
    assert(my_repo_link and "https://github.com" in my_repo_link)
    print(f"Main Repository URL:\n\t{my_repo_link}\n")
    print("Submodule URLs:")
    forked = 0
    darlinghq = 0
    for mod in get_submodules():
        if mod.url not in CLONED_SUBMODULES:
            if ("../" in mod.url) and (UPSTREAM_GITHUB_USER not in mod.url):
                new_link = f"https://github.com/{UPSTREAM_GITHUB_USER}/{mod.url.split('/')[1]}"
                print("\t %-80s -> %-80s" % (mod.url, new_link))
                darlinghq += 1
                replace_submodule_link(mod, new_link)
            else:
                print("\t %-80s (unchanged)" % (mod.url))
        else:
                print("\t %-80s (in CLONED_SUBMODULES)" % (mod.url))
                forked += 1
    print(f"{forked+darlinghq} submodules: {forked} are forked and {darlinghq} darlinghq")