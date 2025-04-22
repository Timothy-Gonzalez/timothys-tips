<div align = center>

# MP NetSec

[![All Tips](https://img.shields.io/badge/Tips-All-blue?style=for-the-badge)](../../README.md)
[![CS 461 Tips](https://img.shields.io/badge/Tips-CS_461-0c0?style=for-the-badge)](../README.md)

</div>

# Getting Started

- Welcome! This is a long MP, not in problems but in difficulty of them.
  Do not be overwhelmed - if you **start early and work step by step** you'll do fine.
- **First things first, read the resources you are given!** Make sure you understand the lectures & content.
  Notably, **this includes the resources on the MP handout!** Generally, **you should know**:
  - What the layers of the internet and how do they depend on each other?
  - What is TCP? What is TLS?
  - What is Wireshark? How do you use it?
  - What is tcpdump? How do you use it?
  - What is dig & curl? How do you use them?
  - What is nmap? How do you use it to map specific ports?
  - What is scapy? How do you it in python?
- Once you feel ready, proceed below. **I'd recommend following the tips in order** since the parts tend to follow into each other.

# 4.1 Checkpoint 1

First things first, follow the setup instructions in the handout to setup your vm!
**Note: make sure to put the right value in `netid.txt`!** If you don't, you'll have to redo everything!

## 4.1.1 Exploring Network Traffic

- Your goal is identify several basic pieces of information using a Wireshark dump.
- If you haven't used Wireshark before, the first discussion gives a demo, but there are also plenty of video online.
- Make sure you're listening on the switch
- If you're having trouble manually tracking each ip and mac used,
  consider there are multiple panels such as the `Statistics` window that can summarize that information for you.
- Gateways rewrite the src and dest mac addresses to forward the packet to the next hop. You can see this in the wireshark response.
- Port scanners typically hit a LOT of ports. They also will hit ports that aren't active, which typically is abnormal traffic.
  See which ip is querying tons of different ports that are unavailable.
- DNS queries are automatically parsed by Wireshark, so you should be able to find them easily
- The same is true for HTTP requests

## 4.1.2 Interacting with Network Hosts

- For this problem, you will use dig and curl to perform requests from attacker. See the handout for how to ssh into attacker.
- If you don't know how to use dig or curl, look it up!

## 4.1.3 Scanning Network Hosts

- The first part of this problem is simply running nmap. Again, if you don't know how to use nmap, look it up! 
  These are all very commonplace tools you will use a bunch if you do any networking at all.
- The second part is a little more tricky - you're making what you just did with nmap in python using scapy
- Note that there is one target IP that you will check ports 1-1024 for
- You want to send a SYN packet to each port, and if you get a SYN-ACK, mark it down as an active port
- `sr1` sends one packet, and records 1 response
- **Make sure you send a RST (reset) if you get a SYN-ACK!** If you don't, you won't get any credit -
  the host will run out of resources to serve network requests (too many open connections, you need to close them after you are done)

# 4.2 Checkpoint 2

Coming soon!

# Conclusion

You did it! Congratulations!

c:
