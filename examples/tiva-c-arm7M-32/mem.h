/**
 * Copyright 2022 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#ifndef __MEM_LAYOUT_H__
#define __MEM_LAYOUT_H__

// System Control Register Map
typedef struct _SYSTEM_CONTROL {
	volatile unsigned long  DID0;
	volatile unsigned long  DID1;
	volatile unsigned long  DC0;
	volatile unsigned long  PADDING_0;
	volatile unsigned long  DC1;
	volatile unsigned long  DC2;
	volatile unsigned long  DC3;
	volatile unsigned long  DC4;
	volatile unsigned long  DC5;
	volatile unsigned long  DC6;
	volatile unsigned long  DC7;
	volatile unsigned long  DC8;
	volatile unsigned long  PBORCTL;
	volatile unsigned long  PADDING_1[3];
	volatile unsigned long  SRCR0;
	volatile unsigned long  SRCR1;
	volatile unsigned long  SRCR2;
	volatile unsigned long  PADDING_2;
	volatile unsigned long  RIS;
	volatile unsigned long  IMC;
	volatile unsigned long  MISC;
	volatile unsigned long  RESC;
	volatile unsigned long  RCC;
	volatile unsigned long  PADDING_3[2];
	volatile unsigned long  GPIOHBCTL;
	volatile unsigned long  RCC2;
	volatile unsigned long  PADDING_4[2];
	volatile unsigned long  MOSCCTL;
	volatile unsigned long  PADDING_5[32];
	volatile unsigned long  RCGC0;
	volatile unsigned long  RCGC1;
	volatile unsigned long  RCGC2;
	volatile unsigned long  PADDING_6;
	volatile unsigned long  SCGC0;
	volatile unsigned long  SCGC1;
	volatile unsigned long  SCGC2;
	volatile unsigned long  PADDING_7;
	volatile unsigned long  DCGC0;
	volatile unsigned long  DCGC1;
	volatile unsigned long  DCGC2;
	volatile unsigned long  PADDING_8[6];
	volatile unsigned long  DSLPCLKCFG;
	volatile unsigned long  PADDING_9;
	volatile unsigned long  SYSPROP;
	volatile unsigned long  PIOSCCAL;
	volatile unsigned long  PIOSCSTAT;
	volatile unsigned long  PADDING_10[2];
	volatile unsigned long  PLLFREQ0;
	volatile unsigned long  PLLFREQ1;
	volatile unsigned long  PLLSTAT;
	volatile unsigned long  PADDING_11[9];
	volatile unsigned long  DC9;
	volatile unsigned long  PADDING_12[3];
	volatile unsigned long  NVMSTAT;
	volatile unsigned long  PADDING_13[87];
	volatile unsigned long  PPWD;
	volatile unsigned long  PPTIMER;
	volatile unsigned long  PPGPIO;
	volatile unsigned long  PPDMA;
	volatile unsigned long  PADDING_14;
	volatile unsigned long  PPHIB;
	volatile unsigned long  PPUART;
	volatile unsigned long  PPSSI;
	volatile unsigned long  PPI2C;
	volatile unsigned long  PADDING_15;
	volatile unsigned long  PPUSB;
	volatile unsigned long  PADDING_16[2];
	volatile unsigned long  PPCAN;
	volatile unsigned long  PPADC;
	volatile unsigned long  PPACMP;
	volatile unsigned long  PPPWM;
	volatile unsigned long  PPQEI;
	volatile unsigned long  PADDING_17[4];
	volatile unsigned long  PPEEPROM;
	volatile unsigned long  PPWTIMER;
	volatile unsigned long  PADDING_18[104];
	volatile unsigned long  SRWD;
	volatile unsigned long  SRTIMER;
	volatile unsigned long  SRGPIO;
	volatile unsigned long  SRDMA;
	volatile unsigned long  PADDING_19;
	volatile unsigned long  SRHIB;
	volatile unsigned long  SRUART;
	volatile unsigned long  SRSSI;
	volatile unsigned long  SRI2C;
	volatile unsigned long  PADDING_20;
	volatile unsigned long  SRUSB;
	volatile unsigned long  PADDING_21[2];
	volatile unsigned long  SRCAN;
	volatile unsigned long  SRADC;
	volatile unsigned long  SRACMP;
	volatile unsigned long  PADDING_22[6];
	volatile unsigned long  SREEPROM;
	volatile unsigned long  SRWTIMER;
	volatile unsigned long  PADDING_23[40];
	volatile unsigned long  RCGCWD;
	volatile unsigned long  RCGCTIMER;
	volatile unsigned long  RCGCGPIO;
	volatile unsigned long  RCGCDMA;
	volatile unsigned long  PADDING_24;
	volatile unsigned long  RCGCHIB;
	volatile unsigned long  RCGCUART;
	volatile unsigned long  RCGCSSI;
	volatile unsigned long  RCGCI2C;
	volatile unsigned long  PADDING_25;
	volatile unsigned long  RCGCUSB;
	volatile unsigned long  PADDING_26[2];
	volatile unsigned long  RCGCCAN;
	volatile unsigned long  RCGCADC;
	volatile unsigned long  RCGCACMP;
	volatile unsigned long  PADDING_27[6];
	volatile unsigned long  RCGCEEPROM;
	volatile unsigned long  RCGCWTIMER;
	volatile unsigned long  PADDING_28[40];
	volatile unsigned long  SCGCWD;
	volatile unsigned long  SCGCTIMER;
	volatile unsigned long  SCGCGPIO;
	volatile unsigned long  SCGCDMA;
	volatile unsigned long  PADDING_29;
	volatile unsigned long  SCGCHIB;
	volatile unsigned long  SCGCUART;
	volatile unsigned long  SCGCSSI;
	volatile unsigned long  SCGCI2C;
	volatile unsigned long  PADDING_30;
	volatile unsigned long  SCGCUSB;
	volatile unsigned long  PADDING_31[2];
	volatile unsigned long  SCGCCAN;
	volatile unsigned long  SCGCADC;
	volatile unsigned long  SCGCACMP;
	volatile unsigned long  PADDING_32[6];
	volatile unsigned long  SCGCEEPROM;
	volatile unsigned long  SCGCWTIMER;
	volatile unsigned long  PADDING_33[40];
	volatile unsigned long  DCGCWD;
	volatile unsigned long  DCGCTIMER;
	volatile unsigned long  DCGCGPIO;
	volatile unsigned long  DCGCDMA;
	volatile unsigned long  PADDING_34;
	volatile unsigned long  DCGCHIB;
	volatile unsigned long  DCGCUART;
	volatile unsigned long  DCGCSSI;
	volatile unsigned long  DCGCI2C;
	volatile unsigned long  PADDING_35;
	volatile unsigned long  DCGCUSB;
	volatile unsigned long  PADDING_36[2];
	volatile unsigned long  DCGCCAN;
	volatile unsigned long  DCGCADC;
	volatile unsigned long  DCGCACMP;
	volatile unsigned long  PADDING_37[6];
	volatile unsigned long  DCGCEEPROM;
	volatile unsigned long  DCGCWTIMER;
	volatile unsigned long  PADDING_38[40];
	volatile unsigned long  PCWD;
	volatile unsigned long  PCTIMER;
	volatile unsigned long  PCGPIO;
	volatile unsigned long  PCDMA;
	volatile unsigned long  PADDING_39;
	volatile unsigned long  PCHIB;
	volatile unsigned long  PCUART;
	volatile unsigned long  PCSSI;
	volatile unsigned long  PCI2C;
	volatile unsigned long  PADDING_40;
	volatile unsigned long  PCUSB;
	volatile unsigned long  PADDING_41[2];
	volatile unsigned long  PCCAN;
	volatile unsigned long  PCADC;
	volatile unsigned long  PCACMP;
	volatile unsigned long  PADDING_42[6];
	volatile unsigned long  PCEEPROM;
	volatile unsigned long  PCWTIMER;
	volatile unsigned long  PADDING_43[40];
	volatile unsigned long  PRWD;
	volatile unsigned long  PRTIMER;
	volatile unsigned long  PRGPIO;
	volatile unsigned long  PRDMA;
	volatile unsigned long  PADDING_44;
	volatile unsigned long  PRHIB;
	volatile unsigned long  PRUART;
	volatile unsigned long  PRSSI;
	volatile unsigned long  PRI2C;
	volatile unsigned long  PADDING_45;
	volatile unsigned long  PRUSB;
	volatile unsigned long  PADDING_46[2];
	volatile unsigned long  PRCAN;
	volatile unsigned long  PRADC;
	volatile unsigned long  PRACMP;
	volatile unsigned long  PADDING_47[6];
	volatile unsigned long  PREEPROM;
	volatile unsigned long  PRWTIMER;
} SYSTEM_CONTROL;

typedef struct _GPIO {
	volatile unsigned long GPIORawData;
	volatile unsigned long DATA[255];
	volatile unsigned long GPIODIR;
	volatile unsigned long GPIOIS;
	volatile unsigned long GPIOIBE;
	volatile unsigned long GPIOIEV;
	volatile unsigned long GPIOIM;
	volatile unsigned long GPIORIS;
	volatile unsigned long GPIOMIS;
	volatile unsigned long GPIOICR;
	volatile unsigned long GPIOAFSEL;
	volatile unsigned long PADDING_2[55];
	volatile unsigned long GPIODR2R;
	volatile unsigned long GPIODR4R;
	volatile unsigned long GPIODR8R;
	volatile unsigned long GPIOODR;
	volatile unsigned long GPIOPUR;
	volatile unsigned long GPIOPDR;
	volatile unsigned long GPIOSLR;
	volatile unsigned long GPIODEN;
	volatile unsigned long GPIOLOCK;
	volatile unsigned long GPIOCR;
	volatile unsigned long GPIOAMSEL;
	volatile unsigned long GPIOPCTL;
	volatile unsigned long GPIOADCCTL;
	volatile unsigned long GPIODMACTL;
	volatile unsigned long PADDING_3[678];
	volatile unsigned long GPIOPeriphlID4;
	volatile unsigned long GPIOPeriphlID5;
	volatile unsigned long GPIOPeriphlID6;
	volatile unsigned long GPIOPeriphlID7;
	volatile unsigned long GPIOPeriphlID0;
	volatile unsigned long GPIOPeriphlID1;
	volatile unsigned long GPIOPeriphlID2;
	volatile unsigned long GPIOPeriphlID3;
	volatile unsigned long GPIOPCellID0;
	volatile unsigned long GPIOPCellID1;
	volatile unsigned long GPIOPCellID2;
	volatile unsigned long GPIOPCellID3;
} GPIO;

// Port Pins
enum GPIO_PINS {
	pin_0 = 0x01,
	pin_1 = 0x02,
	pin_2 = 0x04,
	pin_3 = 0x08,
	pin_4 = 0x10,
	pin_5 = 0x20,
	pin_6 = 0x40,
	pin_7 = 0x80
};

//  Clock Port
enum CLK_PORT {
	clk_port_A = 0x01,
	clk_port_B = 0x02,
	clk_port_C = 0x04,
	clk_port_D = 0x08,
	clk_port_E = 0x10,
	clk_port_F = 0x20
};

#define GPIO_PORT_F ((GPIO *) 0x40025000)
#define SYS_CTRL ((SYSTEM_CONTROL *)0x400FE000)

#endif
