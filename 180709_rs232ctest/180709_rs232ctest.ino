void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
}

uint8_t incomingByte;

void loop()
{
    //if (Serial.available() > 0) {     // 受信したデータが存在する
    //    incomingByte = Serial.read(); // 受信データを読み込む
    //    Serial.write(incomingByte);   // データを出力する
    //}

    Serial.write("Hello");   // データを出力する
    Serial1.write("Hello");   // データを出力する
    delay(1000);                       // wait for a second
}
