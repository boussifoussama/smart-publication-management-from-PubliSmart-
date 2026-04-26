#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Adresse I2C de l'écran LCD (généralement 0x27 ou 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  lcd.setCursor(0, 1);
  lcd.print("Waiting for PC...");
}

void loop() {
  if (Serial.available() > 0) {
    String line = Serial.readStringUntil('\n');
    line.trim();

    if (line.startsWith("D:")) {
      // Commande de Deadline : "D:Nom|Date"
      int separatorIndex = line.indexOf('|');
      if (separatorIndex != -1) {
        String nom = line.substring(2, separatorIndex);
        String date = line.substring(separatorIndex + 1);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(nom);
        lcd.setCursor(0, 1);
        lcd.print("DL: " + date);
      }
    } else if (line == "C") {
      // Effacer l'écran
      lcd.clear();
    } else if (line == "W") {
      // Message de bienvenue
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SmartMarket");
      lcd.setCursor(0, 1);
      lcd.print("LCD Connected");
    }
  }
}
