#define TRIGGER 9
#define ECHO 8
#define VERDE 4
#define AMARILLO 3
#define ROJO 2

void setup() {
  Serial.begin(9600);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(VERDE, OUTPUT);
  pinMode(AMARILLO, OUTPUT);
  pinMode(ROJO, OUTPUT);


  // put your setup code here, to run once:

}

void loop() {
  long t,d;
  digitalWrite(TRIGGER, HIGH);
  delay(100);
  digitalWrite(TRIGGER, LOW);
  // put your main code here, to run repeatedly:
  t = pulseIn(ECHO, HIGH);
  d = t/59;
  Serial.print("Distancia: ");
  Serial.print(d);
  Serial.print(" cm.");
  Serial.println("");
  if (d<=3) {
    digitalWrite(ROJO, HIGH);
  } else if (d>3 && d<=6) {
    digitalWrite(AMARILLO, HIGH);
  }else {
    digitalWrite(VERDE, HIGH);
  }
  delay(100);
  digitalWrite(ROJO, LOW);
  digitalWrite(AMARILLO, LOW);
  digitalWrite(VERDE, LOW);
}
