# Firmware Arduino RFID (UNO + RC522)

Ce sketch envoie les UID au format exact attendu par SmartMarket:

UID=XXXXXXXX

## 1) Bibliotheques Arduino

Installez dans Arduino IDE:
- MFRC522 by GithubCommunity
- Servo (incluse par defaut)

## 2) Cablage RC522 -> Arduino UNO

- SDA (SS)  -> D10
- SCK       -> D13
- MOSI      -> D11
- MISO      -> D12
- RST       -> D9
- 3.3V      -> 3.3V
- GND       -> GND

Important:
- RC522 doit etre alimente en 3.3V (pas 5V).

## 3) Servo (optionnel)

- Signal -> D6
- VCC/GND selon votre alimentation

Le PC envoie:
- OPEN -> ouvre 3 secondes
- DENY -> ferme

## 4) Upload

1. Ouvrir arduino_firmware.ino
2. Board: Arduino Uno
3. Port: COM12 (ou celui detecte)
4. Upload

## 5) Test rapide

Ouvrez Serial Monitor a 9600 bauds.
Posez une carte: vous devez voir par exemple:
- BOOT=RFID_READY
- UID=04A1B2C3

Si vous ne voyez pas UID=..., le probleme est cote firmware/cablage, pas cote Qt.

## 6) SmartMarket

Lancer SmartMarket depuis le meme dossier source modifie.
Si besoin, definir:
- SMARTMARKET_ARDUINO_PORT=COM12
- SMARTMARKET_ARDUINO_BAUD=9600
