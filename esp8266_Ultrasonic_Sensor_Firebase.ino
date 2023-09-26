//source code from er4files github

#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

// Konfigurasi Firebase
#define API_KEY "AIzaSyBpjI0g0YeWkaLW4EUHmcpv3xOshEneF_A" // Ganti dengan API Key Firebase Anda
#define DATABASE_URL "https://realtimeultrasonik.firebaseio.com/" // URL Realtime Database Firebase Anda

// Kredensial WiFi
const char* ssid = "NAMA WIFI";       // Ganti dengan SSID WiFi Anda
const char* password = "PASWORD WIFI"; // Ganti dengan kata sandi WiFi Anda

// Pin-penunjuk sensor Ultrasonik
#define TRIGGER_PIN 14  // GPIO14 sesuai dengan D5 pada Wemos D1 Mini
#define ECHO_PIN 12     // GPIO12 sesuai dengan D6 pada Wemos D1 Mini

// Objek FirebaseData
FirebaseData fbdo;

// Objek otentikasi Firebase
FirebaseAuth auth;

// Objek konfigurasi Firebase
FirebaseConfig config;

void setup() {
  Serial.begin(9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Terhubungkan ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Tidak dapat terhubung ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi");

  // Konfigurasi Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Mendaftar ke Firebase
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Pendaftaran Firebase berhasil");
  } else {
    Serial.printf("Pendaftaran Firebase gagal: %s\n", config.signer.signupError.message.c_str());
  }

  // Inisialisasi Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Kirim data jarak ke Firebase
  if (Firebase.ready()) {
    if (Firebase.RTDB.setInt(&fbdo, "test/distance", distance)) {
      Serial.println("Data jarak dikirim ke Firebase");
    } else {
      Serial.println("Gagal mengirim data jarak ke Firebase");
      Serial.println("Alasan Kesalahan: " + fbdo.errorReason());
    }
  }

  delay(1000);
}
