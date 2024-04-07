<div align = center>

# MP Finding Filesystems

[![Assignment Page](https://img.shields.io/badge/Assignment-Finding_Filesystems-purple?style=for-the-badge)](https://cs341.cs.illinois.edu/assignments/finding_filesystems.html)

</div>

# Installation

To install, download and extract the tar from the [latest release](https://github.com/Timothy-Gonzalez/timothys-tips/releases/latest):

```sh
curl -sSL https://github.com/Timothy-Gonzalez/timothys-tips/releases/latest/download/cs341-mp-finding-filesystems.tar | tar -xv
```

# Usage

1. A visual debugger and several tasks are provided
2. The debugger can be used by using the debugging tab in vscode like normal
3. Tasks can be run using `Ctrl + Shift + P > Run Task`

# Tips

- Stop. Breath. Read the [MP page](https://cs341.cs.illinois.edu/assignments/parallel-make.html) fully.
  Read everything thoroughly. Yes, it's a lot of work, but you will NOT regret it. (I promise c:)
  You might also find the [coursebook resource on filesystems](https://cs341.cs.illinois.edu/coursebook/Filesystems) helpful.
  - Do you know what an inode is?
  - Do you know what the different between direct and indirect datablocks are,
    when you use them,
    and what data is stored in each?
  - What are the four segments of our filesystem, and what is each used for?
  - I would **strongly recommend looking at graphs & watching the lectures** if you are struggling to wrap your head around this.
    Filesystems & data structures in general are a very visual concept and hard to explain with words.
- Before you begin, I'd recommend looking at [minixfs.h](minixfs.h) and [minixfs_utils.h](minixfs_utils.h) to see what you have to work with
  - You do not have to implement the block allocating logic! There is a method that will help!
- I'd recommend starting with `chmod` (`minixfs_chmod`) and `chown` (`minixfs_chown`). They don't require messing with direct or indirect blocks at all,
  just changing several fields on inodes.
  - For `chmod`, make **_sure_** your bitwise operators are correct, or you'll mess up type!
    See [minixfs.h](minixfs.h) for a hint.
  - Don't forget to update the `ctim`!
  - Once you've done this, test! You don't need reading or writing to test both of these, and they are very easy to verify!
- Next, I'd recommend doing read `minixfs_read`.
  I'd **recommend ignoring virtual reading** for now, and come back to it later.
  - Again, it's very important you understand direct and indirect datablocks.
  - I would **strongly** recommend making a helper function to get the `nth` block of an inode,
    so you don't have a bunch of complicated logic.
  - Remember, it's possible to read from just direct datablocks, just indirect datablocks, or both.
  - Make sure you handle `off` properly
  - If you're past or at the end of the file, `return 0`. Don't give an error - it's completely valid to ask if there's anymore data.
  - It's also allowed to ask for more bytes than exist in the file (for example, read 8000 on a 8 byte file). In this case,
    you should only read what exists in the file and return that amount as what you read.
    Be very careful here - you shouldn't use that 8000 number in this case when you only should read 8 bytes.
  - Don't forget to update `atim`!
  - Test, test, TEST! Test without offsets, with offsets, and importantly, with large files.
    If you test with files less than 16KB, you won't test across datablocks,
    and won't even get close to testing your handling of indirect blocks!
- Earlier, I said to ignore virtual reading. Now that you've verified reading works, it's time to implement `minixfs_virtual_read`.
  - Your goal is to make a "fake" read on `virtual/info`. What this means is when that file is read, you'll act like a file but really
    be creating your own output.
  - The expected output format is defined on the MP page.
  - How will you get the number of unused & used data blocks?
    - You may find the `GET_DATA_MAP` macro in [minixfs.h](minixfs.h) helpful!
  - I'd recommend first creating the full string, and then worry about offset & count.
  - Note - you'll get an infinite loop with cat if you always return the full size -
    as it will keep reading until it gets a end of file (no more bytes to read)
  - You should behave the same way you did for read, and only copy as many bytes as request (or less, if the remaining bytes after offset is less)
  - This should be very easy to test and verify is working - so test!
- **Take a break, relax, pat yourself on the back for how smart you are, and when you're ready, return.**
- The next, and final, step is writing!
  - For writing to be able to create new files, you'll need to implement `minixfs_create_inode_for_path`.
  - First, let's focus on `minixfs_write` and then we'll get back to that.
  - Writing is almost identical logic to reading, except for a few things.
  - Firstly, you'll need to allocate new datablocks if you don't have enough space in your current inode.
    - You don't have to write the logic to allocate direct & indirect blocks, there's a provided method that will do that for you!
    - If there's no more space left in the inode, you should throw a specific error.
    - If there's no more datablocks left in the filesystem, you should throw a specific error.
  - If you're increasing the size of the file, make sure you update the inode's size as well!
  - Make sure you handle `off` properly, and change your conditions from read so you don't write too much or too little.
  - Don't forget to update `mtim`!
- The final boss arrives: `minixfs_create_inode_for_path`
  - If the inode exists, our write logic thus far will work just fine.
    In read, we would error out if it doesn't, but write needs to create that file if it doesn't exist.
    This means creating the corresponding inode.
  - First things first, make sure the inode doesn't exist. If it does, we don't need to create one.
  - Otherwise, we need to do three things:
    - Verify our filename is valid
    - Create a new inode
    - Add a dirent (directory entry) to our parent directory
  - Note we are given the guarantee that the parent of the path will exist (as long as valid)
  - A dirent is just text! It's the inode number and the name of the file!
  - Note that for all three of those things, there is a method to do so. Once you get the parent inode, MAKE SURE you:
    - Have enough space for the new dirent (if not, allocate some!)
    - Make sure you update the size of the parent inode when you add the new dirent!
  - Update `mtim` of the parent directory! When you add a new file, you're modifying the directory.
  - As always, test, test, test! You should now be able to create files, modify them, and read them back to verify.
    - To properly test offsets, you'll likely need to write tests in [minixfs_test.c](minixfs_test.c).

c:
