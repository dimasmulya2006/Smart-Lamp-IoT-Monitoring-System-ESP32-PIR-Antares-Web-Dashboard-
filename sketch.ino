#include <WiFi.h>
#include <HTTPClient.h>

// Konfigurasi WiFi Wokwi
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// Kecepatan komunikasi Serial
#define SERIAL_DEBUG_BAUD 115200

// Konfigurasi Pin
#define LED_PIN 4
#define PIR_PIN 12

// Konfigurasi Akun & Perangkat Antares
#define ACCESSKEY "3b67fedff8e25fa8:55adcd187062f331"
#define applicationName "smart_lamp1"
#define deviceName "LampuPintar"

// GANTI URL INI SESUAI WEB LOKAL / NGROK KAMU
// Contoh lokal LAN: http://192.168.1.10:3000/api/update
// Contoh ngrok    : https://abc123.ngrok-free.app/api/update
const char* LOCAL_WEB_URL = "http://192.168.1.10:3000/api/update";

// Status terakhir PIR
int lastPirState = LOW;

void setup() {
  Serial.begin(SERIAL_DEBUG_BAUD);

  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.println("Sistem lampu pintar siap.");

  // Baca kondisi awal PIR
  lastPirState = digitalRead(PIR_PIN);

  if (lastPirState == HIGH) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Kondisi awal: LED ON");

    sendToLocalWeb(1);
    sendDataToAntares(1);
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("Kondisi awal: LED OFF");

    sendToLocalWeb(0);
    sendDataToAntares(0);
  }
}

void loop() {
  int currentPirState = digitalRead(PIR_PIN);

  // Kirim hanya ketika status PIR berubah
  if (currentPirState != lastPirState) {
    lastPirState = currentPirState;

    if (currentPirState == HIGH) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Gerakan terdeteksi. LED Wokwi ON.");

      // Kirim langsung ke dashboard lokal
      sendToLocalWeb(1);

      // Kirim juga ke Antares
      sendDataToAntares(1);

    } else {
      digitalWrite(LED_PIN, LOW);
      Serial.println("Tidak ada gerakan. LED Wokwi OFF.");

      // Kirim langsung ke dashboard lokal
      sendToLocalWeb(0);

      // Kirim juga ke Antares
      sendDataToAntares(0);
    }
  }
}

// Kirim status langsung ke web lokal
void sendToLocalWeb(int state) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String lampStatus = state == 1 ? "ON" : "OFF";
    String url = String(LOCAL_WEB_URL)
                 + "?pirState=" + String(state)
                 + "&lamp=" + lampStatus;

    Serial.println("Kirim ke web lokal:");
    Serial.println(url);

    http.begin(url);
    http.setTimeout(1500);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("Web lokal update. HTTP Code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Gagal update web lokal. Error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi tidak terhubung. Gagal kirim ke web lokal.");
  }
}

// Kirim data ke Antares
void sendDataToAntares(int state) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "https://platform.antares.id:8443/~/antares-cse/antares-id/"
                 + String(applicationName) + "/" + String(deviceName);

    http.begin(url);

    http.addHeader("X-M2M-Origin", ACCESSKEY);
    http.addHeader("Content-Type", "application/json;ty=4");
    http.addHeader("Accept", "application/json");

    http.setTimeout(3000);

    String lampStatus = state == 1 ? "ON" : "OFF";

    String payload = "{\"m2m:cin\":{\"con\":\"{\\\"pirState\\\":"
                     + String(state)
                     + ",\\\"lamp\\\":\\\""
                     + lampStatus
                     + "\\\"}\"}}";

    Serial.println("Kirim ke Antares:");
    Serial.println(payload);

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.print("Data terkirim ke Antares. HTTP Code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Gagal kirim ke Antares. Error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi terputus. Data Antares tidak dikirim.");
  }
}