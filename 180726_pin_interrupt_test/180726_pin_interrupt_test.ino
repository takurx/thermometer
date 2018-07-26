// https://qiita.com/hotchpotch/items/977f5d1d29006c06d675
int ledPin = 13;
int interruptPin = 3;

volatile int state = LOW;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
    //digitalWrite(ledPin, state);
}

void blink() {
    state = !state;
    digitalWrite(ledPin, state);
}
