# Lifetime patterns

By default, C++ objects have **automatic** lifetime, that is, they are destructed and deallocated automatically when they go out of scope - even though an exception is thrown:

```cpp
{
	int i;
	cin >> i;
	// ... exceptions possible ...
} // i deallocated here

{
	Foo foo; // constructor
	// ... exceptions possible ...
} // foo destructed and deallocated here
```

On the other hand, other kind of semantics need explicit syntax:

```cpp
int i = 10;
int& iref = i; // reference semantics
```

How to allocate resources dynamically (e.g. we do not know the size of an array at compile-time)?

## Pointers Headache

To instantiate resources dynamically, the language provides operators which return **pointers** to heap-allocated resources:

```cpp
int* arr = new int[10]{}; // dynamic buffer
// in case of exceptions... :(
delete [] arr;
```

Pointers are cheap but at the same time very messy and error-prone. Apart from problems such as memory leaks, let's see another issue: could you unmistakably answer to these questions about the following function?

```cpp
void Func(int* p)
```

1. should `Func` delete `p`?
2. can `p` be null?
3. is `p` referring to a single instance or an array?
4. is `p` an out parameter?
 
Pointers can be misused especially because their syntax is tied with more than one semantics. As we have just seen, a pointer can reference either one or many instances of a certain type. A pointer can either be null or not. Also, at first sight, we do not know about the ownership of a resource referenced by a pointer.

However, pointers are what the language provide to create and pass around dynamically-allocated objects. 

Since dynamic lifetime does not obey to automatic lifetime rules, ownership is more cumbersome and error-prone. In practical terms, in C++ we give ownership of any *heap-allocated* resource to a *stack-allocated* object whose destructor contains the code to delete or free the resource and also any associated cleanup code. This way we turn dynamic lifetime into automatic lifetime, more or less.

This simple rule is a fundamental C++ lifetime idiom called **RAII**: *Resource Acquisition Is Initialization*:

```cpp
struct Handler
{
	Handler(resource* res)
	    : m_res(res)
	{
	}
	
	~Handler()
	{
	    delete m_res;
	}
	
	void Use()
	{
	    // ...
	    // use m_res...
	}
	
	
private:
    resource* m_res;
};


{
    Handler h( InitResource() );
    
    h.Use();
    
} // the resource is released here
```

The main goal of this idiom is to ensure that resource acquisition occurs at the same time that the object is initialized, so that all resources for the object are created and made ready in one line of code. 

## Rule of Zero

Classes that have custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership (which follows from the *Single Responsibility Principle*). 
Other classes should not have custom destructors, copy/move constructors or copy/move assignment operators.

Generally, we never work explicitly with lifetime code but we use general-purpose or ad hoc stack-allocated levels of indirection. For instance, we use *containers* - even not standard - to manage data structures or file handlers to manage access to filesystem.

Such proxies (or give them another name you like) have some well-known lifetime semantics, that is generally tied with *copy* and *move* operators. For example, `std::vector` can be fully copied into another instance. On the other hand, `std::thread` cannot be copied but only moved because the ownership of threads is *unique*.

For the rest of this section, we'll learn how to use standard general-purpose tools for dynamic lifetime management that will help adopt the rule of zero.

## Getting a glimpse of smart pointers

If you feel you need to use pointers (first consider if you really do), you would normally want to use a *smart pointer* as this can alleviate many of the problems with raw pointers, mainly forgetting to delete the object and leaking memory.

A smart pointer is just a wrapper that *behaves like* a pointer but defines a clear **ownership policy** as to when the object is destroyed and copied/moved.

Smart pointers are allocated on the stack and passed around as normal objects.

The C++ standard library provides 3 smart pointers:

* [`unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr)
* [`shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr)
* [`weak_ptr`](https://en.cppreference.com/w/cpp/memory/weak_ptr)

Clearly, those are general-purpose tools that 98% of time cover our needs. The rest 2% is when we need more and we should implement our own smart pointer or wrapper. The guideline is just: *do not handle dynamic resources by hand but invest on your support library*.

The simplest smart pointer that should be preferred by default is `unique_ptr`, defining a clear *unique* ownership policy that means: the resource will be only owned by that smart pointer:

```cpp
unique_ptr<Resource> handle = Acquire();
// use handle
// ...throw exception...
// released automatically at the end of the scope - although an exception has been thrown
```

The code above would have leaked with raw pointers.

`unique_ptr` can't be copied, since it models a unique ownership model:

```cpp
unique_ptr<Resource> handle = Acquire();
auto copied = handle; // does not compile
```

Instead, `unique_ptr` can **transfer** ownership by `std::`moving:

```cpp
unique_ptr<Resource> handle = Acquire();
// ...
auto newOwner = std::move(handle);
// handle wraps nullptr here
// newOwner owns the resource here
```

`shared_ptr` models a simultaneous (**shared**) ownership policy, meaning that a resource can have multiple owners. As long as an owner is alive, so is the resource:

```cpp
shared_ptr<Resource> owner1 = Acquire();
{
    auto owner2 = owner1;
    // 2 owners here
    {
        auto owner3 = owner2;
        // 3 owners here
    }
    // 2 owners here
}
// 1 owner here

// imagine  owner1 goes out of scope: the resource is finally released
```

`shared_ptr` works by *reference counting* active owners. This, clearly, introduces some overhead.

`weak_ptr` is a smart pointer that holds a non-owning (**weak**) reference to an object that is managed by `shared_ptr`. It must be converted to `shared_ptr` in order to access the referenced object.

`weak_ptr` models temporary ownership: when an object needs to be accessed only if it exists, and it may be deleted at any time by someone else, `weak_ptr` is used to track the object, and it is converted to `shared_ptr` to assume temporary ownership. If the original `shared_ptr` is destroyed at this time, the object's lifetime is extended until the temporary `shared_ptr` is destroyed as well.

## Scope Guard

We conclude this section by recalling a classic idiom that is a general-purpose *finalizer*: an operation always executed at the end of the scope.

The idea is to have a handy class providing a customizable destructor and that can be created inline.

With the advent of lambdas - that we'll see later - and C++17 automatic deduction of class templates this has been dramatically simplified:

```cpp
template<typename F>
struct finalize : F
{
    finalize(F f) : F(f) {}

    ~finalize()
    {
        F::operator()();
    }
};

// business code:

{
	finalize f{[]{cout << "hello"; }};
	// some code
	// ...
} // "hello" printed here

```

Although this is a trivial implementation ([see a possible issue here](https://github.com/Microsoft/GSL/issues/283)), it's acceptable in lots of real scenarios.

As for lambdas, this idiom is useful when you need **anonymous** and disposable finalization code.

Continue Reading:

* [Microsoft on Smart Pointers](https://msdn.microsoft.com/en-us/library/hh279674.aspx)
* [Smart developers use smart pointers](https://www.fluentcpp.com/2017/08/22/smart-developers-use-smart-pointers-smart-pointers-basics/)

### Hands on!

You've just learnt how good smart pointers are and you want to impress your boss by using them in **MicroUrl**.

It seems there is a really good fit for `unique_ptr`: `MicroUrlService` is manually managing `m_idGenerator`'s lifetime. You have to use `std::unique_ptr<IIdGenerator>` instead:

@[Replace manual resource managing with unique_ptr]({"stubs": ["microurl/src/ver2/MicroUrlService.h", "microurl/src/ver2/MicroUrlService.cpp", "microurl/src/ver2/tests/MicroUrlInitializationTest.cpp"],"command": "sh /project/target/run_test.sh ver2"})

## Bonus: using smart pointers with incomplete types

Undefined behavior can occur when you have an **incomplete** type and you call delete on it:

```cpp
class A; // class forwarding
A* a = ...;
delete a;
```

The above is legal code. It will compile. Your compiler may or may not emit a warning for above code like the above. When it executes, bad things will probably happen. If you're very lucky your program will crash. However a more probable outcome is that your program will silently leak memory as ~A() won't be called.

Using smart pointers, when it is necessary to have a complete type, you get a compile-time error if you try to use the smart pointer with an incomplete type at that point. If we have this scenario:

```cpp
class DbIdGenerator; // class forwarding

class MicroUrlService
{
    //...
    unique_ptr<DbIdGenerator> m_idGenerator;
};
```

`MicroUrlService` will win an automatically-generated destructor, that is **inline** (e.g. its body is generated in the header file) and then will require to know how to destroy `DbIdGenerator`. For this reason we have, at least, to declare a destructor:

```cpp
class DbIdGenerator; // class forwarding

class MicroUrlService
{
public:
    ~MicroUrlService() // <-- declared by us
    //...
private:
    unique_ptr<DbIdGenerator> m_idGenerator;
};
```

This way the compiler will just pass on. On the cpp file we can just `default` the implementation:

```cpp
DbIdGenerator::~DbIdGenerator() = default;
```

This is a powerful idiom to remember.

Continue reading:

* [Incomplete types and `shared_ptr` / `unique_ptr`](https://howardhinnant.github.io/incomplete.html)
* [How to implement the pimpl idiom by using unique_ptr](https://www.fluentcpp.com/2017/09/22/make-pimpl-using-unique_ptr/)