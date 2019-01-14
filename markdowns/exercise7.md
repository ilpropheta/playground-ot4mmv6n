# Wrap up

![Congratulations!](https://upload.wikimedia.org/wikipedia/it/thumb/4/46/Dicapriogatsby.JPG/1200px-Dicapriogatsby.JPG)

You have done a great job! I hope you have enjoyed the session.

## The best C++ programmer can adapt on any style

In my opinion, "adapting on any style" in C++ has two meanings. 

First of all, we *express our own C++ style* by mixing idioms and paradigms. We are not tied with any specific paradigm. We are not afraid of using together concepts diametrically opposed to one another. We should do it responsibly though. **We** here is not limited to you. Can be your team, your company, or whatever. During the workshop, we have mixed together some concepts coming from Object Oriented, Functional and Generic Programming.

Second one is *Adaptability*. We should be ready to *adapt* to different scenarios and use-cases. We do not have strict and universal guidelines to follow. Many times we should just swing and balance competing tradeoffs. We have enough freedom and control. 

It's also about how we approach to the C++ growth: we should know how new things work and their impact on the current state of the art - like we did by reviewing the classic idiom to make an *interface*. We need a *vigilant approach*. 

If we decide not to learn how new things work, we'll have troubles when they will show up - if we change team or company, for instance. Don't die like Chuck Norris at the Colosseum!

Bruce Lee advocated the idea of "being fluid and flexible like water". You probably know his famous expression "be water my friend". This idea comes from [Taoism](https://en.wikipedia.org/wiki/Taoism):

> Water is fluid, soft, and yielding. But water will wear away rock, which is rigid and cannot yield. As a rule, whatever is fluid, soft, and yielding will overcome whatever is rigid and hard. This is another paradox: what is soft is strong - Lao Tzu

As in life, what works now might not work for us forever, and it's important to have awareness to change and grow.

Clearly **we have very mature and wise idioms**. Thus many times our decisions are seamless, like being in "autopilot mode". The more we *practice*, the more fluent we are. Some concepts we have met today have been growing up in the C++ ecosystem for decades. Yet some people do not know them. Don't join such group of people.

Whenever new tools and concepts are merged into the standard, we also have an opportunity to ditch our own implementations of the same things. **Adding enables removing** as Kate Gregory once said. Time spent for **removing unnecessary things** and for **simplifying** is always worth. Bruce Lee was used to say "it is not daily increase but daily decrease, hack away the unessential". In C++, this is not only about removing complexity - like applying *the Rule of Zero* - but also about embracing new abstractions which enable us to return to the essence of basic concepts - like `string_view`.

On the other hand, we have more debatable things and additions, those require harder thinking and decisions. For instance, choosing to handle errors with `std::optional` is not so obvious compared to other ways. Which is better? Sometimes the answer just *depends on* the context. We should be **open** to take in consideration the *new*.

To conclude, it's all about a *personal search* - where "personal" can be refer to your company, team or whatever, if you want to apply it more pragmatically to professional C++ development. As Bruce Lee was used to summarize his [Jeet Kune Do](https://en.wikipedia.org/wiki/Jeet_Kune_Do):

```
Research your own experience.
Absorb what is useful.
Reject what is useless.
Add what is specifically your own.
```

"Add what is specifically your own" means that things sometimes need "customization". Strict rules and discipline are just rigid "endings". Sometimes we just cannot accept the *state of the art*. We need to define our own *state of the art*.
Do we need a special smart pointer? Let's go for it! With C++, we have enough freedom and *control* to go beyond the limits. However we should remember that "a great C++ comes with a great responsibility". But that's another story... :)