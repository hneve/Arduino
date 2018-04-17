#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NeoPixelBus.h>
#define pixelCount 8
#define colorSaturation 255
NeoPixelBus strip = NeoPixelBus(pixelCount, 14, NEO_GRB);
RgbColor red = RgbColor(colorSaturation, 0, 0);
RgbColor green = RgbColor(0, colorSaturation, 0);
RgbColor blue = RgbColor(0, 0, colorSaturation);
RgbColor white = RgbColor(colorSaturation);
RgbColor black = RgbColor(0);

const char *ssid = "HITRON-8DB0";
const char *password = "FAZTKFNVJ8Q6";

MDNSResponder mdns;
ESP8266WebServer server ( 80 );
const int led = 13;
void handleRoot() {
    digitalWrite ( led, 1 );
  String out = "<html><head><title>Wifi light</title></head>";
  out += "<body style='background-color:gray'>";
  out += "<span style='display:block; width:100%; font-size:2em; font-family:Verdana; text-align:center'>Choose color</span><br/>";
  out += "<a href='white'><span style='display:block; background-color:white; width:100%; height:6em;'></span></a><br/>";
  out += "<a href='red'><span style='display:block; background-color:red; width:100%; height:6em;'></span></a><br/>";
  out += "<a href='green'><span style='display:block; background-color:green; width:100%; height:6em;'></span></a><br/>";
  out += "<a href='blue'><span style='display:block; background-color:blue; width:100%; height:6em;'></span></a><br/>";
  out += "<a href='black'><span style='display:block; background-color:black; width:100%; height:6em;'></span></a>";
  out += "</body>";
  out += "</html>";
    server.send ( 200, "text/html", out );
    digitalWrite ( led, 0 );
}
void handleNotFound() {
    digitalWrite ( led, 1 );
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for ( uint8_t i = 0; i < server.args(); i++ ) {
        message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
    }
    server.send ( 404, "text/plain", message );
    digitalWrite ( led, 0 );
}
void setColor(RgbColor color) {
  int i;
  for (i=0; i<pixelCount; i++) {
    strip.SetPixelColor(i, color);
  }
  strip.Show();
}
void setup ( void ) {
    pinMode ( led, OUTPUT );
    digitalWrite ( led, 0 );
  strip.Begin();
  strip.Show();

    Serial.begin ( 115200 );
    WiFi.begin ( ssid, password );
    Serial.println ( "" );
    // Wait for connection
    while ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 );
        Serial.print ( "." );
    }
    Serial.println ( "" );
    Serial.print ( "Connected to " );
    Serial.println ( ssid );
    Serial.print ( "IP address: " );
    Serial.println ( WiFi.localIP() );
    if ( mdns.begin ( "esp8266", WiFi.localIP() ) ) {
        Serial.println ( "MDNS responder started" );
    }
    server.on ( "/", []() {handleRoot();} );
  server.on ( "/white", []() {setColor(white); handleRoot();} );
  server.on ( "/red", []() {setColor(red); handleRoot();} );
  server.on ( "/green", []() {setColor(green); handleRoot();} );
  server.on ( "/blue", []() {setColor(blue); handleRoot();} );
  server.on ( "/black", []() {setColor(black); handleRoot();} );
    server.onNotFound ( handleNotFound );
    server.begin();
    Serial.println ( "HTTP server started" );
}
void loop ( void ) {
    mdns.update();
    server.handleClient();
}

