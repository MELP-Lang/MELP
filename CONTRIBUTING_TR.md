# Katkida Bulunma Kilavuzu (Turkce)

MeLP'e katkida bulunmak istediginiz icin tesekkur ederiz. Bu belge, katki surecini hizli ve net sekilde aciklar.

---

## Katki Turleri

| Tur | Nereye |
|---|---|
| Hata bildirimi | Issues -> Bug Report |
| Ozellik istegi | Issues -> Feature Request |
| Belge duzeltmesi | PR acin |
| Ornek program | `examples/` altina ekleyip PR acin |
| Derleyici gelistirme | `stage0/v2` deposu (ayri repo) |

---

## Ornek Program Katkisi (Baslangic Icin)

`examples/` dizinine yeni bir program ekleyebilirsiniz:

```text
examples/
|- base/      # temel ornekler (degisken, dongu, fonksiyon)
|- cli/       # komut satiri araclari
|- stdlib/    # standart kutuphane kullanimi
`- games/     # kucuk oyunlar
```

Kurallar:
1. Program derlenmeli ve dogru cikti vermeli.
2. Dosya adi aciklayici olmali, ornek: `02_fibonacci.mlp`.
3. Ilk satirda kisa aciklama yorumu olmali, ornek: `-- Fibonacci dizisi`.

---

## PR Sureci

1. Repoyu fork edin.
2. Dal acin: `git checkout -b ornek-fibonacci`.
3. Degisikligi yapin ve test edin.
4. Commit atin: `git commit -m "examples: fibonacci dizisi eklendi"`.
5. Fork'unuza push edin: `git push origin ornek-fibonacci`.
6. PR acin.

---

## Commit Mesaji Kurallari

Asagidaki on eklerden birini kullanin:

```text
examples: fibonacci dizisi eklendi
docs: INSTALL.md turkce aciklama duzeltildi
fix: print() null string hatasi giderildi
feat: string interpolasyon eklendi
```

---

## Derleyici Katkisi

Derleyici kaynak kodu bu depoda degildir.

- Repo: `stage0/v2` (su an ozel/internal)
- Alfa doneminde derleyici cekirdegine dis katki acik degildir
- Beta sonrasinda derleyici kaynagi da acilacaktir

---

## Iletisim

- GitHub Issues: hata ve ozellik talepleri icin
- GitHub Discussions: genel sorular ve tartismalar icin

---

## Davranis Kurallari

Tum katkicilarin `CODE_OF_CONDUCT.md` dosyasindaki kurallara uymasi beklenir.

---

## Kaynak

Bu metin, `/home/pardus/PROJELER/MELP/LLVM/STAGE1/CONTRIBUTING.md` iceriginin Turkce karsiligi baz alinarak olusturulmustur.
