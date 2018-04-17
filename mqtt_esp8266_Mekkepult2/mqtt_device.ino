#define DEBUG 1				//Comment out if no need for serial debug
//#define SWITCH 0			//comment out if no need for button/switch 
#define DS18B20 12			//comment out if no need for DS18B20 device, define as pin number
//#define DHT22_PIN 12		//comment out if no need for DHT22 device, define as pin number


// Define debug macros
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
const char* ssid =			"SSID";
const char* password =		"PWD";
const char* mqtt_server =	"192.168.20.200";
const char* tempTopic =		"/garasje/temperatur";			//## Topic to post temperature
const char* humTopic =		"/mekkepult2/fuktighet";		//## Topic to post Humidity
const char* btnTopic =		"/garasje/port_bryter";			//## Topic to post buttonstate
const char*	clientName =	"ESP_Garasje_xxx";				//## Client name. unike

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

String MAC= WiFi.macAddress();

 
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
	delay(10);
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
	DEBUG_PRINT("MAC: ");
	DEBUG_PRINTLN(MAC);
  
} //## END OF setup_wifi()

void callback(char* topic, byte* payload, unsigned int length) { 
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] >");
	
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	
	Serial.print("<\n");
	MAC.toCharArray(msg,18);
	client.publish(msg,"I'm alive!");
}	//## END of callback()

void reconnect() {
	while (!client.connected()) {									//while not connected 
		DEBUG_PRINTLN("\nAttempting MQTT connection...");
		if (client.connect(clientName)) {							//try to connect, and if connected:
			DEBUG_PRINT(clientName);								//debug clientname
			DEBUG_PRINTLN(" connected");
			snprintf(msg, 50,"Device %s connected", clientName);	
			client.publish("Rapport", msg);							//Send a mqtt message
			
			
			MAC.toCharArray(msg,18);							
			client.subscribe( msg );								// Subscribe to own MAC adress topic
		} 
		else {														//if NOT connected
			DEBUG_PRINT("failed, rc=");								//debug 
			DEBUG_PRINT(client.state());
			DEBUG_PRINTLN(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);											//wait 5sek and retry
		}
	}
}

void loop() {
	if (!client.connected()) {		//Check if connected, if not :
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

	// Timekeeping, check every 60s if change 
	long now = millis();
	if (now - lastMsg > 60000) {
		lastMsg = now;
		
		#ifdef DS18B20	//Block for DS18B20 sensor
		ds18b20.requestTemperatures();
		float tempC = ds18b20.getTempCByIndex(0);
		/* Only send message if changed */
		if (tempC != lastValue){
			lastValue = tempC;
			FtoChar2(tempC, msg,1);	//Make text out of float and put in msg
			DEBUG_PRINT("\nPublish message: ");
			DEBUG_PRINT(msg);
			client.publish(tempTopic, msg);
		}
		#endif	
		
		
		#ifdef DHT22_PIN  // Block for DHT22 sensor 
		float humidity = dht.getHumidity();
		float temperature = dht.getTemperature();
		DEBUG_PRINT("\nT:");
		DEBUG_PRINT(temperature);
		DEBUG_PRINT("\tH:");
		DEBUG_PRINT(humidity);
		
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
#endif

char* FtoChar2(float fVal, char* cF, int nDigs) {
	long dSep = pow(10, nDigs);
	signed long slVal = fVal * dSep;
	sprintf(cF, "%d.%d", int(slVal / dSep), abs(int(slVal % int(dSep))));
}
