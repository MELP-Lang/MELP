# MELP Standard Library Reference

**Version:** 1.0  
**Date:** 2026-03-30  
**Stage coverage:** Stage 0 ✅ + Stage 1 ✅ + Stage 2 (planned)  
**Canonical source:** `ORTAK/dil/stdlib/`

---

## I/O

### print

```mlp
print(value)
```

Writes `value` to stdout followed by a newline. Accepts `string`, `numeric`, or `boolean`.

```mlp
print("Hello")       -- Hello
print(42)            -- 42
print(true)          -- true
```

### println

Alias for `print`. Same behavior.

### read_line

```mlp
string line = read_line()
```

Reads one line from stdin (up to newline). Returns the line as a string (newline not included).

> **Stage note:** `read_line` is defined in the spec and mapped in the normalizer. Full STAGE1 codegen support is planned. Use with caution in current builds.

### read_file

```mlp
string contents = read_file(string path)
```

Reads the entire file at `path` and returns its contents as a string.

```mlp
string data = read_file("/tmp/input.txt")
print(data)
```

### write_file

```mlp
numeric ok = write_file(string path; string content)
```

Writes `content` to the file at `path` (overwrite). Returns `1` on success, `0` on failure.

```mlp
numeric result = write_file("/tmp/output.txt"; "Hello\n")
```

### append_file

```mlp
numeric ok = append_file(string path; string content)
```

Appends `content` to the file at `path`. Returns `1` on success, `0` on failure.

### read_arg

```mlp
string arg = read_arg(numeric index)
```

Returns the command-line argument at `index` (1-based). Returns empty string if out of range.

---

## Type Conversion

### str

```mlp
string s = str(value)
```

Converts a `numeric` or `boolean` to its string representation.

```mlp
str(42)       -- "42"
str(3.14)     -- "3.14"
str(true)     -- "true"
str(false)    -- "false"
```

### to_number

```mlp
numeric n = to_number(string s)
```

Parses a string as a number. Returns `0` if the string is not a valid number.

```mlp
to_number("42")     -- 42
to_number("3.14")   -- 3.14
to_number("abc")    -- 0
```

### to_boolean

```mlp
boolean b = to_boolean(numeric n)
```

Returns `false` if `n == 0`, `true` otherwise.

---

## Math

### abs

```mlp
numeric result = abs(numeric x)
```

Returns the absolute value of `x`.

```mlp
abs(-7)     -- 7
abs(7)      -- 7
```

### min

```mlp
numeric result = min(numeric a; numeric b)
```

Returns the smaller of `a` and `b`.

```mlp
min(10; 3)    -- 3
```

### max

```mlp
numeric result = max(numeric a; numeric b)
```

Returns the larger of `a` and `b`.

```mlp
max(10; 3)    -- 10
```

### sqrt

```mlp
numeric result = sqrt(numeric x)
```

Returns the square root of `x`.

```mlp
sqrt(25)    -- 5.0
sqrt(2)     -- 1.4142...
```

### floor

```mlp
numeric result = floor(numeric x)
```

Returns the largest integer not greater than `x`.

```mlp
floor(3.7)    -- 3
floor(-3.2)   -- -4
```

### ceil

```mlp
numeric result = ceil(numeric x)
```

Returns the smallest integer not less than `x`.

```mlp
ceil(3.2)     -- 4
ceil(-3.7)    -- -3
```

---

## String Operations

### len

```mlp
numeric n = len(string s)
```

Returns the number of characters in `s`.

```mlp
len("hello")    -- 5
len("")         -- 0
```

### upper

```mlp
string result = upper(string s)
```

Returns `s` converted to uppercase.

```mlp
upper("hello")    -- "HELLO"
```

### lower

```mlp
string result = lower(string s)
```

Returns `s` converted to lowercase.

```mlp
lower("HELLO")    -- "hello"
```

### trim

```mlp
string result = trim(string s)
```

Removes leading and trailing whitespace from `s`.

```mlp
trim("  hi  ")    -- "hi"
```

### contains

```mlp
boolean result = contains(string s; string sub)
```

Returns `true` if `s` contains the substring `sub`.

```mlp
contains("hello world"; "world")    -- true
contains("hello"; "xyz")            -- false
```

### starts_with

```mlp
boolean result = starts_with(string s; string prefix)
```

Returns `true` if `s` starts with `prefix`.

```mlp
starts_with("hello"; "he")     -- true
starts_with("hello"; "lo")     -- false
```

### ends_with

```mlp
boolean result = ends_with(string s; string suffix)
```

Returns `true` if `s` ends with `suffix`.

```mlp
ends_with("hello"; "lo")     -- true
ends_with("hello"; "he")     -- false
```

### split

```mlp
string[] result = split(string s; string delimiter)
```

Splits `s` on `delimiter` and returns an array of substrings.

```mlp
split("a,b,c"; ",")    -- ["a"; "b"; "c"]
split("hello"; "")     -- ["h"; "e"; "l"; "l"; "o"]
```

### join

```mlp
string result = join(string[] parts; string separator)
```

Joins `parts` with `separator` between each element.

```mlp
join(["a"; "b"; "c"]; ", ")    -- "a, b, c"
join(["a"; "b"; "c"]; "")      -- "abc"
```

### replace

```mlp
string result = replace(string s; string old; string new)
```

Replaces all occurrences of `old` with `new` in `s`.

```mlp
replace("aaa"; "a"; "b")        -- "bbb"
replace("hello world"; " "; "_")  -- "hello_world"
```

### substring

```mlp
string result = substring(string s; numeric start; numeric length)
```

Returns `length` characters of `s` starting at `start` (0-based).

```mlp
substring("hello"; 1; 3)    -- "ell"
substring("hello"; 0; 5)    -- "hello"
```

### index_of

```mlp
numeric pos = index_of(string s; string sub)
```

Returns the 0-based position of the first occurrence of `sub` in `s`. Returns `-1` if not found.

```mlp
index_of("hello"; "ll")     -- 2
index_of("hello"; "xyz")    -- -1
```

---

## Array Operations

Arrays are declared with the element type followed by `[]`:

```mlp
numeric[] nums = [1; 2; 3]
string[] names = ["alice"; "bob"]
```

### .push

```mlp
nums.push(value)
```

Appends `value` to the end of the array.

### .pop

```mlp
numeric last = nums.pop()
```

Removes and returns the last element.

### .length

```mlp
numeric n = nums.length
```

Returns the number of elements.

---

## Hashmap Operations

```mlp
hashmap<string; numeric> map = {}
```

### Indexing

```mlp
map["key"] = 42
numeric val = map["key"]
```

### .has

```mlp
boolean exists = map.has("key")
```

Returns `true` if the key exists.

### .delete

```mlp
map.delete("key")
```

Removes the key-value pair.

### .keys

```mlp
string[] keys = map.keys()
```

Returns an array of all keys.

### .length

```mlp
numeric n = map.length
```

Returns the number of key-value pairs.

---

## Testing Framework (Stage 1)

### test

```mlp
test(string name; lambda() action)
```

Declares a named test case.

```mlp
test("addition"; lambda()
    assert_eq(1 + 1; 2)
end lambda)
```

### describe

```mlp
describe(string suite; lambda() block)
```

Groups related test cases.

```mlp
describe("Math"; lambda()
    test("add"; lambda()
        assert_eq(2 + 3; 5)
    end lambda)
    test("subtract"; lambda()
        assert_eq(5 - 3; 2)
    end lambda)
end lambda)
```

### assert_eq

```mlp
assert_eq(actual; expected)
```

Fails the test if `actual != expected`.

### assert_true

```mlp
assert_true(boolean condition)
```

Fails the test if `condition` is `false`.

### assert_false

```mlp
assert_false(boolean condition)
```

Fails the test if `condition` is `true`.
