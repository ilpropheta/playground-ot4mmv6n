# `std::string`, `const char*`, `QString`, `CString`... oh my!

> Before learning C++, when I was asked to define a string, for me it was just a sequence of characters. When I started learning C++ and putting my hands on real-world codebases, I encountered several different string types like `std::string`, `QString`, `CString`, etc. A string was no longer just a sequence of characters. Finally, after some years of practicing with strings, I found `string_view` and a string returned to be just a sequence of characters. A string returned to be just a string.

*Bruce Lee* about his experience with strings in C++.

(joke inspired by Bruce Lee's [Three stages of cultivation](https://www.brucelee.com/podcast-blog/2017/6/21/51-the-three-stages)).

## `std::string_view` in a nutshell

`string_view` gives us the ability to refer to an existing string in a non-owning way.

An instance of the `string_view` class can be thought of as a "view" into an existing character buffer. Specifically, a `string_view` consists of only a pointer and a length, identifying a section of character data that is not owned by the `string_view` and cannot be modified by the view. Consequently, making a copy of a `string_view` is a shallow operation: no string data is copied.

`string_view` has implicit conversion constructors from both `const char*` and `const string&`, and since `string_view` doesn’t copy, there is no O(n) memory penalty for making a hidden copy. In the case where a `const string&` is passed, the constructor runs in O(1) time. In the case where a `const char*` is passed, the constructor invokes a `strlen()` automatically (or you can use the two-parameter `string_view` constructor).

`string_view` describes the minimum common interface necessary to read string data. It's a return to the **basic essence** of a *string* as a sequence of characters. It doesn’t require that the data be null-terminated, and doesn’t place any restrictions on the data’s lifetime. This gives you type erasure for "free", as a function accepting a `string_view` can be made to work with any string-like type, without making the function into a template, or constraining the interface of that function to a particular subset of string types.

`string_view`'s interface recalls `const string`'s. For example:

```cpp
string_view sv = "this is a string view";
sv = sv.substr(0, 4); // sv is "this"
```

The snippet above does not allocate data, instead it just returns a new string_view.

Many operations can be performed just by narrowing the view on the original string. Here is how we can left-trim:

```cpp
string_view sv = "    trim me";
sv.remove_prefix(std::min(sv.find_first_not_of(" "), sv.size())); // sv is "trim me"
```

Remember that `string_view` **is not required to be null-terminated**.

### Cheap abstractions

Although `string_view` is a very old concept in the C++ ecosystem, it's been officially introduced since C++17.

I have added it to the workshop not only because it's important and useful, but especially because `string_view` will change the way we write C++ code. It's a new *paradigm*. Like `auto`, *smart pointers* or *lambdas* - and in C++20 [`span`](https://en.cppreference.com/w/cpp/container/span) will join the party.

`string_view` is an example of what we mean in C++ with the expression **0-overhead high level abstractions**. We have a high level abstraction on top of a raw buffer of characters (often referred to as *C-style buffer*, a term more for **Systems Programming** than for Application-level programming).

Other languages have been imitating this style of programming, gaining amazing performance. For instance, have a look at [this article on .net core](https://medium.com/@antao.almada/how-to-use-span-t-and-memory-t-c0b126aae652).

Continue reading:

* [C++17 string_view](https://skebanga.github.io/string-view/)
* [std::string_view: The Duct Tape of String Types](https://blogs.msdn.microsoft.com/vcblog/2018/08/21/stdstring_view-the-duct-tape-of-string-types/)
* [string_view odi et amo](https://marcoarena.wordpress.com/2017/01/03/string_view-odi-et-amo/)
* [Tip of the Week #1: string_view](https://abseil.io/tips/1)

## Hands on!

Jane, our Chief Marketing Officer, has just came back from *CppCon* where she heard about `std::string_view` during a coffee break. She is keen on selling to our investors that **Gugol** is already using such C++17 feature into our codebase.

After all, the request is not so silly. The service's interface seems a bit inconsistent: it takes `const char*` but in `Stats` that takes `const string&`. `std::string_view` could be really a good fit to improve the interface and also to avoid copying...

Work on `MicroUrlService` and uniform the parameters of its public functions. Decomment some lines in `StringViewTest.cpp` and make it work.

@[Replace const char* with std::string_view on our service's interface]({"stubs": [
	 "microurl/src/ver5/MicroUrlService.h",
	 "microurl/src/ver5/MicroUrlService.cpp",
	 "microurl/src/ver5/tests/StringViewTest.cpp",
	 "microurl/src/ver5/tests/SubtleTest.cpp",
	],
	"command": "sh /project/target/run_test.sh ver5"})

### On blindly replacing `const string&` with `string_view`

Why did `SubtleTest.cpp` fail?

In general [this guideline](https://abseil.io/tips/1) is insightful:
	
If your API only needs to reference the string data during a single call, and doesn’t need to modify the data, accepting a `string_view` is sufficient. If you need to reference the data later or need to modify the data, you can explicitly convert to a C++ string object.

Adding `string_view` into an existing codebase is not always the right answer: changing parameters to pass by `string_view` can be inefficient if those are then passed to a function requiring a string or a NUL-terminated `const char*`. It is best to adopt `string_view` starting at the utility code and working upward, or with complete consistency when starting a new project.

For the rest of the workshop, we assume that the underlying API will be changed to use `std::string_view` instead of `const char*` and then such a problem won't happen anymore.

::: Do you really give up? :(

Using `std::string_view` is as easy as using `std::string`. For example:

```cpp
std::string MicroUrlService::ClickUrl(std::string_view microUrl)
{
	auto secret = microUrl.substr(microUrl.find_last_of('/') + 1);
	auto& url = m_idToUrl[Ext::Shortener::shortURLtoID(secret.data())];
	url.Clicks++;
	return url.OriginalUrl;
}

```

Remember this simple idiom to construct a `std::string` from `std::string_view`:

```cpp
std::string_view strView = ...;
std::string str {strView.data(), strView.size()};
```

It's very common to have this utility somewhere in our support libraries:

```cpp
std::string to_string(std::string_view sv)
{
    return {sv.data(), sv.size()};
}
```
:::
	
## Bonus: avoid temporary strings in map lookups

C++14 introduced **transparent comparators** to perform **heterogeneous lookup** on associative containers using keys that are not necessarily the same as the associative container key type.

For example:

```cpp
struct Book
{
    string title;
    string author;
    int id;
};

struct BookComparator
{
    bool operator()(const Book &x, const Book &y) const
    {
        return x.title < y.title; // we just use title
    }
};

set<Book> library = {...};

library.find({"title", "", ""});
```

It would be better to pass only the title, wouldn't it?

In C++14 we can turn `BookComparator` into a transparent comparator just by declaring a type `is_transparent`:

```cpp
struct BookComparator
{
    typedef void is_transparent;

    bool operator()(const Book &x, const Book &y) const
    {
        return std::tie(x.title, x.author) < std::tie(y.title, y.author); // std::tie idiom
    }

    bool operator()(const Book &x, string_view title) const
    {
        return x.title < title;
    }
    
    bool operator()(string_view title, const Book &y) const
    {
        return title < y.title;
    }
    
    // other comparisons, as needed
};

library.find("title");
```

In the C++ library, `std::less<void>` or simply `std::less<>` (from C++14) is special: it is a specialization of `std::less` with parameter and return type deduced and it has `is_transparent` declared. This comparator automatically enables `operator<` comparisons among different types, when supported. 

Continue Reading:

[`is_transparent`: How to search a C++ set with another type than its key](https://www.fluentcpp.com/2017/06/09/search-set-another-type-key/)

### Hands on!

IT is worried because the service accepts too many requests per second thus the head of IT decided to implement a very simple load balancing strategy to split the work among several service instances. You know it cannot scale but you decide to help optimize it a bit, in the meantime some people of your team will develop a better strategy.

The load balancing strategy is very naive. Basically, the job is sent to a certain instance of the service depending on the first letter of the url. Since IT people regularly attend [Coding Gym](https://coding-gym.org), they know this lookup can be implemented very easily with `std::map`. The only problem is that their function takes `std::string_view`, triggering a conversion to `std::string` for every lookup.

Can you help them avoid such useless conversion? Accommodate `LoadBalancer` here below:

@[Lookup map of strings by using string_view]({"stubs": [ 
	 "microurl/src/ver5/tests/LoadBalancerTest.cpp",
	],
	"command": "sh /project/target/run_test.sh ver5 [lb]"})

::: Do you really give up? :(

In the exercise above, `std::less<>` can be used to lookup an instance of `std::string_view` without incurring in creating a temporary `std::string`:

```cpp
//...
    
std::string_view MicroUrlServiceIpFor(std::string_view s)
{
	auto it = PrefixToIp.lower_bound(s); // now takes string_view directly
	return std::prev(it)->second;
}
private:
                           // look here ---v
std::map<std::string, std::string, std::less<>> PrefixToIp;
```

:::