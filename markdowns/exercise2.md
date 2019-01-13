# Use Interfaces, Luke

Without a doubt, the **Object-Oriented** paradigm contributed to the diffusion of C++ in the industry in the early 90s.

The concept of **Interface** is one of the pillars of object orientation. An interface is, simply, a description of the actions that an object can do.

C++ does not really have a built-in support for interfaces like Java or C#. A way to declare an interface in C++ constists in declaring an abstract class with pure virtual functions:

```cpp
class Interface
{
public:
    virtual void func() = 0;
    //...
};
```

However, this suffers of undefined behavior in case of **polymorphic deletion**:

```cpp
Interface* base = new Concrete(); // pointer to base class
//...
delete base; // undefined behavior if Interface's destructor is not virtual
```

So the canonical way to create an interface in C++ is to give it a **defaulted virtual destructor**:

```cpp
class Interface
{
public:
    virtual ~Interface() = default;
    virtual void func() = 0;
    //...
};
```

What if you do not want to "pay" a virtual call on destruction?

Many years ago, **Herb Sutter** elaborated the following guideline:

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

This makes polymorphic deletion not compile.

Anyway, when in doubt, the most common and flexible way to declare an interface in C++ consists in declaring a defaulted virtual destructor:

```cpp
class Interface
{
public:
    virtual ~Interface() = default;
    virtual void func() = 0;
    //...
};
```

As you learned from the previous section, declaring a `default`ed virtual destructor is a violation of the **rule of zero** since `Interface` does not deal with ownership. This is one rare exception - `shared_ptr` may help here (read on).

### Adapting to changes

From C++11, declaring (even `=default`) a virtual destructor prevents move operators from being automatically generated and *deprecates* the generation of copy operators. This should not be a problem for interfaces because they **should never** contain any data:

```cpp
class Derived : public Interface
{
    void func() override;
    
    std::vector<State> m_spam; // data in derived class
};

Derived d1;
// this copies the *base* part and moves m_spam
auto d2 = move(d1);
```

However, the code above relies on a deprecated thing: generation of copy operators. We should be ready to adapt in case deprecation turns into actual removal.

So we can say the most futurable way to declare an interface in C++ consists in `=default`-ing all the special operators:

```cpp
class Interface
{
public:
    virtual ~Interface() = default;
    Interface(const Interface&) = default;
    Interface(Interface&&) = default;
    Interface& operator=(const Interface&) = default;
    Interface& operator=(Interface&&) = default;
    virtual void func() = 0;
    //...
};
```

This is commonly called (defaulted) **Rule of Five**.

By the way, it's very unlikely that this deprecation will ever turn into reality.

### Slices and clones

Just for completeness, we shortly describe another issue.

If any copyable polymorphic class is accidentally passed *by value*, *slicing* happens: only the base portion of a derived object will be copied, and the polymorphic behavior will be corrupted.

```cpp
class Derived : public Interface
{
public:
    void func() override;
    // ...
};

void f(Interface& b) 
{
    auto b2 = b; // slicing
}

Derived d;
f(d);
```

For this reason, many people prefer `=delete`-ing special operators of base classes:

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
    
    virtual void func() = 0;
    // ...
};

void f(Interface& b) 
{
    auto b2 = b; // does not compile
}

Derived d;
f(d);
```

It's clearly a tradeoff because the code above makes derived instances not **copyable** nor **movable**.

However, for making deep copies of polymorphic classes we *prefer* a virtual `clone` function instead of copy construction/assignment. Continue reading the guideline [C.130](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rh-copy) for more details.

The discussion can go on a bit more, because some other alternative exists.

As said, the acceptable compromise to create interfaces consists in simply declare a virtual `=default` destructor:

```cpp
class Interface
{
public:
    virtual ~Interface() = default;
    //...functions...
};
```

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
	
### Bonus: `shared_ptr` does some magic

We realized that declaring any destructor - whether public virtual or protected non-virtual - will violate the rule of zero. 

Is there any way to both obey the rule of zero and avoid undefined behavior?

There is but it's really fragile because it relies on how our interface will be used.

Consider this base class:

```cpp
class Animal
{
public:    
    virtual void move() = 0;
    //...
};
```

This works as you expect and it is not affected by UB:

```cpp
shared_ptr<Animal> obj { new Dog() };
```

When `shared_ptr<Animal>` is constructed, its constructor deduces `Dog` and it builds a *deleter* which knows which destructor should be called. The contraption used internally si called **type erasure** and it's a **generic programming** technique. It's beyond the scope of the workshop. 

Type erasure introduces some cost, which is why `unique_ptr` that does not provide such "magic" deletion.

Continue Reading:

* [Enforcing the Rule of Zero](https://accu.org/var/uploads/journals/Overload120.pdf)
* [Ponder the use of unique_ptr to enforce the Rule of Zero](https://marcoarena.wordpress.com/2014/04/12/ponder-the-use-of-unique_ptr-to-enforce-the-rule-of-zero/)