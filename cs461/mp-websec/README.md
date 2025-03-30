<div align = center>

# MP WebSec

[![All Tips](https://img.shields.io/badge/Tips-All-blue?style=for-the-badge)](../../README.md)
[![CS 461 Tips](https://img.shields.io/badge/Tips-CS_461-0c0?style=for-the-badge)](../README.md)

</div>

# Getting Started

- Welcome! This is a long MP built of a few sections which each contain several parts.
  Do not be overwhelmed by the number of parts - if you **start early and work step by step** you'll do fine.
- **First things first, read the resources you are given!** Make sure you understand the lectures & content.
  Notably, **this includes the resources on the MP handout!** Generally, **you should know**:
  - What is SQL? How do you use SQL?
  - What is an SQL injection? How do you protect against it? How can you work around basic preventions?
  - How do websites get data? What is HTTP? What are cookies? What is JavaScript?
  - What is Cross Origin Policy? What is Cross Site Scripting (XSS)? What is a CSRF token and why do we use it?
  - How do you write HTML and JavaScript? How do you write a XSS payload?
  - How do you write python? How do you use mySQL in python?
  - What is JQuery? How do you use it?
  - **What is Bungle?** What pages does it have? How do you use it?
- Once you feel ready, proceed below. **I'd recommend following the tips in order** since the parts tend to follow into each other.

# 2.1 Checkpoint 1

## 2.1.1 VM Setup

- This problem is a give me - just follow the instructions to run bungle on your vm!

## 2.1.2 SQL

- This part is just writing SQL queries. If you've taken CS 411, you'll breeze through this.
  Otherwise, it's time to use the resources in the MP document.

## 2.1.3 Prepared Statements

- Python time! All you have to do is modify the mysql queries to properly implement each behavior in database.py
- Testing is easy! Just use your locally running bungle!
- Make sure the number of arguments into the query is the same as the number of %s -
  mysql will fill in the values with protection against injection for you.

## 2.1.4 Input Sanitization

- This is pretty straightforward - your goal is to encode the `<` and `>` characters as a basic form of XSS prevention
- Don't overthink this - it's just one line of code

## 2.1.5 Token Validation

- Remember CSRF tokens? *If not check the lecture!* You're implementing that now!
- The pseudocode is provided on the handout - all you need to do is set the token to a random 16 byte hex string
- You may find the docs for [`os.urandom`](https://docs.python.org/3/library/os.html#os.urandom) helpful

## Conclusion

...and that's it for checkpoint 1! Checkpoint 2 is longer, so make sure to budget time accordingly!

# 2.2 Checkpoint 2

## 2.2.1 - SQL Injection

Before starting, make sure you understand SQL Injection! **Check the handout for resources!**

### 2.2.1.1 No Defenses

- Your goal is login as victim - without having their real password!
- `victim` must be the username field - you'll need the injection in the password (otherwise you won't login as victim)
- Think of how the actual query might be structures (referencing 2.1.3 might be helpful)
  - `SELECT ... FROM ... WHERE x=a AND y=b`
- Try things out! Getting an error means you have injection - you just don't have the right payload yet

### 2.2.1.2 Simple escaping

- The same as the previous problem, but every single quote `'` is replaced with two `''`
- Adjacent strings in SQL concat `'a''b' = 'ab'`
- At first it may look like this prevents injection, but think about what this defense actually does
- Is there a way to make a quote not count to exit the string?

### 2.2.1.3 Perils of Raw Bytes

- This time, whatever you enter is md5 hashed. So, you can't just do direct injection.
- If we could get the hash to be our payload, we could have injection
- No defenses exist after the hash, so you can test payloads on the 2.2.1.1 no defenses
- You want to find a md5 hash that has your payload in it
- Therefore, you want your payload to be small since the only way to search hashes is manually - hashes are one way
- Start with a large payload, and try to get it down to 3-6 characters

### 2.2.1.4 Spying With SQL Injection

- New target! Instead of logging in as a user, we want to collect information about the database
- See how a normal query behaves
- Try to change the output to an arbitrary string "abc" "def"
- Look at what you have to submit - you should know what exactly you need
- Look at references! Getting the database and version is very simple in mysql!
- All tables in all databases are stored in a special table [`information_schema.tables`](https://dev.mysql.com/doc/refman/8.4/en/information-schema-tables-table.html)
- The same applies to columns
- Generally, with SQL Injection you want to follow a simple process:
  1. Figure out what specific database and version you're working with (mysql, postgres, ect.) v1.2.3
  2. Find the tables it has
  3. For each interesting table, figure out what columns it has
  4. Build a query using the information you gathered to get the interesting data out

## 2.2.2 Cross-site Request Forgery (CSRF)

Your goal is to make two html pages, such that if a user visits them,
they will be logged onto bungle as `attacker` (password `l33th4x`).

You **are not redirecting** to the real bungle - the next time they visit bungle they should be logged in.
Your attack should be invisible - your html pages should show as blank white pages.

### 2.2.2.1 No Defenses

- No CSRF defenses exist! Note: make sure you're **setting csrfdefense and xssdefense for EVERY request** you make,
  or you may get faulty positives when working locally.
- Why can't we just login directly?
- We need the context from which we login to be from bungle
- Try loading bungle in an iframe
- Is there a way to execute the login in the context of that iframe?

### 2.2.2.2 Token Validation

- If you're unfamiliar with JavaScript, starting with 2.2.3* is a good idea, but you'll still need to understand JavaScript basics.
- The CSRF defense you implemented before is now in place! However, there's no longer any XSS defense.
- With the CSRF, we can't use our previous method any more. Does XSS help?
- Any code executed with XSS bypasses cross origin, since it executes from bungle itself
- To login, we need the CSRF token. Unfortunately, it's not on the search results page. How can we get it?
- You don't need a form for this part. Why not make the login request directly with fetch?

## 2.2.3 Cross-site Scripts (XSS)

Your goal for this part is to perform a full takeover of bungle.
If the user clicks on your link, they will open a session of Bungle that is undetectable, logs everything, and is inescapable
(without closing the page). You need to get this attack past various XSS filters.

A word of advice: **Do this part by part, and test along the way**. You cannot do this in one go.
Similarly, **modularize your payload**. You can break it up into multiple functions!

### 2.2.3.1 XSS Practice

- This is a separate practice to get you started, attacking `/multivac`
- Your goal is to change the cs link to an ece one. The crime!
- This should be a simple xss payload which just changes the link's href, and nothing else.

### 2.2.3.2 XSS Defense Level 0: No Defenses

- Your payload should have 3 important functions: Persistence, Spying, and Stealth
  - Persistence means clicking on links or forms should work, but keep the user on the infected bungle
    - You prove this by setting the title of the page
	- This requires wrapping every link and form on the page with a custom event handler
	- `event.preventDefault()` prevents the default behavior of an event. For a link, it prevents navigation.
	- **You should not query individual elements**. It will make your logic much cleaner to handle each link/form the same.
	- Instead, use selectors like `document.querySelectorAll("a")`
	- proxy is used to proxy navigating to another page - it replaces the current page with the html of the href you pass,
	  but keeps you in control of the page
	- Forms are tricky to handle - note that login and create are the SAME form.
	  However, each button (login, create) has a different `formaction` attribute.
	- Be careful of GET vs POST - get requests encode the form payload in the query, while post requests have it in the body
  - Spying means logging everything the user does
    - Log any navigation by links (Note: **searching counts as navigation**)
	- Log any login/account creation (`user` and `pass` (note: the form is `username` and `password` - don't mix these up!))
	- Log any logout
	- **Attach user to all of the above** if the user is logging in or logged in
    - The spying server doesn't actually exist, but you can check the network tab of devtools to verify you're doing this correctly
  - Stealth
    - You might notice that the url bar has a really suspicious link - the goal of stealth is to hide that
	- When the user navigates, searches, logs in, etc. the url should match what the normal bungle would look like
	- The [MDN History API](https://developer.mozilla.org/en-US/docs/Web/API/History) is very helpful - you can replace the url without navigating

### 2.2.3.3 XSS Defense Level 1: Remove "script"

- No more script! Your payload can't have script in it.
- Does it help that script is replaced only once?
- Note: You should **reuse the same payload for all of these filters** so you can be sure it works for everything

### 2.2.3.4 XSS Defense Level 2: Recursively removing “script”

- This time, recursively remove script! You can't do a sneaky trick anymore.
- How can you do xss without a script tag?
  - Stuck? There's 1000s of resources on xss injection tricks! Just lookup `xss cheat sheet`.

### 2.2.3.5 XSS Defense Level 3: Recursively Removing several tags

- No more img, image, body, style, meta, embed, object
- Use your resources!

### 2.2.3.6 XSS Defense Level 4: Remove some punctuation

- No more semicolons, single quotes, or double quotes
- Is there another character you can use to make strings in JavaScript?
- Can you "encode" your payload so it can have whatever characters you want?

### 2.2.3* Bonus

- Bonus round: make navigating back and forward work with bungle
- Up until now, you've been replacing the history. Now, we need to maintain it.
- It's probably a good idea to look at the [MDN History API](https://developer.mozilla.org/en-US/docs/Web/API/History) again
- Don't overthink this - try a few things out and see what happens!
- You need to emulate the navigation - but you **shouldn't refetch the endpoints on history navigation**
  - Emulate what the real bungle site does - going back/forward doesn't rerun a search for example
- **Don't forget to implement spying** - you should fire a nav event when the user goes back/forward in history
- The bonus for this MP is actually simple once you figure it out - about 5 lines

# Conclusion

You did it! Congratulations on surviving.

c:
