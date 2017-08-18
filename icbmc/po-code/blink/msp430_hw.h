/* A model of the MSP430 memory map and CPU functions 
 *
 * THE MSP430 MEMORY MAP
 *
The MSP430 is a very clean 16-bit byte-addressed processor with a 64K unified
address space, and memory-mapped peripherals. The current family ranges from a
20-pin package with 1K of ROM and 128 bytes of RAM to 100-pin packages with 60K
of ROM and 2K of RAM. All current MSP430s share a common memory map. The amount
of each type of memory varies with the device, but the overall layout is common.

At the low end of memory is a 512 byte space for the memory-mapped peripherals.
The first 256 bytes of this are on an 8-bit bus, and can only be accessed 8 bits
at a time. The second 256 bytes are on a 16-bit bus, and can only be accessed 16
bits at a time.

RAM begins at address 0x200. If there is 2K of RAM, it extends from address
0x0200 to 0x9FF. The main ROM is always at the highest addresses. In the 60K
version it extends from address 0x1100 to 0xFFFF (see below for what happens
between 0x1000 and 0x10FF).

Processors with flash memory have a 1K serial bootloader ROM at addresses 0x0C00
to 0x0FFF. This is unalterable, masked, ROM. It contains a factory set program
to erase and reprogram the on board flash memory.  Processors with flash memory
also have an additional 128 or 256 bytes of flash memory between addresses
0x1000 and 0x107F or 0x10FF. The only real difference between this and the main
flash memory is that this is erasable in 128 byte pages. This makes it more
suitable for efficiently handling configuration data.
 *
 *
 * REGISTERS 
 */
unsigned short _R0;    /* R0/PC Program Counter     */
unsigned short _R1;    /* R1/SP Stack Pointer       */
unsigned short _R2;    /* R2/SR/CG1 Status          */
unsigned short _R3;    /* R3/CG2 Constant Generator */
unsigned short _R4;    /* General Purpose           */
unsigned short _R5;    /* General Purpose           */
unsigned short _R6;    /* General Purpose           */
unsigned short _R7;    /* General Purpose           */
unsigned short _R8;    /* General Purpose           */
unsigned short _R9;    /* General Purpose           */
unsigned short _R10;   /* General Purpose           */
unsigned short _R11;   /* General Purpose           */
unsigned short _R12;   /* General Purpose           */
unsigned short _R13;   /* General Purpose           */
unsigned short _R14;   /* General Purpose           */
unsigned short _R15;   /* General Purpose           */

// GIE: General Interrupt Enable
// 0: interrupts disabled; 1: interrupts enabled
// bit 0 in "_R2"

#define     set_GIE (_R2 |= 0x0008)  

#define   unset_GIE (_R2 &= 0xfff7) 

#define enabled_GIE ((_R2 & 0x0008) != 0x0000)

/* PERIPHERALS
 *
-  8-bit SFR             0000h -> 000Fh      
-  8-bit peripherals     0010h -> 00FFh    
- 16-bit peripherals     0100h -> 01FFh  

The MSP430's memory mapped peripheral registers are termed Special Function
Registers (SFRs). Consider a peripheral a normal variable, which is simply
mapped to the specific memory location.
 *
 * 8-bit SFR:
 */
unsigned char _IE1;    /* 0x0000 Interrupt Enable 1 */
unsigned char _IE2;    /* 0x0001 Interrupt Enable 2 */
unsigned char _IFG1;   /* 0x0002 Interrupt Flag 1   */
unsigned char _IFG2;   /* 0x0003 Interrupt Flag 2   */
unsigned char _ME1;    /* 0x0004 Module Enable 1    */
unsigned char _ME2;    /* 0x0005 Module Enable 2    */

/* 8-bit peripherals:
 *
 * Port P1 
 */
unsigned char _P1IN;     /* 0x0020 Input                 */
unsigned char _P1OUT;    /* 0x0021 Output                */
unsigned char _P1DIR;    /* 0x0022 Direction             */
unsigned char _P1IFG;    /* 0x0023 Interrupt Flag        */
unsigned char _P1IES;    /* 0x0024 Interrupt-Edge Select */
unsigned char _P1IE;     /* 0x0025 Interrupt Enable      */
unsigned char _P1SEL;    /* 0x0026 Selection             */

/*
 * Port P2
 */
unsigned char _P2IN;     /* 0x0028 Input                 */
unsigned char _P2OUT;    /* 0x0029 Output                */
unsigned char _P2DIR;    /* 0x002A Direction             */
unsigned char _P2IFG;    /* 0x002B Interrupt Flag        */
unsigned char _P2IES;    /* 0x002C Interrupt-Edge Select */
unsigned char _P2IE;     /* 0x002D Interrupt Enable      */
unsigned char _P2SEL;    /* 0x002E Selection             */

/*
 * Port P3
 */
unsigned char _P3IN;     /* 0x0018 Input                 */
unsigned char _P3OUT;    /* 0x0019 Output                */
unsigned char _P3DIR;    /* 0x001A Direction             */
unsigned char _P3SEL;    /* 0x001B Selection             */

/*
 * Port P4
 */
unsigned char _P4IN;     /* 0x001C Input                 */
unsigned char _P4OUT;    /* 0x001D Output                */
unsigned char _P4DIR;    /* 0x001E Direction             */
unsigned char _P4SEL;    /* 0x001F Selection             */

/*
 * Port P5
 */
unsigned char _P5IN;     /* 0x0030 Input                 */
unsigned char _P5OUT;    /* 0x0031 Output                */
unsigned char _P5DIR;    /* 0x0032 Direction             */
unsigned char _P5SEL;    /* 0x0033 Selection             */

/*
 * Port P6
 */
unsigned char _P6IN;     /* 0x0034 Input                 */
unsigned char _P6OUT;    /* 0x0035 Output                */
unsigned char _P6DIR;    /* 0x0036 Direction             */
unsigned char _P6SEL;    /* 0x0037 Selection             */

/*
 * BrownOUT, SVS
 */
unsigned char _SVSCTL;   /* 0x0055 SVS */ 

/*
 * Basic Clock
 */
unsigned char _BCSCTL2;   /* 0x0058 Basic clock system control2 */
unsigned char _BCSCTL1;   /* 0x0057 Basic clock system control1 */
unsigned char _DCOCTL;    /* 0x0056 DCO clock frequency control */

/*
 * Comparator_A
 */
unsigned char _CAPD;      /* 0x005B Comparator_A port disable   */
unsigned char _CACTL2;    /* 0x005A Comparator_A control2       */
unsigned char _CACTL1;    /* 0x0059 Comparator_A control1       */

/*
 * USART0 (UART or SPImode - the default)
 */
unsigned char _U0TXBUF;   /* 0x0077 Transmit buffer    */
unsigned char _U0RXBUF;   /* 0x0076 Receive buffer     */
unsigned char _U0BR1;     /* 0x0075 Baud rate          */
unsigned char _U0BR0;     /* 0x0074 Baud rate          */
unsigned char _U0MCTL;    /* 0x0073 Modulation control */
unsigned char _U0RCTL;    /* 0x0072 Receive control    */
unsigned char _U0TCTL;    /* 0x0071 Transmit control   */
unsigned char _U0CTL;     /* 0x0070 USART control      */

/*
 * USART1 (MSP430F16x and MSP430F161x only)
 */
unsigned short _U1TXBUF;  /* 0x007F Transmit buffer */
unsigned short _U1RXBUF;  /* 0x007E Receive buffer  */
unsigned short _U1BR1;    /* 0x007D Baud rate       */
unsigned short _U1BR0;    /* 0x007C Baud rate       */
unsigned short _U1MCTL;   /* 0x007B Modulation control      */
unsigned short _U1RCTL;   /* 0x007A Receive control */
unsigned short _U1TCTL;   /* 0x0079 Transmit control        */
unsigned short _U1CTL;    /* 0x0078 USART control   */

/*
 * Watchdog 
 */
unsigned short _WDTCTL;   /* 0x0120 Watchdog Timer control */

/*
 * Flash 
 */
unsigned short _FCTL3;  /* 0x012C Flash control 3        */
unsigned short _FCTL2;  /* 0x012A Flash control 2        */
unsigned short _FCTL1;  /* 0x0128 Flash control 1        */

/*
 * Hardware Multiplier (MSP430F16x and MSP430F161x only) 
 */

unsigned short _SUMEXT; /* 0x013E Sum extend                     */
unsigned short _RESHI;  /* 0x013C Result high word               */
unsigned short _RESLO;  /* 0x013A Result low word                */
unsigned short _OP2;    /* 0x0138 Second operand                 */
unsigned short _MACS;   /* 0x0136 Multiply signed +accumulate/operand1   */
unsigned short _MAC;    /* 0x0134 Multiply+accumulate/operand1   */
unsigned short _MPYS;   /* 0x0132 Multiply signed/operand1       */
unsigned short _MPY;    /* 0x0130 Multiply unsigned/operand1     */

/*
 * Timer_A3
 */
unsigned short _TACCTL2;  /* 0x0166 Capture/compare control 2      */
unsigned short _TACCTL1;  /* 0x0164 Capture/compare control 1      */
unsigned short _TACCTL0;  /* 0x0162 Capture/compare control 0      */
unsigned short _TACTL;    /* 0x0160 Timer_A control                */
unsigned short _TAIV;     /* 0x012E Timer_A interrupt vector       */
unsigned short _TACCR2;   /* 0x0176 Capture/compare register 2     */
unsigned short _TACCR1;   /* 0x0174 Capture/compare register 1     */
unsigned short _TACCR0;   /* 0x0172 Capture/compare register 0     */
unsigned short _TAR;      /* 0x0170 Timer_A count register         */

void set_TACTL_bit2()
{
  _TAR    = 0x0000;
  _TACTL &= 0xFF0B; /* reset [7-6], [5-4], [2] */
}

/* 
 * Timer_B3 
 */
unsigned short _TBCCR6;   /* 0x019E Capture/compare register 6     */
unsigned short _TBCCR5;   /* 0x019C Capture/compare register 5     */
unsigned short _TBCCR4;   /* 0x019A Capture/compare register 4     */
unsigned short _TBCCR3;   /* 0x0198 Capture/compare register 3     */
unsigned short _TBCCR2;   /* 0x0196 Capture/compare register 2     */
unsigned short _TBCCR1;   /* 0x0194 Capture/compare register 1     */
unsigned short _TBCCR0;   /* 0x0192 Capture/compare register 0     */
unsigned short _TBR;      /* 0x0190 Timer_B count register         */
unsigned short _TBCCTL6;  /* 0x018E Capture/compare control 6      */
unsigned short _TBCCTL5;  /* 0x018C Capture/compare control 5      */
unsigned short _TBCCTL4;  /* 0x018A Capture/compare control 4      */
unsigned short _TBCCTL3;  /* 0x0188 Capture/compare control 3      */
unsigned short _TBCCTL2;  /* 0x0186 Capture/compare control 2      */
unsigned short _TBCCTL1;  /* 0x0184 Capture/compare control 1      */
unsigned short _TBCCTL0;  /* 0x0182 Capture/compare control 0      */
unsigned short _TBCTL;    /* 0x0180 Timer_B control                */
unsigned short _TBIV;     /* 0x011E Timer_B interrupt vector       */

void set_TBCTL_bit2()
{
  _TBR    = 0x0000;
  _TBCTL &= 0xFF0B; /* reset [7-6], [5-4], [2] */
}

/*
Reserved 016Eh 
Reserved 016Ch 
Reserved 016Ah 
Reserved 0168h 

Reserved 017Eh 
Reserved 017Ch 
Reserved 017Ah 
Reserved 0178h 
 */

/*
 * ADC12
 */
unsigned short _ADC12IV;        /* 0x01A8 Interrupt-vector-word register */
unsigned short _ADC12IE;        /* 0x01A6 Inerrupt-enable register       */
unsigned short _ADC12IFG;       /* 0x01A4 Inerrupt-flag register */
unsigned short _ADC12CTL1;      /* 0x01A2 Control register 1     */
unsigned short _ADC12CTL0;      /* 0x01A0 Control register 0     */

unsigned short _ADC12MEM[16];   /* 0x0140 Conversion memory 0-15    */
unsigned char  _ADC12MCTL[16];  /* 0x0080 ADC memory-control register0    */

/* 
 * DAC12
 */
unsigned short _DAC12_1DAT;   /* 0x01CA DAC12_1 data     */
unsigned short _DAC12_1CTL;   /* 0x01C2 DAC12_1 control  */
unsigned short _DAC12_0DAT;   /* 0x01C8 DAC12_0 data     */
unsigned short _DAC12_0CTL;   /* 0x01C0 DAC12_0 control  */

/* 
 * DMA 
 */
unsigned short _DMA2SZ;   /* 0x01F6 DMA channel 2 transfer size        */
unsigned short _DMA2DA;   /* 0x01F4 DMA channel 2 destination address  */
unsigned short _DMA2SA;   /* 0x01F2 DMA channel 2 source address       */
unsigned short _DMA2CTL;  /* 0x01F0 DMA channel 2 control              */
unsigned short _DMA1SZ;   /* 0x01EE DMA channel 1 transfer size        */
unsigned short _DMA1DA;   /* 0x01EC DMA channel 1 destination address  */
unsigned short _DMA1SA;   /* 0x01EA DMA channel 1 source address       */
unsigned short _DMA1CTL;  /* 0x01E8 DMA channel 1 control              */
unsigned short _DMA0SZ;   /* 0x01E6 DMA channel 0 transfer size        */
unsigned short _DMA0DA;   /* 0x01E4 DMA channel 0 destination address  */
unsigned short _DMA0SA;   /* 0x01E2 DMA channel 0 source address       */
unsigned short _DMA0CTL;  /* 0x01E0 DMA channel 0 control              */
unsigned short _DMACTL1;  /* 0x0124 DMA module control 1               */
unsigned short _DMACTL0;  /* 0x0122 DMA module control 0               */

/* 
 * MSP430 devices have up to 6 digital I/O ports implemented, P1 - P6. Each 
 * port has eight I/O pins. Every I/O pin is individually configurable for 
 * input or output direction, and each I/O line can be individually read or 
 * written to. Ports P1 and P2 have interrupt capability. All P1 I/O lines 
 * source a single interrupt vector, and all P2 I/O lines source a different, 
 * single interrupt vector. 
 *
 * Each bit in each PxIN (READ-ONLY) register reflects the value of the input signal 
 * at the corresponding I/O pin. Each bit in each PxOUT register is the value to 
 * be output on the corresponding I/O pin (0: low, 1: high). Each bit in each 
 * PxDIR register selects the direction of the corresponding I/O pin (0: input
 * direction, 1: output). Each PxSEL bit is used to select the pin function - 
 * I/O port (0) or peripheral module function (1).
 */

/* TIMER_A, TIMER_B
 * - Interrupt vector register for fast decoding of all Timer_A interrupts
 * - The 16-bit timer/counter register, TAR, increments or decrements (depending
      on mode of operation) with each rising edge of the clock signal. The timer 
      can generate an interrupt when it overflows. TAR may be cleared by setting 
      the TACLR bit. 
 */

/*
 * MSP430 initial conditions after system reset:
 *
 * - [HW] The RST/NMI pin is configured in the reset mode. 
 * - [SW] I/O pins are switched to input mode as described in the Digital I/O chapter. 
 * - [SW] Other peripheral modules and registers are initialized as described in their 
 *   respective chapters in this manual. 
 * - [HW] Status register (SR) is reset. 
 * - [HW] The watchdog timer powers up active in watchdog mode. 
 * - [HW] Program counter (PC) is loaded with address contained at reset vector 
 *   location (0FFFEh). CPU execution begins at that address. 
 *
 * After a system reset, user software must initialize the MSP430 for the 
 * application requirements. The following must occur: 
 * - [HW] Initialize the SP, typically to the top of RAM. 
 * - [HW] Initialize the watchdog to the requirements of the application. 
 * - [SW] Configure peripheral modules to the requirements of the application. 
 *
 */
void set_initial_configuration()
{
  /* I/O ports are initialized in MotePlatformC_Init_init() -- nothing to do */

  /* SR doesn't show up in software; GIE does, as __asm("dint"/"eint") */
  /* reset GIE */

  /* all other initializations seem irrelevant to the software */
}