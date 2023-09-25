[EN](https://github.com/Sadalmalik/CRegExp/blob/main/README.md) | [RU](https://github.com/Sadalmalik/CRegExp/blob/main/README.RU.md)
[EN](https://github.com/Sadalmalik/CRegExp/blob/main/README.md) | [RU](https://github.com/Sadalmalik/CRegExp/blob/main/README.RU.md)

# Table of contents
* [Introduction](#introduction)
* [Conclusions](#conclusions)
* [Implementation](#implementation)


# Introduction
This project is my attempt to learn how to use ChatGPT to do some work (in this case, developing a small library). I used the paid Chat GPT-4 from [openai.com](https://chat.openai.com/). I chose "creating a basic implementation of regular expressions in C" as the work task.

Initially, I tried to do all the code through GPT, but after a number of iterations many problems associated with this became obvious, so I changed my approach to using GPT for consulting and reviewing code. As a result, through joint efforts (in the end, only mine), we managed to implement a version of regular expressions that supports the following characters: `. ? * ( )`


# Conclusions
First of all, the most important conclusion is that I chose a task that was not easy for me. On the one hand, perhaps this did not allow me to fully assess the accuracy of some of the GPT answers, but on the other hand, we ended up with an experiment close to real conditions.

As a result, the use of Chat GPT in work has a huge number of nuances and features that place it in the category of not the worst tool.

* First of all, GPT does not have consciousness or understanding as such. He basically doesn’t understand what he’s doing, but he behaves with absolute confidence. Because that's how I was trained.
* He has no abstract imagination - GPT could not imagine in his head (due to its absence) the structure of the parsing tree, the order of states and the meaning of this order. As a result, he confidently produced code that built connections between objects in an absolutely invalid way.
* GPT makes mistakes all the time. And in the most human way possible. And errors begin to multiply over time (due to GPT memory limitations)
   * For example, he produced code with uninitialized variables. It took me half an hour to find the reason for the program crash. And then ask leading questions twice more so that GPT noticed the error and suggests a correction. It does not analyze code like a program. And he doesn’t analyze it like a programmer - he doesn’t make any effort to find and fix problems.
   * This is generally one of the big problems. He makes no effort to solve any problems. It simply generates a response. As a result, he is not able to solve problems without a large number of leading questions that will lead him to the desired answers.
* Besides this, GPT has another “lazy” trait - it always tries to give the most simplified answer possible. This is especially true for coding tasks. I suspect it has something to do with the way language models work in general - it's likely that if you force him to give a longer answer, he'll start going crazy.

In essence, GPT is such a cast of the collective consciousness of the Internet.
A copy of the Hive Mind, imitating its responses in some form, but is not aware of anything and cannot do anything other than speak. In humans, connected speech without consciousness is called Schizophasia :)

My conclusion:
GPT cannot be considered an AI because it does not possess any traits of intelligence.
It's just a black box filled with very smart chatter.
In general, the hype around GPT is extremely overheated and overrated.

However, GPT can be very useful in some tasks.
* First of all, this is a very good method of organizing information search.
   * When a person cannot accurately formulate a question, GPT can provide a number of approximate answer options, which can greatly help a person get off the ground.
* Even though GPT is always wrong - it can be extremely useful for generating ideas


# Implementation
The implementation is in the files *light_regexp.hpp* and *light_regexp.cpp*.

As a way to implement regular expressions, a simple finite state machine was chosen that checked whether the symbol matches the state.

When parsing an expression, a finite state machine state tree is built - a linked list of states with possible nested states. In this case, for efficient implementation, memory for the tree is allocated in one continuous piece of memory, so the entire state machine can be loaded into the processor cache.

Then there is the Match method, which executes this machine.

The output is a TMatch structure containing a success flag and a list of nested groups.

Usage example:
```cpp
#include "light_regexp.hpp"

...

LiRex::TRegex* reg = LiRex::Create("abc(xyz)*");
LiRex::TMatch match = LiRex::Match(reg, "abcxyzxyz");
if (match.success)
{
    int count = match.groups.size();
    printf("Match founded: %s\nGroups: %d", match.match.c_str(), count);
    for (int i=0 ; i<count ; i++)
        std::printf("  %d: '%s'\n", i, m.groups[i].c_str());
}

...

LiRex::Destroy(reg); // Don't forget to delete regex!
```
