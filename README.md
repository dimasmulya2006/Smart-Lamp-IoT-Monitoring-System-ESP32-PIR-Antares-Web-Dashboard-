# 💡 Smart Lamp IoT Monitoring System (ESP32 + PIR + Antares)

Project ini adalah sistem **Lampu Pintar (Smart Lamp)** berbasis **ESP32** yang menggunakan sensor **PIR (Motion Detection)** untuk mendeteksi gerakan, serta terintegrasi dengan:

- 🌐 Dashboard Web Lokal
- ☁️ Platform IoT **Antares**

---

## 🚀 Fitur Utama

- ✅ Deteksi gerakan otomatis (PIR Sensor)
- ✅ Lampu menyala/mati otomatis
- ✅ Update status ke dashboard web lokal
- ✅ Kirim data ke Antares (IoT Cloud)
- ✅ Monitoring real-time
- ✅ Hemat energi (lampu aktif saat ada gerakan)

---

## 🧠 Cara Kerja Sistem

1. Sensor PIR mendeteksi gerakan
2. Jika ada gerakan:
   - 💡 Lampu (LED) menyala
   - 🌐 Data dikirim ke web lokal
   - ☁️ Data dikirim ke Antares
3. Jika tidak ada gerakan:
   - 💡 Lampu mati
   - Status diperbarui ke sistem monitoring

---

## 🔌 Konfigurasi Hardware

| Komponen  | Pin ESP32 |
|----------|----------|
| LED      | 4        |
| PIR      | 12       |

---

## 📶 Konfigurasi WiFi

```cpp
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
