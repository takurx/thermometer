// http://d.hatena.ne.jp/kougaku-navi/20141105/p1

void setup() {
  String str;
  Serial.begin(38400);
  
  Serial2.begin(9600);
  //Serial2.begin(38400);
  Serial2.print("BCD0\r\n");
  str = Serial2.readStringUntil('\r');
  Serial.println(str);
  //Serial2.print("BST5000001\r\n"); //0001:9600bps
  Serial2.print("BST5000003\r\n"); //0003:38400bps
  str = Serial2.readStringUntil('\r');
  Serial.println(str);
  Serial2.print("BCD3\r\n");
  str = Serial2.readStringUntil('\r');
  Serial.println(str);
  
  //Serial2.begin(9600);
  Serial2.begin(38400);
  //Serial2.setTimeout(15000UL); // タイムアウト設定（15秒）
  pinMode(10,OUTPUT);
}

void loop() {
  if ( Serial2.available() > 0 ) {
    String str = Serial2.readStringUntil('\r');

    Serial2.println();
    if ( str == "on") {
      digitalWrite(10,HIGH);
      Serial2.println("> ok : LED13 turn on");

      Serial.println("> ok : LED13 turn on");
    }
    else if ( str=="off" ) {
      digitalWrite(10,LOW);
      Serial2.println("> ok : LED13 turn off");

      Serial.println("> ok : LED13 turn off");
    }
    else {
      Serial2.print("> error [");
      Serial2.print(str);
      Serial2.println("]");

      Serial.print("> error [");
      Serial.print(str);
      Serial.println("]");
    }
  }
}
