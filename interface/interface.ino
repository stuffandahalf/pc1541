#define DATA_IN 2
#define DATA_OUT 3
#define CLOCK_IN 4
#define CLOCK_OUT 5
#define ATN_A 6
#define DEVICE_A 7
#define DEVICE_B 8
#define ATN_IN 9
#define RESET 10

enum PCProtocol {
    PORT,
    DDR,
    INTERRUPT
};

void setup() {
    Serial.begin(115200);
    
    /*Serial.println(DDRB, HEX);
    Serial.println(PORTB, HEX);
    pinMode(8, INPUT);
    digitalWrite(8, HIGH);
    Serial.println(DDRB, HEX);
    Serial.println(PORTB, HEX);*/
}

void loop() {
    if (Serial.available()) {
        //Serial.print((char)Serial.read());
        PCProtocol requestMessage = Serial.read();
        switch (requestMessage) {
        case PORT:
            break;
        case DDR:
            break;
        case INTERRUPT:
            break;
        }
    }
}
