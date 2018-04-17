#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define AP_SSID			"HITRON-8DB0"
#define AP_PASSWORD		"FAZTKFNVJ8Q6"

#define READ_INTERVAL	60 
#define SENSOR_ID		1

#define SERVER_ADDRESS  "iot.neve.nu"
#define SERVER_PORT     80

#define ONE_WIRE_BUS 2  
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

float oldTemp;
int	counter=0;

void setup() {
	Serial.begin(115200);
	wifiConnect();
	oldTemp = -1;
}

void loop() {
	float temp;
	do {
		DS18B20.requestTemperatures();
		temp = DS18B20.getTempCByIndex(0);
		ANSIclrsHome();
		Serial.print("Temperature: ");
		Serial.println(temp);
	} while (temp == 85.0 || temp == (-127.0));
	
	if (temp != oldTemp)
	{
		sendTemperature(temp);
		oldTemp = temp;
	}
	else {
		Serial.print("Same shit, not sending!");
	}
	
	int cnt = READ_INTERVAL;
	while(--cnt) delay(1000);
}

void wifiConnect()
{	
	ANSIclrsHome();
	Serial.print("Connecting to AP");
	WiFi.begin(AP_SSID, AP_PASSWORD);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.print(".");
	}
	
	Serial.println("");
	Serial.println("WiFi connected");
	delay(2000);
}

void sendTemperature(float temp)
{
	WiFiClient client;
	
	while(!client.connect(SERVER_ADDRESS, SERVER_PORT)) {
		Serial.println("connection failed");
		wifiConnect();
	}

	String url = "/index.php?S=" + String(SENSOR_ID) + "&T=" + String(temp);
	//url += "/index.php?S=" + String(SENSOR_ID) + "&T=" + String(temp);
	
	Serial.print("POST data to URL: ");
	Serial.println(url);
	
	client.print(String("GET ") + url + " HTTP/1.1\r\n" +
	"Host: " + String(SERVER_ADDRESS) + "\r\n\r\n" );

	delay(1000);
	while(client.available()){
		client.readStringUntil('@');	// DEBUG SERVER comment out this line
		String line = client.readString();
		Serial.print(line);
	}
	
	Serial.println();
	client.stop();
	client.flush();
	Serial.print(counter++);
	Serial.print("# Connections closed. Heap:");
	Serial.print(ESP.getFreeHeap());
}

void ANSIclrsHome(){
	Serial.write(0x1b);
	Serial.write("[1J");
	Serial.write(0x1b);
	Serial.write("[H");
}
void ANSIgotoxy(int x, int y){
	Serial.write(0x1b);
	Serial.write("[");
	Serial.print(x);
	Serial.write(';');
	Serial.print(y);
	Serial.write("H");
}

