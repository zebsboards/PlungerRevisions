/*
  Pincab_Controller.cpp

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

#include "Pincab_Controller.h"

#if defined(_USING_HID)

#define Pincab_Controller_REPORT_ID 0x03
#define Pincab_Controller_STATE_SIZE 4

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  
	// Pincab_Controller
	0x05, 0x01,			      // USAGE_PAGE (Generic Desktop)
	0x09, 0x05,			      // USAGE (Gamepad)
	0xa1, 0x01,			      // COLLECTION (Application)
	0x85, Pincab_Controller_REPORT_ID, //   REPORT_ID (3)

/*  // 12 Buttons
	0x05, 0x09,			      //   USAGE_PAGE (Button)
	0x19, 0x01,			      //   USAGE_MINIMUM (Button 1)
	0x29, 0x0c,			      //   USAGE_MAXIMUM (Button 12)
	0x15, 0x00,			      //   LOGICAL_MINIMUM (0)
	0x25, 0x01,			      //   LOGICAL_MAXIMUM (1)
	0x75, 0x01,			      //   REPORT_SIZE (1)
	0x95, 0x20,			      //   REPORT_COUNT (32)
	0x55, 0x00,			      //   UNIT_EXPONENT (0)
	0x65, 0x00,			      //   UNIT (None)
	0x81, 0x02,			      //   INPUT (Data,Var,Abs)
*/
	// X, Y, and Z Axis
	0x05, 0x01,			      //   USAGE_PAGE (Generic Desktop)
	0x15, 0x00,			      //   LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	      //   LOGICAL_MAXIMUM (255)
	0x75, 0x08,			      //   REPORT_SIZE (8)
	0x09, 0x01,			      //   USAGE (Pointer)
	0xA1, 0x00,			      //   COLLECTION (Physical)
	0x09, 0x30,		          //     USAGE (x)
	0x09, 0x31,		          //     USAGE (y)
	0x09, 0x32,		          //     USAGE (z)
	0x09, 0x35,		          //     USAGE (rz)
	0x95, 0x04,		          //     REPORT_COUNT (4)
	0x81, 0x02,		          //     INPUT (Data,Var,Abs)
	0xc0,				      //   END_COLLECTION
                              
	0xc0				      // END_COLLECTION
};

Pincab_Controller_::Pincab_Controller_()
{
	// Setup HID report structure
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
	
	// Initalize State
	xAxis = 0;
	yAxis = 0;
	zAxis = 0;
	zAxisRotation = 0;
	//buttons = 0;
	
}

void Pincab_Controller_::begin(bool initAutoSendState)
{
	autoSendState = initAutoSendState;
	sendState();
}

void Pincab_Controller_::end()
{
}

/*void Pincab_Controller_::setButton(uint8_t button, uint8_t value)
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
void Pincab_Controller_::pressButton(uint8_t button)
{
	bitSet(buttons, button);
	if (autoSendState) sendState();
}
void Pincab_Controller_::releaseButton(uint8_t button)
{
	bitClear(buttons, button);
	if (autoSendState) sendState();
}

*/
void Pincab_Controller_::setXAxis(int8_t value)
{
	xAxis = value;
	if (autoSendState) sendState();
}
void Pincab_Controller_::setYAxis(int8_t value)
{
	yAxis = value;
	if (autoSendState) sendState();
}
void Pincab_Controller_::setZAxis(int8_t value)
{
	zAxis = value;
	if (autoSendState) sendState();
}
void Pincab_Controller_::setZAxisRotation(int8_t value)
{
	zAxisRotation = value;
	if (autoSendState) sendState();
}


void Pincab_Controller_::sendState()
{
	uint8_t data[Pincab_Controller_STATE_SIZE];
	
/*
	// Split 32bit button-state into 4 bytes
	data[0] = buttonTmp & 0xFF;		
	buttonTmp >>= 8;
	data[1] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	data[2] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	data[3] = buttonTmp & 0xFF;
*/
	data[0] = xAxis;
	data[1] = yAxis;
	data[2] = zAxis;
    data[3] = zAxisRotation; 

	// HID().SendReport(Report number, array of values in same order as HID descriptor, length)
	HID().SendReport(Pincab_Controller_REPORT_ID, data, Pincab_Controller_STATE_SIZE);
}

Pincab_Controller_ Pincab_Controller;

#endif