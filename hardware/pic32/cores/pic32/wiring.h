//************************************************************************
//*	wiring.h
//*	
//*	Arduino core files for PIC32
//*		Copyright (c) 2010,2011 by Mark Sproul
//*	
//*	
//************************************************************************
//*	this code is based on code Copyright (c) 2005-2006 David A. Mellis
//*	
//*	This library is free software; you can redistribute it and/or
//*	modify it under the terms of the GNU Lesser General Public
//*	License as published by the Free Software Foundation; either
//*	version 2.1 of the License, or (at your option) any later version.
//*	
//*	This library is distributed in the hope that it will be useful,
//*	but WITHOUT ANY WARRANTY; without even the implied warranty of
//*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.//*	See the GNU
//*	Lesser General Public License for more details.
//*	
//*	You should have received a copy of the GNU Lesser General
//*	Public License along with this library; if not, write to the
//*	Free Software Foundation, Inc., 59 Temple Place, Suite 330,
//*	Boston, MA	02111-1307	USA
//*	
//*	
//************************************************************************
//*	Edit History
//************************************************************************
//*	May  ?,	2011	Brian Schmalz worked on micros timers
//*	May 18,	2011	<MLS> Added prog_xxx defs because there is no "pgmspace.h" file for pic32
//*	May 23,	2011	<MLS> Added definitions for PROGMEM,  pgm_read_byte_near, pgm_read_byte_far
//************************************************************************

#ifndef Wiring_h
#define Wiring_h

#if defined(__AVR__)
	#include <avr/io.h>
#elif defined(__PIC32MX__)
    #include "p32_defs.h"
#endif
#include <inttypes.h>
#include "binary.h"

#include "cpudefs.h"	//*		This file is designed to provide some of the cpu specific definitions
						//*		that are available for avr chips and not for other chips (i.e. pic32)
                        //*     It now contains PIC32 speciffic defines as well.

#ifdef __cplusplus
extern "C"{
#endif

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define OPEN 0x02

#define true 0x1
#define false 0x0

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define INTERNAL 3
#define DEFAULT 1
#define EXTERNAL 0

#define EXTMINUS 4
#define EXTPLUSMINUS 5


// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define abs(x) ((x)>0?(x):-(x))
/* Don't use the Newlib stdlib.h abs() function */
#define _SKIP_STDLIB_ABS

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() INTEnableInterrupts()
#define noInterrupts() INTDisableInterrupts()

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

    // Options for the SoftReset() function's 'options' parameter
#define ENTER_BOOTLOADER_ON_BOOT    1
#define RUN_SKETCH_ON_BOOT          0

//************************************************************************
// Let compile time pre-processor calculate the CORE_TICK_PERIOD
//	clock rate is 80000000ull
#define TOGGLES_PER_SEC			1000
#define CORE_TICK_RATE			(F_CPU / 2 / TOGGLES_PER_SEC)

typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef uint8_t boolean;
typedef uint8_t byte;

void init(void);

void	pinMode(uint8_t pin, uint8_t mode);
uint8_t	getPinMode(uint8_t pin);
void	digitalWrite(uint8_t pin, uint8_t val);
int		digitalRead(uint8_t pin);
int		analogRead(uint8_t);
void	analogReference(uint8_t mode);
void	analogWrite(uint8_t, int);
unsigned int __attribute__((nomips16))  INTEnableInterrupts(void);
unsigned int __attribute__((nomips16)) INTDisableInterrupts(void);


unsigned long	millis(void);
unsigned long	micros(void);
void			delay(unsigned long);
void			delayMicroseconds(unsigned int us);
unsigned long	pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);

void			shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, byte val);

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);
unsigned int executeSoftReset(uint32_t options);

unsigned int attachCoreTimerService(uint32_t (*)(uint32_t count));
unsigned int detachCoreTimerService(uint32_t (*)(uint32_t count));
unsigned int callCoreTimerServiceNow(uint32_t (* service)(uint32_t));

void setup(void);
void loop(void);


#if !defined(__AVR__)
	#define _BV(bit) (1ul << (bit))

	#define PROGMEM
	#define pgm_read_byte(x)		(*((char *)x))
//	#define pgm_read_word(x)		(*((short *)(x & 0xfffffffe)))
	#define pgm_read_word(x)		( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x)))
	#define pgm_read_byte_near(x)	(*((char *)x))
	#define pgm_read_byte_far(x)	(*((char *)x))
//	#define pgm_read_word_near(x)	(*((short *)(x & 0xfffffffe))
//	#define pgm_read_word_far(x)	(*((short *)(x & 0xfffffffe)))
	#define pgm_read_word_near(x)	( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x)))
	#define pgm_read_word_far(x)	( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x))))

	#define	strcpy_P		strcpy
	#define	strcat_P		strcat

	#define	prog_void		const void
	#define	prog_char		const char
	#define	prog_uchar		const unsigned char
	#define	prog_int8_t		const int8_t
	#define	prog_uint8_t	const uint8_t
	#define	prog_int16_t	const int16_t
	#define	prog_uint16_t	const uint16_t
	#define	prog_int32_t	const int32_t
	#define	prog_uint32_t	const uint32_t
	#define	prog_int64_t	const int64_t
	#define	prog_uint64_t	const uint64_t
#endif
#if defined(__PIC32MX__)

//************************************************************************
// This is image header info used by the bootloader and eeprom functionality
// The values are filled in either by the sketch's linker script, or by the bootloader
// If an old bootloader is in use, the bootloader assigned fields will have 0xFFFFs in them.

#if !defined(MPIDEVER)              // usually provided in platforms.txt on the pic32.compiler.define line
    #define MPIDEVER 0x00000000     // if not, define it as the unspecified value of 0      
#endif

// Bootloader Capability bits
// The first 4 bits define what type of STK500 interface is used
// The next 4 bits define what LEDs are used
// The next 8 bits define how the bootloader is to be put in programming/listen mode
// The next 8 bits are how flash is erased instructed by the linker
// The next 4 bits are this and that capabilities instructed by the linker
// The last 4 bits are this and that capabilities
#define blCapUARTInterface                              0x00000001ul        // bootloader talks over a UART
#define blCapUSBInterface                               0x00000002ul        // bootloader talks over the USB for serial
#define blCapBootLED                                    0x00000010ul        // A boot LED is driven
#define blCapDownloadLED                                0x00000020ul        // A download LED is driven
#define blCapAutoResetListening                         0x00000100ul        // There is a short listening delay after reset for avrdude to upload a sketch before automatically loading the in flash sketch              
#define blCapProgramButton                              0x00000200ul        // A program button is supported  
#define blCapVirtualProgramButton                       0x00000400ul        // A virtual program button is supported
#define blCapLkInstrFullFlashEraseLess4KEEProm          0x00010000ul        // The original bootloader method of erasing all of program flash except the last 4K reserved for eeprom
#define blCapLkInstrJustInTimeFlashErase                0x00020000ul        // Only flash pages written too needed by the sketch is erased
#define blCapLkInstrFlashErase                          0x00040000ul        // The linker defines the flash range to erase
#define blCapLkInstrFullFlashErase                      0x00080000ul        // All of flash is erased
#define blCapLkInstrExecutionJumpAddress                0x01000000ul        // the bootloader will jump to the execution jump address immediately after programming
#define blCapLkInstrExecutionJumpToFirstInFlash         0x02000000ul        // the bootloader will jump to the first sketch loaded in flash ignoring the execution jump address immediately after programming
#define blCapSupportsRamHeaderAndPersistentData         0x10000000ul        // the bootloader clears the Ram Header and adds RAM header data like the RCON value
#define blCapSplitFlashBootloader                       0x20000000ul        // This is a split flash bootloader with some of the bootloader in program flash
#define blCapSmartStart                                 0x40000000ul        // To indicate that the bootloader has implemented a smart start sequence to prevent execution of the sketch until all start sequences have stabilized.
#define blCapNotProvided                                0x80000000ul        // If this is set, then all capability bits have no meaning. This will be set if flash is in its erased state and the bootloader did not set any bits.

// Linker assigned image bits, and requested bootloader instructions
// The first 8 bits define what type image this is
// The next 8 bits are undefined
// The next 8 bits are linker instructions to the bootloader as how flash is to be erased
// The next 4 bits are this and that instructions to the bootloader
// The last 4 bits are undefined
#define imageReserved                                   0x00000000ul
#define imageMPIDE                                      0x00000001ul        // This is a normal MPIDE sketch
#define imageBootFlashBootloader                        0x00000002ul        // This is a boot flash bootloader
#define imageProgramFlashBootloader                     0x00000004ul        // This is a program flash bootloader
#define imageSplitFlashBootloader                       0x00000008ul        // This has bootloader code in both boot and program flash
#define imageFullFlashEraseLess4KEEProm                 blCapLkInstrFullFlashEraseLess4KEEProm                 
#define imageJustInTimeFlashErase                       blCapLkInstrJustInTimeFlashErase                       
#define imageLinkerSpecifiedFlashErase                  blCapLkInstrFlashErase                                 
#define imageFullFlashErase                             blCapLkInstrFullFlashErase                             
#define imageExecutionJumpAddress                       blCapLkInstrExecutionJumpAddress                       
#define imageExecutionJumpToFirstInFlash                blCapLkInstrExecutionJumpToFirstInFlash                

typedef void (* FNIMGJMP) (void);

#pragma pack(push,2) 


// The RAM Header is filled in by the bootloader, however it is specified by the sketch in the linker script
// The sketch must place it in the first 1.5K of RAM, as this is the space perserved by the bootloader for the sketch's
// Debug data, RAM Header, and persistent data. The bootloader will only fill in the RAM Header and will not touch 
// the reset of the perserved data. The bootloader is highly protective to not exceed the number of bytes known by the
// sketch as the RAM Header so the bootloader will not stray into the sketch's presistent data. Likewise the bootloader
// will not stray into its own data space, so the bootloader may not write all bytes specified by the sketch if that
// exceeds the 1.5K of reserved space. On return, cbBlHeader is the number of bytes written by the bootloader, any
// data beyond this not touched by the bootloader.
typedef struct {
    uint32_t    cbBlRamHeader;     // the number of bytes of this header as written by the bootloader
    uint32_t    rcon;              // value of RCON before the bootloader clears it
} RAM_HEADER_INFO;

// The header is reserved by the sketch's linker script but 
// written by both the sketch's linker script and by the bootloader.
// The sketch's linker script will fill in all values except
// verBootloader, bootloaderCapabilities, vend, prod, and cbBlPreservedRam, these are
// all filled in by the bootloader. If an old bootloader is on the board
// all bootloader supplied values will be 0xFFFFFFFF; as this is the unprogramed value of flash. 
// Check the MSB of the capabilities and if it is set, then none of the bootloader supplied values are valid.
// In all cases, a value of all FFs is reserved as unknown and should be checked before using.
typedef struct {
    uint32_t  cbHeader;               // length of this structure
    uint32_t  verBootloader;          // version of the booloader that loaded the sketch, it will be 0xFFFFFFFF if the bootloader did not write the version.
    uint32_t  verMPIDE;               // the version number of MPIDE that build the sketch
    uint32_t  bootloaderCapabilities; // capabilities of the bootloader defined by the blCapXXX bits.
    uint16_t  vend;                   // vendor ID as assigned by the bootloader, 0xFF if undefined
    uint16_t  prod;                   // product ID as assigned by the bootloader, 0xFF if undefined, or 0xFE is unassigned
    uint32_t  imageType;              // see image bit field definition above
    FNIMGJMP  pJumpAddr;              // the execution address that the bootloader will jump to
    uint32_t  pProgramFlash;          // also known as base address, that is, the first byte of program flash used by the sketch
    uint32_t  cbProgramFlash;         // the number of bytes of flash used by the sketch as defined by the linker script
    uint32_t  pEEProm;                // pointer to the eeprom area, usually at the end of flash, but now can be defined by the linker script
    uint32_t  cbEEProm;               // the length of eeprom, usually 4K but can now be defined by the linker script
    uint32_t  pConfig;                // physical address pointer to the config bits
    uint32_t  cbConfig;               // length of config bits.
    RAM_HEADER_INFO * pRamHeader;     // pointer to the ram header
    uint32_t  cbRamHeader;            // length of the ram header as specified by the linker and will be cleared/used by the bootloader
    uint32_t  cbBlPreservedRam;       // the amount RAM the bootloader will not touch, 0xA0000000 -> 0xA0000000 + cbBlPerservedRam; Debug data, Ram Header and Persistent data must be in this section
} IMAGE_HEADER_INFO;

#pragma pack(pop)

extern const IMAGE_HEADER_INFO _image_header_info;      // this is the header info right before .rodata, defined by the linker

// psudo function to get the flash header info.
#define getImageHeaderInfoStructure()   (&_image_header_info)
#define _IMAGE_PTR_TABLE_OFFSET         (0x0F8ul)
#define _IMAGE_HEADER_ADDR_OFFSET       (0x0FCul)

#endif

#if defined(__PIC32MX__)
	extern unsigned int	__PIC32_pbClk;
#endif

#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)

// PPS Support for PIC32MX1 and PIC32MX2 parts
// Locks all PPS functions so that calls to mapPpsInput() or mapPpsOutput() always fail.
// You would use this function if you set up all of your PPS settings at the beginning
// of your sketch, and then wanted to prevent any changes while running the rest of the
// sketch. Can be unlocked with unlockPps().
void lockPps();

// Once the PPS system has been locked with logkPps(), this function will unlock it.
// Use this function before making any changes with mapPpsInput() or mapPpsOutput()
// functions.
void unlockPps();

// Use this function to assign a digital pin to a particular digital peripheral function.
// <pin> is the digital pin you want to change
// <func> is the name of the periphreal function to assign <pin> to. (see ppsFunctionType
// enum in p32_defs.h)
// Note that for a given <func> there are only a certain subset of possible pins that
// can be assigned (up to 8 possible, as defined in the PIC32 datasheet). If you pass
// in a <pin> that can't be assigned to <func>, this function will return 'false'.
boolean mapPps(uint8_t pin, ppsFunctionType func);

#endif  // defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)

#ifdef __cplusplus
} // extern "C"
#endif

#endif
