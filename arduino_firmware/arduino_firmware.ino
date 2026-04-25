#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// RC522 pins for Arduino UNO
static const uint8_t SS_PIN = 10;
static const uint8_t RST_PIN = 9;

// Optional lock servo pin
static const uint8_t SERVO_PIN = 6;
static const int SERVO_CLOSED = 0;
static const int SERVO_OPEN = 90;

MFRC522 rfid(SS_PIN, RST_PIN);
Servo lockServo;

String rxBuffer;
unsigned long openUntilMs = 0;
String lastUid;
unsigned long lastUidMs = 0;

String uidToHex(const MFRC522::Uid &uid)
{
  String out;
  for (byte i = 0; i < uid.size; i++) {
    if (uid.uidByte[i] < 0x10) out += "0";
    out += String(uid.uidByte[i], HEX);
  }
  out.toUpperCase();
  return out;
}

void setDoorOpen(bool isOpen)
{
  lockServo.write(isOpen ? SERVO_OPEN : SERVO_CLOSED);
}

void processCommand(const String &cmdRaw)
{
  String cmd = cmdRaw;
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "OPEN") {
    setDoorOpen(true);
    openUntilMs = millis() + 3000;
    Serial.println("ACK=OPEN");
  } else if (cmd == "DENY") {
    setDoorOpen(false);
    openUntilMs = 0;
    Serial.println("ACK=DENY");
  } else if (cmd == "PING") {
    Serial.println("ACK=PONG");
  }
}

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  lockServo.attach(SERVO_PIN);
  setDoorOpen(false);

  Serial.println("BOOT=RFID_READY");
}

void loop()
{
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      if (rxBuffer.length() > 0) {
        processCommand(rxBuffer);
        rxBuffer = "";
      }
    } else {
      rxBuffer += c;
      if (rxBuffer.length() > 64) rxBuffer = "";
    }
  }

  if (openUntilMs > 0 && millis() > openUntilMs) {
    setDoorOpen(false);
    openUntilMs = 0;
  }

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  const String uid = uidToHex(rfid.uid);
  const unsigned long now = millis();

  // Avoid flooding when card stays on the antenna.
  if (uid != lastUid || now - lastUidMs > 1200) {
    Serial.print("UID=");
    Serial.println(uid);
    lastUid = uid;
    lastUidMs = now;
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
