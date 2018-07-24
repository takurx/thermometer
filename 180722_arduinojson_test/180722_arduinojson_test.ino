// Copyright Benoit Blanchon 2014-2016
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!
// http://shuzo-kino.hateblo.jp/entry/2016/05/06/203603

#include <ArduinoJson.h>

void setup() {
  Serial.begin(9600);

  // シリアルの初期化待ち
  while (!Serial) { }

  // JSON用の固定バッファを確保。
  //この例では200文字分。
  StaticJsonBuffer<200> jsonBuffer;

  // char[]の形式でJSONの文字列を持つ。
  // char* 形式では文字列のコピーを保持してしまうためヨロシクナイ
  char json[] =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

  // JsonObjectはJSONBufferに格納、パースされた結果を参照保持する。
  // データを参照する場合はこのJsonObjectを見に行く。
  // メモリはスコープから出た際に開放される。
  JsonObject& root = jsonBuffer.parseObject(json);

  // パースが成功したか確認。できなきゃ終了
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  // データの取り出しは暗黙の型変換
  const char* sensor = root["sensor"];
  long time = root["time"];
  double latitude = root["data"][0];
  double longitude = root["data"][1];

  // データの表示。.
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude, 6);
}

void loop() {
  // 今回は未使用
}
