# The Cubes
## Kotak Penyimpanan Cerdas Berbasis Iot & AI

Proyek ini adalah Sistem Manajemen Loker yang menggunakan mikrokontroler ESP32. Proyek ini mengintegrasikan WiFi, bot Telegram, sensor sidik jari, sensor getaran, relay untuk kunci pintu, dan layar LCD.

## Fitur

- **Konektivitas WiFi**: Terhubung ke jaringan WiFi.
- **Bot Telegram**: Mengirim dan menerima pesan melalui bot Telegram.
- **Sensor Sidik Jari**: Untuk otentikasi pengguna.
- **Sensor Getaran**: Mendeteksi gangguan.
- **Relay**: Mengontrol kunci pintu.
- **Layar LCD**: Menyediakan antarmuka pengguna.

## Kebutuhan Perangkat Keras

- Mikrokontroler ESP32
- Sensor sidik jari
- Sensor getaran
- Relay
- Layar LCD (I2C)
- Buzzer
- LED

## Perpustakaan yang Digunakan

- WiFi
- WiFiClientSecure
- UniversalTelegramBot
- Adafruit_Fingerprint
- Wire
- LiquidCrystal_I2C
- SPIFFS

## Pengaturan

1. **Kredensial WiFi**: Perbarui `WIFI_SSID` dan `WIFI_PASSWORD` dengan kredensial jaringan WiFi Anda.
   ```cpp
   #define WIFI_SSID "SSID_Anda"
   #define WIFI_PASSWORD "PASSWORD_Anda"
   ```

2. **Token Bot Telegram**: Dapatkan token bot dari BotFather dan perbarui `BOT_TOKEN`.
   ```cpp
   #define BOT_TOKEN "Token_Bot_Anda"
   ```

3. **ID Chat Telegram**: Perbarui `CHAT_ID` dengan ID chat Telegram Anda.
   ```cpp
   #define CHAT_ID "ID_Chat_Anda"
   ```

4. **Sensor Sidik Jari**: Pastikan sensor sidik jari terhubung ke pin yang benar.

5. **Sensor Getaran**: Hubungkan sensor getaran ke pin GPIO yang sesuai.

6. **Relay**: Hubungkan relay ke pin GPIO yang sesuai untuk kunci pintu.

7. **Layar LCD**: Pastikan layar LCD terhubung dengan benar dan perbarui alamatnya jika perlu.
   ```cpp
   #define LCD_ADDRESS 0x27
   ```

## Kode

Kode utama disediakan dalam file `loker_man_brebes.ino`. Berikut adalah gambaran singkat dari fungsi utama:

- `setup()`: Menginisialisasi WiFi, bot Telegram, sensor, relay, dan LCD.
- `loop()`: Menangani pesan yang masuk, pembacaan sensor, dan mengontrol relay.
- `handleNewMessages()`: Memproses pesan baru yang diterima melalui bot Telegram.

## Penggunaan

1. Unggah kode ke ESP32 Anda.
2. Buka Serial Monitor untuk memeriksa status koneksi dan pesan debug.
3. Berinteraksi dengan sistem melalui bot Telegram.

## Pemecahan Masalah

- Pastikan semua koneksi aman dan benar.
- Periksa kredensial WiFi dan pastikan jaringan dapat dijangkau.
- Verifikasi token bot Telegram dan ID chat.

## Lisensi

Proyek ini dilisensikan di bawah Lisensi MIT.
