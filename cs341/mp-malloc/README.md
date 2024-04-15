<div align = center>

# MP Malloc

[![All Tips](https://img.shields.io/badge/Tips-All-green?style=for-the-badge)](../../README.md)
[![CS 341 Tips](https://img.shields.io/badge/Tips-CS_341-yellow?style=for-the-badge)](../README.md)
[![Assignment Page](https://img.shields.io/badge/Assignment-malloc-0c0?style=for-the-badge)](https://cs341.cs.illinois.edu/assignments/malloc.html)

</div>

# Installation

To install, download and extract the tar from the [latest release](https://github.com/Timothy-Gonzalez/timothys-tips/releases/latest):

```sh
curl -sSL https://github.com/Timothy-Gonzalez/timothys-tips/releases/latest/download/cs341-mp-malloc.tar | tar -xv
```

# Usage

1. A visual debugger and several tasks are provided
2. The debugger can be used by using the debugging tab in vscode like normal
3. Tasks can be run using `Ctrl + Shift + P > Run Task`

# Part 1 Tips

- **DO NOT** do premature optimization!!! Start as simple as possible
- Start with the basic find first free implementation we did in lecture
- Make your realloc just call free() & malloc(), and then use memcpy
- This should pass tests 1, 3, 4, 5, and 6
- Since 2 does a lot of mallocs and then at the end frees, you will time out if you iterate through every node
  when you don't have too (since there are no free nodes and you check anyways)
- The simple solution for this is a flag `is_free_block` that you set to true or false when you add/remove a free block,
  and only check for a free node in malloc if it is true.
- This should be enough to pass part 1.
- **Note**: writing functions like `add_to_list` and reducing code duplication is KEY to making this MP easy.
  Do not make this complicated, the best malloc from last year was only 200 lines!
- DO NOT START COMPLICATED! Performance does not matter - if your code fails to work, you will get a 0.

# Part 2 Tips

- Instead of using a `is_free_block` flag, you can use a segregated free list (a linked list of just free blocks),
  which will make finding a free block MUCH faster
- Splitting & coalescing are a huge optimization - make sure you implement them correctly
  - If you implement only one without the other, this will cause you to timeout / overuse heap
- **VERY IMPORTANT**: Get splitting and coalescing working **first**. Do not try to be smart - **split in malloc and coalesce in free**
  - If you try to do this anywhere else, and don't do it perfectly - you will make debugging impossible
  - For example: you can't just add split or coalesce to realloc unless you do something else
- TLDR: Keep it simple, stupid. Get it working, than get your #1 spot on the leaderboard if you want to.
- Make sure you're using linked lists properly. It's half of this MP and spending 5 hours debugging to learn you don't handle head properly is not fun.
- Make functions for you lls, like `add_to_free_list` `remove_from_free_list`. Don't write duplicate code.
