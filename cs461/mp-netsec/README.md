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
  - What is a MITM attack? What is ARP? What is ARP Spoofing?
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

**It will be very helpful to know wireshark for this checkpoint!**
Make sure you do, as there's no error messages - it just won't work. If you use wireshark, you can see where things went wrong.

## 4.2.1 MITM attacks

There are two MITM parts you are required to implement. The first is much easier than the second.

### 4.2.1.1 ARP Spoofing

- The goal is this part is a passive MITM - you aren't modifying packets, just extracting data out of them
- To do this, you redirect all traffic so that it flows to you,
  and then you are responsible for forwarding the data unmodified to the proper destination
- To redirect traffic, you need to use ARP Spoofing
  - ARP requests and responses are how the network attaches a device (hardware mac address) to an ip (network address)
  - ARP spoofing is sending out fake responses to attach all ips to your own controlled device
  - This allows you to intercept all packets and do whatever you want
- The starter code is pretty nice - note that you only need to spoof entries that you are interested in inspecting.
  For example, the server will never make a DNS call, so we don't need to spoof that entry.
- To check if arp spoofing works, you can use `arp -n` on the client
- **Make sure to reset arp tables to the original entries on exit**
- After you have traffic flowing through, you should be able to log packets and see a bunch of traffic
- Note: Since all packets go to you now, they will never reach their proper destination unless you send them on
- Additionally, the intercept function will also intercept packets you send,
  so **make sure not to resend packets you send** or you'll enter recursive hell
    - You can do this by simple checking the sending hardware address isn't the attackerMAC
- Once you have all traffic flowing through normally (verify with wireshark), you should begin to extract data
- DNS requests should be very simple to parse, though the docs aren't great
- For the session, it's in a HTTP header
- For the cookie, it's encoded in base64, out of which you need to get the password
- **Verify everything looks the same in wireshark with and without your arp spoofing active**
  - It's normal to see 2 "duplicate" packets in wireshark, since wireshark will see the packet to attacker and from attacker
    - You can set a capture filter of `ether src host attackerMAC` to only see packets you send,
      which will make it much clearer what is actually errors vs wireshark not understanding
      (note capture filters are different from display filters)

### 4.2.1.2 Script Injection

- **Important note:** This problem requires a good understanding of how seq/ack numbers work and how to use them.
  I recommend **starting with 4.2.2 (Mitnick) first** since it's a better starting point - this part is the hardest of the 3.
- For this part, you want to use the same ARP spoofing to perform a MITM, but this time actively modify the packets to inject 
- Start by setting up a more simple version of the ARP spoofing you did in the previous problem.
  This time, it's just between client and server.
- Once you have packets flowing between them, start by injecting the script
  - Read the directions carefully - **you need to inject the script in a very specific spot** -
    if it's off (even by a byte/space), you'll get no credit. *(ask me how i know...)*
  - When modifying packets, you can either modify the existing one and resend it, or create a new one from scratch.
    I think modifying is easier, since it's easier to miss properties.
  - Whichever way you choose, **make sure the checksums (TCP and IP) and length (IP) properties** of the packet are reset.
    Otherwise, the server will just reject the packet.
  - Note that HTTP you can't just change the payload, there's also a header that tells the client how much will be sent.
    **If you don't modify this header, your modified payload will be cut off**.
- After that's working, the hard part is next. Since you're modifying the payload of the packet, the seq and ack numbers will change.
- For example, the server doesn't know about the extra injected content, so it will see ack numbers that are higher 
  (since the client will ack the longer injected content)
- **Draw out what the server and client should see**.
  You need to simultaneously lie to both so the server thinks the client is accepting the original payload,
  while the client thinks the server is sending the injected payload.
- **curl working is not enough**! Also, you'll need to use wireshark anyways when nothing happens, so learn it!
- **Verify everything looks the same in wireshark with and without your script active**
  - Obviously, the only difference will be the seq and acks, but the client and server should successfully FIN-ACK after everything
  - It's normal to see 2 "duplicate" packets in wireshark, since wireshark will see the packet to attacker and from attacker
    - You can set a capture filter of `ether src host attackerMAC` to only see packets you send,
      which will make it much clearer what is actually errors vs wireshark not understanding
      (note capture filters are different from display filters)

#### 4.2.1.2 Bonus

*seriously, we need to do something about these numbers, this is worse than semver, we already know it's mp 4..., and these don't have to be 2 parts under the same category but alas...*

- For this problem, you need to extend your ~~4.2.1.2~~ script injection code to work across packet boundaries
- This means that if `</body>` occurs at the end of the packet, and there's not enough space left to fit the injection,
  you have to span it across multiple packets
- To see if there's enough space, you'll need to look at the Maximum Segment Size (MSS) option of the TCP layer
  - Note that this is not the true max size, as you need to subtract the size of additional TCP options
  - For example, MSS might be 1460 bytes, but the additional TCP options take 12 bytes, so the payload has a max size of 1448 bytes
- You also need to handle very large injections (think 5000 bytes) that require additional packets (you can't fit that size just by modifying existing packets)
  - Here's a helpful script to test large payloads:
    `python3 cp2.1.http.py -i eth0 --clientIP 10.4.22.193 --serverIP 10.4.22.213 -s "alert(\"$(printf '1%.0s' {1..3000})\")"`.
    Feel free to set 3000 to whatever number of bytes you want to test.
- Be very careful with ack/seq numbers - anything off by one will make it not work
- Good luck! Making your code modular will make this much easier.

### 4.2.2 TCP Off-Path Session Spoofing

- As a note before you start, rsh is like ssh but trust is based on specific ips instead of keys. This is less secure, obviously.
- This attack is interesting, make sure to read the documents attached first.
  I found [this one](https://www.giac.org/paper/gsec/1929/kevin-mitnick-hacking/100826) to be helpful.
  **If you don't, you'll be stuck for hours because your payload is formatted wrong**.
- This includes rsh! Know how to format the payload!
- Notably, in our case, the trusted host is offline, you **do not need to do a DOS** attack
- The attack essentially boils down to this: without seeing the response from the server, we need spoof a login from a trusted ip
- Since we can't see the response, we have no way of knowing the "random" initial sequence number to ACK.
  Thankfully, in the version we are attacking, sequence numbers are not really random.
- First things first, you can figure out the sequence number by testing several times to see what the pattern is, and use that to predict the next one.
- Then, without confirmation, you proceed with the rest of the connection, waiting a bit of time in between to allow the server to process
- If all things go well, you're added to the hosts file and can do whatever now. See the handout for more details on verifying.
- **If it doesn't work,** check wireshark. Likely you've formatted the payload wrong, or done the seq/acks wrong.
  If any detail is wrong, it simply won't work. READ THE DOCS.

# Conclusion

You did it! Congratulations!

c:
