#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Definitions for the PCAL9555 registers
#define REG_9555_INPUT_0        0x00
#define REG_9555_INPUT_1        0x01
#define REG_9555_OUTPUT_0       0x02
#define REG_9555_OUTPUT_1       0x03
#define REG_9555_POLARITY_0     0x04
#define REG_9555_POLARITY_1     0x05
#define REG_9555_CONFIG_0       0x06
#define REG_9555_CONFIG_1       0x07
#define REG_9555_IMASK_0        0x4a
#define REG_9555_IMASK_1        0x4b
#define REG_9555_ISTATUS_0      0x4c
#define REG_9555_ISTATUS_1      0x4d

// Definitions for the SX1508B registers
#define REG_1508_INPUT_DISABLE		0x00	//	RegInputDisable Input buffer disable register - I/O[7_0] 0000 0000
#define REG_1508_LONG_SLEW			0x01	//	RegLongSlewA Output buffer long slew register - I/O[7_0] 0000 0000
#define REG_1508_LOW_DRIVE			0x02	//	RegLowDriveA Output buffer low drive register - I/O[7_0] 0000 0000
#define REG_1508_PULL_UP			0x03	//	RegPullUp Pull_up register - I/O[7_0] 0000 0000
#define REG_1508_PULL_DOWN			0x04	//	RegPullDown Pull_down register - I/O[7_0] 0000 0000
#define REG_1508_OPEN_DRAIN			0x05	//	RegOpenDrain Open drain register - I/O[7_0] 0000 0000
#define REG_1508_POLARITY			0x06	//	RegPolarity Polarity register - I/O[7_0] 0000 0000
#define REG_1508_DIR				0x07	//	RegDir Direction register - I/O[7_0] 1111 1111
#define REG_1508_DATA				0x08	//	RegData Data register - I/O[7_0] 1111 1111*
#define REG_1508_INTERRUPT_MASK		0x09	//	RegInterruptMask Interrupt mask register - I/O[7_0] 1111 1111
#define REG_1508_SENSE_HIGH			0x0A	//	RegSenseHigh Sense register for I/O[7:4] 0000 0000
#define REG_1508_SENSE_LOW			0x0B	//	RegSenseLow Sense register for I/O[3:0] 0000 0000
#define REG_1508_INTERRUPT_SOURCE	0x0C	//	RegInterruptSource Interrupt source register - I/O[7_0] 0000 0000
#define REG_1508_EVENT_STATUS		0x0D	//	RegEventStatus Event status register - I/O[7_0] 0000 0000
#define REG_1508_LEVEL_SHIFTER		0x0E	//	RegLevelShifter1 Level shifter register 0000 0000
#define REG_1508_CLOCK				0x0F	//	RegClock Clock management register 0000 0000
#define REG_1508_MISC				0x10	//	RegMisc Miscellaneous device settings register 0000 0000
#define REG_1508_LED_DRIVER_ENABLE	0x11	//	RegLEDDriverEnable LED driver enable register - I/O[7_0] 0000 0000
// Debounce and Keypad Engine		
#define REG_1508_DEBOUNCE_CONFIG	0x12	//	RegDebounceConfig Debounce configuration register 0000 0000
#define REG_1508_DEBOUNCE_ENABLE	0x13	//	RegDebounceEnable Debounce enable register - I/O[7_0] 0000 0000
#define REG_1508_KEY_CONFIG			0x14	//	RegKeyConfig1 Key scan configuration register 0000 0000
#define REG_1508_KEY_DATA			0x15	//	RegKeyData1 Key value (column) 1111 1111
// LED Driver (PWM, blinking, breathing)		
#define REG_1508_I_ON_0				0x16	//	RegTOn0 ON time register for I/O[0] 0000 0000
#define REG_1508_I_ON_1				0x17	//	RegIOn1 ON intensity register for I/O[1] 1111 1111
#define REG_1508_T_ON_2				0x18	//	RegTOn2 ON time register for I/O[2] 0000 0000
#define REG_1508_I_ON_2				0x19	//	RegIOn2 ON intensity register for I/O[2] 1111 1111
#define REG_1508_OFF_2				0x1A	//	RegOff2 OFF time/intensity register for I/O[2] 0000 0000
#define REG_1508_T_ON_3				0x1B	//	RegTOn3 ON time register for I/O[3] 0000 0000
#define REG_1508_I_ON_3				0x1C	//	RegIOn3 ON intensity register for I/O[3] 1111 1111
#define REG_1508_OFF_3				0x1D	//	RegOff3 OFF time/intensity register for I/O[3] 0000 0000
#define REG_1508_T_RISE_3			0x1E	//	RegTRise3 Fade in register for I/O[3] 0000 0000
#define REG_1508_T_FALL_3			0x1F	//	RegTFall3 Fade out register for I/O[3] 0000 0000
#define REG_1508_I_ON_4				0x20	//	RegIOn4 ON intensity register for I/O[4] 1111 1111
#define REG_1508_I_ON_5				0x21	//	RegIOn5 ON intensity register for I/O[5] 1111 1111
#define REG_1508_T_ON_6				0x22	//	RegTOn6 ON time register for I/O[6] 0000 0000
#define REG_1508_I_ON_6				0x23	//	RegIOn6 ON intensity register for I/O[6] 1111 1111
#define REG_1508_OFF_6				0x24	//	RegOff6 OFF time/intensity register for I/O[6] 0000 0000
#define REG_1508_T_ON_7				0x25	//	RegTOn7 ON time register for I/O[7] 0000 0000
#define REG_1508_I_ON_7				0x26	//	RegIOn7 ON intensity register for I/O[7] 1111 1111
#define REG_1508_OFF_7				0x27	//	RegOff7 OFF time/intensity register for I/O[7] 0000 0000
#define REG_1508_T_RISE_7			0x28	//	RegTRise7 Fade in register for I/O[7] 0000 0000
#define REG_1508_T_FALL_7			0x29	//	RegTFall7 Fade out register for I/O[7] 0000 0000
// 	Miscellaneous		
#define REG_1508_HIGH_INPUT		    0x2A	//	RegHighInputA High input enable register _ I/O[7_0] (Bank A) 0000 0000
//  Software Reset		
#define REG_1508_RESET				0x7D	//	RegReset Software reset register 0000 0000

#ifdef __cplusplus
}
#endif