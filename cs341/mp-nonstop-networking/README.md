<div align = center>

# MP Nonstop Networking

[![Assignment Page](https://img.shields.io/badge/Assignment-Nonstop_Networking-magenta?style=for-the-badge)](https://cs341.cs.illinois.edu/assignments/networking_mp.html)

</div>

# Installation

To install, download and extract the tar from the [latest release](https://github.com/Timothy-Gonzalez/timothys-tips/releases/latest):

```sh
curl -sSL https://github.com/Timothy-Gonzalez/timothys-tips/releases/latest/download/cs341-mp-nonstop-networking.tar | tar -xv
```

# Usage

1. A visual debugger and several tasks are provided
2. The debugger can be used by using the debugging tab in vscode like normal
3. Tasks can be run using `Ctrl + Shift + P > Run Task`
4. **Additionally, test files are provided of 3 sizes**
   - [test_files/hello.txt](test_files/hello.txt) is a simple small "Hello, world!" file
   - [test_files/harrypotter.txt](test_files/harrypotter.txt) is a medium sized text file - the entire first book of Harry Potter
   - [test_files/tacos.mp4](test_files/tacos.mp4) is a large video file - this should be your "everything works properly" stress test
   - I recommend starting with the smallest, and once that works work your way up the list

# Part 1 Tips

- Stop. Breath. Read the [MP page](https://cs341.cs.illinois.edu/assignments/networking_mp.html) fully.
  Read everything thoroughly. Yes, this is part 1, but you still need to know the rest so you can integrate everything together.
  Yes, it's a lot of work, but you will NOT regret it. (I promise c:)
  - You might also find the [coursebook resource on networking](https://cs341.cs.illinois.edu/coursebook/Networking) helpful.
  - If you haven't done the [Charming Chatroom](https://cs341.cs.illinois.edu/assignments/charming_chatroom.html) lab,
    you should, as a lot of the code is reusable, especially for part 1.
  - Do you know what the 4 methods you to to implement are?
  - Which methods require sending raw binary data?
  - Which methods require receiving raw binary data?
  - What is an "error" and how do you handle it?
  - Do you know how to traverse through the results of `getaddrinfo`, make a socket, and connect to it?
  - How do you handle `EINTR`? When do you have to handle it?
  - **If you can't answer the above, go find the answer!** You **_should_ know these** from the three links above!
- **Part 1 is not that difficult,** _but_ you can **make** it difficult if you ignore the numerous resources above.
  Also, if you somehow haven't used a debugger yet, this is the mp to learn (it takes 10 minutes, and saves hundreds). **Good luck!**
- Before you begin, I'd recommend looking at [format.h](format.h) and [common.h](common.h) to see what you have to work with
  - Most of what you have is formatters which **you have to use**
  - You can **implement shared methods** between server and client in [common.h](common.h)!
- First things first, you have two references to play with: `server-reference` and `client-reference`.
  These are **very** useful for testing your server/client with without worrying about having to implement a correct implementation of the other!
  Spend some time playing around with them using the examples from the [MP page](https://cs341.cs.illinois.edu/assignments/networking_mp.html)!
- After you understand the task at hand - start with the basics - parse your arguments & put them all into nice variables you can use later.
  - It's also **really helpful for this MP to have a bunch of logging** - **you won't get penalized for it** - see the provided `LOG` macro
- Once you've done that - connect to the server. You can verify this works by checking the output from `server-reference`.
- Now we can begin. The easiest method you should begin with is `LIST`.
  `LIST` is simple - just send a request to the server, print out the response.
  - To easily test this, you'll need to add a few files.
    I recommend starting up a `server-reference` to test with, and a `client-reference` to add the files.
  - It might be helpful to break each request into two parts - **requesting from the server**, and **getting a response from the server**
    - **TIP:** Make helper functions! Make your life easier! Your main method can be:
      1. Parse arguments
      2. Make request
      3. Get response
      4. Cleanup
  - For the `LIST` request, you don't need to send and binary data - you just need to send the `LIST` verb and a newline.
    See the [MP page](https://cs341.cs.illinois.edu/assignments/networking_mp.html) for specific examples.
  - For responses, you need to first check if the request was a success (`OK`), or had an error (`ERROR`).
    - **Note: You MUST handle invalid headers**. This means if the header does not begin with `OK\n` or `ERROR\n`,
      you should stop processing the response and use the relevant format function.
    - If it's an `ERROR`, there can be an error message afterwards.
      **You need to handle this**, and send it to the relevant format function if `ERROR`.
  - For the `LIST` response, after the header you get the message size (encoded as a `size_t`), and the following message.
    - Make sure you read the size correctly! It will always be 8 bytes after the header.
    - When reading the message, make sure you handle the same problems from Charming Chatroom
      - Reads **can fail**
      - You need to handle `EINTR`
    - If the message you read is **too little**, as in `read < expected_size`, call the relevant format function
    - If the message you read it **too large**, as in `read > expected_size`, call the relevant format function
      - Hint: <details><summary></summary>
        If you only ever request `expected_size` bytes, read will only give you that many. How can you check if the server sent more data?
        </details>
      - Hint hint: <details><summary></summary>
        What happens when we request `expected_size + 1` bytes?
        </details>
    - If the message you read is just right, print it out. You're done!
- Next, I'd recommend `DELETE`.
  - For the `DELETE` request, you don't need to send binary data - what do you need to send? (Look at the [MP page](https://cs341.cs.illinois.edu/assignments/networking_mp.html)!!)
  - For the `DELETE` response, you don't need to handle any binary data - just call the right format function if it succeeds
  - If you **haven't implemented error handling** - now is a perfect time. Trying to `DELETE` a file that doesn't exist gives an error.
- And finally, you have `GET` and `PUT`
  - These two methods are opposites, but have a lot in similar.
  - For the `GET` **request**, you need to specify which file.
  - For the `GET` **response**, you need to read the size & write to a file.
  - For the `PUT` **request**, you need to read from a file and send the size of it & the binary data of that file.
  - For the `PUT` **response**, you need to call the right format function.
  - Make sure you're reading & writing properly, and **send the size before** you send any binary data.
    **Make sure you read the size** before you read any binary data.

And that's all... for now. Check back next week for part 2!

c:
