/*
  Pincab_Controller_V2.cpp

  Copyright (c) 2015, www.zebsboards.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Pincab_Controller_V2HB.h"

#if defined(_USING_HID)

#define Pincab_Controller_V2HB_REPORT_ID 0x03
#define Pincab_Controller_V2HB_STATE_SIZE 16

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  
	// Pincab_Controller_V2HB
	0x05, 0x01,			      // USAGE_PAGE (Generic Desktop)
	0x09, 0x05,			      // USAGE (Gamepad)
	0xA1, 0x01,			      // COLLECTION (Application)
	0xA1, 0x00,			      //   COLLECTION (Physical)
	0x85, Pincab_Controller_V2HB_REPORT_ID, //   REPORT_ID (3)
	
    // 32 Buttons
	0x05, 0x09,			      //     USAGE_PAGE (Button)
	0x19, 0x01,			      //     USAGE_MINIMUM (Button 1)
	0x29, 0x20,			      //     USAGE_MAXIMUM (Button 32)
	0x15, 0x00,			      //     LOGICAL_MINIMUM (0)
	0x25, 0x1F,			      //     LOGICAL_MAXIMUM (1)
	0x75, 0x01,			      //     REPORT_SIZE (1)
	0x95, 0x20,			      //     REPORT_COUNT (32)
	0x55, 0x00,			      //     UNIT_EXPONENT (0)
	0x65, 0x00,			      //     UNIT (None)
	0x81, 0x02,			      //     INPUT (Data,Var,Abs)

	// X, Y, Z, Gain, Tilt
	0x05, 0x01,			      //     USAGE_PAGE (Generic Desktop)
	0x09, 0x30,               //     USAGE (X)      (nudge x)			   30
	0x09, 0x31,               //     USAGE (Y)      (nudge y)              31
	0x09, 0x32,               //     USAGE (Z)      (plunge z)			   32
	0x09, 0x35,				  //	 USAGE (ZROT)   (plunge -z)		       35
	0x09, 0x36,				  //	 USAGE (Slider)	(Gain)				   36
	0x09, 0x36,				  //	 USAGE (Slider)	(Tilt)				   36
	0x16, 0x01,  0x80,        //     LOGICAL_MINIMUM (-32767)			   (16,01,80) -32767	(15, 00) 0
	0x26, 0xFF,	 0x7F,        //     LOGICAL_MAXIMUM (+32767) 		       (0x80=-127,0x7F=127) (0x26, 0x7F, 0x00,) 255 (26, FF, 03) 1023   (26, FF, 7F) 32767
	0x75, 0x10,			      //     REPORT_SIZE (16)
	0x95, 0x06,			      //     REPORT_COUNT (6)
	0x81, 0x02,			      //     INPUT (Data,Var,Abs)
	    	
	0xc0,				      //   END_COLLECTION
                              
	0xc0				      // END_COLLECTION
};

Pincab_Controller_V2HB_::Pincab_Controller_V2HB_()
{
	// Setup HID report structure
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
	
	// Initalize State
	
	buttons = 0;
	xAxis = 0;
	yAxis = 0;
	zAxis = 0;
	zAxisRot = 0;
	gain = 0;
	tilt = 0;
	
	
}

void Pincab_Controller_V2HB_::begin(bool initAutoSendState)
{
	autoSendState = initAutoSendState;
	sendState();
}

void Pincab_Controller_V2HB_::end()
{
}

void Pincab_Controller_V2HB_::setButton(uint8_t button, uint8_t value)
{
	if (value == 0)
	{
		releaseButton(button);
	}
	else
	{
		pressButton(button);
	}
}
void Pincab_Controller_V2HB_::pressButton(uint8_t button)
{
	bitSet(buttons, button);
	if (autoSendState) sendState();
}
void Pincab_Controller_V2HB_::releaseButton(uint8_t button)
{
	bitClear(buttons, button);
	if (autoSendState) sendState();
}


void Pincab_Controller_V2HB_::setXAxis(int16_t value)
{
	xAxis = value;
	if (autoSendState) sendState();
}
void Pincab_Controller_V2HB_::setYAxis(int16_t value)
{
	yAxis = value;
	if (autoSendState) sendState();
}
void Pincab_Controller_V2HB_::setZAxis(int16_t value)
{
	zAxis = value;
	if (autoSendState) sendState();
}
void Pincab_Controller_V2HB_::setZAxisRot(int16_t value)
{
	zAxisRot = value;
	if (autoSendState) sendState();
}
void Pincab_Controller_V2HB_::setGain(int16_t value)
{
	gain = value;
	if (autoSendState) sendState();
}
void Pincab_Controller_V2HB_::setTilt(int16_t value)
{
	tilt = value;
	if (autoSendState) sendState();
}


void Pincab_Controller_V2HB_::sendState()
{
	uint8_t data[Pincab_Controller_V2HB_STATE_SIZE];
	uint32_t buttonTmp = buttons;
	uint16_t xAxisTmp = xAxis;
	uint16_t yAxisTmp = yAxis;
	uint16_t zAxisTmp = zAxis;
	uint16_t zAxisRotTmp = zAxisRot;
    uint16_t gainTmp = gain;
	uint16_t tiltTmp = tilt;
	uint8_t highbyte;
	uint8_t lowbyte;
	
	// Split 32bit button-state into 4 bytes
	
	data[0] = buttonTmp & 0xff;
	buttonTmp >>= 8;
	data[1] = buttonTmp & 0xff;
	buttonTmp >>= 8;
	data[2] = buttonTmp & 0xff;
	buttonTmp >>= 8;
	data[3] = buttonTmp & 0xff;
	buttonTmp >>= 8;
	
	
    // Axis Data
	highbyte = (uint8_t)(xAxisTmp >> 8);
	lowbyte = (uint8_t)(xAxisTmp & 0x00FF);
	data[4] = lowbyte;
	data[5] = highbyte;
	highbyte = (uint8_t)(yAxisTmp >> 8);
	lowbyte = (uint8_t)(yAxisTmp & 0x00FF);
	data[6] = lowbyte;
	data[7] = highbyte;
	highbyte = (uint8_t)(zAxisTmp >> 8);
	lowbyte = (uint8_t)(zAxisTmp & 0x00FF);
	data[8] = lowbyte;
	data[9] = highbyte;
	highbyte = (uint8_t)(zAxisRotTmp >> 8);
	lowbyte = (uint8_t)(zAxisRotTmp & 0x00FF);
	data[10] = lowbyte;
	data[11] = highbyte;
	highbyte = (uint8_t)(gainTmp >> 8);
	lowbyte = (uint8_t)(gainTmp & 0x00FF);
	data[12] = lowbyte;
    data[13] = highbyte;
	highbyte = (uint8_t)(tiltTmp >> 8);
	lowbyte = (uint8_t)(tiltTmp & 0x00FF);
	data[14] = lowbyte;
	data[15] = highbyte;

	// HID().SendReport(Report number, array of values in same order as HID descriptor, length)
	HID().SendReport(Pincab_Controller_V2HB_REPORT_ID, data, Pincab_Controller_V2HB_STATE_SIZE);
}

Pincab_Controller_V2HB_ Pincab_Controller_V2HB;

#endif