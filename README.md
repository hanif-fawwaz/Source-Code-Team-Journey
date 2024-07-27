
# Locker Management System

This project is a Locker Management System that uses an ESP32 microcontroller. It integrates WiFi, a Telegram bot, a fingerprint sensor, vibration sensors, relays for door locks, and an LCD display.

## Features

- **WiFi Connectivity**: Connects to a WiFi network.
- **Telegram Bot**: Sends and receives messages via a Telegram bot.
- **Fingerprint Sensor**: For user authentication.
- **Vibration Sensors**: Detects tampering.
- **Relays**: Controls door locks.
- **LCD Display**: Provides a user interface.

## Hardware Requirements

- ESP32 microcontroller
- Fingerprint sensor
- Vibration sensors
- Relays
- LCD display (I2C)
- Buzzer
- LEDs

## Libraries Used

- WiFi
- WiFiClientSecure
- UniversalTelegramBot
- Adafruit_Fingerprint
- Wire
- LiquidCrystal_I2C
- SPIFFS

## Setup

1. **WiFi Credentials**: Update the `WIFI_SSID` and `WIFI_PASSWORD` with your WiFi network credentials.
   ```cpp
   #define WIFI_SSID "Your_SSID"
   #define WIFI_PASSWORD "Your_PASSWORD"
   ```

2. **Telegram Bot Token**: Obtain a bot token from BotFather and update `BOT_TOKEN`.
   ```cpp
   #define BOT_TOKEN "Your_Bot_Token"
   ```

3. **Telegram Chat ID**: Update `CHAT_ID` with your Telegram chat ID.
   ```cpp
   #define CHAT_ID "Your_Chat_ID"
   ```

4. **Fingerprint Sensor**: Ensure the fingerprint sensor is connected to the correct pins.

5. **Vibration Sensors**: Connect the vibration sensors to the appropriate GPIO pins.

6. **Relays**: Connect the relays to the appropriate GPIO pins for door locks.

7. **LCD Display**: Ensure the LCD is connected correctly and update the address if necessary.
   ```cpp
   #define LCD_ADDRESS 0x27
   ```

## Code

The main code is provided in the `loker_man_brebes.ino` file. Below is a brief overview of the main functions:

- `setup()`: Initializes WiFi, the Telegram bot, sensors, relays, and the LCD.
- `loop()`: Handles incoming messages, sensor readings, and controls the relays.
- `handleNewMessages()`: Processes new messages received via the Telegram bot.

## Usage

1. Upload the code to your ESP32.
2. Open the Serial Monitor to check the connection status and any debug messages.
3. Interact with the system via the Telegram bot.

## Troubleshooting

- Ensure all connections are secure and correct.
- Check WiFi credentials and ensure the network is reachable.
- Verify the Telegram bot token and chat ID.

## License

This project is licensed under the MIT License.
