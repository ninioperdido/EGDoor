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

#include "Arduino.h"

#ifndef Bluetooth_h
#define Bluetooth_h

#include <SoftwareSerial.h>


// Start String
#define SS \r\n
// End String
#define ES \r\n

// Working Mode
#define WM +STWMOD=
// Name
#define SN +STNA=


class Bluetooth
{

 public:
  /*
  Put pinModes and inits serial communication
  with the BT module
  */
  Bluetooth(int RX, int TX, int BaudRate);

  /*
  Set working mode:
  0: slave.
  1: master.
  */
  void setWorkingMode(String mode);

  void setBaudRate(int br);
  void setDeviceName(String name);
  void autoConnect(bool activate);
  void permitConnect(bool activate);
  void setPinCode(long pin);
  void deletePinCode();

  String readLocalAddress();
  String inquiry();
  String connectToDevice(String address);
  String status();

 private:
  int rx;
  int tx;
  int baudrate;
  SoftwareSerial *serial;
  String pincode;
  String mode;
  bool autoconnect;
  bool permitconnect;
}

#endif

