#ifndef pcap_types_h
#define pcap_types_h


typedef union Opcode {
  unsigned char bytes[3];

  struct __attribute__((packed)) {
      unsigned int data : 8;
      unsigned int addr : 10;
      unsigned int op_code : 6;
  } nvram;

  struct __attribute__((packed)){
      unsigned int data : 8;
      unsigned int addr : 6;
      unsigned int op_code : 10;
  } config;

  struct __attribute__((packed)){
      unsigned short : 8;
      unsigned short data : 8;
      unsigned short addr : 6;
      unsigned short op_code : 2;

  } result;

  struct __attribute__((packed)){
      unsigned int : 16;
      unsigned char op_code : 8;
  } command;

  struct __attribute__((packed)){
      unsigned short : 8;
      unsigned short fixed : 8;
      unsigned short op_code : 8;
  } testread;
};


typedef union pcap_results_regs_t {
  unsigned short bytes[34];
  struct {
    float RES0;
    float RES1;
    float RES2;
    float RES3;
    float RES4;
    float RES5;
    float RES6;
    float RES7;

    struct {
      bool RUNBIT : 1;
      bool CDC_ACTIVE : 1;
      bool RDC_READY : 1;
      bool : 1;
      bool AUTOBOOT_BUSY : 1;
      bool POR_CDC_DSP_COLL : 1;
      bool POR_FLAG_CONFIG : 1;
      bool POR_FLAG_WDOG : 1;
    } STATUS_0;

    struct {
      unsigned char : 4;
      bool RDC_ERR : 1;
      bool MUP_ERR : 1;
      bool ERR_OVERFL : 1;
      bool COMB_ERR : 1;
    } STATUS_1;

    struct {
      bool C_PORT_ERR_INT : 1;
      bool C_PORT_ERR0 : 1;
      bool C_PORT_ERR1 : 1;
      bool C_PORT_ERR2 : 1;
      bool C_PORT_ERR3 : 1;
      bool C_PORT_ERR4 : 1;
      bool C_PORT_ERR5 : 1;
      bool : 1;
    } STATUS_2;
  } registers;
};

#endif
