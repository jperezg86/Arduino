/**
* Éste programa es para calcular la distancia
* a partir de un sensor ultrasónico HC-SR04 
* y muestra la salida en un display.
*
**/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define ECHO 3
#define TRIGGER 2
#define TOO_CLOSER 3
#define ALMOST_CLOSER 10
#define FAR_FAR_AWAY 20

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
 lcd.init();
 lcd.backlight();
 Serial.begin(9600);
 pinMode(ECHO, INPUT);
 pinMode(TRIGGER, OUTPUT);
}

long calculateDistance(long t) {
  if(t==NULL) {
    return 0;
  }
  
  return t / 59;
}

void printDistanceLCD(long d) {
    lcd.setCursor(0,0);
    lcd.print("Dist:");
    lcd.print(d);
    lcd.print(" cm");

    lcd.setCursor(0,1);
    lcd.print(d);
    lcd.print("cm.");

    lcd.setCursor(0,1);
    if(d <= TOO_CLOSER) {
      lcd.print("Too closer");
    } else if(d > TOO_CLOSER && d <= ALMOST_CLOSER) {
      lcd.print("Almost closer");
    } else {
      lcd.print("Far far away");
    }
}

void clearLCD() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
}

void loop() {
  long t, d;
  clearLCD();
  digitalWrite(TRIGGER, HIGH);
  delay(10);
  digitalWrite(TRIGGER, LOW);
  t = pulseIn(ECHO, HIGH);
  d = calculateDistance(t);
  printDistanceLCD(d);
 delay(1000);
}
