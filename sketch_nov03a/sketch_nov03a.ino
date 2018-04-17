void setup() {
 Serial.begin(115200);
 

}

void loop() {
 Serial.println("print ;");
 Serial.write("print #");
Serial.println();
}
