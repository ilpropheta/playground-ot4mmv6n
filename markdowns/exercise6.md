# Handling errors

Currently, `ClickUrl` and `Stats` do not complain when receiving invalid urls. We can assume they have a precondition: only well-formed urls are admitted. That is, we can assume the input has always this form:

```
https://micro.url/SECRET
```

However, if `SECRET` is unknown, `ClickUrl` and `Stats` do not complain and strange things can happen.

It's time to handle such an error.

In C++ there are too many ways to manage errors that it's very hard to state which is better. As always, each one comes with pros and cons. Traditional ways include:

* exceptions
* error codes
* bool flags
* callbacks/receivers
* ... your method

I will not compare those, instead I introduce another way to handle failures coming from another paradigm:

**Option type**

The **option** type is a *sum* type which adds "emptyness" semantic to a type. Basically, an *option* value may or may not contain a meaningful value of a certain type.

The **option** type is a *monad* thus it opens doors to composition and mapping.

## `optional<T>`

Being a hit for many years (in C++ - e.g. boost - and in other languages/paradigms), C++ has recently (C++17) welcomed on bord `std::optional`, a value that may or may not be present. In other words, `optional` is like a "typed" box: it either contains or not contain a value `T`. We can ask the box if it contains an instance of `T` and, if so, we can retrieve it.

If an `optional<T>` contains a value, the value is **guaranteed to be allocated as part of the optional object footprint** (no dynamic memory allocation ever takes place).

`optional<T>` is a common construct in other languages (e.g. C# has `T?`, Haskell has `Maybe`, etc) so it's worth knowing it. Optional types come from functional programming and are *monads* (e.g. chainable).

A common use case for `optional` is the return value of a function that may fail. As opposed to other approaches, such as `std::pair<T,bool>`, `optional` handles expensive-to-construct objects well and is more readable, as the intent is expressed explicitly.

Another use case for `optional` is the *lazy construction* of objects. Other approaches, such as smart pointers, usually require dynamic allocation or being able to model a null state (like an empty string).

### Examples

Classical example:

```cpp
std::optional<int> try_parse_int(const std::string& s)
{
    //try to parse an int from the given string,
    //and return "nothing" if you fail
}
```

Optional arguments:

```cpp
std::vector<std::pair<std::string, double>> search(
    std::string query,
    std::optional<int> max_count,
    std::optional<double> min_match_score);
```

### Optional references

Does it `optional<T&>` make sense? It might, but it's not allowed in C++17.

Suppose we want to look up an entry into a certain data store:

```cpp
? find(const Key& key);
```

A good choice could be returning `std::optional<Value&>` to avoid copying `Value`, instead of using pointers (our intent would be much clearer). A commond workaround consists in using `reference_wrapper` we met some time ago:

```cpp
optional<reference_wrapper<Value>> find(const Key& key);
// or
optional<reference_wrapper<const Value>> find(const Key& key);
```

Continue Reading:

* [Clearer interfaces with `optional<T>`](https://www.fluentcpp.com/2016/11/24/clearer-interfaces-with-optionalt/)
* [Refactoring with C++17 `std::optional`](https://www.bfilipek.com/2018/04/refactoring-with-c17-stdoptional.html)
* [The Boost C++ Libraries: Boost.Optional](https://theboostcpplibraries.com/boost.optional)

## Hands on!

Mark, head of IT Security Department at **Gugol**, asked your team to make `ClickUrl` and `Stats` more defensive by handling unknown SECRETs.

Your team has decided to handle failures with `optional`. The signatures have been already acommodated, complete the bodies and make the test pass:

@[Handling invalid urls with optional]({"stubs": [ 
	 "microurl/src/ver7/MicroUrlService.h",
	 "microurl/src/ver7/MicroUrlService.cpp",
	 "microurl/src/ver7/MicroUrlInfo.h",
	 "microurl/src/ver7/tests/UrlInfoTest.cpp",
	],
	"command": "sh /project/target/run_test.sh ver7 [optional]"})

::: Do you really give up? :(

A possible solution:

```cpp
std::optional<std::string> MicroUrlService::ClickUrl(std::string_view microUrl)
{
	if (auto it = m_idToUrl.find(UrlToId(microUrl)); it != end(m_idToUrl))
	{
		it->second.Clicks++;
		return it->second.OriginalUrl;
	}
	return nullopt;
}

std::optional<UrlInfo> MicroUrlService::Stats(std::string_view microUrl) const
{
	if (auto it = m_idToUrl.find(UrlToId(microUrl)); it != end(m_idToUrl))
	{
		return it->second;
	}
	return {};
}
```

You should complain because the logic to access the map is duplicated!

Since `ClickUrl` touches the map we cannot just change the result of `Stats` - that returns a copy of the requested `UrlInfo`.

If we returned a reference from `Stats` we would have another problem: `Stats` is `const` and thus the reference would be `const`.

We could create a utility function which returns either a `const` or mutable reference, depending on the `const`-ness of the map:

```cpp
template<typename IdToUrlMap>
auto TryLookup(IdToUrlMap& m, string_view str) -> decltype( std::optional{std::ref(m.find(UrlToId(str))->second)} )
{
    if (auto it = m.find(UrlToId(str)); it != end(m))
	{
		return std::ref(it->second);
	}
	return nullopt;
}
```

This function returns `optional<OptionalType>` where `OptionalType` depends on the `const`-ness of the input map.

Thus, we can rewrite the functions by calling `TryLookup` first:

```cpp
std::optional<std::string> MicroUrlService::ClickUrl(std::string_view microUrl)
{
	auto optRef = TryLookup(m_idToUrl, microUrl);
	if (optRef)
	{
		auto& urlInfo = optRef->get();
		urlInfo.Clicks++;
		return urlInfo.OriginalUrl;
	}
	return nullopt;
}	

std::optional<UrlInfo> MicroUrlService::Stats(std::string_view microUrl) const
{
	return TryLookup(m_idToUrl, microUrl);
}
```

In general, `optional` is more common (and probably easier) to use when transform *values* by applying operations that may fail. Anyway, `optional` with references is an interesting use case that it's worth knowing and for this reason has been presented here.

:::
	
## Bonus: chaining

`optional` becomes much more powerful when it is used in **composition**, as its characteristics allow for pipelines to be created which don't need to explicitly handle errors at each step.

The simplest way to chain `optional` is defining an operator like:

```cpp
template<typename T, typename F>
auto operator||(std::optional<T> opt, F f)
{
	return opt ? f(opt.value()) : std::nullopt;
}
```

The operator above enables chaining:

```cpp
auto value = 
    (Parse(formula)
    || Compile
    || Optimize
    || Evaluate).value_or(NAN);
```

Each stage of the pipeline is executed only if the previous has returned a good value (not `nullopt`) and has to return an instance of `optional`. The latter can be inconvenient:

```cpp
    return 
        Parse // return optional<string>
        || [](const string& s) { return s.substr(0, 3); } // suppose cannot fail
```

Instead, we have to write:

```cpp
    return 
        Parse
        || [](const string& s) { return optional<string>{s.substr(0, 3)}; } 
```

It's possible to tweak a bit `operator||` to make the first snippet work:

```cpp
template<typename T>
auto wrap(std::optional<T> opt)
{
	return opt;
}

template<typename T>
auto wrap(T value)
{
	return std::optional<T>(value);
}

template<typename T, typename F>
auto operator||(std::optional<T> opt, F f)
{
	return opt ? wrap(std::invoke(f, opt.value())) : std::nullopt;
}
```

Here `std::invoke` is convenient because it enables this syntax:

```cpp	
return 
	MakeUrlInfo // might return UrlInfo
	|| &UrlInfo::OriginalUrl; // <-- instead of [](const UrlInfo& url) { return url.OriginalUrl; };
}
```

`std::invoke` is a little gem from C++17 that invokes callables in a generic way. It works smoothly with member variables and functions too.

Applying chaining to your code is beyond the scope of this workshop - even though the operators are already included. Do it at home and have fun!

Here is a possible solution:

```cpp
std::optional<std::string> MicroUrlService::ClickUrl(std::string_view microUrl)
{
	return 
		TryLookup(m_idToUrl, microUrl)
		|| [](auto& url) { url.Clicks++; return url; }
		|| &UrlInfo::OriginalUrl;
	// or just:
	// return 
	//	TryLookup(m_idToUrl, microUrl)
	//	|| [](auto& url) { url.Clicks++; return url.OriginalUrl; }
}

std::optional<UrlInfo> MicroUrlService::Stats(std::string_view microUrl) const
{
	return TryLookup(m_idToUrl, microUrl);
}
```

Although you can justly think that the pipeline is more useful when each step may fail (e.g. in `ClickUrl` the steps after `TryLookup` never fail), we defend this approach because the composability is very high. For example, we may decide not to increment the number of clicks or to add another step afterwards. These decisions will fit the pipeline just by adding or removing callable blocks.

Similarly, if add extra verification steps such as *expiration* or *access control* (e.g. private stats), they will map naturally as new pipeline blocks.