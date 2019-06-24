#define DATA_IN 2
#define DATA_OUT 3
#define CLOCK_IN 4
#define CLOCK_OUT 5
#define ATN_A 6
#define DEVICE_A 7
#define DEVICE_B 8
#define ATN_IN 9
#define RESET 10

#define LED 13

enum class InterfaceProtocol : uint8_t {
    Ready = 0,
    SetDirection = 1,
    SetPort = 2,
    GetPort = 3,
    Invalid = 0xFF
};

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    
    Serial.print((char)InterfaceProtocol::Ready);
    InterfaceProtocol response = InterfaceProtocol::Invalid;
    while (response != InterfaceProtocol::Ready) {
        if (Serial.available()) {
            response = (InterfaceProtocol)Serial.read();
        }
        digitalWrite(LED, HIGH);
        delay(50);
        digitalWrite(LED, LOW);
        delay(50);
        digitalWrite(LED, HIGH);
        delay(50);
        digitalWrite(LED, LOW);
        delay(250);
    }
    
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
        //InterfaceProtocol com = (InterfaceProtocol)Serial.read();
        InterfaceProtocol com = (InterfaceProtocol)Serial.read();
        Serial.print((char)com);
        /*switch (com) {
        case InterfaceProtocol::GetPort:
            delay(5000);
            digitalWrite(LED, HIGH);
            delay(1000);
            digitalWrite(LED, LOW);
            delay(1000);
            digitalWrite(LED, HIGH);
            delay(1000);
            digitalWrite(LED, LOW);
            break;
        case InterfaceProtocol::SetPort:
            break;
        case InterfaceProtocol::SetDirection:
            break;
        default:
            Serial.print((char)InterfaceProtocol::Invalid);
            break;
        }*/
    }
    else {
        /*digitalWrite(LED, HIGH);
        delay(250);
        digitalWrite(LED, LOW);
        delay(250);*/
    }
}
