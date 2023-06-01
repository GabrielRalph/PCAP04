#include "basic.h"
#include "default_ram.h"

void setup(){
  Wire.begin();
  Serial.begin(115200);
}

void loop(){
  byte result = testRead();
  Serial.print("Test read results: ");
  printBIN(result);
  Serial.println("");

  for (int i = 0; i < 564; i++) {
    writeNVRAM(i, PCap04v1_standard_v1[i]);
  }

  for (int i = 0; i < 64; i++) {
    writeConfig(i, PCap04v0_standard_v1_configuration[i]);
  }

  result = readConfig(1);
  Serial.print("CG1 results: ");
  printBIN(result);
  Serial.println("");
  delay(100000);
}
