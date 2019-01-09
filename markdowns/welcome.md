# The best C++ programmer can adapt on any style

## Welcome!

Welcome to my workshop "The best C++ programmer can adapt on any style", hosted by [tech.io](https://tech.io)!

This playground is an interactive environment where you can face with challenges directly on your browser.

This workshop is an improvement version of my [previous one](https://tech.io/playgrounds/098a2aa83ce82c5ad96efacc3ba2734b02122/).

## Introduction

![Congratulations!](https://raw.githubusercontent.com/ilpropheta/playground-ot4mmv6n/master/bruce-lee-quote.png)

I think that if *Bruce Lee* was alive, he would have loved C++!

Bruce Lee was used to say:

> The best fighter is not a Boxer, Karate or Judo man. The best fighter is someone who can adapt on any style.

Bruce Lee's quote has two meanings: first of all, Bruce Lee could **not accept to obey to a single discipline or style** because any discipline or teaching is "conclusive" - all the answers have been already answered. What works for you can work differently for me. You should always *express yourself* and for this reason you should not limit yourself into a single doctrine. That's why for Bruce Lee was natural just to mix several styles and arts into his own *expression of the human body*. For instance, did you know that Bruce Lee was a professional dancer?

C++ implements this idea very well, since it is **independent from any particular programming paradigm or style**. You can design object oriented applications with a bit of functional touch, crunch data from hardware very quickly through C-like interfaces, craft sophisticated generic libraries, make compile-time code generators, and so on.

The second aspect of his quote is about **adaptability**: for Bruce Lee, life is a constant process of changing. What works now might not work forever. If you do not adapt to change, you are going to have troubles. Bruce Lee represented this concept in his famous fighting with *Chuck Norris* at the Colosseum (*The Way of the Dragon*): Chuck Norris was winning the match so Bruce Lee adapted to the style of his opponent and changed something in his movements. Chuck did not and for this reason he died.

C++ grows very quickly. New tools and concepts are merged into the standard once every 3 years now. This requires a *vigilant* approach to C++ development. You should not ignore such changes because you might be involved in projects with people who use recent additions. Or you might just lose opportunities to simplify your code. You should be ready to **adapt** and **adjust** to change, conveniently.

In this interactive session, we will put our hands on a simple project where we will work on several aspects by applying different paradigms and idioms.

## Playground structure

Everything is self-contained and embedded in this **playground**. Under the hood, this environment is generated from this [github repo](https://github.com/ilpropheta/playground-ot4mmv6n).

You will work on different parts of a toy project that I have developed for this workshop.

Incremental versions of the project are deployed as successive sections of the playground so you'll never get behind! Everything is set up with CMake, so you can even build the [full repo](https://github.com/ilpropheta/playground-ot4mmv6n) on your machine and work "offline".

## Tools

This workshop will show you some C++11, C++14 and C++17 features. Offline, I have tested the codebase with Visual Studio 2017 15.7. Online, **GCC 7.2** runs under our playground.

### Catch

Behind the scenes, this codebase has a few unit tests made with [Catch](https://github.com/catchorg/Catch2). In case a test fails you will se the full output.

## MicroUrl, an url shortener toy project

You will dress the part of an enthusiastic C++ programmer working at **Gugol**, a software giant leading the industry.

**Gugol** has just acquired **MicroUrl** from **NeverCroSoft**, a revolutionary url shortner service which will disrupt the digital market and will be next **Gugol**'s top product.

**Gugol** CIO has great plans for **MicroUrl** but the codebase seems a bit old-fashioned and before digging into Quantum Computing Support and Blockchain protocols he wants you to improve some bits.

# Getting familiar with the environment

This first excercise is just to stretch your hands and let you explore the environment.

## Hands on!

**MicroUrl** is quite old and it gets back plain HTTP urls. Your boss heard that Google Chrome - the fierce competitor of **Gugol GROM** - will mark all HTTP sites as 'not secure' starting in July and he asked you to fix such problem.

Fix the code and make the test pass:

@[Fix MicroUrlService.cpp]({"stubs": [
    "microurl/src/ver1/tests/NiceToMeetYou.cpp",
    "microurl/src/ver1/MicroUrlService.cpp", 
    "microurl/src/ver1/MicroUrlService.h"],"command": "sh /project/target/run_test.sh ver1"})