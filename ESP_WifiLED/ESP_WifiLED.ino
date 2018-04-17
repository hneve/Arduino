#define DEBUG 1

#ifdef DEBUG
#define DEBUG_PRINT(x)		Serial.print (x)
#define DEBUG_PRINTDEC(x)   Serial.print (x, DEC)
#define DEBUG_PRINTLN(x)	Serial.println (x)
#define DEBUG_PRINTHEX(x)	Serial.print (x, HEX)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTDEC(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTHEX(x)
#endif


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NeoPixelBus.h>
 #include <stdlib.h>
 
const char* ssid =			"MekkeBua";
const char* password =		"AKLgXhQmq9";
const char* mqtt_server =	"192.168.20.200";
const char* SubTopic =		"sbv9/LED";								//## Topic to get commands from
const char*	clientName =	"ESP_LED";						//## Client name. unike

WiFiClient espClient;
PubSubClient client(espClient);

const uint16_t PixelCount = 1;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);

char msg[50];
float lastMsg;
bool MQTTcontrol = 0;
void setup()
{
	Serial.begin(115200);
	strip.Begin();
	strip.Show();
	
	setup_wifi();
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
	




}

void loop()
{
	if (!client.connected()) {
		reconnect();
	}
	client.loop();
	
	if(!MQTTcontrol){
		long now = millis();
		if (now - lastMsg > 5000) {
			lastMsg = now;
			
			strip.SetPixelColor(0, RgbColor(0,128,0));
			strip.Show();
			delay(2000);
			strip.SetPixelColor(0, RgbColor(0,0,128));
			strip.Show();
			
		}
	}
}

void callback(char* topic, byte* payload, unsigned int length) {
DEBUG_PRINT("\nMessage arrived [");
DEBUG_PRINT(topic);
DEBUG_PRINT("] ");
for (int i=0;i<length;i++) {
	DEBUG_PRINT((char)payload[i]);
}
MQTTcontrol = true;
byte red = hex8(payload[0], payload[1]);
byte grn = hex8(payload[2], payload[3]);
byte blu = hex8(payload[4], payload[5]);
DEBUG_PRINT(red);
DEBUG_PRINT(":");
DEBUG_PRINT(grn);
DEBUG_PRINT(":");
DEBUG_PRINT(blu);
DEBUG_PRINTLN(" ");




strip.SetPixelColor(0, RgbColor(red,grn,blu) );
strip.Show();

}

void setup_wifi()
{
	// WIFI setup
	delay(10);
	// We start by connecting to a WiFi network
	DEBUG_PRINT("\nConnecting to ");
	DEBUG_PRINTLN(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		DEBUG_PRINT(".");
	}

	DEBUG_PRINTLN("\nWiFi connected");
	DEBUG_PRINTLN("IP address: ");
	DEBUG_PRINTLN(WiFi.localIP());

} //## END OF setup_wifi()

void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
		//DEBUG_PRINTLN("\nAttempting MQTT connection...");
		// Attempt to connect
		if (client.connect(clientName)) {
			DEBUG_PRINT(clientName);
			DEBUG_PRINTLN(" connected");
			snprintf(msg, 50,"Device %s connected", clientName);	// Once connected, publish an announcement...
			client.publish("Rapport", msg);
			// Here is a good place to (re)subscribe to a topic
			client.subscribe(SubTopic);
			} else {
			//DEBUG_PRINT("failed, rc=");
			//DEBUG_PRINT(client.state());
			//DEBUG_PRINTLN(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

int hex8(char Fin, char Sin)
{
	uint8_t c, h;

	c = Fin;

	if (c <= '9' && c >= '0') {  c -= '0'; }
	else if (c <= 'f' && c >= 'a') { c -= ('a' - 0x0a); }
	else if (c <= 'F' && c >= 'A') { c -= ('A' - 0x0a); }
	else return(-1);

	h = c;

	c = Sin;

	if (c <= '9' && c >= '0') {  c -= '0'; }
	else if (c <= 'f' && c >= 'a') { c -= ('a' - 0x0a); }
	else if (c <= 'F' && c >= 'A') { c -= ('A' - 0x0a); }
	else return(-1);

	return ( h<<4 | c);
}