# Wrap up

![Congratulations!](https://upload.wikimedia.org/wikipedia/it/thumb/4/46/Dicapriogatsby.JPG/1200px-Dicapriogatsby.JPG)

You have done a great job! I hope you have enjoyed the workshop.

The best is yet to come: you should delve into the topics you liked the most and practice a bit more.

> Only a sith lord deals in absolutes

After this workshop I hope you have understood my point: **C++ is not about a single paradigm**. C++ does not have absolute rules though, we can just try narrowing down the range of alternatives with openness to new tools and with balancing compromises. As Bruce Lee was used to say: *the best fighter is someone who can adapt on any style*, we can reshape as *the best C++ programmer is someone who can adapt on any paradigm*. 

## Adopt or adapt

In my opinion, in C++ "adapting on any paradigm" means two things: first of all, you can *express your own C++ style* through your mixture of styles, idioms and paradigms. Second, you know how to adapt to different "enemies": if you decide not to use certain C++ tools is ok, but you should know how they work or you'll have troubles in the future. This is probably a simple point that holds in any discipline. It's just worth reiterating.

Some concepts we met have been growing up in the C++ ecosystem for decades but some people do not know them yet.

Other concepts are more new and debatable, open to balancing compromises and often they imply to use a certain "style". For instance, `std::optional`, that has been in the industry (and in many other languages) for many years but it is a competitor of other styles and ways to handle errors.

## Recap

Let's recap the topics we have met so far:

* **RAII** is probably the most important idiom in C++ and **smart pointers** are general-purpose tools implementing such an idiom
   * the general rule is: don't let plain pointers own resources;
* **Interfaces** are powerful building blocks of object oriented programming. Although the support for interfaces in C++ is not as explicit as in other languages, you should learn the idioms to build them correctly and safely;
* **Generic Programming** is a fundamental paradigms in C++, enabled by **templates** and used to generate code at compile-time;
* **tuples** are useful to manipulate structured data under a generic and common interface;
* `std::string_view` is a powerful tool for **referencing** any char sequence as an immutable `std::string`. It open doors to new styles of programming (cf. [.net core's Span and Memory](https://medium.com/@antao.almada/how-to-use-span-t-and-memory-t-c0b126aae652));
* the introduction of **lambdas** revolutionized how we write code; 
* we have many ways for handling errors in C++ (and, generally, in programming). `std::optional` is a "monadic" way for encapsulating errors and missing values. It's a *vocabulary type*, meaning that it's the default way to express *nullable value semantics*.

See you next time!