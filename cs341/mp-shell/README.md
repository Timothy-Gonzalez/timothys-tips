<div align = center>

# MP Shell

[![All Tips](https://img.shields.io/badge/Tips-All-green?style=for-the-badge)](../../README.md)
[![CS 341 Tips](https://img.shields.io/badge/Tips-CS_341-yellow?style=for-the-badge)](../README.md)
[![Assignment Page](https://img.shields.io/badge/Assignment-shell-yellow?style=for-the-badge)](https://cs341.cs.illinois.edu/assignments/shell.html)

</div>

# Installation

To install, download and extract the tar from the [latest release](https://github.com/Timothy-Gonzalez/timothys-tips/releases/latest):

```sh
curl -sSL https://github.com/Timothy-Gonzalez/timothys-tips/releases/latest/download/cs341-mp-shell.tar | tar -xv
```

# Usage

1. You can simply replace the Makefile you have with the one provided, it adds a test target
2. To run tests, run `make test && ./shell-test`. You can also use the tasks in `.vscode/tasks.json` using `Ctrl + Shift + B`
3. `tests/` contains all the tests that will be run using the Caught testing framework.
4. `.vscode` provides the tasks mentioned as well as visual debugger profiles for running the shell & running the tests with the debugger

These tests are very conclusive, but the following are **not tested**:

- ps & background processes
- signal commands (`kill`, `stop`, `cont`)

To test these manually, here are some tips:

- Try using sleep commands in the background and have them show up in ps
- Make sure shell shows up in ps
- To test time, use the `dd` command from the assignment page
  - Note: your shell and commands like sleep will not take CPU time because they are fast,
    and the main time block is waiting for stdin or scheduling a resumption.
    Things like an infinite loop, or writing large files in `dd`'s case, actually take CPU time.
- Look at course handbook for help with signals, but you should just send the signal with `kill` and call it a day.
- ps, background processes, and signal commands all verify each other
