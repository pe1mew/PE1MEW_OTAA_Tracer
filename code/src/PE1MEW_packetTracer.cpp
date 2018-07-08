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

/// \file tracer.cpp
/// \brief implementation of tracer functions
/// \author Remko Welling
/// \version 0.1   21-3-2018   Initial version

#include "PE1MEW_packetTracer.h"

packetTracer::packetTracer():
_deviceSelector(0),
_rssiBuffer(),
_rssiBufferIterator(0)
{
  //ctor
  initialize();
}

packetTracer::packetTracer(SH1106 *display):
_deviceSelector(0),
_rssiBuffer(),
_rssiBufferIterator(0)
{
  //ctor
  _display = display;
  initialize();
}

packetTracer::~packetTracer()
{
    //dtor
}

void packetTracer::initialize(void)
{
  // Initilize _rssiBuffer with '0' to identify that field is not filled with a real value.
  // This will help with a smooth build-up of the dipslay
  for (int i = 0; i < RSSIBUFFERSIZE; i++)
  {
    _rssiBuffer[i] = 0;
  }
}

bool packetTracer::addRSSI(int8_t rssiValue, uint8_t *message)
{
  // Temprary variable for return value.
  bool joinRequestDetectedFromSelectedDevice = false;

  // depending on the LoRaWAN packet type perfom actions.
  switch(getMhdrMtype(message[0]))
  {
    case MHDR_MTYPE_JOIN_REQUEST:
      if(compareDataSet(message, _deviceSelector))
      {
        // all positions in the buffer ware set with real values.
        // Now we have to shift all positions to make room for the new rssi vlaue.
        for(int i = 1; i < RSSIBUFFERSIZE; i++)
        {
          // copy the value of i to the previous position (i-1)
          _rssiBuffer[i-1] = _rssiBuffer[i];
        }
        // Add the new rssi value to the last position of the buffer.
        _rssiBuffer[RSSIBUFFERSIZE-1] = rssiValue;
        joinRequestDetectedFromSelectedDevice = true;
      }
      break;
    default:
      // Nothing to do
      break;
  }
  return joinRequestDetectedFromSelectedDevice;
}

void packetTracer::displayRSSI(char *buffer)
{
  _display->clear();
  _display->setFont(ArialMT_Plain_10);
  _display->setTextAlignment(TEXT_ALIGN_LEFT);

  int16_t x = {0};
  int16_t y = {0};

  for(int8_t i = 0; i < RSSIBUFFERSIZE; i++)
  {
    //x = 0;
    y = i * VERTICAL_LINE_HEIGHT;

    if(_rssiBuffer[i] != 0)
    {
      // Write the value of the buffer
      _display->drawString(x, y, String(_rssiBuffer[i]));
      // Fill the rectangle
      int16_t barSize = {0};
      barSize = _rssiBuffer[i] + 125; // 23 is offset
      _display->fillRect((x + 23), (y + 3), barSize, 7);

      // write the RSSI-bar for this value
    }
  }
  y += VERTICAL_LINE_HEIGHT;

  _display->drawString(x + 23, y, String(buffer));
  _display->drawString(x, y, String(_deviceSelector));
  _display->display();
}

bool packetTracer::compareDataSet(uint8_t *payload, const uint8_t testRecordNumber)
{
  bool returnValue = true;

  uint8_t row = testRecordNumber;

  for( int i = 0; i < TESTDATA_RECORDS; i++)
  {
    // Test if appeui is the same
    if(_arrayAppDevEUI[row][0][i] != payload[8-i])
    {
      returnValue = false;
    }

    // Test is deveui is the same
    if(_arrayAppDevEUI[row][1][i] != payload[16-i])
    {
      returnValue = false;
    }
  }
  return returnValue;
}

uint8_t packetTracer::getMhdrMtype(const uint8_t mhdrByte)
{
    uint8_t returnValue = 0;
    uint8_t tempMhdrByte = mhdrByte & MHDR_MTYPE_MASK;

    switch (tempMhdrByte)
    {
    case MHDR_MTYPE_JOIN_REQUEST:
       {
           returnValue = MHDR_MTYPE_JOIN_REQUEST;
       }
       break;
    case MHDR_MTYPE_JOIN_ACCEPT:
       {
           returnValue = MHDR_MTYPE_JOIN_ACCEPT;
       }
       break;
    case MHDR_MTYPE_UNCONFIRMED_DATA_UP:
       {
           returnValue = MHDR_MTYPE_UNCONFIRMED_DATA_UP;
       }
       break;
    case MHDR_MTYPE_UNCONFIRMED_DATA_DOWN:
       {
           returnValue = MHDR_MTYPE_UNCONFIRMED_DATA_DOWN;
       }
       break;
    case MHDR_MTYPE_CONFIRMED_DATA_UP:
       {
           returnValue = MHDR_MTYPE_CONFIRMED_DATA_UP;
       }
       break;
    case MHDR_MTYPE_CONFIRMED_DATA_DOWN:
       {
           returnValue = MHDR_MTYPE_CONFIRMED_DATA_DOWN;
       }
       break;
    case MHDR_MTYPE_RFU:
       {
           returnValue = MHDR_MTYPE_RFU;
       }
       break;
    case MHDR_MTYPE_PROPRIARITY:
       {
           returnValue = MHDR_MTYPE_PROPRIARITY;
       }
       break;
    default:
       {
           returnValue = MHDR_MTYPE_MASK;
       }
       break;
    }
    return returnValue;
}

void packetTracer::changeSelectedDevice(void)
{
  if(_deviceSelector < TESTDATA_RECORDS-1)
  {
    _deviceSelector++;
  }
  else
  {
    _deviceSelector = 0;
  }

  showSelectedDevice();
}

void packetTracer::showSelectedDevice(void)
{
  char buffer[30];

  _display->clear();
  _display->setFont(ArialMT_Plain_10);

  sprintf(buffer, "Tracing device: %i", getDeviceSelector());
  _display->drawString(0, 0, String(buffer));

  sprintf(buffer, "AppEUI:");
  _display->drawString(0, 14, String(buffer));

  for (int i = 0; i < 8; i++ )
  {
      sprintf((buffer+(i*2)), "%02X", _arrayAppDevEUI[_deviceSelector][0][i]);
  }
  _display->drawString(0, 27, String(buffer));

  sprintf(buffer, "DevEUI:");
  _display->drawString(0, 40, String(buffer));

  for (int i = 0; i < 8; i++ )
  {
      sprintf((buffer+(i*2)), "%02X", _arrayAppDevEUI[_deviceSelector][1][i]);
  }
  _display->drawString(0, 53, String(buffer));
  _display->display();
}
