// http://d.hatena.ne.jp/kougaku-navi/20141105/p1

void setup() {
  Serial.begin(38400);
  Serial.setTimeout(15000UL); // タイムアウト設定（15秒）
  pinMode(10,OUTPUT);
}

void loop() {
  if ( Serial.available() > 0 ) {
    String str = Serial.readStringUntil('\r');

    Serial.println();
    if ( str == "on") {
      digitalWrite(10,HIGH);
      Serial.println("> ok : LED13 turn on");
    }
    else if ( str=="off" ) {
      digitalWrite(10,LOW);
      Serial.println("> ok : LED13 turn off");
    }
    else {
      Serial.print("> error [");
      Serial.print(str);
      Serial.println("]");
    }
  }
}
