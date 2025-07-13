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
	int16_t	 xAxis;
	int16_t	 yAxis;
	int16_t	 zAxis;
	int16_t	 zAxisRot;
	int16_t	 gain;
	int16_t	 tilt;
	uint32_t buttons;
	
public:
	Pincab_Controller_V2HB_();

	void begin(bool initAutoSendState = true);
	void end();

	void setXAxis(int16_t value);
	void setYAxis(int16_t value);
	void setZAxis(int16_t value);
	void setZAxisRot(int16_t value);
	void setGain(int16_t value);
	void setTilt(int16_t value);
	
	void setButton(uint8_t button, uint8_t value);
	void pressButton(uint8_t button);
	void releaseButton(uint8_t button);

	void sendState();
};
extern Pincab_Controller_V2HB_ Pincab_Controller_V2HB;

#endif
#endif
