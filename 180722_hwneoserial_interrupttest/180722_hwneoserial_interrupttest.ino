// http://d.hatena.ne.jp/kougaku-navi/20141105/p1
// https://github.com/SlashDevin/NeoHWNeoSerial

#include <NeoHWSerial.h>

volatile uint32_t newlines = 0UL;

static void handleRxChar( uint8_t c)
{
  if(c == '\n')
  {
    newlines++;
    NeoSerial.println(newlines);
  }
}

void setup()
{
  NeoSerial2.attachInterrupt(handleRxChar);
  NeoSerial2.begin(38400);
  
  NeoSerial.begin(38400);
}

void loop()
{
  
}

/*
void setup()
{
  String str;
  NeoSerial.begin(38400);
  
  NeoSerial2.begin(9600);
  //NeoSerial2.begin(38400);
  NeoSerial2.print("BCD0\r\n");
  str = NeoSerial2.readStringUntil('\r');
  NeoSerial.println(str);
  //NeoSerial2.print("BST5000001\r\n"); //0001:9600bps
  NeoSerial2.print("BST5000003\r\n"); //0003:38400bps
  str = NeoSerial2.readStringUntil('\r');
  NeoSerial.println(str);
  NeoSerial2.print("BCD3\r\n");
  str = NeoSerial2.readStringUntil('\r');
  NeoSerial.println(str);
  
  //NeoSerial2.begin(9600);
  NeoSerial2.begin(38400);
  //NeoSerial2.setTimeout(15000UL); // タイムアウト設定（15秒）
  pinMode(10,OUTPUT);
}

void loop() 
{
  if ( NeoSerial2.available() > 0 ) {
    String str = NeoSerial2.readStringUntil('\r');

    NeoSerial2.println();
    if ( str == "on") {
      digitalWrite(10,HIGH);
      NeoSerial2.println("> ok : LED13 turn on");

      NeoSerial.println("> ok : LED13 turn on");
    }
    else if ( str=="off" ) {
      digitalWrite(10,LOW);
      NeoSerial2.println("> ok : LED13 turn off");

      NeoSerial.println("> ok : LED13 turn off");
    }
    else {
      NeoSerial2.print("> error [");
      NeoSerial2.print(str);
      NeoSerial2.println("]");

      NeoSerial.print("> error [");
      NeoSerial.print(str);
      NeoSerial.println("]");
    }
  }
}
*/
