# Converts relative urls to remote urls
# For example, a link in a_dir/README.md:
# ../ -> https://github.com/Timothy-Gonzalez/timothys-tips/tree/main/

from io import TextIOWrapper
import sys
import re
import os

ROOT = "./"
REMOTE = "https://github.com/Timothy-Gonzalez/timothys-tips/tree/main"
LINK_REGEX = re.compile(r"\[(.*)\]\((.*)\)")
MAKE_LINK = lambda text, link: f"[{text}]({link})"

def replace_links(file: TextIOWrapper, dir: str):

    def replace_link(match: re.Match[str]):
        text, link = match.groups()

        path = os.path.abspath(os.path.join(dir, link))

        if os.path.exists(path) and not path.startswith(os.path.abspath(dir)):
            rel = os.path.relpath(path).replace("\\", "/")
            link = f"{REMOTE}/{rel}"

        return MAKE_LINK(text, link)

    new_lines = []

    for line in file.readlines():
        new_lines.append(re.sub(LINK_REGEX, replace_link, line))

    file.seek(0)
    file.write("".join(new_lines))


for dir, _, files in os.walk(ROOT):
    for file in files:
        if not file.endswith('.md'):
            continue
        path = os.path.join(dir, file)
        with open(path, "r+", newline="") as f:
            replace_links(f, dir)
