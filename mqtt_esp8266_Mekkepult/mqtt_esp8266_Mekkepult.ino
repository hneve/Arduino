#define DEBUG 1
//#define SWITCH 0
#define DS18B20 12
//#define DHT22_PIN 12

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
#include <DallasTemperature.h>
#include <DHTesp.h>



//##################################  setup
const char* ssid =			"MekkeBua";
const char* password =		"AKLgXhQmq9";
const char* mqtt_server =	"192.168.20.200";
const char* tempTopic =		"sbv9/mekkepult2/temperatur";			//## Topic to post temperature
const char* humTopic =		"sbv9/mekkepult2/fuktighet";			//## Topic to post Humidity
const char* btnTopic =		"sbv9/mekkepult/port_bryter";			//## Topic to post buttonstate
const char*	clientName =	"ESP_Mekkepult2";						//## Client name. unike

WiFiClient espClient;
PubSubClient client(espClient);

#ifdef DS18B20	
	OneWire oneWire(DS18B20);											// onewire on pin 12
	DallasTemperature ds18b20(&oneWire);
	DeviceAddress insideThermometer;
#endif
#ifdef DHT22_PIN
	DHTesp dht;
	
	float humidity;
	float temperature;
	float last_humidity;
	float last_temperature; 
	char Tmsg[10];
	char Hmsg[10];
#endif


long lastMsg = 0;
float lastValue = 0;
char msg[50];
int value = 0;
bool button;
bool old_button;

void setup(){
	Serial.begin(115200);
#ifdef SWITCH  	
	pinMode(SWITCH,INPUT);
#endif
#ifdef DS18B20		
	ds18b20.begin();
#endif
#ifdef DHT22_PIN
	dht.setup(12);
#endif
	//########################### WIFI begin
	setup_wifi();
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
	

#ifdef DS18B20

	//########################### Sensor serial debug
	DEBUG_PRINT("Locating devices...");
	DEBUG_PRINT("Found ");
	DEBUG_PRINTDEC(ds18b20.getDeviceCount());
	DEBUG_PRINTLN(" devices.");
	if (!ds18b20.getAddress(insideThermometer, 0)) DEBUG_PRINTLN("Unable to find address for Device 0"); 
	DEBUG_PRINT("Device 0 Address: ");
	printAddress(insideThermometer);
	DEBUG_PRINTLN();

	ds18b20.setResolution(insideThermometer, 11);	// (Sensor_adress, bits)
 
	DEBUG_PRINT("Device 0 Resolution: ");
	DEBUG_PRINTDEC(ds18b20.getResolution(insideThermometer)); 
#endif
}	//## END OF setup()

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

void callback(char* topic, byte* payload, unsigned int length) { 
// callback function when subscribed message arrives
  Serial.print("Message arrived [");
  Serial.print(topic);

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    DEBUG_PRINTLN("\nAttempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName)) {
      DEBUG_PRINT(clientName);
	  DEBUG_PRINTLN(" connected");
	  snprintf(msg, 50,"Device %s connected", clientName);	// Once connected, publish an announcement...
      client.publish("Rapport", msg);
	  // Here is a good place to (re)subscribe to a topic
      //client.subscribe("inTopic");
    } else {
      DEBUG_PRINT("failed, rc=");
      DEBUG_PRINT(client.state());
      DEBUG_PRINTLN(" try again in 5 seconds");
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
	
/* Switch doesent really need any time keeping */
#ifdef SWITCH  
	button = !digitalRead(SWITCH);  //GPIO 0 is normal high, low when button pressed
	if(button != old_button) { // Dont send same message more than once.
		old_button = button;
		if(button)  client.publish(btnTopic,"1");
		else		client.publish(btnTopic,"0");
	}
#endif

/* Timekeeping, check every 60s if change */
	long now = millis();
	if (now - lastMsg > 60000) {
		lastMsg = now;
		
		/* Block for DS18B20 sensor*/		
	#ifdef DS18B20			
		ds18b20.requestTemperatures();
		float tempC = ds18b20.getTempCByIndex(0);
		if (tempC != lastValue){
			lastValue = tempC;
			FtoChar2(tempC, msg,1);	//Make text out of float and put in msg
			DEBUG_PRINT("\nPublish message: ");
			DEBUG_PRINT(msg);
			client.publish(tempTopic, msg);
		}
	#endif	
		/* Block for DHT22 sensor */
	#ifdef DHT22_PIN
		float humidity = dht.getHumidity();
		float temperature = dht.getTemperature();
		DEBUG_PRINT("\nT:");
		DEBUG_PRINT(temperature);
		DEBUG_PRINT("\tH:");
		DEBUG_PRINT(humidity);
		/* Only send message if changed */
		if((temperature != last_temperature) || (humidity != last_humidity) ) {
			last_humidity = humidity;
			last_temperature = temperature;
		
			FtoChar2(temperature,Tmsg,1);
			FtoChar2(humidity,Hmsg,1);
			Serial.print(" Publishing messages: T:");
			Serial.print(Tmsg);
			client.publish(tempTopic, Tmsg);
			Serial.print(" H:");
			Serial.println(Hmsg);
			client.publish(humTopic, Hmsg);
		}
#endif
	
	} // end of Timekeeping
}// END of loop

#ifdef DS18B20	
void printAddress(DeviceAddress deviceAddress){
	for (uint8_t i = 0; i < 8; i++){
		if (deviceAddress[i] < 16) Serial.print("0");
		DEBUG_PRINTHEX(deviceAddress[i]);
	}
}

float GetTemperature(DeviceAddress deviceAddress){
	float tempC = ds18b20.getTempCByIndex(0);
	DEBUG_PRINT("\nTemp C: ");
	DEBUG_PRINT(tempC);
	DEBUG_PRINT("\t");
	return tempC;
}
#endif

char* FtoChar2(float fVal, char* cF, int nDigs) {
	long dSep = pow(10, nDigs);
	signed long slVal = fVal * dSep;
	sprintf(cF, "%d.%d", int(slVal / dSep), abs(int(slVal % int(dSep))));
}
