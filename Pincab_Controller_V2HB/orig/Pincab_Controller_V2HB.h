/*
  Pincab_Controller_V2.h

  Copyright (c) 2016, www.zebsboards.com

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

#ifndef Pincab_Controller_V2HB_h
#define Pincab_Controller_V2HB_h

#include "HID.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

//================================================================================
//================================================================================
//  Pincab_Controller_V2HB (Nudge/Button Encoder)

class Pincab_Controller_V2HB_
{
private:
	bool     autoSendState;
	int8_t	 xAxis;
	int8_t	 yAxis;
	int8_t	 zAxis;
	//int8_t	 gain;
	//int8_t	 tilt;
	//int8_t	 xAxisRotation;
	//int8_t	 yAxisRotation;
	int8_t	 zAxisRotation;
	uint32_t buttons;
	
public:
	Pincab_Controller_V2HB_();

	void begin(bool initAutoSendState = true);
	void end();

	void setXAxis(int8_t value);
	void setYAxis(int8_t value);
	void setZAxis(int8_t value);
	//void setXAxisRotation(int8_t value);
	//void setYAxisRotation(int8_t value);
	void setZAxisRotation(int8_t value);
	//void setGain(int8_t value);
	//void setTilt(int8_t value);
	
	void setButton(uint8_t button, uint8_t value);
	void pressButton(uint8_t button);
	void releaseButton(uint8_t button);

	void sendState();
};
extern Pincab_Controller_V2HB_ Pincab_Controller_V2HB;

#endif
#endif
