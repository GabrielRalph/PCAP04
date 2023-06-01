#ifndef pcap042_h
#define pcap042_h


#include <Wire.h>
#include "defines.h"
#include "types.h"
#include "registers.h"


class PCAP04 {
public:
  // 1KB NVRAM of the PCAP04
  __PCAP_CONFIG_REGS_T config_regs;
  pcap_results_regs_t pcap_results;
  unsigned char i2c_address = 0b00101000;

//  PCAP04(int a){
//
//  };

  unsigned char* getCFGAddrs(){
    return (unsigned char*) &config_regs;
  }

  void send_command(unsigned char opcode){
    Opcode code = {.command = {.op_code = opcode}};
    send_opcode(code);
  };


  unsigned char send_opcode(Opcode code){
    unsigned char data = 0;
    if (code.bytes[2]>>7) {
      Serial.println("write opcode");
      Wire.beginTransmission(i2c_address); // transmit to device #4
      Wire.write(code.bytes[2]);              // sends opcode + address
      Wire.write(code.bytes[1]);              // sends address
      Wire.write(code.bytes[0]);              // sends data
      Wire.endTransmission();    // stop transmitting
    } else {
      Serial.println("read opcode");
      Wire.beginTransmission(i2c_address); // transmit to device #4
      Wire.write(code.bytes[2]);              // sends opcode + address
      Wire.write(code.bytes[1]);              // sends address
      byte res = Wire.endTransmission();    // stop transmitting
      Serial.print("read opcode sent ");
      Serial.println(res);
      
      Wire.requestFrom(i2c_address, 1);
      if (Wire.available()) { // peripheral may send less than requested
        data = Wire.read(); // receive a byte as character
      }
    }

    return data;
  };

  /* ~~~~~~~~~~~~~~~~~~~~~ CONFIGURATION ~~~~~~~~~~~~~~~~~~~~~ */
  /* writeall_config
     coppies configuration register values from the pcap_nvram to the pcap. */
  void writeall_config(){
    static unsigned char data = 0;

    for (size_t addr = 0; addr < PCAP_NVRAM_CFG_SIZE; addr++){
      if (addr != 47){
        data = *(getCFGAddrs() + addr);
        write_config(addr, data);
      }
    }
    write_config(47, config_regs.CFG47.REGVAL);
  };

  /* write_config
     write's configuration register in pcap, given register address and
     register value */
  void write_config(unsigned char addr, unsigned char data){
    if ((addr >= 0) && (addr < PCAP_NVRAM_CFG_SIZE - 2)) {
      Opcode config = {.config = {.data = data, .addr = addr, .op_code = WR_CONFIG}};
      send_opcode(config);
    } else if ((addr == PCAP_NVRAM_CFG_SIZE - 2) || (addr == PCAP_NVRAM_CFG_SIZE - 1)) {
      Serial.print("write config to address: ");
      Serial.print(addr);
      Serial.println(" - skipped (CHARGE_PUMP)");
    } else {
      Serial.println("write config address not in range [0-63] :");
      Serial.print(addr);
    }
  };

  /* read_config
     coppies configuration register, of a given address from the pcap to the
     pcap_nvram */
  unsigned char read_config(unsigned char addr){
    unsigned char data = 0;
    if ((addr >= 0) && (addr < PCAP_NVRAM_CFG_SIZE)){
      Opcode config = {.config = {.data = 0, .addr = addr, .op_code = RD_CONFIG}};
      data = send_opcode(config);
      *(getCFGAddrs() + addr) = data;
    } else {
      Serial.print("read config address not in range [0-63] :");
      Serial.println(addr);
    }
    return data;
  };

  void readall_config(){
    for (size_t addr = 0; addr < PCAP_NVRAM_CFG_SIZE; addr++){
      read_config(addr);
    }
  };


  /* ~~~~~~~~~~~~~~~~~~~~~ NVRAM ~~~~~~~~~~~~~~~~~~~~~ */
  /* write_nvram
    coppies values from pcap_nvram into the pcap.*/
  void write_nvram(unsigned int addr, unsigned int data){
    if ((addr >= 0) && (addr < PCAP_NVRAM_SIZE)) {
      Opcode wr_mem = {.nvram = {.data = data, .addr = addr, .op_code = WR_NVRAM}};
//      if (addr >= 0 && addr < PCAP_NVRAM_MAX_INDEX_FW) {
//        wr_mem.nvram.data = pcap_nvram.FW.data[addr];
//      } else if (addr >= PCAP_NVRAM_MAX_INDEX_FW && addr < PCAP_NVRAM_MAX_INDEX_FW_CAL0) {
//        wr_mem.nvram.data = pcap_nvram.FW_CAL0.data[addr - PCAP_NVRAM_MAX_INDEX_FW];
//      } else if (addr >= PCAP_NVRAM_MAX_INDEX_FW_CAL0 && addr < PCAP_NVRAM_MAX_INDEX_FW_CAL1) {
//        wr_mem.nvram.data = pcap_nvram.FW_CAL1.data[addr - PCAP_NVRAM_MAX_INDEX_FW_CAL0];
//      } else if (addr >= PCAP_NVRAM_MAX_INDEX_FW_CAL1 && addr < PCAP_NVRAM_SIZE) {
//        wr_mem.nvram.data = *(getCFGAddrs() + addr - PCAP_NVRAM_MAX_INDEX_FW_CAL1);
//      }
      send_opcode(wr_mem);
    } else {
      Serial.print("write nvram address not in range [0-1023] :");
      Serial.println(addr);
    }
  };

  /* writeall_nvram,
     coppies entire pcap_nvram into the pcap */
//  void writeall_nvram(){
//    for (size_t addr = 0; addr < PCAP_NVRAM_SIZE; addr++){
//      if (addr != PCAP_NVRAM_RUNBIT_INDEX){
//        write_nvram(addr);
//      }
//    }
//    write_nvram(PCAP_NVRAM_RUNBIT_INDEX);
//  };


//  void readall_nvram(){
//    for (size_t addr = 0; addr < PCAP_NVRAM_SIZE; addr++){
//      read_nvram(addr);
//    }
//  };

  unsigned char read_nvram(unsigned int addr){
    unsigned char data = 0;
    // if valid address
    if ((addr >= 0) && (addr < PCAP_NVRAM_SIZE)){
      Opcode rd_mem = {.nvram = {.data = 0, .addr = addr, .op_code = RD_NVRAM}};
      data = send_opcode(rd_mem);

      // store data in pcap_nvram
      // firmware ram
//      if (addr >= 0 && addr < PCAP_NVRAM_MAX_INDEX_FW){
//        pcap_nvram.FW.data[addr] = data;
//      // calibration 0 ram
//      }else if (addr >= PCAP_NVRAM_MAX_INDEX_FW && addr < PCAP_NVRAM_MAX_INDEX_FW_CAL0){
//        pcap_nvram.FW_CAL0.data[addr - PCAP_NVRAM_MAX_INDEX_FW] = data;
//      // calibration 1 ram
//      } else if (addr >= PCAP_NVRAM_MAX_INDEX_FW_CAL0 && addr < PCAP_NVRAM_MAX_INDEX_FW_CAL1){
//        pcap_nvram.FW_CAL1.data[addr - PCAP_NVRAM_MAX_INDEX_FW_CAL0] = data;
//      // configuration ram
//      } else if (addr >= PCAP_NVRAM_MAX_INDEX_FW_CAL1 && addr < PCAP_NVRAM_SIZE){
//        *(getCFGAddrs() + addr - PCAP_NVRAM_MAX_INDEX_FW_CAL1) = data;
//      }
    } else {
      Serial.print("read nvram address not in range [0-1023] :");
      Serial.println(addr);
    }

    return data;
  };

  /* ~~~~~~~~~~~~~~~~~~~~~ RESULT REGISTERS ~~~~~~~~~~~~~~~~~~~~~ */

  void read_result(unsigned char addr){
    if ((addr >= 0) && (addr < PCAP_RESULTS_SIZE + PCAP_STATUS_SIZE)){
      Opcode rd_result = {.result = {.data = 0, .addr = addr, .op_code = RD_RESULT}};
      unsigned char data = send_opcode(rd_result);
      pcap_results.bytes[addr] = data;
    } else {
      Serial.print("read result address not in range [0-34] :");
      Serial.println(addr);
    }
  };

  void readall_result(){
    for (size_t addr = 0; addr < PCAP_RESULTS_SIZE + PCAP_STATUS_SIZE; addr++){
      read_result(addr);
    }
  };
};

#endif
