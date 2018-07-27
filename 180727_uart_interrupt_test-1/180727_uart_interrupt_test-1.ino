// https://qiita.com/hotchpotch/items/977f5d1d29006c06d675
#include <avr/interrupt.h>
int ledPin = 13;
int interruptPin = 3;

volatile int state = LOW;

volatile uint8_t data;
char TXBUFFER[64]; // UART PDC Transmit buffer
//void UART_Handler (void) __attribute__ ((weak));
void UART_Handler (void);

void UART_Handler(void) {
	Serial.IrqHandler();
	//cli();
	//Serial.println("hellohello");
	blink();
  //Serial.IrqHandler();
  //sei();
}

void serialEvent()
{
	Serial.println("dada");
	Serial.read();
}

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    //attachInterrupt(0, blink, CHANGE);
    Serial.begin(38400);
}

void loop() {
    //digitalWrite(ledPin, state);
}

void blink() {
    state = !state;
    digitalWrite(ledPin, state);
}
