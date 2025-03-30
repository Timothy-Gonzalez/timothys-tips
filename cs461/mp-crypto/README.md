<div align = center>

# MP Crypto

[![All Tips](https://img.shields.io/badge/Tips-All-blue?style=for-the-badge)](../../README.md)
[![CS 461 Tips](https://img.shields.io/badge/Tips-CS_461-0c0?style=for-the-badge)](../README.md)

</div>

# Getting Started

- Welcome! This is a long MP, not in problems but in difficulty of them.
  Do not be overwhelmed - if you **start early and work step by step** you'll do fine.
- **First things first, read the resources you are given!** Make sure you understand the lectures & content.
  Notably, **this includes the resources on the MP handout!** Generally, **you should know**:
  - What is a substitution cipher?
  - What is a hash? What is MD5 vs SHA3?
  - What is encryption? What is public & private keys? What is RSA vs AES?
  - What is a length extension vulnerability?
  - What is a collision? Why is it an issue?
  - How do you use python to manipulate numbers, hex, and bytes? How do you encrypt/decrypt/hash in python?
- Once you feel ready, proceed below. **I'd recommend following the tips in order** since the parts tend to follow into each other.

# 3.1 Checkpoint 1

This first part ensures you know the python basics - do not skip this! If you struggle with python, you'll have a bad time later.

## 3.1.1 Exercise

- This is a basic warm up problem to ensure you know python - follow the instructions in the hand out
- Make sure you listen to the note about hex numbers - `0xabc` vs `abc` are different outputs - which one should you use?

## 3.1.2 Substitution Cipher

- Your program is given a ciphertext file and a key file, and should output the plaintext
- `sys.argv` is useful for getting arguments (`arg_0, arg_1, arg_2, arg_3 = sys.argv`)
- The keyfile represents ith index -> substituted character
- You should reverse this conversion
- The output will be a jeopardy clue, make sure to write it to the file provided in arguments

## 3.1.3 AES: Decrypting AES

- Your program is given a ciphertext file, key file, iv file, and should output the plaintext
- This isn't anything advanced, just calling the decrypt function with the right arguments
- Make sure you're parsing the input files correctly - the hex representation is a string of hex chars, not a number

## 3.1.4 AES: Breaking A Weak AES Key

- The key used is very weak - only 5 bytes are actually used
- This means there are `2^5=32` possible keys - which is easily bruteforceable
- Try every key, see which one gives a output that makes sense
- You should submit the key that gives the jeopardy clue

## 3.1.5 Decrypting a ciphertext with RSA

- Your program is given a ciphertext file, key file, modulo file, and should output the plaintext prime number
- Remember that RSA is implemented as:
  - Encryption: c = me mod(n)
  - Decryption: m = cd mod(n)
  - Where each variable is:
    - e - public prime (commonly 3 or 65537)
    - n - public modulus
    - d - secret
    - m - plaintext
    - c - ciphertext
  - All you need to do is perform the decryption operation
  - python's `math.pow` takes a 3rd argument for modulus - this can be used for fast pow modulus

## 3.1.6 Weak Hashing Algorithm

- You are given a program `WHA` which is a weak hashing algorithm
- Your goal is to find an input that collides with the jeopardy clue input. That is, `WHA(jeopardyClue) = WHA(input)`.
- Analyze the `WHA` function - try to find what makes it prone to collision
- Try making a basic collision first - find a collision of `WHA("abc")`
- The algorithm is per-byte - is there two sequences of bytes that result in the same output?
- If you're really stuck: try rearranging bytes and working out what happens at each intermediate layer

# 3.2 Checkpoint 2

In progress, please hold c:

# Conclusion

You did it! Congratulations!

c:
