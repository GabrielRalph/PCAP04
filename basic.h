#ifndef basic
#define basic

#include "Wire.h"
#include "defines.h"
#include "arduino.h"

#define MLOG(a, nl) (nl*2)//(nl?Serial.println(a):Serial.print(a))

byte i2c_address = 0x28;

void printBIN(unsigned int a, byte num){
  for (byte i = 0; i < num; i++) {
    Serial.print((a>>(num - i - 1)) & 1);
  }
  Serial.print(" [0x");
  Serial.print(a, HEX);
  Serial.print("]");
}

void printBIN(unsigned int a) {printBIN(a, 8);}

byte testRead(){
  // Send the test read byte to the PCAP04
  Wire.beginTransmission(i2c_address);
  Wire.write(TEST_READ_HIGH);
  // Wire.write(TEST_READ_LOW);   ** If this line is uncommented test read does not work          
  byte res = Wire.endTransmission(false);

  MLOG("transmission sent result was: ", 0);
  MLOG(res, 1);


  // Request and read the response byte
  byte data = 0;
  Wire.requestFrom(i2c_address, 1);
  if (Wire.available()) {
    data = Wire.read();
    MLOG("data received",1);
  }

  return data;
}

void writeNVRAM(short regi, byte value) {
  byte b1 = 0b10100000 | ((regi >> 8) & 0b11);
  byte b2 = regi & 0xFF;

  Wire.beginTransmission(i2c_address);
  Wire.write(b1);
  Wire.write(b2);
  Wire.write(value);
  byte res = Wire.endTransmission();

  MLOG("transmission sent result was: ", 0);
  MLOG(res, 1);
}

byte readNVRAM(short regi){
  // as per
  byte b1 = 0b00100000 | ((regi >> 8) & 0b11);
  byte b2 = regi & 0xFF;

  printBIN(b1);
  printBIN(b2);
  Serial.println("");

  Wire.beginTransmission(i2c_address);
  Wire.write(b1);
  Wire.write(b2);
  byte res = Wire.endTransmission(false);
  MLOG("transmission sent result was: ", 0);
  MLOG(res, 1);

  byte data = 0xff;
  Wire.requestFrom(i2c_address, 1, true);
  while (Wire.available()) { // peripheral may send less than requested
    data = Wire.read(); // receive a byte as character
    MLOG("data received",1);
  }

  return data;
}

byte readConfig(byte regi) {
  return readNVRAM(((short) regi & 0b00111111) | 0b1111000000);
}

void writeConfig(byte regi, byte value) {
  writeNVRAM(((short) regi & 0b00111111) | 0b1111000000, value);
}
#endif
