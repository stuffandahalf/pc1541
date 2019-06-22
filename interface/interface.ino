void setup() {
    Serial.begin(57600);
    
    Serial.println(DDRB, HEX);
    Serial.println(PORTB, HEX);
    pinMode(8, INPUT);
    Serial.println(DDRB, HEX);
}

void loop() {
    if (Serial.available()) {
        Serial.print((char)Serial.read());
    }
}
