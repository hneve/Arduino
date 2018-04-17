

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <DHTesp.h>

DHTesp dht;

const char* ssid = "MekkeBua";
const char* password = "AKLgXhQmq9";
const char* mqtt_server = "192.168.20.12";
const char* mqtt_clientname = "DHT_esp_000000001"; 

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
float lastValue = 0;
char msg[50];
int value = 0;

float humidity;
float temperature;
float last_humidity;
float last_temperature;

char Tmsg[10];
char Hmsg[10];

void setup() {
	Serial.begin(115200);
	dht.setup(12);
	

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
		if (client.connect(mqtt_clientname)) {
			Serial.println("connected");
			// Once connected, publish an announcement...
			snprintf(msg, 50, "%s connected", mqtt_clientname);
			client.publish("Rapport", msg);
			// ... and resubscribe
			//client.subscribe("inTopic");
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
		Serial.print(humidity);
		if((temperature != last_temperature) || (humidity != last_humidity) ) {
			last_humidity = humidity;
			last_temperature = temperature;
			
			FtoChar2(temperature,Tmsg,1);
			FtoChar2(humidity,Hmsg,1);
			Serial.print(" Publishing messages: T:");
			Serial.print(Tmsg);
			client.publish("sbv9/mekkebua/klima/temperatur2", Tmsg);
			Serial.print(" H:");
			Serial.println(Hmsg);
			client.publish("sbv9/mekkebua/klima/luftfuktighet2", Hmsg);
		}
	}
}

char* FtoChar2(float fVal, char* cF, int nDigs) {
	// Force number of decimal places to full 2
	int iDigs = nDigs;
	long dSep = pow(10, iDigs);
	signed long slVal = fVal * dSep;

	sprintf(cF, "%d.%u", int(slVal / dSep), int(slVal % int(dSep)));
}