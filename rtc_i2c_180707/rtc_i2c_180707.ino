#include <Wire.h>
 
// 外部割り込みの状態
volatile int state = LOW; 
 
// RTCのレジスタテーブル(16byte)
int RegTbl[16];  
 
// デバイスアドレス(スレーブ)
// ※Arduinoの仕様では8bitのアドレスを右に1bitシフトした「7bit」を使用する
// ※[8bit]Write : 0xA2 = 10100010 Read : 0xA3 = 10100011
byte DEVICE_ADDRESS= 0x51;  
 
// 2進化10進数(BCD)を10進数に変換
byte BCDtoDec(byte value){
  return ((value >> 4) * 10) + (value & 0x0F) ;
}
 
// 外部割り込みが発生した時に動作します。
void myInterrupt() {
  state = !state;     
}
 
void setup() {
  Serial.begin(9600);  
 
  // タイマー割り込みの解除用
  pinMode(8,INPUT);
    
  // 2ピンの状態がLOWからHIGHに変化した場合に外部割り込みを発生させる
  attachInterrupt(digitalPinToInterrupt(2), myInterrupt, RISING);
  
  // マスタとしてI2Cバスに接続する
  Wire.begin(); 
  
  // I2Cスレーブに対して送信処理を開始する
  Wire.beginTransmission(DEVICE_ADDRESS);
    
    // レジスタのアドレスを指定する(0-15の内、先頭から)
    Wire.write(0x00);
 
    // ---------------------------------
    //  コントロールレジスタ
    // ---------------------------------    
    // [00]Control1
    Wire.write(0x00);
    // [01]Control2    
    // 0x02:アラーム割り込み時に3ピン[INT]をLOWにする
    Wire.write(0x02);
    
    // ---------------------------------
    //  時計・カレンダーレジスタ
    // ---------------------------------     
    // アドレスの[02-05]及び[08-11]は2進化10進数(BCD)で指定します。
     
    // [02]Seconds(15秒)
    Wire.write(0x30);
    // [03]Minutes(20分)
    Wire.write(0x20);
    // [04]Hours(12時)
    Wire.write(0x12);
    // [05]Days(25日)    
    Wire.write(0x25);
    // [06]Weekdays(月) 
    // 0:日 1:月 2:火 3:水 4:木 5:金 6:土 
    Wire.write(0x01);    
    // [07]Month/Century(21世紀の12月)
    // ・Month(01-12) BCD形式
    // ・Century(先頭bit 0:20世紀[19xx年代],1(0x80):21世紀[20xx年代])
    Wire.write(0x12 | 0x80);        
    // [08]Years(2017年)     
    Wire.write(0x17);
    
    // ---------------------------------
    //  アラームレジスタ
    // ---------------------------------  
    // 先頭bitに1がある場合(0x80)はアラーム対象外となる
    
    // 例)次のコードは「25日(月)12時21分」にアラームが設定されていますが、
    //    「25日」と「(月)」には0x80がありますので、実際のアラームは「12時21分」で毎日となります。
    
    // [09]Minutes alarm(21分)
    Wire.write(0x21);
    // [0A]Hours Alarm(12時)
    Wire.write(0x12);
    // [0B]Days Alarm(25日)
    Wire.write(0x25 | 0x80);
    // [0C]Weekdays Alarm(月)  
    // 0:日 1:月 2:火 3:水 4:木 5:金 6:土   
    Wire.write(0x01 | 0x80);
 
    // ---------------------------------
    //  クロック出力レジスタ
    // --------------------------------- 
    
    // [0D]CLKOUT Frequency
    // ・クロック出力機能を有効にする。
    //   ※有効:0x80 無効 :0x00
    // ・クロック周波数は1Hz(1秒間に1回)とする
    //   ※0x00:32768Hz, 0x01:1024Hz, 0x02:32Hz, 0x03:1Hz
    // 
    // 次の設定だと2ピン[CLKOUT]から1秒に1回、クロック出力される     
    Wire.write(0x80 | 0x03);
 
  // I2Cスレーブへの送信完了
  Wire.endTransmission();
  
  Serial.println("RTCの初期化完了。");
}
 
void loop(){
  int i;
  
  // 外部割込みが発生した場合
  if(state == HIGH){
    // レジスタのアドレスを先頭にする
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission(); 
 
    // I2Cスレーブに16byteのレジスタデータを要求する
    Wire.requestFrom(DEVICE_ADDRESS,16);
                
    // 16byteのデータを取得する
    for (i=0; i<16; i++){
      while (Wire.available() == 0 ){}
      RegTbl[i] = Wire.read();
    }
    
    // 現在日時
    Serial.print("現在：");
    Serial.print(String(BCDtoDec(RegTbl[8])+ 2000)  + "年");
    Serial.print(String(BCDtoDec(RegTbl[7] & 0x1F)) + "月");
    Serial.print(String(BCDtoDec(RegTbl[5] & 0x3F)) + "日");
     
    switch(RegTbl[6] & 0x07){
      case 0 :  Serial.print("(日)");break;
      case 1 :  Serial.print("(月)");break;
      case 2 :  Serial.print("(火)");break;
      case 3 :  Serial.print("(水)");break;
      case 4 :  Serial.print("(木)");break;
      case 5 :  Serial.print("(金)");break;
      case 6 :  Serial.print("(土)");break;
    }
    Serial.print(" ");
    Serial.print(String(BCDtoDec(RegTbl[4] & 0x3F)) + "時");
    Serial.print(String(BCDtoDec(RegTbl[3] & 0x7F)) + "分");
    Serial.print(String(BCDtoDec(RegTbl[2] & 0x7F)) + "秒");
    
    // アラーム日時
    Serial.print(" アラーム：");
    if((RegTbl[11] & 0x80) == 0){
      Serial.print(String(BCDtoDec(RegTbl[11] & 0x3F)) + "日");
    }
    
    if((RegTbl[12] & 0x80) == 0){
      switch(RegTbl[12] & 0x07){
        case 0 :  Serial.print("(日) ");break;
        case 1 :  Serial.print("(月) ");break;
        case 2 :  Serial.print("(火) ");break;
        case 3 :  Serial.print("(水) ");break;
        case 4 :  Serial.print("(木) ");break;
        case 5 :  Serial.print("(金) ");break;
        case 6 :  Serial.print("(土) ");break;
      }    
    }
    
    if((RegTbl[10] & 0x80) == 0){
      Serial.print(String(BCDtoDec(RegTbl[10] & 0x3F)) + "時");
    }
    
    if((RegTbl[9] & 0x80) == 0){
      Serial.print(String(BCDtoDec(RegTbl[9] & 0x7F)) + "分");
    }
    
    Serial.println("");
    state = !state;  
  }
 
  // ボタン押下時にアラーム割り込みイベントをクリアする(LEDを消灯する)
  if(digitalRead(8) == HIGH){
    // レジスタのアドレスをControl2にする
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(0x01);
    
    // アラーム割り込みが発生した際に
    // Control2の4bit目のAFフラグが「1」になるので「0」にする 
    // それにより、3ピン[INT]がHIGHになりLEDが消灯します。
    Wire.write(0x02);
    Wire.endTransmission(); 
  }  
}
