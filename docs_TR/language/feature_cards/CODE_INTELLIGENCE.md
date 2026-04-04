# CODE_INTELLIGENCE (Kod Zekasi) — Ozellik Karti

**Ozellik:** Kod analizi ve refactoring ozellikleri  
**Durum:** 🔴 TODO (Faz 4.2)  
**Sozdizimi Surumu:** Yok (IDE ozelligi)

---

## 🎯 PLANLANAN OZELLIKLER

### Sembol Yeniden Adlandirma
```mlp
numeric function eskiAd()  -- yeniAd olarak degistir
    return 42
end function

-- Tum referanslar otomatik guncellenir
```

### Fonksiyon Cikartma (Extract Function)
```mlp
-- Once:
function main()
    numeric x = 10
    numeric y = 20
    print(x + y)
end function

-- Sonra:
numeric function topla(numeric a; numeric b)
    return a + b
end function

function main()
    print(topla(10; 20))
end function
```

### Kod Lensi
- Referans sayisini goster
- Test kapsamini goster
- Performans ipuclari goster

---

## 📌 DURUM NOTU

Bu ozellik IDE katmani (LSP / Electron / VSIX) tarafindan saglanacak; dil sozdizimini etkilemez. Faz 4.2 kapsaminda tanimlanmistir.

---

**Faz:** 4.2 (7. Ay)  
**Ilgili:** LSP sunucu (`melp_lsp`), VSIX, Electron IDE
