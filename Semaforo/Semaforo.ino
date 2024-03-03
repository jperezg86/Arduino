#define TRIGGER_PIN_PORT 2
#define ECHO_PIN_PORT 3
#define RED_LED_PORT 4
#define YELLOW_LED_PORT 5
#define GREEN_LED_PORT 6

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED_PORT, OUTPUT);
  pinMode(GREEN_LED_PORT, OUTPUT);
  pinMode(YELLOW_LED_PORT, OUTPUT);
  pinMode(TRIGGER_PIN_PORT, OUTPUT);
  pinMode(ECHO_PIN_PORT, INPUT);
  digitalWrite(TRIGGER_PIN_PORT, LOW);
}

void loop() {

  turnOffLeds();
  
  long t;
  long d;

  digitalWrite(TRIGGER_PIN_PORT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_PORT, LOW);
  t = pulseIn(ECHO_PIN_PORT, HIGH);

  d = t/59;

  Serial.print("Distancia: ");
  Serial.print(d);
  Serial.print(" cm");
  Serial.println();
  if( d <= 5) {
    Serial.print("PRENDE EL ROJO!");
    digitalWrite(RED_LED_PORT, HIGH);
  } else if( d > 5 && d < 20) {
    Serial.print("PRENDE EL AMARILLO");
    digitalWrite(YELLOW_LED_PORT, HIGH);
  } else {
    Serial.print("PRENDE EL VERDE");
    digitalWrite(GREEN_LED_PORT, HIGH);
  }

  delay(100);

  /*digitalWrite(GREEN_LED_PORT, HIGH);
  delay(5000);
  digitalWrite(GREEN_LED_PORT, LOW);
  digitalWrite(YELLOW_LED_PORT, HIGH);
  delay(2000);
  digitalWrite(YELLOW_LED_PORT, LOW);
  digitalWrite(RED_LED_PORT, HIGH);
  delay(5000);
  digitalWrite(RED_LED_PORT,LOW);*/
}

void turnOffLeds() {
  digitalWrite(GREEN_LED_PORT, LOW);
  digitalWrite(RED_LED_PORT, LOW);
  digitalWrite(YELLOW_LED_PORT, LOW);
}
