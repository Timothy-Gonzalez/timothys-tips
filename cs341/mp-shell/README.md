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

1. A visual debugger and several tasks are provided
2. The debugger can be used by using the debugging tab in vscode like normal
3. Tasks can be run using `Ctrl + Shift + P > Run Task`
4. A test suite which tests Part 1 & Part 2
  - To use it, run `make -f TestMakefile` and `./shell-test`
  - Tests are in `tests/`, feel free to add your own!
  - The testing library is [Caught](https://github.com/Timothy-Gonzalez/caught), feel free to use it in the future!
  - **Things that are not tested**:
    - The following were not easy to automate testing for, so you will have to manually test them.
    - Part 1: `SIGINT` handling
    - Part 2: ps, background processes, and signal commands (`kill`, `stop`, `cont`)
    - See the tips below for tips with testing these
  - These tests are very strict, and the autograder is less strict.
    However, if you pass my test suite, you will pass the autograder for those same tests.

# Part 1 Tips

- First, pause, and read the [MP page](https://cs341.cs.illinois.edu/assignments/shell.html) fully. **Not just part 1**.
  You may also find the [Processes](https://cs341.cs.illinois.edu/coursebook/Processes) section in the coursebook helpful.
  - Planning and designing your code in a way that you can add part 2 later is **crucial**. Don't make your life harder.
  - **Use a debugger!** You will save **hundreds of hours** of debugging in the course of this course using the debugger.
    If you are writing print statements to debug, you are wasting time. The debugger does it for you, don't make your life harder!
  - Here's some questions you should know the answer too:
    - What is fork-exec-wait, and how do we do it?
    - How can you read what the user types?
    - How can you output a response?
    - How do you "handle" `SIGINT`. `signal` is not the right answer!
  - Some questions you should know from the [MP page](https://cs341.cs.illinois.edu/assignments/shell.html):
    - What's the difference between a built in command and foreground command?
    - What are all of the built in commands?
    - How does history work?
    - How do script files work?
- Your first goal should be very simple - whenever the user types something - output it back.
  - Note that this happens when the user presses enter - which sends a `\n` or newline to `stdin`.
  - You might find `getline` useful here
  - You should do this in a loop, so multiple commands can be run
- Next, you need to process that input. You may find your string splitting functions from the previous MP useful here.
  Note that each argument in a command is separated by a space (` `).
  - If you split by a space, that's your `argv`! Remember the first element of `argv` is the name of the command or file ran.
  - For example, the `argv` of `echo hello world` is `["echo", "hello", "world"]`.
- Now that you have processed input, you should start running commands. This is where fork-exec-wait comes into play.
  - **Do not forget about flushing or you will fail the AG!**
    See the [MP page](https://cs341.cs.illinois.edu/assignments/shell.html#flush-before-forking) for more info.
  - Once you successfully implement this, you should be able to run commands!
  - Do not forget to print out the prompt! Make sure your output matches the [MP page](https://cs341.cs.illinois.edu/assignments/shell.html).
  - Try `echo`, `cat`, even `make`!
  - You can even run your own shell from within your shell from within your shell! Shellception!
  - Verify this works, as this is the base of your shell mp.
- At this point, I'll say it again - **organize your code**. Use functions.
  Make everything modular, and each function have one purpose. Do not make your life harder.
- Now that you have basic foreground commands working, you'll add the built-in commands: `cd`, `!history`, `#<n>`, `!<prefix>`.
  - Recognizing built-in commands is easy - just check what the command starts with!
  - If it doesn't start with a built-in, treat it as a foreground command.
  - `cd` is just changing the current working directory. Check the [MP page](https://cs341.cs.illinois.edu/assignments/shell.html) for how you do that!
    - Make sure you also update this when outputting your prompt!
  - `!history` is just outputting everything in the history - you might want to store the history list somehow.
    - What data structure is good for storing a list of things?
    - You should update this after every command run.
  - `#n` will run the nth command from history. Having a generic "run this command" function makes this really easy.
  - `!prefix` will search the history for a command. Same as above, but searches instead of selects.
  - Make sure you're using the proper format functions on errors!
- When your shell is started, there's two optional arguments you've been ignoring until now:
  `-h` for specifying a history file, and `-f` for specifying a script file.
  - First things first, parse these arguments. `getopt` makes this easy,
    see the [MP page](https://cs341.cs.illinois.edu/assignments/shell.html#starting-your-shell).
  - If you're currently doing `getline` on `stdin`, using `-f` is simple - use `fopen` on the file provided and `getline` that instead.
    `-f` is literally just running easy command from a file.
    - **However**, because it is a file, you also need to print out the commands you are running,
      so again check the reference on the [MP page](https://cs341.cs.illinois.edu/assignments/shell.html#starting-your-shell).
  - `-h` does two things:
    - If the file exists, use it to initialize your shell's history. Otherwise, don't worry about it for now and start with empty history.
    - When you shell finishes, write your history to this file, creating it if it doesn't exist.
    - Again, reference the docs, test, and verify it works.
- Finally, the last part of part 1 is logical operators.
  Read the [MP Page](https://cs341.cs.illinois.edu/assignments/shell.html#logical-operators) to make sure you understand them!
  - **If you've made your code modular, this will be very easy to implement.**
  - First, detect if there's any logic operators. If there is, split the command into its subcommands, and run those.
  - For `;`, you just run both, one after the other
  - For `&&`, you run the second only if the first succeeds
  - For `||`, you run the second only if the first fails
  - You do not need to handle built-ins
- Before you submit, **test, test, test**, and test some more.
  - I've included a test suite which fully tests part 1.
    Note that it also tests some of part 2, so ignore those failing for now.
  - See [Installation](#installation) to install it, and [Usage](#usage) to use it
  - You should also do some sanity testing to verify everything works.
  - My tests do not covert `SIGINT` which should stop the foreground process but not the shell. Test this.

# Part 2 Tips

- You should have already read the [MP page](https://cs341.cs.illinois.edu/assignments/shell.html) fully.
  If you haven't, do so now.
  - You should be able to answer the following questions. If not, you may need to revisit the coursebook.
    - How do we tell when a command is a background command?
    - How do we handle background processes?
    - What should `ps` output? Where do we find that info, and how do we parse it?
    - How do we send signals?
    - How do we redirect input or output? Why do we need `dup`?
- First things first - add background processes
  - If it has a `&` at the end, it becomes a background command! Make sure to remove that `&` when actually running the command.
  - Since it's a background command, you shouldn't wait for it to finish - just start it and let it exist
  - You do, however, have to keep track of these background processes, so you can clean them up when they become zombies
- Next, add `ps`. `ps` will help you keep track of your background processes, and debug what is going on.
  Also, it provides a convenient way to detect zombies!
  - Before you write any come, go to the man pages (hint hint `/proc/`), and figure out what you need to parse, and where.
  - **Be very careful of units!** You will need to do some conversions, especially when working with time.
  - **You cannot just execute ps**!
  - Start with the easy ones first (state, command name), and then slowly add and test more as you add them
  - Make sure your own shell shows up in `ps`!
  - Note that commands only use cpu time when actively doing something. Try running `dd` in the background to test your time.
  - Once you have logic to check state, you **should clean up zombies**. If a process state is `z`, it is a zombie!
    - To clean it up, you should wait on it, and then remove it from your list.
    - Make sure completed processes do not show up in `ps`!
    - Think about when you should check if processes are zombies (hint: where does your code run repeatedly)
- For signal commands, you are just sending a signal. You should know how to do this from the lectures,
  check the coursebook on [Signals](https://cs341.cs.illinois.edu/coursebook/Signals) if you need to.
  - Make sure you handle the right error outputs if it fails!
  - You only need to handle processes that are children of your shell
- For redirection operators, you are effectively changing stdin and stdout when executing a command.
  You will find the man pages for [`dup`](https://man7.org/linux/man-pages/man3/dup.3p.html) helpful.
  - The basic idea here is that for the child process, before the command is executed, we change stdin/stdout to point to a different fd.
  - This allows you to make a command output into a file, or read from a file, and so on.
  - For example, `wc < test.txt` will get the word counts for `test.txt`, but behave as if you just typed it out
  - If you do `echo "wow" > wow.txt`, there will be no output, but `wow.txt` will have the output.
  - `>>` is the append version of `>`, that is it doesn't get rid of any content that already exists.
  - Test this!
- Finally, **test, test, test,** and test some more.
  - I've included a test suite which tests part of part 2.
  - See [Installation](#installation) to install it, and [Usage](#usage) to use it.
  - At this point, everything should pass.
  - You should also do some sanity testing to verify everything works.
  - Because of the nature of processes, the **provided test suite does not test them**.
  - This means you have to manually test background commands, `ps`, and the kill commands.
  - Some tips for testing:
    - Try sing sleep commands in the background and have them show up in ps
    - Make sure shell shows up in ps
    - To test time, use the `dd` command from the assignment page
      - Note: your shell and commands like sleep will not take CPU time because they are fast,
        and the main time block is waiting for stdin or scheduling a resumption.
        Things like an infinite loop, or writing large files in `dd`'s case, actually take CPU time.
    - Verify `kill`, `stop`, `cont` all work. If `ps` works, this should be fairly straightforward
    - ps, background processes, and signal commands all verify each other - use them to test the others

c:
