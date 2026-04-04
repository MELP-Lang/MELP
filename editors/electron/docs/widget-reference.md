# Widget Referans Kılavuzu

> MELP GUI Designer — Tüm Widget Türleri ve Özellikleri

---

## İçindekiler

1. [Button (Buton)](#button)
2. [Label (Etiket)](#label)
3. [TextBox (Metin Kutusu)](#textbox)
4. [CheckBox (Onay Kutusu)](#checkbox)
5. [RadioButton (Seçenek Düğmesi)](#radiobutton)
6. [ListBox (Liste Kutusu)](#listbox)
7. [ComboBox (Açılır Liste)](#combobox)
8. [Panel (Panel)](#panel)
9. [GroupBox (Grup Kutusu)](#groupbox)
10. [PictureBox (Resim Kutusu)](#picturebox)
11. [ProgressBar (İlerleme Çubuğu)](#progressbar)
12. [Ortak Özellikler](#ortak-özellikler)

---

## Ortak Özellikler

Her widget aşağıdaki temel özelliklere sahiptir:

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `id` | string | otomatik | Widget benzersiz kimliği (`btn1`, `lbl2`, vb.) |
| `name` | string | `""` | Widget görünen adı |
| `x` | integer | `10` | Yatay konum (piksel) |
| `y` | integer | `10` | Dikey konum (piksel) |
| `width` | integer | (türe göre) | Genişlik (piksel) |
| `height` | integer | (türe göre) | Yükseklik (piksel) |
| `visible` | boolean | `true` | Görünür mü? |
| `enabled` | boolean | `true` | Etkileşimli mi? |
| `foreColor` | string | `"#000000"` | Ön plan rengi (hex) |
| `backColor` | string | `"#FFFFFF"` | Arka plan rengi (hex) |
| `font` | string | `"Arial"` | Yazı tipi ailesi |
| `fontSize` | integer | `12` | Yazı tipi boyutu (pt) |
| `tabIndex` | integer | `0` | Sekme sırası |
| `tooltip` | string | `""` | Araç ipucu metni |

---

## Button

**Açıklama:** Kullanıcı tıklamasına tepki veren buton.

**Oluşturulan MLP Kodu:**
```
GUI_BUTTON_CREATE(pencere, id, "metin", x, y, genişlik, yükseklik)
GUI_BUTTON_ON_CLICK(id, onTikla)
```

### Özellikler

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `text` | string | `"Buton"` | Buton üzerindeki metin |
| `width` | integer | `100` | Genişlik |
| `height` | integer | `30` | Yükseklik |
| `onClick` | string | `""` | Tıklama olayı handler adı |

### Örnek

```json
{
  "type": "button",
  "id": "btn1",
  "text": "Kaydet",
  "x": 10, "y": 20,
  "width": 100, "height": 30,
  "onClick": "btnKaydetTiklandi"
}
```

---

## Label

**Açıklama:** Salt okunur metin görüntüleme alanı.

**Oluşturulan MLP Kodu:**
```
GUI_LABEL_CREATE(pencere, id, "metin", x, y, genişlik, yükseklik)
```

### Özellikler

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `text` | string | `"Etiket"` | Gösterilen metin |
| `width` | integer | `120` | Genişlik |
| `height` | integer | `20` | Yükseklik |
| `alignment` | string | `"left"` | Metin hizası: `left`, `center`, `right` |
| `wordWrap` | boolean | `false` | Metin taşmada sarma |

### Örnek

```json
{
  "type": "label",
  "id": "lbl1",
  "text": "Ad Soyad:",
  "x": 10, "y": 10,
  "width": 80, "height": 20
}
```

---

## TextBox

**Açıklama:** Kullanıcı metin girişi için tek satırlı alan.

**Oluşturulan MLP Kodu:**
```
GUI_TEXTBOX_CREATE(pencere, id, x, y, genişlik, yükseklik)
GUI_TEXTBOX_SET_TEXT(id, "başlangıç metni")
```

### Özellikler

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `text` | string | `""` | Başlangıç metni |
| `placeholder` | string | `""` | Yer tutucu metin |
| `width` | integer | `150` | Genişlik |
| `height` | integer | `24` | Yükseklik |
| `maxLength` | integer | `0` | Maksimum karakter (0 = sınırsız) |
| `readOnly` | boolean | `false` | Salt okunur |
| `passwordChar` | string | `""` | Şifre maskesi karakteri |
| `onChange` | string | `""` | Metin değişimi handler adı |
| `onEnter` | string | `""` | Enter tuşu handler adı |

### Örnek

```json
{
  "type": "textbox",
  "id": "txt1",
  "placeholder": "Adınızı girin...",
  "x": 100, "y": 10,
  "width": 200, "height": 24,
  "onChange": "txtAdDegisti"
}
```

---

## CheckBox

**Açıklama:** İki durumlu (işaretli/işaretsiz) onay kutusu.

**Oluşturulan MLP Kodu:**
```
GUI_CHECKBOX_CREATE(pencere, id, "metin", x, y, genişlik, yükseklik)
GUI_CHECKBOX_SET_CHECKED(id, 1)
```

### Özellikler

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `text` | string | `"CheckBox"` | Yanındaki metin |
| `checked` | boolean | `false` | İşaretli mi? |
| `width` | integer | `120` | Genişlik |
| `height` | integer | `22` | Yükseklik |
| `onChange` | string | `""` | Durum değişimi handler adı |

### Örnek

```json
{
  "type": "checkbox",
  "id": "chk1",
  "text": "Beni hatırla",
  "checked": false,
  "x": 10, "y": 50,
  "width": 130, "height": 22,
  "onChange": "chkHatirlaToggle"
}
```

---

## RadioButton

**Açıklama:** Grup içinde tek seçim yapılabilen seçenek düğmesi.

**Oluşturulan MLP Kodu:**
```
GUI_RADIOBUTTON_CREATE(pencere, id, "metin", x, y, genişlik, yükseklik)
GUI_RADIOBUTTON_SET_CHECKED(id, 1)
```

### Özellikler

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `text` | string | `"RadioButton"` | Yanındaki metin |
| `checked` | boolean | `false` | Seçili mi? |
| `group` | string | `"group1"` | Grup adı (aynı gruptakiler birbirini dışlar) |
| `width` | integer | `120` | Genişlik |
| `height` | integer | `22` | Yükseklik |
| `onChange` | string | `""` | Seçim değişimi handler adı |

---

## ListBox

**Açıklama:** Kaydırılabilir çok öğeli liste.

**Oluşturulan MLP Kodu:**
```
GUI_LISTBOX_CREATE(pencere, id, x, y, genişlik, yükseklik)
GUI_LISTBOX_ADD_ITEM(id, "öğe1")
GUI_LISTBOX_ADD_ITEM(id, "öğe2")
```

### Özellikler

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `items` | array | `[]` | Başlangıç öğe listesi |
| `selectedIndex` | integer | `-1` | Seçili öğe indeksi |
| `width` | integer | `150` | Genişlik |
| `height` | integer | `100` | Yükseklik |
| `multiSelect` | boolean | `false` | Çoklu seçim |
| `onSelect` | string | `""` | Seçim değişimi handler adı |

### Örnek

```json
{
  "type": "listbox",
  "id": "lst1",
  "items": ["Elma", "Armut", "Muz"],
  "x": 10, "y": 10,
  "width": 150, "height": 100,
  "onSelect": "lstMeyveSecildi"
}
```

---

## ComboBox

**Açıklama:** Açılır seçenek listesi.

**Oluşturulan MLP Kodu:**
```
GUI_COMBOBOX_CREATE(pencere, id, x, y, genişlik, yükseklik)
GUI_COMBOBOX_ADD_ITEM(id, "öğe1")
```

### Özellikler

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `items` | array | `[]` | Açılır liste öğeleri |
| `selectedIndex` | integer | `0` | Seçili öğe indeksi |
| `editable` | boolean | `false` | Kullanıcı metin girebilir mi? |
| `width` | integer | `150` | Genişlik |
| `height` | integer | `24` | Yükseklik |
| `onChange` | string | `""` | Seçim değişimi handler adı |

---

## Panel

**Açıklama:** Diğer widget'ları gruplamak için şeffaf kapsayıcı.

**Oluşturulan MLP Kodu:**
```
GUI_PANEL_CREATE(pencere, id, x, y, genişlik, yükseklik)
```

### Özellikler

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `width` | integer | `200` | Genişlik |
| `height` | integer | `150` | Yükseklik |
| `borderStyle` | string | `"none"` | Kenarlık: `none`, `single`, `raised`, `sunken` |
| `children` | array | `[]` | İçindeki widget ID'leri |

---

## GroupBox

**Açıklama:** Başlıklı kenarlıkla çevrilmiş gruplandırma konteyneri.

**Oluşturulan MLP Kodu:**
```
GUI_GROUPBOX_CREATE(pencere, id, "başlık", x, y, genişlik, yükseklik)
```

### Özellikler

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `text` | string | `"GroupBox"` | Başlık metni |
| `width` | integer | `200` | Genişlik |
| `height` | integer | `150` | Yükseklik |
| `children` | array | `[]` | İçindeki widget ID'leri |

---

## PictureBox

**Açıklama:** Resim (PNG/JPG/BMP) görüntüleme alanı.

**Oluşturulan MLP Kodu:**
```
GUI_PICTUREBOX_CREATE(pencere, id, x, y, genişlik, yükseklik)
GUI_PICTUREBOX_SET_IMAGE(id, "resim.png")
```

### Özellikler

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `imagePath` | string | `""` | Resim dosyası yolu |
| `sizeMode` | string | `"normal"` | `normal`, `stretch`, `zoom`, `center` |
| `width` | integer | `100` | Genişlik |
| `height` | integer | `100` | Yükseklik |

---

## ProgressBar

**Açıklama:** Yüzde tabanlı ilerleme göstergesi.

**Oluşturulan MLP Kodu:**
```
GUI_PROGRESSBAR_CREATE(pencere, id, x, y, genişlik, yükseklik)
GUI_PROGRESSBAR_SET_VALUE(id, 0, 100, değer)
```

### Özellikler

| Özellik | Tür | Varsayılan | Açıklama |
|---------|-----|------------|----------|
| `value` | integer | `0` | Güncel değer |
| `minimum` | integer | `0` | Minimum değer |
| `maximum` | integer | `100` | Maksimum değer |
| `style` | string | `"blocks"` | `blocks` veya `continuous` |
| `width` | integer | `200` | Genişlik |
| `height` | integer | `20` | Yükseklik |

---

## Notlar

- Tüm koordinatlar ebeveyn pencerenin sol üst köşesine göreli piksel değerleridir.
- `id` değerleri proje içinde benzersiz olmalıdır.
- `font` özelliği MLP runtime'ın desteklediği yazı tiplerini kullanmalıdır (Arial, Times New Roman, Courier New).
- Renkler `#RRGGBB` hex formatında belirtilir.
