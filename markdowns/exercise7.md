# Wrap up

![Congratulations!](https://upload.wikimedia.org/wikipedia/it/thumb/4/46/Dicapriogatsby.JPG/1200px-Dicapriogatsby.JPG)

You have done a great job! I hope you have enjoyed the workshop.

The best is yet to come: you should delve into the topics you liked the most and practice a bit more.

> Only a sith lord deals in absolutes

After this workshop I hope you understood that C++ is not about a single paradigm. C++ does not have absolute rules though, we can just try narrowing down the range of alternatives with openness to new tools and with balancing compromises. As Bruce Lee was used to say: *the best fighter is someone who can adapt on any style*, we can reshape as *the best C++ programmer is someone who can adapt on any paradigm*. In my opinion, the new generation of C++ is about expressing C++ through a mixture of styles, idioms and paradigms.

Some concepts we met have been growing up in the C++ ecosystem for decades, meaning that we have many strong guidelines to apply them. For example the **RAII** idiom, a general-purpose rule for managing any kind of dynamic resources.
Other concepts are more debatable and open to balancing compromises, and often they imply to use a certain style. For instance, `std::optional`, that has been in the industry (and in many other languages) for many years but it is a competitor of other styles and ways to handle errors that may be equivalent.

Let's recap the topics we have met so far:

* **RAII** is probably the most important idiom in C++ and **smart pointers** are general-purpose tools implementing such idiom
   * the general rule is: don't let plain pointers own resources;
* **Interfaces** are powerful building block of object oriented programming. Although the support for interfaces in C++ is not as explicit as in other languages, you should learn the idioms to build them correctly and safely. In addition, you could use smart pointers to effectively craft **factory functions**;
* **tuples** and **structure bindings** are useful to manipulate structured data under a generic and common interface. Along the way, you have learnt `std::tie` idiom to easily and efficiently pack any data into a - light - tuple of references;
* `std::string_view` is a powerful tool for **referencing** sequences of chars as `std::string`-like readonly objects;
* the introduction of **lambdas** revolutionized how we write code in C++, with the first effect of empowering the usage of STL algorithms ("no more excuses"). In addition, it opens to scenarios where functional programming could be a good fit. We cannot really speak of "functional programming" but it's a very good start;
* we have many ways and paradigms for handling errors in C++ (and, generally, in software). `std::optional` is a "monadic" way for encapsulating errors and missing values. It's a *vocabulary type*, meaning that it's the default way to express *nullable value semantics*.

See you next time!