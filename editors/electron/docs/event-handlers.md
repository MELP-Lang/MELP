# Event Handler Yazma Rehberi

> MELP GUI Designer'da olay yöneticilerini nasıl kullanırsınız?

---

## İçindekiler

1. [Giriş](#giriş)
2. [Olay Türleri](#olay-türleri)
3. [Handler Tanımlama](#handler-tanımlama)
4. [Widget'a Handler Bağlama](#widgeta-handler-bağlama)
5. [Örnekler](#örnekler)
6. [İpuçları ve En İyi Uygulamalar](#ipuçları)

---

## Giriş

MELP GUI Designer'da her etkileşimli widget, kullanıcı eylemlerine yanıt veren **event handler** (olay yöneticisi) fonksiyonlarına sahip olabilir. Bu fonksiyonlar MLP kodunuzda tanımlanır ve MELP editörü tarafından otomatik olarak iskelet oluşturulur.

---

## Olay Türleri

### onClick — Tıklama Olayı

**Destekleyen widget'lar:** Button, Label, PictureBox, Panel

Kullanıcı widget'a sol tıkladığında tetiklenir.

```mlp
' Özellik panelinde onClick = "btnKaydetTiklandi" olarak ayarlandıysa:
fonksiyon btnKaydetTiklandi()
    ' Buraya kodunuzu yazın
    GUI_LABEL_SET_TEXT(lblDurum, "Kaydedildi!")
bitfonksiyon
```

---

### onChange — Değer Değişim Olayı

**Destekleyen widget'lar:** TextBox, CheckBox, RadioButton, ComboBox

Widget değeri değiştiğinde tetiklenir.

```mlp
' TextBox için onChange = "txtAdDegisti"
fonksiyon txtAdDegisti()
    degisken metin = GUI_TEXTBOX_GET_TEXT(txtAd)
    GUI_LABEL_SET_TEXT(lblOnizleme, metin)
bitfonksiyon

' CheckBox için onChange = "chkTumunuSecToggle"
fonksiyon chkTumunuSecToggle()
    eger GUI_CHECKBOX_GET_CHECKED(chkTumuSec) = 1 ise
        GUI_CHECKBOX_SET_CHECKED(chk1, 1)
        GUI_CHECKBOX_SET_CHECKED(chk2, 1)
    degilse
        GUI_CHECKBOX_SET_CHECKED(chk1, 0)
        GUI_CHECKBOX_SET_CHECKED(chk2, 0)
    biteger
bitfonksiyon
```

---

### onSelect — Seçim Olayı

**Destekleyen widget'lar:** ListBox, ComboBox

Listeden bir öğe seçildiğinde tetiklenir.

```mlp
' ListBox için onSelect = "lstSehirSecildi"
fonksiyon lstSehirSecildi()
    degisken indeks = GUI_LISTBOX_GET_SELECTED(lstSehir)
    eger indeks >= 0 ise
        degisken secilenMetin = GUI_LISTBOX_GET_ITEM_TEXT(lstSehir, indeks)
        GUI_LABEL_SET_TEXT(lblSehirAdi, secilenMetin)
    biteger
bitfonksiyon
```

---

### onEnter — Enter Tuşu Olayı

**Destekleyen widget'lar:** TextBox

Metin kutusuna odaklanmışken Enter tuşuna basıldığında tetiklenir.

```mlp
' TextBox için onEnter = "txtAramaEnterBasildi"
fonksiyon txtAramaEnterBasildi()
    degisken aramaMetni = GUI_TEXTBOX_GET_TEXT(txtArama)
    ' Aramayı başlat
    handleArama(aramaMetni)
bitfonksiyon
```

---

### onLoad — Yükleme Olayı (Pencere)

**Destekleyen widget'lar:** Ana pencere (`app_start` bloğunda)

Uygulama penceresi oluşturulduğunda bir kez tetiklenir. `app_start` bloğunda başlangıç kodunu buraya yazın.

```mlp
app_start "BenimUygulamam"
    ' Pencere oluştur
    degisken pencere = GUI_WINDOW_CREATE("Ana Pencere", 800, 600)
    
    ' Widget'ları oluştur
    degisken btnMerhaba = GUI_BUTTON_CREATE(pencere, "btn1", "Merhaba!", 10, 10, 100, 30)
    GUI_BUTTON_ON_CLICK(btn1, btnMerhabaTiklandi)
    
    ' Pencereyi göster
    GUI_WINDOW_SHOW(pencere)
    GUI_MAIN_LOOP()
end_app
```

---

## Handler Tanımlama

### Yöntem 1: Özellik Panelinden

1. Tasarım alanında bir widget seçin
2. Sağ taraftaki **Özellikler** panelini açın
3. İlgili olay alanına (`onClick`, `onChange`, vb.) bir fonksiyon adı yazın
4. **Kod editörüne** geçin — MELP otomatik olarak iskelet oluşturur:

```mlp
fonksiyon btnKaydetTiklandi()
    ' TODO: tıklama işlemini buraya yazın
bitfonksiyon
```

### Yöntem 2: Doğrudan Kod Yazma

Kod editöründe handler fonksiyonunu kendiniz yazabilirsiniz:

```mlp
fonksiyon btnHesaplaTiklandi()
    degisken sayi1 = sayiye_cevir(GUI_TEXTBOX_GET_TEXT(txtSayi1))
    degisken sayi2 = sayiye_cevir(GUI_TEXTBOX_GET_TEXT(txtSayi2))
    degisken sonuc = sayi1 + sayi2
    GUI_LABEL_SET_TEXT(lblSonuc, sayiden_yazi(sonuc))
bitfonksiyon
```

---

## Widget'a Handler Bağlama

Tasarımcı otomatik olarak aşağıdaki MLP bağlama çağrılarını üretir:

| Widget Türü | Üretilen Bağlama Kodu |
|-------------|----------------------|
| Button → onClick | `GUI_BUTTON_ON_CLICK(id, handlerAdi)` |
| TextBox → onChange | `GUI_TEXTBOX_ON_CHANGE(id, handlerAdi)` |
| TextBox → onEnter | `GUI_TEXTBOX_ON_ENTER(id, handlerAdi)` |
| CheckBox → onChange | `GUI_CHECKBOX_ON_CHANGE(id, handlerAdi)` |
| RadioButton → onChange | `GUI_RADIOBUTTON_ON_CHANGE(id, handlerAdi)` |
| ListBox → onSelect | `GUI_LISTBOX_ON_SELECT(id, handlerAdi)` |
| ComboBox → onChange | `GUI_COMBOBOX_ON_CHANGE(id, handlerAdi)` |

---

## Örnekler

### Örnek 1: Basit Hesap Makinesi

```mlp
app_start "HesapMakinesi"
    degisken pnc = GUI_WINDOW_CREATE("Hesap Makinesi", 300, 200)
    
    degisken txtA = GUI_TEXTBOX_CREATE(pnc, "txtA", 10, 10, 80, 24)
    degisken lblArti = GUI_LABEL_CREATE(pnc, "lblArti", "+", 100, 12, 20, 20)
    degisken txtB = GUI_TEXTBOX_CREATE(pnc, "txtB", 130, 10, 80, 24)
    degisken btnEsit = GUI_BUTTON_CREATE(pnc, "btnEsit", "=", 220, 10, 40, 24)
    degisken lblSonuc = GUI_LABEL_CREATE(pnc, "lblSonuc", "0", 10, 50, 250, 24)
    
    GUI_BUTTON_ON_CLICK(btnEsit, hesapla)
    
    GUI_WINDOW_SHOW(pnc)
    GUI_MAIN_LOOP()
end_app

fonksiyon hesapla()
    degisken a = sayiye_cevir(GUI_TEXTBOX_GET_TEXT(txtA))
    degisken b = sayiye_cevir(GUI_TEXTBOX_GET_TEXT(txtB))
    GUI_LABEL_SET_TEXT(lblSonuc, sayiden_yazi(a + b))
bitfonksiyon
```

---

### Örnek 2: Giriş Formu Doğrulama

```mlp
fonksiyon btnGirisYapTiklandi()
    degisken kullanici = GUI_TEXTBOX_GET_TEXT(txtKullanici)
    degisken sifre = GUI_TEXTBOX_GET_TEXT(txtSifre)
    
    eger uzunluk(kullanici) = 0 ise
        GUI_LABEL_SET_TEXT(lblHata, "Kullanıcı adı boş olamaz!")
        GUI_WIDGET_SET_VISIBLE(lblHata, 1)
        dondur
    biteger
    
    eger uzunluk(sifre) < 6 ise
        GUI_LABEL_SET_TEXT(lblHata, "Şifre en az 6 karakter olmalı!")
        GUI_WIDGET_SET_VISIBLE(lblHata, 1)
        dondur
    biteger
    
    GUI_WIDGET_SET_VISIBLE(lblHata, 0)
    ' Giriş başarılı — ana ekrana geç
    anaEkranaGit()
bitfonksiyon
```

---

### Örnek 3: Dinamik Liste

```mlp
fonksiyon btnEkleListeyeTiklandi()
    degisken yeniOge = GUI_TEXTBOX_GET_TEXT(txtYeniOge)
    eger uzunluk(yeniOge) > 0 ise
        GUI_LISTBOX_ADD_ITEM(lstOgeler, yeniOge)
        GUI_TEXTBOX_SET_TEXT(txtYeniOge, "")
    biteger
bitfonksiyon

fonksiyon btnSilTiklandi()
    degisken indeks = GUI_LISTBOX_GET_SELECTED(lstOgeler)
    eger indeks >= 0 ise
        GUI_LISTBOX_REMOVE_ITEM(lstOgeler, indeks)
    biteger
bitfonksiyon

fonksiyon lstOgelerSecildi()
    degisken indeks = GUI_LISTBOX_GET_SELECTED(lstOgeler)
    eger indeks >= 0 ise
        degisken metin = GUI_LISTBOX_GET_ITEM_TEXT(lstOgeler, indeks)
        GUI_LABEL_SET_TEXT(lblSecili, "Seçili: " + metin)
    biteger
bitfonksiyon
```

---

## İpuçları

### 1. Handler Adlandırma Kuralı

Tutarlı bir adlandırma kullanın:

```
[widgetId][Olay]     →  btnKaydet + Tiklandi  = btnKaydetTiklandi
[widgetId]On[Olay]   →  txtAd     + OnChange  = txtAdOnChange
handle[Eylem]        →  handleKaydet, handleArama
```

### 2. Boş Handler Kontrolü

```mlp
fonksiyon txtAramaDegisti()
    degisken metin = GUI_TEXTBOX_GET_TEXT(txtArama)
    eger uzunluk(metin) = 0 ise
        listeTemizle()
        dondur
    biteger
    aramaSonuclariniGuncelle(metin)
bitfonksiyon
```

### 3. Birden Fazla Widget — Tek Handler

Aynı handler birden fazla widget'a bağlanabilir:

```mlp
' Özellik panelinde: chk1.onChange = "secimGuncelle"
'                    chk2.onChange = "secimGuncelle"
'                    chk3.onChange = "secimGuncelle"

fonksiyon secimGuncelle()
    degisken toplam = 0
    eger GUI_CHECKBOX_GET_CHECKED(chk1) = 1 ise toplam = toplam + 1 biteger
    eger GUI_CHECKBOX_GET_CHECKED(chk2) = 1 ise toplam = toplam + 1 biteger
    eger GUI_CHECKBOX_GET_CHECKED(chk3) = 1 ise toplam = toplam + 1 biteger
    GUI_LABEL_SET_TEXT(lblSeciliSayisi, sayiden_yazi(toplam) + " seçildi")
bitfonksiyon
```

### 4. Widget'ı Handler İçinde Devre Dışı Bırakma

Uzun işlemler sırasında butonu devre dışı bırakın:

```mlp
fonksiyon btnIslemTiklandi()
    GUI_WIDGET_SET_ENABLED(btnIslem, 0)
    GUI_LABEL_SET_TEXT(lblDurum, "İşleniyor...")
    
    ' Uzun işlemi yap
    uzunIslemYap()
    
    GUI_LABEL_SET_TEXT(lblDurum, "Tamamlandı!")
    GUI_WIDGET_SET_ENABLED(btnIslem, 1)
bitfonksiyon
```

---

## Oluşturulan Kod Yapısı

MELP Designer bir proje kaydettiğinde/dışa aktardığında kod şu yapıya sahiptir:

```mlp
app_start "ProjAdı"
    ' 1. Pencere oluştur
    degisken pnc = GUI_WINDOW_CREATE("Başlık", genişlik, yükseklik)
    
    ' 2. Widget'ları oluştur (tasarımcı sırası)
    degisken btn1 = GUI_BUTTON_CREATE(pnc, "btn1", "Tamam", 10, 10, 80, 30)
    ...
    
    ' 3. Event handler'ları bağla
    GUI_BUTTON_ON_CLICK(btn1, btnTamamTiklandi)
    ...
    
    ' 4. Pencereyi göster ve döngüye gir
    GUI_WINDOW_SHOW(pnc)
    GUI_MAIN_LOOP()
end_app

' === Event Handler'lar ===

fonksiyon btnTamamTiklandi()
    ' TODO
bitfonksiyon
```
