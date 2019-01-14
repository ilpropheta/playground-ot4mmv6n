# Wrap up

![Congratulations!](https://upload.wikimedia.org/wikipedia/it/thumb/4/46/Dicapriogatsby.JPG/1200px-Dicapriogatsby.JPG)

You have done a great job! I hope you have enjoyed the session.

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

> Only a sith lord deals in absolutes

I hope you have got my point: **C++ is not about a single paradigm**. C++ does not have strict style rules either. This is both good and bad ("a great C++ comes with great responsibility"). You can just try narrowing down the range of alternatives with openness to new tools and with balancing compromises. As Bruce Lee was used to say: *the best fighter is someone who can adapt on any style*, we can reshape as *the best C++ programmer is someone who can adapt on any style*. 

## The best C++ programmer can adapt on any style

In my opinion, "adapting on any style" in C++ has two meanings. 

First of all, we *express our own C++ style* by mixing idioms and paradigms. We are not tied with any specific paradigm. We are not afraid of using together concepts diametrically opposed to one another. We should do it responsibly though.

Second, we are ready to adapt to different scenarios and use-cases. We have enough freedom and control. 

Moreover, adaptability is about how we approach to C++: we can reject certain idioms and tools, but we should know how they work. We need a *vigilant approach*. Otherwise, we'll have troubles in the future when we'll meet them in action or when they twist the meaning of what we already know. This is probably a simple point that holds in any discipline. It's just worth reiterating it. Don't die like Chuck Norris at the Colosseum!

About *adaptability*, Bruce Lee advocated the idea of "being fluid and flexible like water". You probably know his famous expression "be water my friend". This idea comes from [Taoism](https://en.wikipedia.org/wiki/Taoism):

> Water is fluid, soft, and yielding. But water will wear away rock, which is rigid and cannot yield. As a rule, whatever is fluid, soft, and yielding will overcome whatever is rigid and hard. This is another paradox: what is soft is strong - Lao Tzu

As in life, what works now might not work for us forever, and it's important to have awareness to change and grow.

**Adapting in C++ is mandatory**. We do not have strict and universal guidelines to follow. Many times we should just swing and balance competing tradeoffs.

Clearly **we have very good and wise idioms**. Thus many times our decisions are seamless, like being in "autopilot mode". The more we *practice*, the more fluent we are. Some concepts we have met today have been growing up in the C++ ecosystem for decades. Yet some people do not know them. Don't join such group of people.

Whenever new tools are merged into the standard, we have an opportunity to ditch our own implementations of the same concepts and constructs. **Adding enables removing** as Kate Gregory once said. This requires a *vigilant and responsible approach*. Time spent for **removing unnecessary things** and for **simplifying** is always worth. Bruce Lee was used to say "it is not daily increase but daily decrease, hack away the unessential".

Other concepts are more new and debatable, those require balancing compromises and often imply to commit to a certain style. For instance, `std::optional`, that has been in the industry (and in many other languages) for many years is a competitor of other styles and ways to handle errors. What is better? Sometimes the answer *depends on* the context. We should be **open** to *take in consideration* new styles, idioms and tools. As Bruce Lee was used to summarize his [Jeet Kune Do](https://en.wikipedia.org/wiki/Jeet_Kune_Do):

```
Research your own experience.
Absorb what is useful.
Reject what is useless.
Add what is specifically your own.
```

"Add what is specifically your own" means that things sometimes need "customization". Strict rules and discipline are just rigid "endings". Sometimes we just cannot accept the *state of the art*. We need to define our own *state of the art*.
Do we need a special smart pointer? Let's go for it! With C++, we have enough freedom and *control* to go beyond the limits. However we should remember that "a great C++ comes with a great responsibility". But that's another story... :)