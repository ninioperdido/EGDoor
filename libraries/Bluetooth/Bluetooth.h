/*
 *  Bluetooth.h - Library for controlling
 *  the seeeduino bluetooth shield.
 *  (cc) 2012 Gerardo Puerta <gerardo>at<gnuardo.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  
 *  02110-1301  USA
 *
 */

#ifndef Bluetooth_h
#define Bluetooth_h

#include "Arduino.h"
#include "SoftwareSerial.h"
#include <stdio.h>

// Debugging in serial console
#ifndef DEBUG
#define DEBUG true
#endif

// Max AT command buffer
#define MAX_COMMAND_BUFFER 1024

// AT Commmands

// Start String
#define SS "\r\n"
// End String
#define ES "\r\n"

// Working Mode
#define WM "+STWMOD="
// Name
#define SN "+STNA="
// Pin code
#define PN "+STPIN="
// AutoConnect
#define AC "+STAUTO="
// Permit Connection
#define PC "+STOAUT="
// Connect to device
#define CD "+CONN="
// Inquire
#define IN "+INQ="
// Autoreconnect
#define AR "+LOSSRECONN="
// Input Pin Code
#define IP "+RTPIN="
// Pin too short message
#define PTS "The PIN must be at least 4 character long"
// Inquiry
#define IN "+INQ="
// Read local address
#define RT "+RTADDR"
// Connect to device
#define CD "+CONN="

using namespace std;

// Aux functions
char* command(char *subcommands, ... );

class Bluetooth
{

 public:
  /*
  Put pinModes and inits serial communication
  with the BT module
  */
  Bluetooth(uint8_t RX, uint8_t TX, int BaudRate);

  /*
  Set working mode:
  False: slave.
  True: master.
  */
  void setWorkingMode(bool mode);
  const char* getWorkingMode();
  
  //void setBaudRate(int br);

  void setName(const char* name);
  const char* getName();

  void setAutoConnect(bool activate);
  bool getAutoConnect();

  void setPermitConnect(bool activate);
  bool getPermitConnect();

  void setPinCode(const char* pin);
  const char* getPinCode();

  void connectToDevice(String address);
  void inquire(bool activate);
  // Autoreconnecting when master is beyond range
  void setAutoReconnect(bool activate);
  void inputPinCode(int pin);

  void readLocalAddress();
  char* lastCommand();
  void reply();

  static inline void handle_interrupt();

 private:
  uint8_t rx;
  uint8_t tx;
  int baudrate;
  SoftwareSerial serial;
  const char *name;
  const char *pin;
  bool master;
  bool autoconnect;
  bool permitconnect;
  bool inquiring;
  char *lastcommand;
  char *lastreply;
  static Bluetooth *active;
};

#endif

