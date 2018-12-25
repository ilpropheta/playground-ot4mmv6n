# Generic Programming and tuples

## Tuples

An **algebraic data type** is a data type defined out of a combination of two constructions: **products** and **sums**. Thus, you can think of an *ADT* as a composite type, formed by combining other types.

A **product** is a way to combine multiple types into one (e.g. `struct`, `tuple`).

A **sum** holds a value that could take on several different, but fixed, types (e.g. `variant`).

A **tuple** is a fixed-size and ordered collection of heterogeneous values - a product type.

In C++, tuples are supported at the library level - not in the language itself:

```cpp
#include <tuple>


std::tuple<string, int, bool> checkedIn = {"marco", 31, true};
```

Since in C++ tuples are not named (e.g. anonymous), they constitute a sort of *lingua franca* for structured data - e.g. you can pack your data into a tuple and process it with a uniform interface. For this reason, tuples are also an important building block of *generic programming* in C++. For example, tuples are the simplest form of static "list of types" and can be used for several purposes in libraries and also business code.

Operations on *ADT*s can be defined by using **pattern matching**:

```cpp
template<typename Info>
void process(tuple<int, Info> ints);

template<typename Info>
void process(tuple<string, Info> strings);


tuple<string, Record> entry = LoadString();
process(entry); // string

tuple<int, Record> entry = LoadInt();
process(entry); // int

tuple<double, Record> entry = LoadDouble();
process(entry); // do not compile
```

**Pattern matching** is a powerful tool that is widely used in **functional**, **declarative** and **generic** programming.

## Structure bindings

C++17 introduces the possibility to decompose tuple-like objects into **references** to their componenents:

```cpp
tuple<int, int, string> make();
auto [a, b, c] = make();

/* merely equivalent to:
auto __tmp = make();
auto& a = get<0>(__tmp);
auto& b = get<1>(__tmp);
auto& c = get<2>(__tmp);
*/
```

It works with **destructurable** types:

* tuple-like types
* arrays
* public data members

For instance:

```cpp
array<int, 2> arr = {1, 2};
auto [a, b] = arr;

/* merely equivalent to:
auto __tmp = arr;
auto& a = __tmp [0]; // get<0>(__tmp )
auto& b = __tmp [1]; // get<1>(__tmp )
*/
```

And:

```cpp
struct Rgb
{
    int r, g, b;
    float alpha;
};

Rgb rgb = ...;
auto [r, g, b, a] = rgb;
```

More specifically, a type is destructurable if:

* either all non-static data members: 
   * must be public
   * must be direct members of the type or members of the same public base class of the type
   * Cannot be anonymous unions
* or, it has: 
   * an `obj.get<>()` method or an ADL-able `get<>(obj)` overload
   * specializations of `std::tuple_size<>` and `std::tuple_element<>`

**Important detail**: The portion of the declaration preceding `[` applies to the hidden variable `e`, not to the introduced identifiers.

```cpp
int a = 1, b = 2;
const auto& [x, y] = std::tie(a, b); // x and y are of type int&
auto [z, w] = std::tie(a, b);        // z and w are still of type int&
assert(&z == &a);                    // passes
```

Continue Reading:

- [Reference](http://en.cppreference.com/w/cpp/language/structured_binding)
- [Adding C++17 structured bindings support to your classes](https://blog.tartanllama.xyz/structured-bindings/)

## `std::tie` idiom

We have met structured bindings and understood how it works, more or less. Before putting our hands on the code, let's meet another important tool of post-modern C++: `std::tie`.

`std::tie` is a function which creates a `std::tuple` of **references** to the parameters we pass it:

```cpp
tuple<int, int, int, float> ReadRGBA();

int r, g, b;
float a;
tie(r, g, b, a) = ReadRGBA(); // r, g, b, a are assigned
```

The example above is a compact way to assign multiple values from an expression. As you can imagine, this is really similar to structured bindings but it covers the case when we already have variables and we just want to assign them.

In addition, we can `std::ignore` uninteresting fields:

```cpp
int r, g, b;
tie(r, g, b, std::ignore) = ReadRGBA(); // I don't need alpha
```

In general, `std::tie` is useful when we need to create a *light* tuple on the fly and use tuple's capabilities.

A common idiom consists in implementing **lexicographical comparison**:

```cpp
struct S 
{
    int n;
    std::string s;
    float d;
    
    bool operator<(const S& rhs) const
    {
        // compares n to rhs.n,
        // then s to rhs.s,
        // then d to rhs.d
        return std::tie(n, s, d) < std::tie(rhs.n, rhs.s, rhs.d);
    }
};
```

This very effective idiom could save time and make the code simpler. Try writing the same by hand and compare the result with the snippet above.

## Hands on!

Let's get familiar with these concepts.

Your collegue Pete is obsessed with making tests clearer and more compact, and you have decided to support his cause by applying structured bindings to a working test.

You have a pending refactoring. It's time to get back and finish your work:

@[Use structure bindings]({"stubs": [
	 "microurl/src/ver4/tests/UrlInfoTest.cpp",
	 "microurl/src/ver4/MicroUrlInfo.h",
	 "microurl/src/ver4/MicroUrlService.h"
	],
	"command": "sh /project/target/run_test.sh ver4 [binding]"})

A very important client of our company is asking a new feature: she wants to sort shortened urls in some way. Can you implement a simple `operator<` for this purpose? Instead of specs, she provided a test:

@[Implement client's operator<]({"stubs": [
	"microurl/src/ver4/tests/UrlInfoCompareTest.cpp",
	"microurl/src/ver4/MicroUrlInfo.h",
	],
	"command": "sh /project/target/run_test.sh ver4 [less]"})

::: Do you really give up? :(

Using structure bindings on `UrlInfoTest.cpp`:

```cpp
auto[original, micro, clicks] = service.Stats(microUrl.c_str());
REQUIRE (original == urlToShorten);
REQUIRE (micro == microUrl);
REQUIRE (initStat == 0);
```

The client wants an `operator<` which is just a lexicographical comparison with the following order:

1. `OriginalUrl`
2. `Clicks`
3. `MicroUrl`

We could use the `tie` idiom:

```cpp
inline auto AsClientTuple(const UrlInfo& info)
{
	return std::tie(info.OriginalUrl, info.Clicks, info.MicroUrl);
}

inline bool operator<(const UrlInfo& left, const UrlInfo& right)
{
	return AsClientTuple(left) < AsClientTuple(right);
}
```
:::

## Generic Programming

**Generic Programming** is a style of programming in which functionalities and components are developed in terms of types that will be known at a later time - when they will be instantiated.

Generic Programming in C++ is enabled by **templates** which are a powerful mechanism to generate code and take decisions at compile-time:

```cpp
template<typename T>
T sum(T a, T b)
{
	return a + b;
}
```

`sum` works on *any type* supporting `operator+`.

Templates are extensively used across the entire C++ standard library.

Generic Programming in C++ consists of several techniques, such as:

- type traits
- tag dispatching
- SFINAE
- concepts
- policy-based design
- mixins

The topic is very big and convoluted, deserving a dedicated workshop (or more than one).

Let's get our hands dirty just by facing a final - easy - challenge involving both tuples and generic programming.

At the coffee machine, James - a template expert - threw down the gauntlet: you have to implement `unpack_to` properly.

Luke, a friend of you, left a message to help you:

@[Implement unpack_to]({"stubs": [
	"microurl/src/ver4/tests/UnpackTest.cpp",
	],
	"command": "sh /project/target/run_test.sh ver4 [unpack]"})
	
::: Do you really give up? :(

[`std::apply`](https://en.cppreference.com/w/cpp/utility/apply) invokes a callable by decomposing a tuple into its arguments.

The challenge is basically about creating a simple wrapper on top of `std::apply`:

```cpp
template<typename F>
struct unpack_to : F
{
	unpack_to(F f) : F(f) {}

	template<typename Arg>
	auto operator()(Arg&& arg) const
	{
		return std::apply(*static_cast<const F*>(this), std::forward<Arg>(arg));
	}
};
``` 

Or:

```cpp
template<typename F>
struct unpack_to
{
	unpack_to(F f) : m_f(f) {}

	template<typename Arg>
	auto operator()(Arg&& arg) const
	{
		return std::apply(m_f, std::forward<Arg>(arg));
	}
private:
	F m_f;
};

``` 
:::