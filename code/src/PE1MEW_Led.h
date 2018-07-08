/*--------------------------------------------------------------------
  This file is part of the PE1MEW code libraries.

  The PE1MEW code libraries are free software:
  you can redistribute it and/or modify it under the terms of a Creative
  Commons Attribution-NonCommercial 4.0 International License
  (http://creativecommons.org/licenses/by-nc/4.0/) by
  PE1MEW (http://pe1mew.nl) E-mail: pe1mew@pe1mew.nl

  The PE1MEW code libraries are distributed in the hope that
  it will be useful, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.
  --------------------------------------------------------------------*/

/// \file PE1MEW_Led.h
/// \brief LED class to control a single LED connected to a pin o the Arduino board without use of delay() functions
/// \author Remko Welling (PE1MEW)
/// \version 1.0	13-3-2017  Initial version
/// \version 1.1  18-4-2018  removed define for LED_BUILTIN

#ifndef __PE1MEW_LED_H_
#define __PE1MEW_LED_H_

#include <stdint.h>

/// Default delay of 150 ms
static uint16_t  DEFAULT_DELAY	= 150;

/// \brief defines states of the LED
typedef	enum {
   STATE_IDLE = 0,    ///< LED is switched off
   STATE_BLINK_ON,    ///< LED is blinking and on
   STATE_BLINK_OFF,   ///< LED is blinking and off
   STATE_ON           ///< LED is switched on
} eState;

/// \class PE1MEW_Led
/// \brief LED class
class PE1MEW_Led
{
//variables
public:
protected:
private:

   /// pin number of the pin to wich the LED is connected
   uint8_t _ledPin;

   /// Current state of the LED
   eState	_currentState;

   /// Next state of the LED set by external commands
   eState	_nextState;

   /// Times the LED can blink after start. Maximum is 255.
   uint8_t	_blinkCounter;

   /// Half the duty cycle of a blink.
   unsigned long 	_blinkPeriodTime;

   /// start time used for timeout functions.
   unsigned long	_blinkStartTime;

   /// True is continous blinking is selected
   bool	_blinkContinous;

//functions
public:

	/// \brief default constructor.
	/// This default constructor will configure the built in led of an Arduino board
	PE1MEW_Led();

	/// \brief overloaded constructor to configure hardware pin connected to a led
	/// \param[in] ledPin hardware pin at Arduino at which led is connected.
	PE1MEW_Led(uint8_t ledPin);

	~PE1MEW_Led();

	/// \brief process function to handle all house keeping functions in the led class.
	/// This function shall be called from the main loop
	void process(void);

	/// \brief Start blinking the led continuously at default duty cycle (300 ms)
	void startBlink(void);

	/// \brief Start blinking n times at default duty cycle (300 ms)
	/// \param n count of blinks
	void startBlink(uint8_t n);

	/// \brief Start blinking n times at period in ms (duty cycle is 2 x period)
	/// \param n count of blinks
	/// \param ms half-period time in ms
	void startBlink(uint8_t n, unsigned long ms);

	/// \brief Start blinking
	/// \param continous true: continous blinking, false blink n times
	/// \param n count of blinks
	/// \param ms half-period time in ms
	void startBlink(bool continous, uint8_t n, unsigned long ms);

	/// \brief stop blinking, switch LED off (same as setOff() )
	void stopBlink(void);

	/// \brief switch LED on
	void setOn(void);

	/// \brief switch LED off
	void setOff(void);

protected:
private:

	/// \brief Switch on LED
	void on(void);

	/// \brief switch off LED
	void off(void);

	/// \brief reverse LED state
	void reverse(void);

}; //ledNonBlocking

#endif //__PE1MEW_LED_H_
