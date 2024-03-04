#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "arduino_secrets.h"
#include <DHT.h>

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char *ssid = SECRET_SSID;        // your network SSID (name)
char *pass = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

#define LED_PIN 2
#define DHT_PIN 4
#define DHT_TYPE DHT11

const char *broker = MQTT_BROKER_URI;
const char *broker_password = MQTT_PASWWORD;
const char *broker_username = MQTT_USERNAME;
int        broker_port     = 8883;
const char topic[]  = "ledStatus";
const char topic2[]  = "temperature";
const char topic3[]  = "humidity";

struct WeatherData {
  float humidity;
  float temperature;
};

static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";


void callback(char *topic, byte *message, unsigned int lengt){
  Serial.print("Nuevo mensaje en el tópico");
  Serial.println(topic);

  String messageTmp; 
  for(int i=0; i<lengt; i++) {
    Serial.print((char) message[i]);
    messageTmp += (char) message[i];
  }

  if(String(topic) == "ledStatus") {
    if(messageTmp == "on") {
      digitalWrite(LED_PIN, HIGH);
    }else {
      digitalWrite(LED_PIN, LOW);
    }
  }
}

WiFiClientSecure wifiClient;
PubSubClient client(wifiClient);
DHT dht(DHT_PIN, DHT_TYPE);

void setupWifi() {
  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   randomSeed(micros());
  Serial.println();
  Serial.println("Wifi conectado");
  Serial.print("IP Asignada: ");
  Serial.println(WiFi.localIP());
}

void pushMessage(char *topic, char *payload) {
  client.publish(topic, payload);
}

void reconnectMqtt() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    Serial.print(broker_username);
    Serial.print("|");
    Serial.println(broker_password);
    if (client.connect("arduinoClient", broker_username, broker_password)) {
      Serial.println("connected");
      client.subscribe("ledStatus");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  // attempt to connect to Wifi network:
  setupWifi();
  wifiClient.setInsecure();
  wifiClient.setCACert(root_ca); 

  client.setServer(broker, broker_port);
  client.setCallback(callback);

  dht.begin();
}

WeatherData getSensorData() {
  WeatherData dht11Data;
  dht11Data.humidity = dht.readHumidity();
  dht11Data.temperature = dht.readTemperature();
  Serial.print("Humedad: ");
  Serial.print(dht11Data.humidity);
  Serial.print(" Temperatura: ");
  Serial.print(dht11Data.temperature);
  Serial.println(" °C");
  return dht11Data;
}

void publishSensorData (WeatherData sensorData){
  char buffer[20];
  sprintf(buffer, "%f", sensorData.humidity);
  client.publish("humidity", buffer );
  sprintf(buffer, "%f", sensorData.temperature);
  client.publish("temperature", buffer );
}




void loop() {

  if(!client.connected()){
    reconnectMqtt();
  }

  WeatherData sensorData = getSensorData();
  publishSensorData(sensorData);

  client.loop();
  delay(300);
 
}