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

/// \file PE1MEW_Button.h
/// \brief Button class to control a single button switch to a pin on the Arduino board without use of delay() functions
/// \date 30-3-2017
/// \author Remko Welling (PE1MEW)
/// \version 1.0	Initial version
/// \version 1.1  Modified state for better detection of event.


#ifndef __PE1MEW_BUTTON_H__
#define __PE1MEW_BUTTON_H__

#include <stdint.h>

#define PRESS_TIME_T1 2000  ///< Time the button has to be pressed in ms (default is 4 sec)
#define PRESS_TIME_T2 10000  ///< Time the button has to be pressed in ms (default is 4 sec)

#define PRESS_CONTER_MAX  5  ///< counter to prevent from jitter on the buttnon/


/// \brief defines states of the LED
typedef	enum {
   STATE_NOT_PRESSED = 0,   ///< Button is not pressed
   STATE_PRESSED,           ///< Button is pressed
   STATE_PRESSED_READ,      ///< Button is pressed and information is read
   STATE_PRESSED_T1,        ///< Button is pressed longer than T1 period
   STATE_PRESSED_T1_READ,   ///< Button is pressed and information is read
   STATE_PRESSED_CONTINOUS  ///< Button is pressed continous
} eStateButton;


/// \class PE1MEW_Button
/// \brief Button class
class PE1MEW_Button
{
//variables
public:
protected:
private:

  uint8_t _buttonPin;					///< pin number of the pin to which the Button is connected

  /// Current state of the LED
  eStateButton	_currentState;

  /// Next state of the LED set by external commands
  eStateButton	_nextState;

  /// conter that holds teh sample cout at which te button is pressed.
  int8_t _testCounter;

  /// Time in ms at which the button press started.
  unsigned long _pressStartTime;

//functions
public:

  /// \brief Default constructor
  /// \param[in] pin at which button is connected
	PE1MEW_Button(uint8_t pin);

  /// \brief Destructot
	~PE1MEW_Button();

        /// \brief process function to handle all house keeping functions in the led class.
	/// This function shall be called from the main loop
  /// \return state of the button: Not pressed, pressed, > 4 seconds pressed.
	eStateButton process(void);

  /// \brief returns state of the button.
  /// \return state of the button: Not pressed, pressed, > 4 seconds pressed.
  eStateButton getState(void);

  /// \brief reset to restart button evaluation
  void reset(void);

protected:
private:

  /// \brief read pin state and save result iternal in class.
  void testButton(void);

  /// \brief process state after read of state.
  void processRead(void);

}; //PE1MEW_Button

#endif //__PE1MEW_BUTTON_H__
