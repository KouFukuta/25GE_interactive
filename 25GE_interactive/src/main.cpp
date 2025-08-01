#include <M5Unified.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

bool tuningMode = false;
unsigned long previousMillis = 0;
const long interval = 500;  // 500msごとにトグル

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  SerialBT.begin("MindHound_Nuigurumi");
  M5.Lcd.println("Bluetooth Started");
}

void loop() {
  M5.update();

  if (SerialBT.available()) {
    String input = SerialBT.readStringUntil('\n');
    input.trim();

    if (input == "connected") {
      M5.Lcd.println("Connected to MindHound system.");
      M5.Speaker.tone(4000, 2000);
    }
    else if (input == "response") {
      M5.Lcd.println("Response creating...");
      M5.Power.setLed(255);
    }
    else if (input == "finishResponse") {
      M5.Lcd.println("Response finished.");
      M5.Power.setLed(0);
      M5.Speaker.tone(4000, 500);
    }
    else if (input == "Tuning") {
      M5.Lcd.println("FineTuning mode activated.");
      tuningMode = true;
      previousMillis = millis();
    }
    else if (input == "TuningEnd") {
      M5.Lcd.println("FineTuning mode ended.");
      tuningMode = false;
      M5.Power.setLed(0);
      M5.Speaker.end();  // スピーカー停止
    }
    else if (input == "ping") {
      SerialBT.println("pong");
    }
  }

  if (tuningMode) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      static bool ledOn = false;
      if (ledOn) {
        M5.Power.setLed(0);
        M5.Speaker.end();  // 音停止
        ledOn = false;
      } else {
        M5.Power.setLed(255);
        M5.Speaker.tone(4000);  // 音再生（無限）
        ledOn = true;
      }
    }
  }
}
