#ifndef ConfigVariables_H
#define ConfigVariables_H
#include <Arduino.h>

//*********************************** Button Variables *********************************************************
//****************************** Configured through utility *************************************
byte GamepadFlag;                     // Gamepad/Keyboard Mode Switch  0 = Keyboard Mode / 1 = Gamepad Mode                          -  8bit EEPROM VALUE / Switchable by BAT command file
byte CoinDoorInUse;                   // (0 = Not Used, 1 = Use Coin Door routine - Single Click)
byte CoinDoorButton;                  // button number assigned to Coin Door Input
byte LaunchBallButton;                // Launch Ball Button Assignment 
byte LBKey;                           // Launch Ball Keypress Assignment
byte useHatSwitch;                    // (0 = No Hatswitch, 1 = Use Hatswitch)
byte KeyCommands[32] = {0,0,0,0,0,    // Keyboard to Gamepad Button assignments 
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0};
byte LabelCodes[32] = {0,0,0,0,0,0,   // Keyboard to button assignment label codes 
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0};
//*************************************** Internal *******************************************
//*********************************************************************************************

//*********************************** Nudge Variables **********************************************************
//***************************** Configured through utility *************************************
byte UseNudging;                    // nudging enabled = 1, OFF = 0
byte Sensitivity;                   // nudging sensitivity +-2g,4g,6g,8g,16g
byte NuOrientation;                 // nudge sensor orientation (0 = Vertical, 1 = Horizontal)
byte NuXGain;                       // nudge X axis amplification
byte NuYGain;                       // nudge Y axis amplification
byte XDZ;                           // nudge X axis deadzone value
byte YDZ;                           // nudge Y axis deadzone value
byte UseTilt;                       // (1 = Enable Tilt routines, 0 = Disable tilt routines)
byte TiltButton;                    // button number assigned to Tilt input
byte Xtilt;                         // XTilt value received
byte Ytilt;                         // YTilt value received
byte Debounce;                      // Tilt debounce value recieved
//************************************** Internal *******************************************
int16_t NuXatRest;                  // nudge X axis Resting Point
int16_t NuYatRest;                  // nudge Y axis Resting Point
int16_t NuZatRest;                  // nudge Z axis Resting Point
int16_t NuXdeadzone;                // convert received/stored byte to 16bit value (approx val)
int16_t NuYdeadzone;                // convert received/stored byte to 16bit value (approx Val)
int16_t TiltXval;                   // X value that Tilt occurs at (approx val)
int16_t TiltYval;                   // Y value that tilt occurs at (approx val)
int16_t BounceTime;
//*********************************************************************************************

//*********************************** Plunger Variables ********************************************************
//****************************** Configured through utility *************************************
int16_t pushMax;                   // maximum number of push counts from calibration
int16_t pullMax;                   // maximum number of pull counts from calibration
//************************************** Internal *******************************************
int position;                      // current plunger reading 
int analogAtRest;                  // value of 0 typically (1023 counts on pot)
int MaxMoved;                      // furthest value that plunger has been PUSHED to
bool analogMode = true;            // plunger mode flag ( 1 = analog; 0 = digital)
bool push = false;                 // flag for plunger movement being a push - determines method of handling readings and whether nudge should be looked at or not 
bool pull = false;                 // flag for plunger movement being a pull - determines method of handling readings and whether nudge should be looked at or not 
long timer;                        // timer to indicate that the plunger has stopped at the resting point - reset timer

 #endif // ConfigVariables.h