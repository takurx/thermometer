#include <DueTimer.h>

// https://miso-engine.hatenablog.com/entry/2015/07/20/221014
// #include <MsTimer2.h>

volatile int state = LOW;

void timerFire() {
  //500ms毎にここが呼び出される
  state = !state;
  digitalWrite(LED_BUILTIN, state);   // turn the LED on (HIGH is the voltage level)
}

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  //500ms毎にtimer発火
  Timer2.attachInterrupt(timerFire);
  Timer2.start(100000); // 500ms = 500,000us
}

void loop() {

}

/*
//https://qiita.com/hotchpotch/items/977f5d1d29006c06d675
int ledPin = 13;
int interruptPin = 2;
volatile int state = LOW;

void setup() {
    pinMode(ledPin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
    digitalWrite(pin, state);
}

void blink() {
    state = !state;
}
*/
