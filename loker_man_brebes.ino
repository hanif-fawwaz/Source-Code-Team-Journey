#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPIFFS.h>

// WiFi credentials
#define WIFI_SSID "Labkom"
#define WIFI_PASSWORD "pakekocakep"

// Telegram BOT Token (Get from BotFather)
#define BOT_TOKEN "7205706467:AAEKhS9G9jTYE1uwQxxtXtpVBJIfaXC01hc"

// Telegram Chat ID
#define CHAT_ID "5396867286"

// LCD address (typically 0x27 or 0x3F)
#define LCD_ADDRESS 0x27

const unsigned long BOT_MTBS = 1000; // Mean time between scan messages

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // Last time messages' scan has been done

// GPIO Pin Definitions
int limitPins[] = {13, 4, 5, 15, 23}; // GPIO pins for limit switches
int limitStatus[] = {0, 0, 0, 0, 0};   // Status of each limit switch
int prevLimitStatus[] = {0, 0, 0, 0, 0}; // Previous status of each limit switch

// GPIO Pin Definitions for vibration sensors
const int vibrationPins[]  = {36, 39, 34, 35, 32}; // GPIO pins for vibration sensors
int vibrationStatus[] = {0, 0, 0, 0, 0};   // Status of each vibration sensor
int prevVibrationStatus[] = {0, 0, 0, 0, 0}; // Previous status of each vibration sensor

int relayPins[] = {12, 27, 14, 18, 25}; // GPIO pins for relays (door locks)

int buzz = 26;

int LEDR = 19;    // GPIO2 (onboard LED)


int ledStatus = 0;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

bool isStoringPhone = false;
bool isEnrolling = false;
bool isRequestingID = false;
bool isRequestingName = false;
bool isActive = false;
int userId = 0;
String userName = "";
unsigned long enrollmentStartTime;

// LCD object
LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);

void handleNewMessages(int numNewMessages) {
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    String text = bot.messages[i].text;
    String chat_id = CHAT_ID;
    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "Buka loker 1") {
      digitalWrite(relayPins[0], LOW); // Unlock the locker
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("loker 1 buka...");
      bot.sendMessage(chat_id, "Loker 1 dibuka ..!!!");
    } else if (text == "Buka loker 2") {
      digitalWrite(relayPins[1], LOW); // Unlock the locker
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("loker 2 buka...");
      bot.sendMessage(chat_id, "Loker 2 dibuka ..!!!");
    } else if (text == "Buka loker 3") {
      digitalWrite(relayPins[2], LOW); // Unlock the locker
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("loker 3 buka...");
      bot.sendMessage(chat_id, "Loker 3 dibuka ..!!!");
    } else if (text == "Buka loker 4") {
      digitalWrite(relayPins[3], LOW); // Unlock the locker
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("loker 4 buka...");
      bot.sendMessage(chat_id, "Loker 4 dibuka ..!!!");
    } else if (text == "Buka loker 5") {
      digitalWrite(relayPins[4], LOW); // Unlock the locker
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("loker 5 buka...");
      bot.sendMessage(chat_id, "Loker 5 dibuka ..!!!");

    } else if (text == "Tutup loker") {
      digitalWrite(relayPins[0], HIGH); // Unlock the locker
      digitalWrite(relayPins[1], HIGH);
      digitalWrite(relayPins[2], HIGH);
      digitalWrite(relayPins[3], HIGH);
      digitalWrite(relayPins[4], HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("loker aktif...");
      bot.sendMessage(chat_id, "Loker di tutup ..!!!");
      
    } 
     
     else if (text == "Daftar") {
      isEnrolling = true;
      isRequestingID = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tempelkan jari");
      lcd.setCursor(0, 1);
      lcd.print("anda...");
      bot.sendMessage(chat_id, "Tempelkan jari anda...", "");
    } else if (isRequestingID && text.toInt() > 0) {
      userId = text.toInt();
      isRequestingID = false;
      isRequestingName = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ID: " + String(userId));
      lcd.setCursor(0, 1);
      lcd.print("Nama?");
      bot.sendMessage(chat_id, "ID yang diterima: " + String(userId) + ". Sekarang, silakan masukkan nama Anda.", "");
    } else if (isRequestingName && text.length() > 0) {
      userName = text;
      isRequestingName = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nama: " + userName);
      lcd.setCursor(0, 1);
      lcd.print("ID: " + String(userId));
      delay(2000); // Display for 2 seconds
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Menyiapkan...");
      enrollmentStartTime = millis();
    } else if (text == "Aktif") {
      isActive = true;
      bot.sendMessage(chat_id, "Mode aktif diaktifkan.", "");
    } else if (text == "storephone") {
      isStoringPhone = true;
      bot.sendMessage(chat_id, "Mode penyimpanan ponsel diaktifkan.", "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(57600, SERIAL_8N1, 16, 17); // ESP32 UART2 for fingerprint sensor (RX=16, TX=17)
  finger.begin(57600);

  // Initialize GPIO pins for limit switches and vibration sensors
  for (int i = 0; i < 5; i++) {
    pinMode(limitPins[i], INPUT_PULLUP);
    pinMode(vibrationPins[i], INPUT_PULLUP);
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH);
  }

  pinMode(LEDR, OUTPUT);
  pinMode(buzz, OUTPUT);

  digitalWrite(buzz,HIGH);

  // Setup WiFi
  Serial.print("Connecting to WiFi ");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP address: " + WiFi.localIP().toString());

  // Initialize fingerprint sensor
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  // Initialize LCD
  lcd.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(2000); // Display for 2 seconds

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  // Initialize locker status file
  File file = SPIFFS.open("/locker_status.txt", FILE_READ);
  if (!file) {
    // Create file if it doesn't exist
    file = SPIFFS.open("/locker_status.txt", FILE_WRITE);
    if (file) {
      for (int i = 1; i <= 5; i++) {
        file.println("empty");
      }
      file.close();
    } else {
      Serial.println("Failed to create locker status file");
    }
  } else {
    file.close();
  }
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }

  // Fingerprint enrollment logic
  if (isEnrolling && !isRequestingID && !isRequestingName) {
    // Initialize LED as off
  digitalWrite(LEDR, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mendapatkan gambar...");
    int p = finger.getImage();
    if (p == FINGERPRINT_OK) {
      lcd.setCursor(0, 1);
      lcd.print("Gambar diambil...");
      p = finger.image2Tz(1);
      if (p == FINGERPRINT_OK) {
        lcd.setCursor(0, 1);
        lcd.print("Menyimpan gambar...");
        p = finger.getImage();
        if (p == FINGERPRINT_OK) {
          lcd.setCursor(0, 1);
          lcd.print("Gambar diambil...");
          p = finger.image2Tz(2);
          if (p == FINGERPRINT_OK) {
            p = finger.createModel();
            if (p == FINGERPRINT_OK) {
              p = finger.storeModel(userId);
              if (p == FINGERPRINT_OK) {
                File file = SPIFFS.open("/user_data.txt", FILE_APPEND);
                if (!file) {
                  bot.sendMessage(CHAT_ID, "Gagal membuka file untuk penyimpanan.", "");
                } else {
                  digitalWrite(buzz,LOW);
                  delay(100);
                  digitalWrite(buzz,HIGH);
                  delay(100);
                  digitalWrite(buzz,LOW);
                  delay(100);
                  digitalWrite(buzz,HIGH);
                  file.println(String(userId) + ":" + userName);
                  file.close();
                  String message = "Sidik jari berhasil terdaftar dengan ID: " + String(userId) + " dan Nama: " + userName;
                  bot.sendMessage(CHAT_ID, message, "");
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Sidik jari");
                  lcd.setCursor(0, 1);
                  lcd.print("berhasil terdaftar");
                }
              } else {
                bot.sendMessage(CHAT_ID, "Gagal menyimpan model sidik jari.", "");
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Gagal menyimpan");
                lcd.setCursor(0, 1);
                lcd.print("model sidik jari.");
              }
            } else {
              bot.sendMessage(CHAT_ID, "Gagal membuat model sidik jari.", "");
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Gagal membuat");
              lcd.setCursor(0, 1);
              lcd.print("model sidik jari.");
            }
          } else {
            bot.sendMessage(CHAT_ID, "Gagal mengonversi gambar sidik jari.", "");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Gagal mengonversi");
            lcd.setCursor(0, 1);
            lcd.print("gambar sidik jari.");
          }
        } else {
          bot.sendMessage(CHAT_ID, "Gagal mengambil gambar sidik jari.", "");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Gagal mengambil");
          lcd.setCursor(0, 1);
          lcd.print("gambar sidik jari.");
        }
      } else {
        bot.sendMessage(CHAT_ID, "Gagal mengonversi gambar sidik jari.", "");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Gagal mengonversi");
        lcd.setCursor(0, 1);
        lcd.print("gambar sidik jari.");
      }
    } else {
      bot.sendMessage(CHAT_ID, "Gagal mengambil gambar sidik jari.", "");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gagal mengambil");
      lcd.setCursor(0, 1);
      lcd.print("gambar sidik jari.");
    }

    isEnrolling = false;
  }



  // Check for fingerprint and storing phone in active mode
  if (isActive) {
    displayLockerStatus();
    // Initialize LED as off
    digitalWrite(LEDR, HIGH);
    if (finger.getImage() == FINGERPRINT_OK) {
      if (finger.image2Tz() == FINGERPRINT_OK) {
        if (finger.fingerFastSearch() == FINGERPRINT_OK) {
          int id = finger.fingerID;
          String name = getNameFromSPIFFS(id);
          delay(2000);

          if (id == 7) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Loker di buka... ");
            lcd.setCursor(0, 1);
            lcd.print("ID: " + String(id));
            lcd.setCursor(0, 2);
            lcd.print("Nama: " + name);
            digitalWrite(buzz,LOW);
            delay(100);
            digitalWrite(buzz,HIGH);
            delay(100);
            digitalWrite(buzz,LOW);
            delay(100);
            digitalWrite(buzz,HIGH);
            digitalWrite(relayPins[0], LOW);
            digitalWrite(relayPins[1], LOW);
            digitalWrite(relayPins[2], LOW);
            digitalWrite(relayPins[3], LOW);
            digitalWrite(relayPins[4], LOW);
            bot.sendMessage(CHAT_ID, "ID " + String(id) + ", Nama: " + name + " telah membuka loker .", "");
          }
          for (int i = 0; i < 5; i++) {
           
          limitStatus[i] = digitalRead(limitPins[i]);
          if (id != 7 && limitStatus[i] == LOW){
            digitalWrite(buzz,LOW);
            delay(100);
            digitalWrite(buzz,HIGH);
                int lokerPosisi = i + 1;
                String name = getNameFromSPIFFS(id);
                String message = "ID: " + String(id) + ", Nama: " + name + " telah menyimpan HP di loker " + String(lokerPosisi);
                bot.sendMessage(CHAT_ID, message, "");
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("HP disimpan di");
                lcd.setCursor(0, 1);
                lcd.print("loker " + String(lokerPosisi));
                delay(2000);
                // Lock the locker by activating the relay
                
                digitalWrite(relayPins[lokerPosisi], LOW); 
              
              prevLimitStatus[i] = limitStatus[i];
            
          }
        }
        }
      }
    }
    // Check for vibration
    int highestVibrationIndex = -1;
    int highestVibrationValue = 0;

    for (int i = 0; i < 5; i++) {
      vibrationStatus[i] = digitalRead(vibrationPins[i]);

      if (vibrationStatus[i] == LOW && prevVibrationStatus[i] == HIGH) {
        // Vibration detected, determine the highest value
        int currentValue = analogRead(vibrationPins[i]);
        if (currentValue > highestVibrationValue) {
          highestVibrationValue = currentValue;
          highestVibrationIndex = i;
        }
      }

      prevVibrationStatus[i] = vibrationStatus[i];
    }

    if (highestVibrationIndex != -1) {
      // Position determined
      int lokerPosisi = highestVibrationIndex + 1;
      if (finger.getImage() == FINGERPRINT_OK) {
        if (finger.image2Tz() == FINGERPRINT_OK) {
          if (finger.fingerFastSearch() == FINGERPRINT_OK) {
            int id = finger.fingerID;
            String name = getNameFromSPIFFS(id);
            String message = "Ada panggilan telepon! ID: " + String(id) + ", Nama: " + name + " terdeteksi di loker " + String(lokerPosisi);
            bot.sendMessage(CHAT_ID, message, "");

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Panggilan di");
            lcd.setCursor(0, 1);
            lcd.print("loker " + String(lokerPosisi));
            delay(2000);
          }
        }
      }
    }
  }
}

String getNameFromSPIFFS(int id) {
  File file = SPIFFS.open("/user_data.txt", FILE_READ);
  if (!file) {
    return "";
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    int separatorIndex = line.indexOf(':');
    if (separatorIndex != -1) {
      String fileIdStr = line.substring(0, separatorIndex);
      String fileName = line.substring(separatorIndex + 1);
      if (fileIdStr.toInt() == id) {
        file.close();
        fileName.trim();
        return fileName;
      }
    }
  }

  file.close();
  return "";
}

String getLockerStatus() {
  File file = SPIFFS.open("/locker_status.txt", FILE_READ);
  if (!file) {
    return "Gagal membuka file status loker.";
  }

  String statusMessage = "Status Loker:\n";
  int lokerNumber = 1;
  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    statusMessage += "Loker " + String(lokerNumber) + ": " + line + "\n";
    lokerNumber++;
  }

  file.close();
  return statusMessage;
}

void updateLockerStatus(int loker, String status) {
  File file = SPIFFS.open("/locker_status.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Gagal membuka file status loker untuk penulisan.");
    return;
  }

  for (int i = 1; i <= 5; i++) {
    if (i == loker) {
      file.println(status);
    } else {
      file.println(getLockerStatus().substring(getLockerStatus().indexOf("Loker " + String(i) + ": ") + ("Loker " + String(i) + ": ").length()));
    }
  }

  file.close();
}

// Function to display locker statuses on the LCD
void displayLockerStatus() {
  
  for (int i = 0; i < 5; i++) {
    limitStatus[i] = digitalRead(limitPins[i]);
    if (limitStatus[i] != prevLimitStatus[i]) { 
    lcd.clear();
    for (int i = 0; i < 4; i++) {
      lcd.setCursor(0, i);
      lcd.print("Loker " + String(i + 1) + ": " + (limitStatus[i] == LOW ? "Kosong" : "Terisi"));
    }
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Loker 5: " + String(limitStatus[4] == LOW ? "Kosong" : "Terisi"));
    delay(2000);

    // Update previous status
    prevLimitStatus[i] = limitStatus[i];
    }
  }
}
