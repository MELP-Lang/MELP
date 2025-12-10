# MELP Control Flow Reference

**Version:** 1.0  
**Last Updated:** December 10, 2025  
**Language:** English

---

## Overview

MELP uses VB.NET/Pascal-style control flow with explicit block endings. This document covers all control flow structures.

---

## Conditional Statements

### If-Then-End If

```mlp
if condition then
    -- code
end if
```

### If-Then-Else

```mlp
if condition then
    -- true branch
else
    -- false branch
end if
```

### Else-If Chain

```mlp
if condition1 then
    -- branch 1
else if condition2 then
    -- branch 2
else if condition3 then
    -- branch 3
else
    -- default branch
end if
```

**Note:** Only one `end if` is needed at the very end.

---

## Loop Structures

### While Loop

```mlp
while condition do
    -- code
end while
```

**Example:**
```mlp
numeric i = 0
while i < 10 do
    print(i)
    i = i + 1
end while
```

### Do-While Loop

Executes at least once:

```mlp
do
    -- code (runs at least once)
while condition
```

**Example:**
```mlp
numeric i = 0
do
    print(i)
    i = i + 1
while i < 10
```

### For Loop

#### Basic For Loop

```mlp
for i = 0 to 10
    print(i)
end for
```

#### With Step

```mlp
for i = 0 to 100 step 10
    print(i)  -- 0, 10, 20, ..., 100
end for
```

#### Downward Iteration

```mlp
for i = 10 downto 0
    print(i)  -- 10, 9, 8, ..., 0
end for
```

### For-In Loop (Iterator)

Iterate over collections:

```mlp
numeric[] numbers = [1, 2, 3, 4, 5]
for num in numbers
    print(num)
end for
```

#### With Range Function

```mlp
-- range(end): 0 to end-1
for i in range(5)
    print(i)  -- 0, 1, 2, 3, 4
end for

-- range(start, end): start to end-1
for i in range(2, 8)
    print(i)  -- 2, 3, 4, 5, 6, 7
end for

-- range(start, end, step)
for i in range(0, 10, 2)
    print(i)  -- 0, 2, 4, 6, 8
end for
```

---

## Exit Statements (VB.NET Style)

MELP uses `exit X` instead of `break` or `continue`:

| Statement | Effect |
|-----------|--------|
| `exit if` | Exit from if block |
| `exit for` | Exit from for loop |
| `exit while` | Exit from while loop |
| `exit function` | Exit from function early |
| `exit switch` | Exit from switch block |

**Example:**

```mlp
for i = 0 to 100
    if i == 50 then
        exit for  -- Exits the for loop
    end if
    print(i)
end for
```

**Why not `break`/`continue`?**

MELP follows VB.NET philosophy where you explicitly state which construct you're exiting. This is clearer in nested loops:

```mlp
for i = 0 to 10
    for j = 0 to 10
        if condition then
            exit for  -- Clear: exits inner for loop
        end if
    end for
end for
```

---

## Switch-Case

```mlp
switch value
    case 1
        print("one")
    case 2
        print("two")
    case 3
        print("three")
    default
        print("other")
end switch
```

**Important:** 
- No fall-through! Each case automatically breaks.
- No `break` statement needed.

---

## Pattern Matching

More powerful than switch for complex matching:

### Single Value Match

```mlp
match x
    case 1 => print("one")
    case 2 => print("two")
    case _ => print("default")
end match
```

### Multiple Values

```mlp
match x
    case 1, 2 => print("one or two")
    case 3, 4, 5 => print("three to five")
    case _ => print("other")
end match
```

### Range Match

```mlp
match score
    case 0 to 49 => print("Fail")
    case 50 to 69 => print("Pass")
    case 70 to 89 => print("Good")
    case 90 to 100 => print("Excellent")
    case _ => print("Invalid")
end match
```

**Pattern Matching Features:**
- `=>` (fat arrow) separates pattern from action
- `_` is wildcard (matches anything)
- No fall-through
- Ranges with `to` keyword

---

## Generators

Functions that yield values one at a time:

```mlp
function squares(numeric n) yields numeric
    for i = 0 to n
        yield i * i
    end for
end function

-- Usage
for sq in squares(5)
    print(sq)  -- 0, 1, 4, 9, 16, 25
end for
```

**Keywords:**
- `yields` in function signature
- `yield` to produce a value

---

## Exception Handling

### Try-Catch-Finally

```mlp
try
    -- risky code
    throw RuntimeError("Something went wrong!")
catch RuntimeError e
    print("Runtime error caught")
catch ValueError e
    print("Value error caught")
catch e
    print("Any other error")
finally
    print("Always executes")
end try
```

### Throw Statement

```mlp
throw ErrorType("Error message")
```

### Catch Order

More specific exceptions should come first:

```mlp
try
    -- code
catch SpecificError e
    -- handle specific
catch GeneralError e
    -- handle general
catch e
    -- catch all
end try
```

---

## Goto and Labels

For special cases (use sparingly):

```mlp
goto @label_name

-- ... code ...

@label_name:
-- continues here
```

**Warning:** Overuse of `goto` makes code hard to follow. Prefer structured control flow.

---

## Debug Control Flow

Special keywords for debugging:

```mlp
debug goto @label    -- Jump in debug mode only
debug pause          -- Breakpoint in debug mode
debug label @name    -- Debug-only label
debug print x        -- Print only in debug mode
```

**Note:** These only work when compiled with `--debug` flag.

---

## Quick Reference

### Block Endings

| Start | End |
|-------|-----|
| `if ... then` | `end if` |
| `while ... do` | `end while` |
| `for ... to` | `end for` |
| `do` | `while condition` |
| `switch` | `end switch` |
| `match` | `end match` |
| `try` | `end try` |
| `function` | `end function` |

### Exit Keywords

| Exit | From |
|------|------|
| `exit if` | If block |
| `exit for` | For loop |
| `exit while` | While loop |
| `exit function` | Function |
| `exit switch` | Switch |

---

## Common Patterns

### Early Return

```mlp
function process(numeric x) returns numeric
    if x < 0 then
        return 0  -- Early return
    end if
    -- continue processing
    return x * 2
end function
```

### Infinite Loop with Exit

```mlp
while true do
    -- process
    if done then
        exit while
    end if
end while
```

### Nested Loop Exit

```mlp
boolean found = false
for i = 0 to 10
    for j = 0 to 10
        if matrix[i][j] == target then
            found = true
            exit for  -- Exit inner loop
        end if
    end for
    if found then
        exit for  -- Exit outer loop
    end if
end for
```

---

*This document is part of the MELP Language Documentation.*
