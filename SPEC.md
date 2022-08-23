# G Programming Language Specification

The G programming language is intended to be an extension of the C programming language. Backwards compatible but with some extra features to reduce code redundancy and improve style and readability.

We'll either be using LLVM or transpiling to C89 directly: whatever offers the most flexibility long-term.

This is a living, breathing document that is still a major work-in-progress and subject to change.

## Braces and Indentation

Curly braces (`{` and `}`) are now optional. Outside of scalar initializers and function invocations, increased levels of indentation will be translated into `{`, and decreased levels of indentation will be translated into `}`.

## Integer Type Sizes

C does not define how large or small any integer types should be: `int` is 16-bit on some systems and 32-bit on others. We want to guarantee sizes for each integer type:

| Type | Bits |
| --- | --- |
| byte/char | 8   |
| short | 16  |
| int | 32  |
| long | 64  |

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

### strong

For giving every `enum` its own namespace and throwing warnings or errors when it is used in places it doesn't belong.

```c
strong enum Fruit { Apple, Orange, Banana, Pineapple };

const string *FruitName(enum Fruit Fruit)
{
    return Fruit.AsString();
}
```

In the above code, the function `FruitName` would not accept an `int` as an argument: only an `enum Fruit`. This is because `enum Fruit` is strongly typed.

Strongly typed enumerations also open up the possibility of different enumerated lists containing items of the same name. For example:

```c
strong enum LoadFileReturnCode { SUCCESS, ERROR_FREAD, ERROR_OTHER };
strong enum SaveFileReturnCode { SUCCESS, ERROR_FWRITE, ERROR_OTHER };
```

`strong` may end up being the default behavior, and we instead specify `weak` in places where we want weakly typed variables. Let's discuss this.

### list (and similar)

Builtin support for linked lists, doubly linked lists, and other common programming structures would be nice. We can declare and access them using array notation, but they will function under the hood as lists. This would be abstracted for the programmer.

```c
int list mylist = new(1, 2, 3);
```

## Optional Arguments

Just like JavaScript:

```c
void *LoadFile(const char *Filename, bool IsTextFile = false);

BinaryData = LoadFile("Hello.bin");
Data = LoadFile("README.md", true);
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
