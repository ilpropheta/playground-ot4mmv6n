# Use Interfaces, Luke

> Life is a constant process or relating.

Bruce Lee.

Without a doubt, the **Object-Oriented** paradigm contributed to the diffusion of C++ in the industry in the early 90s.

**Interfaces** constitute a powerful basic concept of Object Orientation. An interface is, simply, a description of the actions that an object can do.

C++ does not really have a built-in support for interfaces like Java or C#. A way to declare an interface in C++ constists in declaring an abstract class with pure virtual functions:

```cpp
class Interface
{
public:
    virtual void func() = 0;
    //...
};
```

Actually, the canonical way to create an interface in C++ is to give it a **defaulted virtual destructor**:

```cpp
class Interface
{
public:
    virtual ~Interface() = default;
    virtual void func() = 0;
    //...
};
```

This can be avoided only if you are really sure that this class will never be deleted polymorphically. This is a polymorphic deletion:

```cpp
Interface* base = Factory::Create(); // will craft an instance of some concrete implementation
//...
delete base; // undefined behavior if Interface's destructor is not virtual
```

Many years ago, **Herb Sutter** elaborated a neat guideline:

> A base class destructor should be either public and virtual, or protected and nonvirtual.

Thus, we could have:

```cpp
class Interface
{
protected:
    ~Interface() = default;
public:    
    virtual void func() = 0;
    //...
};
```

This makes impossible to do such polymorphic deletion.

Anyway, the most common and flexible way to declare an interface in C++ consists in declaring a defaulted virtual destructor - unless you really cannot afford a virtual destructor:

```cpp
class Interface
{
public:
    virtual ~Interface() = default;
    virtual void func() = 0;
    //...
};
```

### `shared_ptr` does some magic

Imagine we decided to stay with the protected non-virtual destructor:

```cpp
class Animal
{
protected:
    ~Animal() = default;
public:    
    virtual void move() = 0;
    //...
};
```

Does the following code compile?

```cpp
shared_ptr<Animal> obj = Factory::CreateDog(); // get back shared_ptr<Dog>
```

Yes, because in this case `shared_ptr` knows that the actual object is of type `Dog`, then it knows how to delete it. `shared_ptr` dynamically creates a specific deleter for that purpose.

This is not the case for `unique_ptr`, that does not introduce such overhead.

### Slicing and clones

If an interface (ar any polymorphic class) is accidentally passed *by value*, with the implicitly generated copy constructor and assignment, we risk *slicing*: only the base portion of a derived object will be copied, and the polymorphic behavior will be corrupted.

For this reason, many people prefer `=delete`-ing special operators of interfaces:

```cpp
class Interface 
{ 
public:
    Interface() = default;
    virtual ~Interface() = default;
    Interface(const Interface&) = delete;
    Interface& operator=(const Interface&) = delete;
    Interface(Interface&&) = delete;
    Interface& operator=(Interface&&) = delete;
    // ...
};

class Derived : public Interface
{
public:
    // ...
};

void f(Interface& b) 
{
    auto b2 = b; // does not compile
}

Derived d;
f(d);
```

It's clearly a tradeoff because the code above makes derived instances not copyable.

Generally, for making deep copies of polymorphic classes we prefer a virtual `clone` function instead of copy construction/assignment. Continue reading the guideline [C.130](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rh-copy) for more details.

Continue Reading:

* [C.21: If you define or =delete any default operation, define or =delete them all](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-five)
* [C.67: A polymorphic class should suppress copying](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-copy-virtual)

### Enforce overriden functions with `override`

Since C++11, we can enforce that a derived class overrides a base function by using `override`:

```cpp
class Derived : public Interface
{
public:
    void func() override;
};
```

This is very expressive. For non-abstract class, this prevents errors like:

```cpp
class Base
{
public:
    virtual void func1();
    virtual void func2();
};

class Derived
{
public:
    void Func1(); // capital 'F'
};
```

The code compiled although `Derived` is not really overriding `Base::func1`. On the other hand:

```cpp
class Derived
{
public:
    void Func1() override;
};
```

This does not compile.

Use `override` as much as possible (many refactoring tools will automatically do).

## Hands on!

In the dusty corners of **MicroUrl**, you have found `IIdGenerator.h` that is probably an old attempt that a former developer did to extract the id generation concept into an interface.

It's time to fulfill such effort and complete the job!

You tasks:

- Review `IIdGenerator`
- Make `DbIdGenerator` implement `IIdGenerator`
- Make `MicroUrlService` use `IIdGenerator` instead of `DbIdGenerator`
- Make possible to inject an `IIdGenerator` into `MicroUrlService` (you will find a new constructor to accommodate)

Let's go:

@[Use interfaces, Luke!]({"stubs": [
	 "microurl/src/ver3/IIdGenerator.h",
	 "microurl/src/ver3/DbIdGenerator.h",
	 "microurl/src/ver3/DbIdGenerator.cpp",
	 "microurl/src/ver3/tests/MicroUrlCustomIdGeneratorTest.cpp",
	 "microurl/src/ver3/MicroUrlService.h",
	 "microurl/src/ver3/MicroUrlService.cpp",
	],
	"command": "sh /project/target/run_test.sh ver3"})