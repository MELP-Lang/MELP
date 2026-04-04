# MELP — Transitional Documentation Index

This tree is kept as a compatibility layer.
The preferred Stage1 reference tree is [../belgeler_v2/README.md](../belgeler_v2/README.md).

Turkish version → [README.md](README.md)

---

## Canonical Entry Points

- [../belgeler_v2/README.md](../belgeler_v2/README.md) — Primary Stage1 documentation entry point.
- [../belgeler_v2/dil/referans.md](../belgeler_v2/dil/referans.md) — Canonical syntax reference.
- [rehberler/ffi.md](rehberler/ffi.md) — Short FFI guide.
- [rehberler/bellek.md](rehberler/bellek.md) — Short manual memory guide.
- [rehberler/hata_yonetimi.md](rehberler/hata_yonetimi.md) — Short error-handling guide.
- [KAPSAM.md](KAPSAM.md) — Migration map from the older tree to the active tree.

---

## Transition Note

New Stage1 reference content should land in `belgeler_v2/` unless the user requests otherwise.

---

## Example (English MLP syntax)

```
numeric function add(a; b)
  return a + b
end function

function main()
  numeric result = add(3; 5)
  print(result)
end function
```
