<div align = center>

# MP Password Cracker

[![All Tips](https://img.shields.io/badge/Tips-All-green?style=for-the-badge)](../../README.md)
[![CS 341 Tips](https://img.shields.io/badge/Tips-CS_341-yellow?style=for-the-badge)](../README.md)
[![Assignment Page](https://img.shields.io/badge/Assignment-Password_Cracker-seagreen?style=for-the-badge)](https://cs341.cs.illinois.edu/assignments/password_cracker.html)

</div>

# Visual debugger & tasks setup

To add the visual debugger & tasks, download and extract the tar from the [latest release](https://github.com/Timothy-Gonzalez/timothys-tips/releases/latest):

```sh
curl -sSL https://github.com/Timothy-Gonzalez/timothys-tips/releases/latest/download/cs341-mp-password-cracker.tar | tar -xv
```

# Usage

1. A visual debugger and several tasks are provided
2. The debugger can be used by using the debugging tab in vscode like normal
3. Tasks can be run using `Ctrl + Shift + P > Run Task`, or `Ctrl + Shift + B` to run a default task (which can be changed)
   a. These tasks let you compile & run your code without typing commands to do so

# Version 1 Tips

- First, pause, and read the [MP page](https://cs341.cs.illinois.edu/assignments/password_cracker.html) fully. Not just version 1.
- Now, look at the utilities you're given. You do not need to code rotation or finding the next password to try - you are given those utility functions
- As the MP page recommends - your first step should be to parse all of the passwords and add them as tasks to a task queue
  - Read directly from stdin - `strchr` or `strtok` might be helpful for parsing
  - A `task_t` struct might be helpful for this
  - Think about what you're going to need in your thread - what data is needed to "crack" a password
  - Remember you can't store anything local to the scope as it will be on the stack - everything in your task including the task itself needs to be on the heap
  - Make sure you handle the no-unknowns case! (For example: `abc` instead of `ab.`)
- After you verify that works (debugger is great for this!), create the number of threads
- Then, join all of them
- Each thread should pull a task from the queue, solve it, and repeat
- Think about what you could add to the queue to know when you should stop looking for tasks (a string uses `\0`, argv uses `NULL`, what could a queue use?)
- Test, test, test! Make sure your code works. The 10k input file is a great stress test to verify everything is working. Make sure you don't have memory leaks & pass TSAN.

# Version 2 Tips

- This time, instead of having each thread crack it's own password - we split the search space between all of our threads
- Take your code from version 1 - we want to modify it to use our new behavior
- We can't use a queue anymore - each password is individual and we want all of our threads to be in sync
  - For example, if I have 2 threads, both need to finish before I can go on to the next password
  - Think about what datatype waits for multiple threads to reach the same place - hint hint it begins with a b
- MAKE SURE you reuse the same threads for each password - you should only ever create `num_threads` threads
- Again, think about when you actually want to join the threads
  - You can't do it right after creation in this case because you need multiple threads to process and stop processing and so on for multiple times in a row
- Again, test! Make sure your code doesn't deadlock - you may have to run it a few times to be sure. Make sure you don't have memory leaks & pass TSAN.
