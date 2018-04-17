#include <U8glib.h>
#include <NewPing.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <DHT.h>

#define TRIGGER_PIN 3 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 2 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define DHTPIN 4 // Pin for temp/fukt måler
#define DHTTYPE DHT11 // DHT 11
U8GLIB_PCD8544 u8g(13, 11, 10, 9, 8); // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
setSyncProvider(RTC.get); // the function to get the time from the RTC 
} 

void loop() {
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();



// picture loop
u8g.firstPage();
do {
u8g.setContrast(128);
u8g.drawRFrame(0, 0, 84, 10, 3); //Data1 frame
u8g.drawRFrame(0, 12, 84, 10, 3); //Data1 label frame
u8g.drawRFrame(0, 24, 84, 10, 3); //Data2 label frame
u8g.drawRFrame(0, 36, 84, 10, 3); //Data2 frame
u8g.setFont(u8g_font_5x7);
u8g.setPrintPos(4, 20);
u8g.print(hour());
u8g.print(":");
u8g.print(minute());
u8g.print(" ");
u8g.print(day());
u8g.print("/");
u8g.print(month());
u8g.print("/");
u8g.print(year());
u8g.drawStr(4, 8, "AKVARIUM");
u8g.drawStr(4, 32, "Temp");
u8g.drawStr(4, 44, "Avstand");
delay(50);
u8g.setPrintPos(50, 44);
u8g.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
u8g.print("cm");
} while ( u8g.nextPage() );
delay(2600);

u8g.firstPage();
do {
u8g.setContrast(128);
u8g.drawRFrame(0, 0, 84, 10, 3); //Data1 frame
u8g.drawRFrame(0, 12, 84, 10, 3); //Data1 label frame
u8g.drawRFrame(0, 24, 84, 10, 3); //Data2 label frame
u8g.drawRFrame(0, 36, 84, 10, 3); //Data2 frame
u8g.setFont(u8g_font_5x7);
u8g.drawStr(4, 8, "ROMMET");
u8g.setPrintPos(4, 20);
u8g.print(hour());
u8g.print(":");
u8g.print(minute());
u8g.print(" ");
u8g.print(day());
u8g.print("/");
u8g.print(month());
u8g.print("/");
u8g.print(year());
// u8g.drawStr(4, 20, "Data6");
u8g.drawStr(4, 32, "Temp");
u8g.setPrintPos(50, 32);
u8g.print(t);
u8g.print("C");
u8g.drawStr(4, 44, "Fukt");
u8g.setPrintPos(50, 44);
u8g.print(h);
u8g.print("%");
//u8g.setPrintPos(4, 15);
//u8g.print();
//u8g.setPrintPos(4, 46);
// u8g.print();
} while ( u8g.nextPage() );
delay(2600);
}
