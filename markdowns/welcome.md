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

C++ implements this idea very well, since it is **independent from any particular programming paradigm or style**.

You can design object oriented applications with a bit of functional touch, crunch data from hardware very quickly through C-like interfaces, craft sophisticated generic libraries, make compile-time code generators, and so on.

In my opinion, the *next generations* of C++ programmers should not be afraid of learning and using idioms and constructs coming from any paradigm and style. Since software and programming is constantly evolving, C++ programmers should practice **adapting** and **adjusting** to change, conveniently.

In this workshop we'll put our hands on a simple project applying different paradigms and idioms.

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