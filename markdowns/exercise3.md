# Generic Programming and tuples

**Generic Programming** is a style of programming in which functionalities and components are developed in terms of types that will be known at a later time - when they will be instantiated. Such components and functionalities can be associated with a *constraint*, which specifies the *requirements* on such types.

Generic Programming in C++ is enabled by **templates** which are a powerful mechanism to generate code and take decisions at compile-time:

```cpp
template<typename T>
T sum(T a, T b)
{
	return a + b;
}
```

`sum` works on *any type* supporting `operator+`.

Templates are extensively used across the entire C++ standard library that has been developed with Generic Programming (**Alexander Stepanov**, one of the fathers and advocate of generic programming, is one of the primary designer and implementer of the C++ Standard Template Library).

*Algorithms* are decoupled from *containers* through the generic *iterator* abstraction.

Generic Programming in C++ consists of several techniques, such as:

- type traits
- tag dispatching
- SFINAE
- concepts
- policy-based design
- mixins

The topic is very big and convoluted, deserving a dedicated workshop (or more than one).

At the end of this section, we'll face with a challenge about generic programming and tuples.

## Tuples

In type theory, when we create new types, we *compose* other types together. There are two main types of combinations: **products** and **sums**. The result of such a composition is called **Algebraic Data Type** (*ADT*).

A **product** is a way to combine multiple types into one (e.g. `struct`, `tuple`) and it's probably the most common type of composition of types:

Example:

```cpp
struct Record
{
    std::string name;
    int age;
    bool checkIn;
};
```

On the other hand, a **sum** holds a value that could take on several different, but fixed, types (e.g. `variant`).

Example:

```cpp
using Move = std::variant<Rock, Paper, Scissors>;

Move player1 = Rock; // commit to Rock
Move player2 = Paper; // commit to Paper

play(player1, player2);
```

A **tuple** belongs to the former category: it is a fixed-size and ordered collection of heterogeneous values - a product type.

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

Example of pattern matching on `std::variant`s:

```cpp
struct Paper{};
struct Rock{};
struct Scissors{};

using Move = variant<Paper, Rock, Scissors>;

struct Game
{
    int operator()(Paper, Rock) const { return 1; }
    int operator()(Rock, Paper) const { return 2; }
    int operator()(Rock, Scissors) const { return 1; }
    int operator()(Scissors, Rock) const { return 2; }
    int operator()(Scissors, Paper) const { return 1; }
    int operator()(Paper, Scissors) const { return 2; }
    
    template<typename T1, typename T2>
    int operator()(T1, T2) const 
    { 
        return 0; // deuce 
    }
};

int main()
{
    Move m1 = Scissors{};
    Move m2 = Paper{};
    Game g;
    cout << visit(g, m1, m2);
}
```

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

Structure bindings serves to destructure a tuple-like object into its components. Many times we need to do the opposite operation: make a tuple from some unstructured data. Packing data into a tuple opens doors to use tuple's interface on such data.

Since creating a tuple involves creating a new object, possibly we do not want to copy data. We would like to *reference* such data, as we do with structure bindings. That's a job for `std::tie`: a function which creates a `std::tuple` of **references** to the parameters we provide:

```cpp
int r, g, b;
float a;
auto tRefs = tie(r, g, b, a); // aka: tuple<int&, int&, int&, float&>
```

Once we have a tuple, we can use any of it supported functions.

For example, we can use `operator=` for a member-wise assignment:

```cpp
tuple<int, int, int, float> ReadRGBA();

int r, g, b;
float a;
tie(r, g, b, a) = ReadRGBA(); // r, g, b, a are assigned in order
```

Differently from structure bindings, we can `std::ignore` fields we do not want to reference:

```cpp
int r, g, b;
tie(r, g, b, std::ignore) = ReadRGBA(); // I don't need alpha
```

Another common idiom consists in using `tie` to implement **lexicographical comparison** (e.g. `operator<`):

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

## `tuple` and Generic Programming

`tuple` is very important for metaprogramming in C++. It's a statically-sized list of heterogenous values. Since the structure of a tuple is known at compile-time, many interesting things can be done.

First of all, even though `tuple` has values, it can be used as a simple *list* of types. Every metaprogramming library provides this kind of concept (generally referred as *typelist*).

Another important role of `tuple` comes with *variadic templates*: if you want to store and pass around a *variadic pack*, you can use a std::tuple:

```cpp
template <typename... T>
struct Packer
{
  Packer(T... vals) 
    : values(vals...)
  {
  }
 
private:
  std::tuple<T...> values;
};
```

Once you have packed your values into `tuple`s, it's like having a "list" at compile-time. You can do manipulations, concatenation, search, etc, and the operations are not limited to what the language provides: as usual, we have a very sophisticated generic programming environment where we can craft our own functions and code generators.

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

## Tuples unpacking - Hands on!

Let's get our hands dirty just by facing a final - easy - challenge involving both tuples and generic programming.

As we discussed, `tuple` is an important brick for both generic and business code. We are about to see an interesting use case that involves `tuple` and one standard function recently added to the library.

You met James - a template expert - at the coffee machine and discussed about invoking lambdas on tuples. James told you he has implemented a utility to improve the way to call lambdas on tuple-like objects. Usually, we have:

```cpp
vector<tuple<string, int, int>> v = ...;

find_if(begin(v), end(v), [](auto& t) {
	return get<1>(t) == 20;
});
```

The parameter `t` does not really say anything about its content. Instead, James is implementing *something* to enable a richer syntax:

```cpp
vector<tuple<string, int, int>> v = ...;

find_if(begin(v), end(v), unpack_to([](auto name, auto urls, auto clicks) {
	return age == 20;
});
```

You are intrigued by James' utility and you decide to roll your own implementation. Luke, a friend of you, has some insights for you and left a comment in the code.

Can you implement `unpack_to`?

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

At home, if you feel brave, try implementing your own `std::apply`!

:::