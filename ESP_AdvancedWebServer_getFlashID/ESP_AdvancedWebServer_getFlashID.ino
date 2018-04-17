

/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * INFO ON FLASH CHIP ID:
 * // GigaDevice
        case 0x1740C8: // GD25Q64B
            return (8_MB);
        case 0x1640C8: // GD25Q32B
            return (4_MB);
        case 0x1540C8: // GD25Q16B
            return (2_MB);
        case 0x1440C8: // GD25Q80
            return (1_MB);
        case 0x1340C8: // GD25Q40
            return (512_kB);
        case 0x1240C8: // GD25Q20
            return (256_kB);
        case 0x1140C8: // GD25Q10
            return (128_kB);
        case 0x1040C8: // GD25Q12
            return (64_kB);

        // Winbond
        case 0x1640EF: // W25Q32
            return (4_MB);
        case 0x1540EF: // W25Q16
            return (2_MB);
        case 0x1440EF: // W25Q80
            return (1_MB);
        case 0x1340EF: // W25Q40
            return (512_kB);

 * 
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "HITRON-8DB0";
const char *password = "FAZTKFNVJ8Q6";
MDNSResponder mdns;

ESP8266WebServer server ( 80 );

EspClass object;
const int led = 13;
const int led2 = 14;


void handleRoot() {
	digitalWrite ( led, 1 );
	char temp[1000];
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;
  uint32_t chip = object.getFlashChipId();
  uint8_t cpufrq = object.getCpuFreqMHz();
  uint32_t heap = object.getFreeHeap();
	snprintf ( temp, 1000,

//"<html><head><meta http-equiv='refresh' content='30'/><title>ESP8266 Demo</title>\
//<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>\
//</head><body>\
//<h1>Hello from ESP8266!</h1>\
//<p>Uptime: %02d:%02d:%02d</p><p> Flash chip ID:%8x</p><p>uc clock:%4d</p><p>Free Heap:%8d</p>\
//<img src=\"/test.svg\" />\
//</body></html>"

"<!DOCTYPE html><html><head><meta name=viewport content='width=250'><style type='text/css'>.btn {-webkit-border-radius: 15;-moz-border-radius: 15;border-radius: 15px;font-family: Courier New;color: #000000;font-size: 20px;padding: 10px 20px 10px 20px;border: solid #000000 2px;text-decoration: none;text-align: center;float: left;}</style>\
</head><body>\
<button class='btn' onclick='pincontrol(14,1)'>14-ON</button> <button class='btn' onclick='pincontrol(14,0)'>14-OFF</button>\
<label> heap:%8d</label>\
<script>function pincontrol(pin,state) {var xhttp = new XMLHttpRequest();xhttp.open('GET', 'http://192.168.0.15/x?'+pin+'='+ state, true);xhttp.send();}</script></body></html>"

	,heap	//,hr, min % 60, sec % 60, chip,cpufrq,heap
	);
	server.send ( 200, "text/html", temp );
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
void handleDigitalWrite(){
  String message = "OK\n\n";
  String portString = server.argName(0);
  message += portString;
  message += "pin is set to:";
  int portnr = portString.toInt();
  String stateString = server.arg (0);
  message += stateString;
  int statenr = stateString.toInt();
  digitalWrite(portnr,statenr);
  server.send (200, "text/plain", message );
}
void handleAnalogWrite(){
  String message = "OK\n\n";
  String portString = server.argName(0);
  message += portString;
  message += "pin is set to:";
  int portnr = portString.toInt();
  String stateString = server.arg (0);
  message += stateString;
  int statenr = stateString.toInt();
  analogWrite(portnr,statenr);
  server.send (200, "text/plain", message );
}
void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x+= 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send ( 200, "image/svg+xml", out);
}

void setup ( void ) {
	pinMode ( 13, OUTPUT );
  pinMode ( 14, OUTPUT );
  pinMode ( 12, OUTPUT );
  pinMode ( 15, OUTPUT );
  pinMode ( 2, OUTPUT );
  pinMode ( 16, OUTPUT );
	digitalWrite ( led, 0 );
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

	if ( mdns.begin ( "esp1" ,3600) ) {
		Serial.println ( "MDNS responder started" );
	}


  server.on ( "/", handleRoot );
  server.on ( "/x", handleDigitalWrite );
  server.on ( "/y", handleAnalogWrite );
	server.on ( "/test.svg", drawGraph );
	server.on ( "/inline", []() {
		server.send ( 200, "text/plain","Inline test" );
	} );
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );
 mdns.addService("http", "tcp", 80);
}

void loop ( void ) {
	mdns.update();
	server.handleClient();
}


