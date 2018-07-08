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

/// \file PE1MEW_Led.cpp

#include "PE1MEW_Led.h"

#include "Arduino.h"

// default constructor
PE1MEW_Led::PE1MEW_Led():
	_ledPin(LED_BUILTIN),	// set output pin to built in led
	_currentState(STATE_ON),
	_nextState(STATE_IDLE),
	_blinkCounter(-1),
	_blinkPeriodTime(DEFAULT_DELAY),
	_blinkStartTime(0),
	_blinkContinous(false)
{
	// initialize digital pin as an output.
	pinMode(_ledPin, OUTPUT);
} //PE1MEW_Led

PE1MEW_Led::PE1MEW_Led(uint8_t ledPin):
	_ledPin(ledPin),	// set output pin to ledPin specified
	_currentState(STATE_ON),
	_nextState(STATE_IDLE),
	_blinkCounter(-1),
	_blinkPeriodTime(DEFAULT_DELAY),
	_blinkStartTime(0),
	_blinkContinous(false)
{
	// initialize digital pin as an output.
	pinMode(_ledPin, OUTPUT);
} //PE1MEW_Led

// default destructor
PE1MEW_Led::~PE1MEW_Led()
{
} //~PE1MEW_Led

void PE1MEW_Led::process(void)
{
  // pre process next state.
  // Make all required changes.
  if (_currentState != _nextState)
  {
    // Process new state (current)
    switch (_nextState)
    {
      case STATE_IDLE:
        off(); // switch off LED
      break;

      case STATE_BLINK_ON:
        on(); // Switch on LED and wait for timeout
      break;

      case STATE_BLINK_OFF:
        off();  // Switch off LED and wait for timeout
      break;

      case STATE_ON:
        on(); // Switch LED off
      break;

      default:
        off();  // Wrong state; Switch LED off.
      break;
    }// end switch
  } // end if

	// switch led state to next state
	_currentState = _nextState;

  // Process current state activities.
  // This switch case is processing timeout timers
	switch (_currentState)
	{
		case STATE_BLINK_ON:
			if (millis() - _blinkStartTime > _blinkPeriodTime)
			{
				_nextState = STATE_BLINK_OFF;
				_blinkStartTime = millis();
			}
		break;

		case STATE_BLINK_OFF:
			if (millis() - _blinkStartTime > _blinkPeriodTime)
			{
				_nextState = STATE_BLINK_ON;
				_blinkStartTime = millis();

				if (!_blinkContinous)
				{
					_blinkCounter--;
					if (_blinkCounter == 0)
					{
						_nextState = STATE_IDLE;
					}
				}
			}
		break;

		default:

		break;
	} // end switch
}

void PE1MEW_Led::startBlink(void)
{
	startBlink(true, 1, DEFAULT_DELAY);
}

void PE1MEW_Led::startBlink(uint8_t n)
{
	startBlink(false, n, DEFAULT_DELAY);
}

void PE1MEW_Led::startBlink(uint8_t n, unsigned long ms)
{
	startBlink(false, n, ms);
}

void PE1MEW_Led::startBlink(bool continous = true, uint8_t n = 1, unsigned long ms = DEFAULT_DELAY)
{
	_blinkContinous = continous;
	_blinkCounter = n;
	_blinkStartTime = millis();
	_blinkPeriodTime = ms;
	_nextState = STATE_BLINK_ON;
  process();   // run process to make changes take effect
}

void PE1MEW_Led::stopBlink(void)
{
	_nextState = STATE_IDLE;
  process();   // run process to make changes take effect
}

void PE1MEW_Led::setOn(void)
{
	_nextState = STATE_ON;
  process();   // run process to make changes take effect
}

void PE1MEW_Led::setOff(void)
{
	_nextState = STATE_IDLE;
  process();   // run process to make changes take effect
}

void PE1MEW_Led::on(void)
{
	digitalWrite(_ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void PE1MEW_Led::off(void)
{
	digitalWrite(_ledPin, LOW);    // turn the LED off by making the voltage LOW
}
