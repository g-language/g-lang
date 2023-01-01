# G Programming Language Specification

The G programming language is intended to be an extension of the C programming language. Backwards compatible but with some extra features to reduce code redundancy and improve style and readability.

We'll either be using LLVM or transpiling to C89 directly: whatever offers the most flexibility long-term.

This is a living, breathing document that is still a major work-in-progress and subject to change.

## Braces and Indentation

Curly braces (`{` and `}`) are now optional. Outside of scalar initializers and function invocations, increased levels of indentation will be translated into `{`, and decreased levels of indentation will be translated into `}`.

## Integer Type Sizes

C does not define how large or small any integer types should be: `int` is 16-bit on some systems and 32-bit on others. We want to guarantee sizes for each integer type:

| Type      | Bits |
| --------- | ---- |
| byte/char | 8    |
| short     | 16   |
| int       | 32   |
| long      | 64   |

Also unlike C, we want to guarantee consistent signedness. In C, some compilers will treat `char` as `unsigned` by default. In G, all integer types will be `signed` by default.

## Preprocessor Extras

### incbin

```c
#incbin "embedded.bin"
```

### Compile-time Functions

Functions that are evaluated during compilation, such as a custom string hashing function.

### Loops

Preprocessor loops might be helpful for reducing redundancy.

## Extra Keywords

### unhashed

For speedy string comparisons, strings are hashed by default upon initialization. This makes using strings in a `switch-case` fast. For situations where it is mission-critical strings remain unhashed, the `unhashed` keywords has been introduced. Here are a couple examples:

```c
unhashed const string *hello = "Hello, world!";

switch (unhashed "Hello, world!")
{
    case "Hello, world!":
        break;
}
```

### weak

By default, every `enum` has its own namespace and throwing warnings or errors when it is used in places it doesn't belong.

```c
enum Fruit { Apple, Orange, Banana, Pineapple };

const string *FruitName(enum Fruit Fruit)
{
    return Fruit.AsString();
}
```

In the above code, the function `FruitName` would not accept an `int` as an argument: only an `enum Fruit`. This is because `enum Fruit` is strongly typed by default.

Strongly typed enumerations also open up the possibility of different enumerated lists containing items of the same name. For example:

```c
enum LoadFileReturnCode { SUCCESS, ERROR_FREAD, ERROR_OTHER };
enum SaveFileReturnCode { SUCCESS, ERROR_FWRITE, ERROR_OTHER };
```

Because `strong` is the default behavior, we have introduced the `weak` keyword for situations where you want to use weakly typed variables. `weak enum Fruit` would function as enumerations ordinarily do in C.

### list (and similar)

Builtin support for linked lists, doubly linked lists, and other common programming structures would be nice. We can declare and access them using array notation, but they will function under the hood as lists. This would be abstracted for the programmer.

```c
int list mylist = new(1, 2, 3);
```

### default

In a data structure, members can be assigned default values:

```c
struct MyStruct
{
    int Wow = 123;
};
```

Using `default`, you can check whether its value has changed by testing `if (Wow == default)`.

### private

Variables and functions within a `struct` can be declared as `private` so they aren't accessible outside of its scope.

```c
struct MyStruct
{
    private int Wow = 123;
    
    int GetWow(void)
    {
        return .Wow;
    }
}
```

### `static` in structures

Variables within a `struct` can be declared as `static` so that all instances of that structure reference the same value.

```c
struct MyStruct
{
    static int CommonValue = 0;
}

int main(void)
{
    struct MyStruct a;
    struct MyStruct b;
    
    a.CommonValue = 123;
    printf("%d\n", b.CommonValue); // prints 123
}
```

### meta

Allows specifying callbacks that will be fired when certain operations are performed on the most basic types (`int`, `float`, pointers, etc). Think of them as functioning identically to getters and setters without having to manually name or invoke them as functions.

A couple of examples:
```c
typedef meta float {
    onwrite {
        return wrap(this, -180.0f, 180.0f);
    }
} DegF;

example:
DegF v = 178;
v += 1; // 179
v += 1; // 180
v += 1; // v = 181, v = v.onwrite(), v = -180
```

```c
struct GameInst
{
    meta struct GameSprite *sprite {
        onwrite {
            .frame = 0 // . selects outer scope
            return this
        }
    }
    int frame = 0
};

example:
struct GameInst inst = ...
inst.sprite = GameSpriteLookup("Player/Jump") // inst.frame = 0 happens on this change
```

### `with` statement

Allows for running code within another `struct`'s scope. This reduces redundancy, improving readability.

```
For example, this...
sibling.health = 100
sibling.target = GetPlayer()
sibling.direction = Normalize()
sibling.partner = this

... becomes this:
with (sibling)
    health = 100
    target = GetPlayer()
    direction = Normalize()
    partner = other.this
```

## Optional Arguments

Just like JavaScript:

```c
void *LoadFile(const char *Filename, bool IsTextFile = false, void *CustomAllocator(size_t sz) = 0);

BinaryData = LoadFile("Hello.bin");
Data = LoadFile("README.md", true);
BinaryDataX = LoadFile("Hello.bin", , malloc);
```

## Named Arguments

```c
void *LoadFile(const char *Filename, bool IsTextFile = false, size *Size);

Data = LoadFile(.Filename = "Hello.bin", .Size = &DataSize);
```

## Structure Serialization and Deserialization

One option so programmers can quickly and easily load data from JSON.

## JSON Data Type

Another option so programmers can quickly and easily load data from JSON. This proposal is lighter to implement than full structure (de)serialization and easier to do error-checking for.

```c
/* this G code... */
Json Test = JsonLoad("Hello.json");
String *DayOfWeek = Test.DayOfWeek;
Int Temperature = Test.Forecast.Temperature;

/* ... gets translated into more verbose C code */
Json *Test = JsonLoad("Hello.json");
const char *DayOfWeek = JsonGetString(Test, "DayOfWeek"));
Int Temperature = JsonGetInt(JsonGet(Test, "Forecast"), "Temperature"));
```

The type of getter used (`String`, `Int`, etc) is decided based on the type of L-value being assigned to it. Child members within a nested JSON object are themselves JSON objects, retrieved using `JsonGet`, and are thus able to have values retrieved from them.

String hashing and/or binary search are used internally to speed up the find-by-name process.

## Honorable Mentions

- Optional semicolons (`;`)

- `foreach`

- Nested functions

- Declare-anywhere functions
