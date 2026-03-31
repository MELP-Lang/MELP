# MELP Programlama Dili

> **MELP** — *Multi Language Programming* kısaltmasından türetilmiştir.  
> Küçük **e**, "eritme kazanı" anlamını simgeler: farklı dilleri ve sözdizimlerini tek bir derleme sürecinde eritir.

> Kendi doğal dilinizde yazın. GC yok, borrow checker yok. Memory safe.  
> Tek derlenmiş dil — binary'e sahip olmak mantığa sahip olmak **anlamına gelmez**.

> *"Rust makes state safe. MELP questions whether you need state at all."*

![Lisans](https://img.shields.io/badge/lisans-MIT-blue)
![Platform](https://img.shields.io/badge/platform-Linux%20x86__64-lightgrey)
![Durum](https://img.shields.io/badge/durum-alfa-orange)
![LLVM](https://img.shields.io/badge/backend-LLVM-blueviolet)

---

## Neden MELP?

Bugün bir dil seçerken üç şeyden birini feda etmek zorundasınız:

| | Hız | Memory Safety | Basitlik |
|---|:---:|:---:|:---:|
| C / C++ | ✅ | ❌ | ❌ |
| Java / Python | ❌ (GC) | ✅ | ✅ |
| Rust | ✅ | ✅ | ❌ (borrow checker) |
| **MELP** | ✅ | ✅ | ✅ |

MELP, ownership sorununu *takip ederek* değil, persistent state'i minimize ederek çözer. Sahip olunacak bir şey yoksa sahiplik problemi de doğmaz.

| Problem | Rust | MELP |
|---|---|---|
| Memory leak | Ownership + Drop | Persistent state yok → sızdıracak bir şey yok |
| Use-after-free | Borrow checker | Tutulan referans yok |
| Double-free | Tek sahiplik kuralı | Ownership modeli gerekmiyor |
| Dangling pointer | Lifetime sistemi | Uzun ömürlü pointer yok |
| State geçiş maliyeti | Veri kopyalanır, bağımlılıklar taşınır | 4 byte FBO indeksi — veri seyahat etmez |

> *"State is not forbidden. It's discouraged by design."*

> *"Rust: You own the car, but the rules are strict. MELP: You never own the car. You just ride."*

Ve buna ek olarak hiçbir derlenmiş dilde bulunmayan bir özellik daha geliyor: **yapısal binary güvenliği**.

---

## Yapısal Binary Güvenliği — EOK *(Beta)*

> *"C, C++, Go, Rust, Zig dahil hiçbir derlenmiş dilde bu düzeyde yapısal güvenlik yoktur.  
> Diğer dillerde binary'e sahip olan kişi mantığa da sahiptir. MELP'te bu geçerli değildir."*

**EOK (Encryptic OK)**, MELP'in derleme mimarisine sıfırdan entegre edilmiş bir güvenlik katmanıdır. Sonradan eklenen bir obfuscator değil; dilin çalışma biçiminin doğal sonucudur.

### Nasıl Çalışır?

MELP'in FBO (Frontend-Backend Optimization) katmanı, semantiği değil **semantiğin indeksini** binary'e yazar. Bu, temel bir asimetri yaratır:

```
Klasik derleme:
  Kaynak → Binary [mantık + veri + kontrol akışı → IDA Pro ile okunabilir]

MELP (FBO tabanlı):
  Kaynak → Binary [sadece indeks numaraları → çözücü + anahtar olmadan anlamsız]
```

| Saldırı Senaryosu | Sonuç |
|---|---|
| Binary ele geçirildi, anahtar yok | Yürütülemez. İndeksler anlamsız sayılar. |
| Memory dump alındı (çalışırken) | Bellekte yalnızca indeksler var; mantık hiç açığa çıkmaz. |
| IDA Pro / Ghidra ile analiz | Kontrol akışı okunaksız; strateji tablosu şifreli. |

### EFBO — Kuantum Dirençli Koruma *(Araştırma)*

EOK'un ileri katmanı olan **EFBO**, FBO dispatch tablosunu post-quantum şifreleme ile koruyarak kuantum bilgisayar destekli ters mühendislik saldırılarına karşı dayanıklılık sağlamayı hedefler. Savunma sanayi, bankacılık ve kritik altyapı yazılımları için tasarlanmıştır.

> EOK Beta aşamasında devreye girecektir. Mevcut alfa sürümü FBO altyapısını içermektedir.

---

## Öne Çıkan Özellikler

**Çok Sözdizimli Yazım** — MELP'i kendi alışkın olduğunuz sözdizim stiliyle yazın. Normalizer, C, Python, Go, Rust, VBNet veya native MELP stilini aynı binary'e derler. Ayrıca tüm anahtar kelimeler Türkçe, İngilizce, Rusça, Japonca dahil herhangi bir doğal dilde yazılabilir. Başka hiçbir derlenmiş dil bunu yerleşik olarak sunmaz.

```melp
-- C stili
function topla(numeric a, numeric b) {
    return a + b;
}

-- Python stili
def topla(numeric a, numeric b):
    return a + b

-- Native MELP stili (Türkçe)
numeric fonksiyon topla(numeric a; numeric b)
    döndür a + b
fonksiyon sonu
```

**Memory Safe, GC'siz, Annotation'sız** — `unsafe` blok yok, lifetime annotation yok, borrow checker kavramı yok. Bellek güvenliği stateless mimari sayesinde tasarım gereği sağlanır.

**FBO Tablosu: 4.3 Milyar Girişlik Strateji Havuzu** *(Beta)* — FBO, 32-bit indeks alanıyla 4.294.967.296 girişlik bir strateji deposudur. Tüm math kütüphanesi, tüm string operasyonları, tüm runtime kararları buraya yüklenir. Çağıran taraf **sadece indeksi** kullanır; kütüphane seyahat etmez.

```
Geleneksel yaklaşım:
  modül A → modül B:  [fonksiyon kodu + bağımlılıklar + veri]

MELP FBO:
  modül A → modül B:  0x002A  →  "math.sqrt stratejisini kullan"
```

**"MELP'te state, değerin kendisi değil; değere giden yoldur. Ve yol her zaman 4 byte."**

**State: FBO'nun Çalışma Zamanı Uzantısı** *(Beta)* — Geleneksel dillerde state geçişi maliyetlidir: veri kopyalanır, bağımlılıklar taşınır, bellek sızar. MELP'te state FBO tablosunun bir indeksidir — modüller arası geçiş 4 byte, erişim O(1) L1 cache hit, kütüphane seyahat etmez. **Performans maliyeti neredeyse sıfır.** Kalan tek maliyet kasıtlıdır: `state.init()` ve `state.close()` ile state nerede başlar nerede biter *görünür* olur. MELP state'i yasaklamaz, onu **görünür** kılar. *(Planlanan: Stage 7)*

```melp
import state

numeric function sayac()
    state.init()
    numeric n = state.get("n"; 0)
    state.set("n"; n + 1)  -- depolar: indeks, değerin kendisi değil
    state.close()
    return n
end function
```

**FBO — 4.3 Milyar Girişlik Strateji Havuzu** *(Beta)* — Derleme hattının omurgası. Tüm kütüphaneler, tip operasyonları ve optimizasyon kararları 32-bit indeksli bu havuzda yaşar. Modüller arası transfer = 4 byte indeks; kütüphane seyahat etmez. EOK güvenlik katmanının ve ROK çalışma zamanı optimizasyonunun matematiksel temelidir.

**Gerçek Binary Üretimi** — MELP sözdizimsel bir deney değildir. LLVM IR üzerinden native binary üretir; optimize edilmiş, taşınabilir, gerçek ürün kodu.

**Platform Bağımsız** *(Beta)* — Bir kere yaz, her yerde çalıştır. LLVM altyapısı sayesinde aynı kaynak kod Linux, macOS ve Windows için native binary üretir. Çapraz derleme için ayrı araç gerekmez.

**Struct + Fonksiyon, Class Yok** — Veri struct ile, davranış bağımsız fonksiyonlarla tanımlanır. OOP hiyerarşisi olmadan aynı modelleme gücü, çok daha az karmaşıklık.

**Spawn + Channel** — Yerleşik eşzamanlılık modeli. Thread oluşturmak için kütüphane gerekmez.

**Modüler Mimari** — Her modül tek sorumluluğa sahiptir; merkezi orkestratör yasaktır. Derleme sırası otomatik dependency graph'tan belirlenir.

**Öz-Derleme Hedefi** — MELP'in derleyicisi sonunda MELP ile yazılacak. v1.0'da derleyici kendini derleyecek.

**OK Katmanı — STO · SAO · FBO** — Üç aşamalı derleme zamanı optimizasyon sistemi. STO (Smart Type Optimization) tip çıkarımını, SAO (Semantic-Aware Optimization) erişim örnttilerini analiz eder; FBO semantii 4-byte indekse indirger. Agresif ve güvenli derleme zamanı optimizasyonu.

**Yerleşik `debug` Anahtar Kelimesi** — Ayrı kütüphane ya da araç gerekmez. `debug` dil düzeyindedir; `--release` derlemesinde sıfır maliyet — kod ve sembol üretilmez.

**Ölü Modül Felsefesi** — Modüller varsayılan olarak ölüdür: kendi başına kaynak tahsis etmez, bekletek kaplamaz. Kullanan modül diriltir ve öldürür — atıl kaynak sıfır, GC gereksizdir.

**Yerel Sayı Notasyonu** — `a = 3,45` ve `b = 1.000.000,45` doğrudan yazılabilir. Türkçe virgüllü ondalik ve noktalı binlik ayracı sözdizimi düzeyinde desteklenir.

**Sınırsız Hassasiyet — 3 Tip, Sıfır Tip Derdi** — MELP'te yalnızca üç tip vardır:
- `numeric` → BigDecimal — 1 bit'ten binlerce byte'a kadar her sayı
- `string` → BigString — herhangi bir uzunlukta metin
- `boolean` — doğru / yanlış

Kullanıcı "bu değişkene kaç byte ayırmalıyım?" derdine düşmez. STO (Smart Type Optimization) derleme zamanında değişkenin gerçek kullanımını analiz eder ve bellekte minimum yer kaplayacak şekilde temsil eder. IEEE 754 kısıtı yoktur; finansal ve akademik hesaplamalar RAM sınırına kadar %100 doğrulukla çalışır.

**Adlandırılmış Kapsamlar (`scope`)** — `scope ad` ile iç içe bloklardan temiz koşullu çıkış. Ayrı bayrak değişkeni ya da exception gerektirmez.

---

## Merhaba Dünya

```melp
function main()
    print("Merhaba Dünya!")
end function
```

Türkçe yazım da desteklenir:

```melp
fonksiyon giriş()
    yaz("Merhaba Dünya!")
fonksiyon sonu
```

Aynı program, aynı derleyici, farklı doğal dil.

---

## Hemen Dene

Tarayıcı tabanlı Web IDE ile MELP'i kurulum yapmadan deneyebilirsiniz:

**[melp-lang.github.io/melp-ide-web/](https://melp-lang.github.io/melp-ide-web/)**

---

## Hızlı Başlangıç (VS Code)

1. [Releases](https://github.com/MELP-Lang/MELP/releases) sayfasından `melp-ide-*.vsix` indirin
2. VS Code: `Ctrl+Shift+P` → **Install from VSIX** → dosyayı seçin
3. `.mlp` dosyanızı açın ve uzantı üzerinden çalıştırın

Derleyici uzantının içine gömülüdür — ayrıca kurulum gerekmez.

---

## Hızlı Başlangıç (Yerel)

```bash
bin/run_melp ornek.mlp
bin/run_melp ornek.mlp --ir
```

---

## Sözdizimi Turu

> **Not:** MELP'te birden fazla parametre `;` ile ayrılır — virgül değil.  
> Bu bilinçli bir tasarım kararıdır: virgül string içinde belirsizliğe yol açar.

```melp
-- Temel tipler
numeric x      = 42
numeric pi     = 3,14           -- Virgül ondalik ayırıcıdır
numeric miktar = 1.000.000,45   -- Noktalı binlik ayraç
string isim = "MELP"
boolean aktif = true

-- Fonksiyon
numeric function topla(a; b)
    return a + b
end function

-- String interpolation
string mesaj = "Merhaba {isim}!"

-- Array
numeric[] sayilar = [10; 20,56; 30; 40; 50] -- burada 20,56 bilinçli olarak ondalık sayı yazılmıştır.
for each n in sayilar
    print(n)
end for

-- for downto
for i = 5 downto 1
    print(i)
end for

-- Hashmap
hashmap<string; numeric> puan = {}
hashmap_set(puan; "Ali"; 95)

-- Generics
T function kimlik<T>(T x)
    return x
end function

-- Lambda + Higher-Order Function
numeric[] ikiler = array_map(sayilar; lambda(x) -> x * 2)

-- Closure
function sayac_olustur(numeric baslangic)
    numeric n = baslangic
    return lambda()
        n = n + 1
        return n
    end lambda
end function

-- Spawn + Channel
numeric ch = channel_create()
spawn
    send(ch; 42)
end spawn
receive(ch)

-- Try / Catch
try
    numeric sonuc = bol(10; 0)
catch hata
    print("Hata: {hata}")
end try

-- Struct + Enum + Match
struct Nokta
    numeric x
    numeric y
end struct

enum Renk
    Kirmizi
    Yesil
    Mavi
end enum

match renk
case Kirmizi
    print("kırmızı")
end case
end match

-- Event
event baslangicta()
    print("uygulama başladı")
end event

-- Adlandırılmış kapsam (iç içe döngüden temiz çıkış)
scope islem
    for i = 0 to 100
        if bul(i) then
            exit scope islem
        end if
    end for
end scope

-- Debug (--release ile sıfır maliyet)
debug
```

---

## Çok Dilli Destek

```
program.mlp      ← İngilizce (kanonik)
program.tr.mlp   ← Türkçe → normalizer tarafından dönüştürülür
program.ru.mlp   ← Rusça → aynı şekilde
```

Normalizer, her dil varyantını derleme öncesi kanonik forma çevirir. Dil ekleme sadece bir `keywords.json` dosyası gerektirir. Kanonik kaynak: `ORTAK/dil/tr/keywords.json`.

### Sözdizimi Stilleri

MELP aynı zamanda birden fazla sözdizimi stilini destekler:

| Stil | Örnek |
|---|---|
| Native MELP | `end function` |
| C-Stili | `function main() { }` |
| Python-Stili | `def main():` |
| Go-Stili | `func main() { }` |
| VB.NET-Stili | `End Function` |

---

## Editör Seçenekleri

| Araç | Ne Zaman |
|---|---|
| Web IDE | İlk kez deneme, eğitim, gösterim |
| VS Code uzantısı | Düzenli yazım, IntelliSense, hata vurgulama |
| GUI IDE *(alfa)* | Görsel ortam, Linux odaklı |

Tüm editörler aynı derleyiciyi kullanır. Editörler arası geçiş sıfır maliyetlidir.

---

## Örnekler

[examples/](examples/) dizininde 40+ örnek program bulunur.

| Klasör | Konular |
|---|---|
| `base/` | Temel sözdizimi, kontrol akışı, struct, enum |
| `cli/` | Argüman ayrıştırma, dosya G/Ç |
| `stdlib/` | Matematik, string, sıralama |
| `games/` | Sayı tahmin oyunu, metin macerası |

---

## Yol Haritası

| Kilometre Taşı | Sürüm |
|---|---|
| Alfa: derleyici + editörler + 40+ örnek | ✅ Alfa |
| Topluluk Açılışı: GitHub + melp.dev + katkı kılavuzu | 🔧 Alfa |
| macOS/Windows + LSP/DAP + **EOK güvenlik katmanı** | 🔴 Beta |
| OK mimarisi: COK-lite → FBO_table → ROK entegrasyonu | 🔴 Beta |
| Derleyici tamamen MELP ile yazılmış (öz-derleme) | 🔴 RC |
| EFBO — kuantum dirençli binary koruma | 🔴 Kararlı Sürüm |

---

## Platform Desteği

| Platform | Alfa | Beta |
|---|---|---|
| Linux x86_64 | ✅ | ✅ |
| macOS | — | 🔄 |
| Windows | — | 🔄 |

---

## Lisans

[MIT](LICENSE)
