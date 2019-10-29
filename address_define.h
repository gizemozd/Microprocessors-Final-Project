/* This files provides address values that exist in the system */
//address_map_arm.h

#define BOARD                 "DE1-SoC"

/* Memory */
#define DDR_BASE              0x00000000
#define DDR_END               0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_END         0xFFFFFFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define FPGA_PIXEL_BUF_BASE   0xC8000000
#define FPGA_PIXEL_BUF_END    0xC803FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_END         0xC9001FFF
#define PIXEL_BACK_BUF_BASE	  0xFF203024
#define PIXEL_STATUS          0xFF20302C

/* Cyclone V FPGA devices */
#define LED_BASE	      0xFF200000
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define JP1_BASE              0xFF200060
#define JP2_BASE              0xFF200070
#define PS2_BASE              0xFF200100
#define PS2_DUAL_BASE         0xFF200108
#define JTAG_UART_BASE        0xFF201000
#define JTAG_UART_2_BASE      0xFF201008
#define IrDA_BASE             0xFF201020
#define TIMER_BASE            0xFF202000
#define TIMER_2_BASE          0xFF202020
#define AV_CONFIG_BASE        0xFF203000
#define RGB_RESAMPLER_BASE    0xFF203010
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define AUDIO_BASE            0xFF203040
#define VIDEO_IN_BASE         0xFF203060
#define EDGE_DETECT_CTRL_BASE 0xFF203070
#define ADC_BASE              0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE        0xFF709000
#define I2C0_BASE             0xFFC04000
#define I2C1_BASE             0xFFC05000
#define I2C2_BASE             0xFFC06000
#define I2C3_BASE             0xFFC07000
#define HPS_TIMER0_BASE       0xFFC08000
#define HPS_TIMER1_BASE       0xFFC09000
#define HPS_TIMER2_BASE       0xFFD00000
#define HPS_TIMER3_BASE       0xFFD01000
#define FPGA_BRIDGE           0xFFD0501C

/* ARM A9 MPCORE devices */
#define   PERIPH_BASE         0xFFFEC000    // base address of peripheral devices
#define   MPCORE_PRIV_TIMER   0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF      0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR                0x00          // offset to CPU interface control reg
#define ICCPMR                0x04          // offset to interrupt priority mask reg
#define ICCIAR                0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR               0x10          // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST       0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR                0x00          // offset to distributor control reg
#define ICDISER               0x100         // offset to interrupt set-enable regs
#define ICDICER               0x180         // offset to interrupt clear-enable regs
#define ICDIPTR               0x800         // offset to interrupt processor targets regs
#define ICDICFR               0xC00         // offset to interrupt configuration regs


//defines.h
#define		EDGE_TRIGGERED			0x1
#define		LEVEL_SENSITIVE		0x0
#define		CPU0						0x01	// bit-mask; bit 0 represents cpu0
#define		ENABLE					0x1
#define		DISABLE					0x0

#define		KEY0 						0
#define		KEY1 						1
#define		KEY2						2
#define		KEY3						3
#define		NONE						4

#define		RIGHT						1
#define		LEFT						2

#define		USER_MODE				0b10000
#define		FIQ_MODE					0b10001
#define		IRQ_MODE					0b10010
#define		SVC_MODE					0b10011
#define		ABORT_MODE				0b10111
#define		UNDEF_MODE				0b11011
#define		SYS_MODE					0b11111

#define		INT_ENABLE				0b01000000
#define		INT_DISABLE				0b11000000

//interrupt_ID.h
/* This file provides interrupt IDs */

/* FPGA interrupts (there are 64 in total; only a few are defined below) */
#define	INTERVAL_TIMER_IRQ				72
#define	KEYS_IRQ		 						73
#define	FPGA_IRQ2	 						74
#define	FPGA_IRQ3	 						75
#define	FPGA_IRQ4	 						76
#define	FPGA_IRQ5	 						77
#define	AUDIO_IRQ							78
#define	PS2_IRQ		 						79
#define	JTAG_IRQ		 						80
#define	IrDA_IRQ		 						81
#define	FPGA_IRQ10							82
#define	JP1_IRQ								83
#define	JP2_IRQ								84
#define	FPGA_IRQ13							85
#define	FPGA_IRQ14							86
#define	FPGA_IRQ15							87
#define	FPGA_IRQ16							88
#define	PS2_DUAL_IRQ						89
#define	FPGA_IRQ18							90
#define	FPGA_IRQ19							91

/* ARM A9 MPCORE devices (there are many; only a few are defined below) */
#define	MPCORE_GLOBAL_TIMER_IRQ			27
#define	MPCORE_PRIV_TIMER_IRQ			29
#define	MPCORE_WATCHDOG_IRQ				30

/* HPS devices (there are many; only a few are defined below) */
#define	HPS_UART0_IRQ	   				194
#define	HPS_UART1_IRQ	   				195
#define	HPS_GPIO0_IRQ	        		  	196
#define	HPS_GPIO1_IRQ	        		  	197
#define	HPS_GPIO2_IRQ	        		  	198
#define	HPS_TIMER0_IRQ	        		 	199
#define	HPS_TIMER1_IRQ	        		 	200
#define	HPS_TIMER2_IRQ	        		 	201
#define	HPS_TIMER3_IRQ	        		 	202
#define	HPS_WATCHDOG0_IRQ	     			203
#define	HPS_WATCHDOG1_IRQ	     			204
