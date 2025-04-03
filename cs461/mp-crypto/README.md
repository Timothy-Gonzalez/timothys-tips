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

## 3.2.1 Length Extension

- First, make sure you understand how length extension attacks work.
  The handout has an example you should follow to be familiar with how to do so in python.
- Your program is given query, command, and output files
- Your goal is extend the normal query to have a third, malicious command
- You can't just send this because token is a hash of the password and query combined, so you can't generate your own hash
- Instead, you have to extend the existing one
- To do this correctly, you need to figure out what the original padding was, and add it to query
  - This is because the original message when hashed had padding added to it - to do length extension you need the original content hashed
  - Make sure you use `quote_from_bytes`, as you can't just put the bytes in a query string
- If it's not working, **test**! Write your own tests with your own password - verify everything hashes to the same token.

## 3.2.2 MD5 Collisions

- MD5 is vulnerable to collisions, and they can be generated efficiently
- Follow the handout and learn how fastcoll works
  - fastcoll is a program that generates two suffixes to files such that the hash of each is the same
- This allows us to have two different files with the same hash
- Your goal is to have a good file and evil file which print different things, but have the same hash
- Using the prefix from the handout, you can have a common prefix and suffix, but different blobs in the middle
- The hash will remain the same because the suffix is the same, so length extension applies
- You can generate the blob using fastcoll
- To determine what to output, `sha256(blob.encode()).hexdigest()` will let you tell which blob is which with a common suffix
- Once you think you have it right, good should output one output and evil should output the other.
  The md5 of both should be the same, but sha256 different.

## 3.2.3 Exploiting a Padding Oracle

- Your goal is to abuse a small vulnerability - a padding oracle - to decrypt the ciphertext into plaintext without the key
- First things first, understand how the padding works. What is valid padding? Why do we need padding?
  - Note that there always must be padding
- Next, understand what a padding oracle is.
  A padding oracle means the server tells us **if the decrypted plaintext has valid padding**.
- Therefore, if we control what we modify, we can try different inputs until we get a valid padding.
  When we get a valid padding, **we know the plaintext has a certain output for certain bytes**.
- **If you don't know what intermediates, ciphertext, initialization vectors, and plaintext means in RSA decryption**,
  please look at a diagram. The next part will not make sense if you don't.
- For example, consider ciphertext block 1 (C1) as the initialization vector for ciphertext block 2 (C2).
  The plaintext output is determined by the xor of C1 and the intermediate values of block 2 (I2).
  Therefore, if we modify the last byte of C1 (G) until we get valid padding, we know the last byte of the plaintext of block 2 (P2) MUST BE `0x10`.
  That gives us `G XOR I2[15] = 0x10`, or `I2[15] = G XOR 0x10`. We can do this recursively to get all the intermediates for each block.
- If we have the intermediates, we can get the plaintext of the key, since the intermediates are from the ciphertext being decrypted with the key!
  Simply put, `P = I XOR IV`, or the plaintext is the intermediates xor'd with the previous block/iv.
- If this doesn't make sense, I would recommend drawing this out, as that's what helped it click for me.
  Otherwise, try starting and see where you go from there.
- Some tricks to note:
  - Each block is dependent on the previous block (initialization vector for the first block), which means you can solve in left to right or right to left order for the blocks
  - Padding needs to be solved from right to left as guessing 1 byte is easier than guessing 15 at once
  - Make sure you shift the padding over when moving to guessing the next byte!
    For example: `0xa 0xb 0xc 0x10` -> `0xa 0xb 0x10 0x0f` -> `0xa 0x10 0x0f 0xe`.
    Note that you cannot just use the same values you used before, as the padding changes.
    To get the new values, XOR the intermediate with the new padding value.
  - You'll know you're doing it right if the final block has padding and it forms a grammatical text.
    If you get gibberish, try walking through it again.

## 3.2.4 Creating Colliding Certificates

- Certificates are effectively signed documents by a certificate authority that verify someone is who they say they are (most common use is websites (https))
- Your goal is to generate two certificates that have different public keys but the same signature
- Note that certain fields specified must be identical, but all others are fair game
- Your first thought might be to just have identical prefixes, then use `fastcoll` to generate the modulus.
  While this works, you need to be able to factor the modulus (p and q) to have a private key (and a useful certificate),
  which you are required to prove you can do.
- So, we have to use something called lenstra's attack
  - This means generating two modulus which each have a pq pair but still result in the same suffix to output from fastcoll
- Generally, you should follow the steps in the handout directly - I won't repeat them here so this remains up to date
- Be very careful - lenstra's attack takes a long time and so tiny mistakes can result in a lot of wasted time
- Knowing how to use `xxd` and `openssl` commands can help a lot with debugging - they are given to you!
- Some things to note:
  - You are provided with a certbuilder.py - use it to make your certificates and generate your keys from p and q
  - **Make sure to use cert.tbs_certificate_bytes** when generating prefixes, aligning things, and passing values into fastcoll.
    Only use the final `cert.public_bytes(Encoding.DER)` to output the certificate when you are done.
  - Make sure your padding is lined up so your modulus starts a new block.
    A sign you haven't done this is if your modulus ends up with a prefix of a lot of 0s - you can't have that in your modulus as the final key generated won't have that prefix.
  - Serial numbers exist for certificates - make sure you set this to a fixed value and or your prefixes won't match
- If you're stuck, look at the hex output to see what went wrong.
  The prefix and suffix of both cert's tbs_certificate_bytes should be identical - with only a tiny difference in the modulus.
  Obviously, they should md5 hash to the same value.

# 3.3 Bonus (Optional)

# 3.3.1 Mining your Ps and Qs

- This bonus is harder than the others, but not awful. The difficulty comes from the need to implement batch_gcd.
- You are given a list of moduli
  - Remember from before that the public key (moduli) is the multiplication of two large primes p and q (m = p * q)
- Typically, this wouldn't be very helpful as each encryption should use different ps and qs
- However, in this case, some of those primes are shared.
  Normally, factoring a single moduli would take forever, keeping encryption secure.
  The vulnerability comes from the fact that finding the gcd between two moduli is very fast.
  If a prime is shared between two moduli, it will be the gcd of them.
- So, your goal is to find the gcd between each moduli, and for each gcd that isn't 1, try it as a key to decrypt
- The problem is, there's 10,000 of them, and 10,000 * 10,000 pairs does not scale to be fast enough to solve (in 2 weeks)
- Therefore, you need to read the whitepaper provided (or look up batch gcd) to learn how to find gcd of all of these pairs efficiently
- I won't explain how batch gcd works here, but a helpful guide can be found at https://facthacks.cr.yp.to/batchgcd.html
- Some tips:
  - Finding the gcds can take a while, but if you optimize your code it took me ~5 minutes on the course vm
  - Once you have the gcds, finding the q is as easy as moduli / gcd. (gcd is p)
  - Private keys are computed as `number.inverse(e, (p - 1) * (q - 1))`
  - Try every pair until you get no errors, then you should have the plaintext
- **The most important thing is to test each part of the batch_gcd algorithm**. Test the product tree, remainders, and gcd.
  You do not want to waste hours just to learn you did your math wrong - test on a small array of data to verify everything works.

# Conclusion

You did it! Congratulations!

c:
