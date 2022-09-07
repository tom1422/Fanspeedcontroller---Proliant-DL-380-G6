#include <DallasTemperature.h>
#include <OneWire.h>

// PWM output @ 25 kHz, only on pins 9 and 10.
// Output value should be between 0 and 320, inclusive.

int currSpeed = 100;
int noComSpeed = 100;

int speedToSetFan = 100;

unsigned long lastNumRecv = 0;


void analogWrite25k(int pin, int value)
{
    switch (pin) {
        case 9:
            OCR1A = value;
            break;
        case 10:
            OCR1B = value;
            break;
        default:
            // no other pin will work
            break;
    }
}

void setup()
{

    Serial.begin(9600);    //Start diagnostic serial port
    
    // Configure Timer 1 for PWM @ 25 kHz.
    TCCR1A = 0;           // undo the configuration done by...
    TCCR1B = 0;           // ...the Arduino core library
    TCNT1  = 0;           // reset timer
    TCCR1A = _BV(COM1A1)  // non-inverted PWM on ch. A
           | _BV(COM1B1)  // same on ch; B
           | _BV(WGM11);  // mode 10: ph. correct PWM, TOP = ICR1
    TCCR1B = _BV(WGM13)   // ditto
           | _BV(CS10);   // prescaler = 1
    ICR1   = 320;         // TOP = 320

    // Set the PWM pins as output.
    pinMode( 9, OUTPUT);
    pinMode(10, OUTPUT);

    setFanspeeds(25);
}

void loop() {
    if (Serial.available() > 0) {
      int newInt = Serial.parseInt(SKIP_ALL);
      setFanspeeds(newInt);
      clearExtraBytes();
    }

    if (millis() - lastNumRecv > 10000) {
      //Has been 10 secons since last number was recieved - set fans to 100
      setFanspeeds(noComSpeed);
    }
}

void setFanspeeds(int newInt) {
    lastNumRecv = millis();
    Serial.println("Setting fan speed to: " + String(newInt));
    currSpeed = newInt;
    speedToSetFan = map(currSpeed, 0, 100, 26, 100);
    analogWrite25k( 9, map(speedToSetFan,0,100,320,0)); 
}

void clearExtraBytes() {
    while (Serial.available() > 0) {
        int temp = Serial.read();
        Serial.print("Discarding: ");
        Serial.println(temp, DEC);
    }
}
