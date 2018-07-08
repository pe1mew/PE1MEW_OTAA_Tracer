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

/// \file tracer.h
/// \brief Function declarations and defines for tracer
/// \date 21-3-2018
/// \author Remko Welling
/// \version 0.1   21-3-2018   Initial version

#ifndef PACKETTRACER_H
#define PACKETTRACER_H

/// Includes
#include "stdint.h" // Include for uint8_t like variables.
#include "SH1106.h" // include to SH1106 library

/// defines for detection of the type of LoRaWAN frame
#define MHDR_MTYPE_MASK                   0xE0
#define MHDR_MTYPE_JOIN_REQUEST	          0x00	/// JOIN_REQUEST message
#define MHDR_MTYPE_JOIN_ACCEPT	          0x20  /// JOIN_ACCEPT message
#define MHDR_MTYPE_UNCONFIRMED_DATA_UP	  0x40	/// UNCONFIRMED_DATA_UP message
#define MHDR_MTYPE_UNCONFIRMED_DATA_DOWN  0x60	/// UNCONFIRMED_DATA_DOWN message
#define MHDR_MTYPE_CONFIRMED_DATA_UP	    0x80	/// CONFIRMED_DATA_UP message
#define MHDR_MTYPE_CONFIRMED_DATA_DOWN	  0xA0	/// ONFIRMED_DATA_DOWN message
#define MHDR_MTYPE_RFU                    0xC0  /// RFU message
#define MHDR_MTYPE_PROPRIARITY            0xE0  /// PROPRIARITY message
#define MHDR_RFU_MASK                     0x1C

/// Defines for  display layout.
#define RSSIBUFFERSIZE        5  /// maximum number of RSSI values stored in the object.
#define VERTICAL_LINE_HEIGHT  9  /// Distance between lines in the oLed display.

/// Arrays with App- and devEUI's.
/// These arrays hold for each device, up to a maximum set in TESTDATA_RECORDS, the appEUI and devEUI pairs.
/// Trough precompile defines a array can be selected using define TESTDATA.
/// When set to 1 the test data array is selected. this array holds 6 pairs all set for one device.
/// When set to 0 the production set of pairs is selected.

// Selection of test data
#define TESTDATA 0              /// Set this define to 1 to use test data
#define TESTDATA_RECORDS 6      /// number of devices in tracer administrated

#if TESTDATA==1
  /// test data for development of the tool.
  const uint8_t _arrayAppDevEUI[TESTDATA_RECORDS][2][8] = {
  {{0x70, 0xB3, 0xD5, 0x7E, 0xF0, 0x00, 0x51, 0x93}, {0x00, 0x04, 0xA3, 0x0B, 0x00, 0x1F, 0x86, 0x21}}, // test deveui
  {{0x70, 0xB3, 0xD5, 0x7E, 0xF0, 0x00, 0x51, 0x93}, {0x00, 0x04, 0xA3, 0x0B, 0x00, 0x1F, 0x86, 0x21}}, // test deveui
  {{0x70, 0xB3, 0xD5, 0x7E, 0xF0, 0x00, 0x51, 0x93}, {0x00, 0x04, 0xA3, 0x0B, 0x00, 0x1F, 0x86, 0x21}}, // test deveui
  {{0x70, 0xB3, 0xD5, 0x7E, 0xF0, 0x00, 0x51, 0x93}, {0x00, 0x04, 0xA3, 0x0B, 0x00, 0x1F, 0x86, 0x21}}, // test deveui
  {{0x70, 0xB3, 0xD5, 0x7E, 0xF0, 0x00, 0x51, 0x93}, {0x00, 0x04, 0xA3, 0x0B, 0x00, 0x1F, 0x86, 0x21}}, // test deveui
  {{0x70, 0xB3, 0xD5, 0x7E, 0xF0, 0x00, 0x51, 0x93}, {0x00, 0x04, 0xA3, 0x0B, 0x00, 0x1F, 0x86, 0x21}}  // test deveui
  };
#else
  /// _arrayAppDevEUI structure
  /// This array holds all AppEUI and DevEUI of the devices that can be traced.
  /// Both EUI are in MSB format and HEX.
  const uint8_t _arrayAppDevEUI[TESTDATA_RECORDS][2][8] = {
  {{0x70, 0xB3, 0xD5, 0x7E, 0xF0, 0x00, 0x51, 0x93}, {0x00, 0x04, 0xA3, 0x0B, 0x00, 0x1F, 0x86, 0x21}}, /// test deveui (0)
  {{0x00, 0x59, 0xAC, 0x00, 0x00, 0x01, 0x00, 0xB6}, {0xA8, 0x17, 0x58, 0xFF, 0xFE, 0x03, 0x0E, 0xCC}}, /// Target deveui 1
  {{0x00, 0x59, 0xAC, 0x00, 0x00, 0x01, 0x00, 0xB6}, {0xA8, 0x17, 0x58, 0xFF, 0xFE, 0x03, 0x08, 0x75}}, /// Target deveui 2
  {{0x00, 0x59, 0xAC, 0x00, 0x00, 0x01, 0x00, 0xB6}, {0xA8, 0x17, 0x58, 0xFF, 0xFE, 0x03, 0x0E, 0xDA}}, /// Target deveui 3
  {{0x00, 0x59, 0xAC, 0x00, 0x00, 0x01, 0x00, 0xB6}, {0xA8, 0x17, 0x58, 0xFF, 0xFE, 0x03, 0x10, 0xB7}}, /// Target deveui 4
  {{0x00, 0x59, 0xAC, 0x00, 0x00, 0x01, 0x00, 0xB6}, {0xA8, 0x17, 0x58, 0xFF, 0xFE, 0x03, 0x25, 0x12}}  /// Target deveui 5
  };
#endif

/// \class packetTracer
/// Class that holds all functions for the tracer functionality.
class packetTracer
{
    // Variables
    public:
    protected:
    private:
      /// device  Pointer
      uint8_t _deviceSelector;

      /// buffer to store last 3 RSSI values
      int8_t _rssiBuffer[RSSIBUFFERSIZE];

      /// iterator for the _rssiBuffer array
      uint8_t _rssiBufferIterator;

      /// Pointer to the dipplay object that is created outside of this class.
      SH1106 *_display;

    // functions
    public:
        /// \brief default constructor
        packetTracer();

        /// \brief overloaded constructor
        /// \param display is pointer to teh display object.
        packetTracer(SH1106 *display);

        /// \brief default destructor
        virtual ~packetTracer();

        /// \brief Add new RSSI value and LoRaWAN payload for packet type evaluation.
        /// \param rssiValue is the new RSSI value, expected between -125 and -25 dBm.
        /// \param message pointer to LoRaWAN payload
        /// \return true if a joinrequest was received of the selected AppEUI and DevEUI pair
        bool addRSSI(int8_t rssiValue, uint8_t *message);

        /// \brief display current buffer content
        /// \param pointer to the buffer taht contains time information prepared externaly from this function.
        void displayRSSI(char *buffer);

        /// \brief get message type
        /// \param mhdrByte MAC header that contains the message type of the physical payload.
        /// \return message type using defines. See header file.
        uint8_t getMhdrMtype(const uint8_t mhdrByte);

        /// \brief compare AppEUI and DevEUI received with target data.
        /// \param payload pointer to payload data that shall be evaluated.
        /// \param testRecordNumber Data set to which payload data shall be compared.
        /// \return true when received data is from the target data.
        bool compareDataSet(uint8_t *payload, const uint8_t testRecordNumber);

        // /// \brief function for future use?
        // uint8_t compareDataSet(uint8_t *payload);

        /// \brief getter for current device being traced.
        /// \return number of the device traced.
        uint8_t getDeviceSelector(void) { return _deviceSelector;};

        /// \brief select the next device in the device list
        /// After selecting the AppEUI and DevEUI of active device is displayed.
        void changeSelectedDevice(void);

        /// \brief display the AppEUI and DevEUI of active device that is being traced.
        void showSelectedDevice(void);

    protected:
    private:
        /// \brief initialize de object at creation.
        void initialize(void);
};

#endif // PACKETTRACER_H
