#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// RC522 pins for Arduino UNO
static const uint8_t SS_PIN = 10;
static const uint8_t RST_PIN = 9;

// --- SECTION OUTPUT : CONFIGURATION DU SERVOMOTEUR ---
// Broche de signal pour le servomoteur qui contrôle le verrou
static const uint8_t SERVO_PIN = 6;
// Angle en degrés pour la position "verrouillé" (porte fermée)
static const int SERVO_CLOSED = 0;
// Angle en degrés pour la position "déverrouillé" (porte ouverte)
static const int SERVO_OPEN = 90;

MFRC522 rfid(SS_PIN, RST_PIN);
// Instance de l'objet Servo pour piloter physiquement le moteur
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

/**
 * @brief Contrôle l'état physique du servomoteur.
 * 
 * Cette fonction est la partie "Actionneur" (Output) principale.
 * Elle envoie la commande d'angle au servomoteur en fonction de l'état souhaité.
 * 
 * @param isOpen true pour ouvrir (90°), false pour fermer (0°)
 */
void setDoorOpen(bool isOpen)
{
  // Envoi de la position angulaire au servo via la bibliothèque Servo
  lockServo.write(isOpen ? SERVO_OPEN : SERVO_CLOSED);
}

void processCommand(const String &cmdRaw)
{
  String cmd = cmdRaw;
  cmd.trim();
  cmd.toUpperCase();

  // --- LOGIQUE DE SORTIE (OUTPUT) ---
  
  if (cmd == "OPEN") {
    // Action d'ouverture : on active le servo
    setDoorOpen(true);
    // On définit un délai (timestamp futur) pour la fermeture automatique (3 secondes)
    openUntilMs = millis() + 3000;
    // Réponse série pour confirmer l'exécution de la commande de sortie
    Serial.println("ACK=OPEN");
  } else if (cmd == "DENY") {
    // Action de refus : on force la fermeture immédiate du servo
    setDoorOpen(false);
    openUntilMs = 0;
    // Confirmation du refus
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

  // --- INITIALISATION DE LA SORTIE ---
  // Liaison de l'objet lockServo à la broche physique définie (D6)
  lockServo.attach(SERVO_PIN);
  // Initialisation du servo en position fermée par défaut au démarrage
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

  // --- GESTION TEMPORELLE DU SERVO (FERMETURE AUTO) ---
  // Si un délai d'ouverture est actif et que le temps actuel dépasse ce délai
  if (openUntilMs > 0 && millis() > openUntilMs) {
    // On ferme physiquement la porte (retour du servo à 0°)
    setDoorOpen(false);
    // On réinitialise le marqueur de temps
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
