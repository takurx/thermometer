#include <Streaming.h>
#include <PString.h>

// 抵抗値/温度リスト
float rtLsit[100]; 
// 抵抗分圧用の10kΩ抵抗
int R1 = 10000; 

char serialBuffer[4800];
PString serialStr(serialBuffer, sizeof(serialBuffer));

// NTCサーミスタの現在温度から抵抗値を算出する
// B:B定数 R0:基準温度(T1)での抵抗値 T1:基準温度 T2:現在温度 
float NTCThermistor_res(int B, int R0 ,float T1, float T2){
  const float e = 2.7182818284; // ネイピア数
  float exponent = B * ((1.0 / (T2 + 273.15)) - (1.0 / (T1 + 273.15)));
  return (R0 * pow(e,exponent));
}
 
void setup() {
  Serial.begin(38400);
 
  // 0-100℃までの抵抗値/温度リストを生成
  for (int i =0; i < 100; i++){
    // 温度から抵抗値を算出する
    rtLsit[i] = NTCThermistor_res(3435,10000,25,i);
  }
}
 
void loop() {
  analogReadResolution(10);
  serialStr << "test, ADC 10bit: " << analogRead(A0) << ", ";
  
  // アナログ入力の値を電圧(V)に変換
  float voltage = (analogRead(A0) / 1024.0) * 3.3;
 
  serialStr << "Voltage_in: " << voltage << "V Resistor_Val: ";
  
  // サーミスタの抵抗値
  float resistance = voltage / (3.3 - voltage) * R1;
  if(resistance > 999){
    serialStr << resistance / 1000 << "k";
  }else{
    serialStr << resistance;
  }    
  serialStr << "Ohm ";
  
  // サーミスタの周辺温度
  serialStr << "Tempurature: ";
  for (int i =0; i < 100; i++){
    if (resistance == rtLsit[i]){
      serialStr << i << "deg.";
      break;
    }
    if (resistance > rtLsit[i]){
      serialStr << i-1 << "deg.";
      break;
    }    
  }

  serialStr << endl;
  Serial.print(serialStr);
  serialStr = "";
  
  delay(1000);
}
