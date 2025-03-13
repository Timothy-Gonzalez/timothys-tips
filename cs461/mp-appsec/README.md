<div align = center>

# MP AppSec

[![All Tips](https://img.shields.io/badge/Tips-All-blue?style=for-the-badge)](../../README.md)
[![CS 461 Tips](https://img.shields.io/badge/Tips-CS_461-0c0?style=for-the-badge)](../README.md)

</div>

# Getting Started

- Welcome! This is **not a difficult MP, but it is a long one** - if you **start early and work step by step** you'll do fine.
- **First things first, read the resources you are given!** Make sure you understand the lectures & content.
  Notably, **this includes the resources on the MP handout!** Generally, **you should know**:
  - What is assembly? What instructions does it have?
  - How does the stack work? Where is the ret address stored? What is the stack frame pointer?
  - What is a buffer overflow? How can we exploit it?
  - How can we pop a shell?
  - What is a ROP gadget? Why would we use it instead of just buffer overflowing shellcode?
  - How do you use GDB?
    - How can I see assembly instructions for a specific function?
    - How do I control the execution of a program by setting breakpoints and stepping?
    - How do I examine a specific value in memory or the values around it?
  - What is the basic syntax of python? How do you write a payload to stdout?
- Once you understand the MP, make sure you setup the VM. Instructions should be on the handout.
- Finally, proceed below.

# 1.1 Checkpoint 1

## 1.1.1 GDB Practice

- This problem should be simple - follow the instructions
- This is a good test to make sure you understand the basics of GDB
  - If you're stuck, now is a good time to learn GDB!

## 1.1.2 Assembly Practice

- This is another simple problem - if you understand how the stack works, just write 3 lines of asm to complete it
- If you're stuck, try looking at the lecture slides and make sure you understand assembly

## 1.1.3 Assembly practice with pointer(s)

- Instead of directly putting values on the stack, you now need to put pointers to values on the stack
- Note that since the values have to still be in the right place to be treated as arguments,
  you'll need to use registers to store positions of values

## 1.1.4 Assembly practice with pointer(s) and string(s)

- Same as before, but now with strings.
  Remember that numbers are little endian, which affects how you convert the string into numbers.
- If you're unsure what number, try looking up an ascii table.
- Note that the latest value you push to the stack will be the top of the stack, so order of pushing matters.

## 1.1.5 Introduction to Linux function calls

- It's time to pop a shell!
- Note that arguments are not passed through the stack for system calls, but through registers.
- Looking at what the shellcode from lecture does may be helpful, though make sure you understand how it works!
  You'll need to for the next part.

# 1.2 Checkpoint 2

## 1.2.1 Overwriting a variable on the stack

- Examine the c code. Where is the vulnerability?
- What's the layout of the variables on the stack?
- How can you use that vulnerability to overwrite other values on the stack?

## 1.2.2 Overwriting the return address

- Examine the c code
- Your goal is to run `print_good_grade`
- Where is the return address stored?
- How can you overwrite the return address?

## 1.2.3 Redirecting control to shellcode

- Your goal for this and the remaining problems is to pop a shell (i.e. execute shellcode)
- For this problem, your input is passed as arguments. This means if you have any space characters or null characters,
  you'll send multiple arguments, which will result in the program outputting incorrect usage.
- First things first, get your shellcode on the stack
- Then, your goal is to get the program to execute it
- Where do you need to overflow to?

## 1.2.4 Overwriting the return address indirectly

- Since this program uses `strncpy`, **you cannot overwrite the return address just by overflow**
- However, the size passed to `strncpy` is incorrect. How much is it off by?
- If you can't overflow the return address, what can you overflow?
- How would changing that allow you to set the return address?

## 1.2.5 Beyond strings

- This program takes an input file as an argument.
  This means the output of your program will be written to `tmp`, and the program will be passed `tmp` to read in.
- Examine what the program does. It reads in a size for the vector, and data to put in that vector.
- Since it only reads count items, overflow may look impossible. Can you make the amount allocated less than `count * 4`?
- What happens if count is a very large value like `0x40000000`, how much space is allocated?
- If the space written is larger than the space allocated, buffer overflow is possible
- If buffer overflow is possible, you can pop a shell

## 1.2.6 Bypassing DEP

- DEP is in place! That means code on the stack can't be executed.
- Even if you can overwrite the return address, you can't pop a shell by running code on the stack!
- However, this program uses `system`
- `system("/bin/sh")` pops the shell you want, if only you could change the arguments to `system`...
- Try disassembling `greetings`. How could you setup the stack the way you want to pop a shell?
- If you're stuck, remember that the return address can be any instruction, not just the start of a function

## 1.2.7 Variable buffer position

- In the previous programs, you could just hardcode an address since they didn't change
- In this problem, addresses are not the same each run, and are offset by a random amount
- Your program needs to work no matter the offset
- **For testing purposes, you can change the fopen to read an arbitrary file**,
  which makes testing the extremes of the offset range easier.
- In lecture, we learned about NOP sleds. How could they be useful here?
- The return address should be in such a place that for both the max and min offset, the execution lands in your NOP sled

## 1.2.8 Returned-oriented Programming

- This is the hardest problem because it takes a lot time to do
- Make sure you understand how ROP chains work
  - Essentially, we have instructions that we want to run, up to a `ret` instruction
  - Running each of these gadgets after each other, we can pop a shell
  - **We need to set up the stack such that each `ret` goes to the next gadget**
    - Note that `pop` will remove a value from the stack, so **you may need dummy values**
- My advice is to **build your payload step by step, and verify it works**.
  Do not wait to run it until you have all of your gadgets, as you'll find extra work.
- Since the payload is passed through argv and `strcpy`, **the following bytes cannot be present in your payload**:
  - `0x0` - NULL terminates `strcpy`
  - `0x09` - tab creates a second argument after it **this means you can't use the `0x809...` gadgets**
  - `0x20` - space creates a second argument after it
- For searching the object dump file, regex can be helpful to get a gadget you want
- **Test, test, test** - you can gdb breakpoint at the end of each of your gadgets and do `info reg` to verify everything is working
- **Be careful not to undo work you've already done** - you may need to reorder your gadgets to not overwrite setup registers

## 1.2.9 (Bonus) Linked list exploitation

- This is an optional bonus assignment, but **I recommend bonuses because the 5% overflows into other sections, and the exams are hard**
- Also, this one is easier than the ROP chain you just did
- Since the vulnerability allows you to overflow the heap and not the stack, you cannot directly overflow the return address
- Look closely at the linked list implementation. If you overrode certain pointers, could you change the return address?
- Once you've done that, you may notice that part of your program gets overwritten no matter what you do. Jumps can help.

## Conclusion

You did it! Congratulations!

c:
