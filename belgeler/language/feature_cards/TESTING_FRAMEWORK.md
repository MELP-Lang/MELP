# TESTING_FRAMEWORK - Özellik Kartı

**Özellik:** Testing Framework  
**Durum:** 🔴 TODO (Phase 4.5)  
**Sözdizimi Sürümü:** pmlp v1.0

---

## 🎯 SÖZDİZİMİ REFERANSI

### Test Definition
```mlp
test("addition works"; lambda()
  numeric result = add(2; 3)
  assert(result == 5; "2 + 3 should be 5")
end_lambda)
```

### Test Suite
```mlp
describe("Math Tests"; lambda()
  test("addition"; lambda()
    assert(add(1; 1) == 2)
  end_lambda)
  
  test("subtraction"; lambda()
    assert(sub(5; 3) == 2)
  end_lambda)
end_lambda)
```

### Assertions
```mlp
assert(condition; message)
assert_eq(actual; expected)
assert_ne(actual; expected)
assert_true(condition)
assert_false(condition)
```

### Running Tests
```bash
$ mlp test
Running 5 tests...
✓ addition works
✓ subtraction works
✓ multiplication works
✓ division works
✗ modulo works
  Expected: 2, Got: 3

4/5 tests passed
```

---

## 📝 GRAMER KURALLARI

```ebnf
test_def   ::= "test" "(" string ";" lambda_expr ")"

describe_def ::= "describe" "(" string ";" lambda_expr ")"

assert_stmt  ::= "assert" "(" expr ";" string ")"

assert_eq    ::= "assert_eq" "(" expr ";" expr ")"
```

---

**Phase:** 4.5 (Month 7)
