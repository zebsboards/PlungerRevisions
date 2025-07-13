/*
  Pincab_Controller_VTest.cpp

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
#define Pincab_Controller_V2HB_STATE_SIZE 10

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  
	// Pincab_Controller_VTest
	0x05, 0x01,			      // USAGE_PAGE (Generic Desktop)
	0x09, 0x05,			      // USAGE (Gamepad)
	0xa1, 0x01,			      // COLLECTION (Application)
	0x85, Pincab_Controller_V2HB_REPORT_ID, //   REPORT_ID (3)

     // 18 Buttons
	0x05, 0x09,			      //   USAGE_PAGE (Button)
	0x19, 0x01,			      //   USAGE_MINIMUM (Button 1)
	0x29, 0x20,			      //   USAGE_MAXIMUM (Button 32)
	0x15, 0x00,			      //   LOGICAL_MINIMUM (0)
	0x25, 0x20,			      //   LOGICAL_MAXIMUM (32)
	0x75, 0x01,			      //   REPORT_SIZE (1)
	0x95, 0x20,			      //   REPORT_COUNT (32)
	0x55, 0x00,			      //   UNIT_EXPONENT (0)
	0x65, 0x00,			      //   UNIT (None)
	0x81, 0x02,			      //   INPUT (Data,Var,Abs)

	// X, Y, 
	0x05, 0x01,			      //   USAGE_PAGE (Generic Desktop)
	//0x09, 0x01,			      //   USAGE (Pointer)
	0xA1, 0x00,			      //   COLLECTION (Physical)
	0x16, 0x00, 0x80,	      //   LOGICAL_MINIMUM (-32768)
	0x26, 0xFF, 0x7F,         //   LOGICAL_MAXIMUM (32767) 		(0x81=-127,0xFF=255,0x7F=127)
	0x09, 0x30,               //   USAGE (X)   (nudge x)
	0x09, 0x31,               //   USAGE (Y)   (nudge y)
	0x75, 0x10,               //   REPORT_SIZE (16)  
	0x95, 0x02,               //   REPORT_COUNT (4)
	0x81, 0x02,			      //   INPUT (Data,Var,Abs)
	
	/* 2 8bit Axis */
	0x09, 0x32,							/*     USAGE (Z) */
	0x09, 0x35,							/*     USAGE (Rz) */
	0x15, 0x80,							/*     LOGICAL_MINIMUM (-128) */
	0x25, 0x7F,							/*     LOGICAL_MAXIMUM (127) */
	0x75, 0x08,							/*     REPORT_SIZE (8) */
	0x95, 0x02,							/*     REPORT_COUNT (2) */
	0x81, 0x02,							/*     INPUT (Data,Var,Abs) */
	0xc0,								/*   END_COLLECTION */
                              
	0xc0				      // END_COLLECTION
};

Pincab_Controller_V2HB_::Pincab_Controller_V2HB_()
{
	// Setup HID report structure
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
	
	// Initalize State
	xAxis = 0;
	yAxis = 0;
	zAxis = 0;
	zAxisRotation = 0;
	buttons = 0;
	
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
void Pincab_Controller_V2HB_::setZAxis(int8_t value)
{
	zAxis = value;
	if (autoSendState) sendState();
}
void Pincab_Controller_V2HB_::setZAxisRotation(int8_t value)
{
	zAxisRotation = value;
	if (autoSendState) sendState();
}


void Pincab_Controller_V2HB_::sendState()
{
	uint8_t data[Pincab_Controller_V2HB_STATE_SIZE];
	uint16_t dataX = xAxis;
	uint16_t dataY = yAxis;
	uint32_t buttonTmp = buttons;

	// Split 32bit button-state into 4 bytes
	
	data[0] = buttonTmp & 0xff;
	buttonTmp >>= 8;
	data[1] = buttonTmp & 0xff;
	buttonTmp >>= 8;
	data[2] = buttonTmp & 0xff;
	buttonTmp >>= 8;
	data[3] = buttonTmp & 0xff;	
	
    // Axis Data
	data[4] = dataX & 0xff;
	dataX >>= 8;
	data[5] = dataX & 0xff;
	data[6] = dataY & 0xff;
	dataY >>= 8;
	data[7] = dataY & 0xff;
	data[8] = zAxis;
    data[9] = zAxisRotation; 

	// HID().SendReport(Report number, array of values in same order as HID descriptor, length)
	HID().SendReport(Pincab_Controller_V2HB_REPORT_ID, data, Pincab_Controller_V2HB_STATE_SIZE);
}

Pincab_Controller_V2HB_ Pincab_Controller_V2HB;

#endif