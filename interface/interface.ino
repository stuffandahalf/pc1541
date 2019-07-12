//#define NDEBUG

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

/*#define BIT_DATA_IN 1
#define BIT_DATA_OUT 2
#define BIT_CLK_IN 4
#define BIT_CLK_OUT 8
#define BIT_ATN_A 16
#define BIT_DEVICE_A 32
#define BIT_DEVICE_B 64
#define BIT_ATN_IN 128*/

uint8_t portPins[] = { DATA_IN, DATA_OUT, CLOCK_IN, CLOCK_OUT, ATN_A, DEVICE_A, DEVICE_B, ATN_IN };

uint8_t directionRegister;
uint8_t value;

enum class InterfaceProtocol : uint8_t {
    Ready = 0,
    SetDirection = 1,
    SetPort = 2,
    GetPort = 3,
    Invalid = 0xFF
};

// true = output, false = input
inline void setPinDirection(uint8_t pin, bool direction) {
    pinMode(pin, direction ? OUTPUT : INPUT);
#ifndef NDEBUG
    //digitalWrite(LED, !digitalRead(LED));
    digitalWrite(LED, direction);
#endif
}

void setup() {
    Serial.begin(115200);
    while (!Serial) { }
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
        //Serial.print((char)com);
        switch (com) {
        case InterfaceProtocol::GetPort:
            /*delay(5000);
            digitalWrite(LED, HIGH);
            delay(1000);
            digitalWrite(LED, LOW);
            delay(1000);
            digitalWrite(LED, HIGH);
            delay(1000);
            digitalWrite(LED, LOW);*/
            
            
            break;
        case InterfaceProtocol::SetPort:
            break;
        case InterfaceProtocol::SetDirection:
            while (!Serial.available());
            directionRegister = (uint8_t)Serial.read();
            
            /*setPinDirection(DATA_IN, directionRegister & BIT_DATA_IN);
            setPinDirection(DATA_OUT, directionRegister & BIT_DATA_OUT);
            setPinDirection(CLOCK_IN, directionRegister & BIT_CLK_IN);
            setPinDirection(CLOCK_OUT, directionRegister & BIT_CLK_OUT);
            setPinDirection(ATN_A, directionRegister & BIT_ATN_A);
            setPinDirection(DEVICE_A, directionRegister & BIT_DEVICE_A);
            setPinDirection(DEVICE_B, directionRegister & BIT_DEVICE_B);
            setPinDirection(ATN_IN, directionRegister & BIT_ATN_IN);*/
            
            for (int i = 0; i < 8; i++) {
                setPinDirection(portPins, (directionRegister >> i) & 1);
                delay(500);
            }
            
            Serial.print((char)InterfaceProtocol::Ready);
            break;
        default:
            Serial.print((char)InterfaceProtocol::Invalid);
            break;
        }
    }
    else {
        /*digitalWrite(LED, HIGH);
        delay(250);
        digitalWrite(LED, LOW);
        delay(250);*/
    }
}
