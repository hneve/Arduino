

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <DHTesp.h>

DHTesp dht;

const char* ssid = "MekkeBua";
const char* password = "AKLgXhQmq9";
const char* mqtt_server = "192.168.20.12";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
float lastValue = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  dht.setup(14);
  

  //###### ESP stuff #############
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
      } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    Serial.print("T:");
    Serial.print(temperature);
    Serial.print("\tH:");
    Serial.println(humidity);
    char Tmsg = FtoChar2(temperature,1);
    char Hmsg = FtoChar2(humidity,1);
    Serial.println("Publishing messages: ");
    Serial.println(Tmsg);
    client.publish("sbv9/mekkebua/klima/temperatur2", Tmsg);
    Serial.println(Hmsg);
    client.publish("sbv9/mekkebua/klima/luftfuktighet2", Hmsg);
  }
}

char* FtoChar2(float fVal, char* cF, int nDigs) {
  // Force number of decimal places to full 2
  int iDigs = nDigs;
  // Separator = 10 ˆ Number of decimal places
  long dSep = pow(10, iDigs);
  // DEBUG
  // Serial.print(F("FtoChar2 dSep: "));
  // Serial.println(dSep);
  // Display value = floal value * separator
  signed long slVal = fVal * dSep;

  sprintf(cF, "%d.%d", int(slVal / dSep), int(slVal % int(dSep)));
}
