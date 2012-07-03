/*
 *  Bluetooth.cpp - Library for controlling
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

#include "Bluetooth.h"

Bluetooth::Bluetooth(uint8_t RX, uint8_t TX, int BaudRate=38400) : serial(SoftwareSerial(RX, TX)
{ 
  rx = RX;
  tx = TX;
  baudrate = BaudRate;
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);
  serial = SoftwareSerial(rx,tx, false);
  // TODO: Check valid baud rates
  serial.begin(baudrate);
  serial.flush();
}

/*
void Bluetooth::setWorkingMode(bool mode)
{
  this.mode = mode;
  if master
  {
    serial.print(SS+WM+"1"+ES);
  }
  else
  {
    serial.print(SS+WM+"0"+ES);
  }
}

String Bluetooth:getWorkingMode()
{
  return this.mode;
}

void Bluetooth::setName(String name)
{
  //TODO: Check max size for name
  serial.print(SS+SN+name+ES);
  this.name = name;
}

String Bluetooth::getName()
{
  return this.name;
}

void Bluetooth::setPinCode(String pin)
{
    if (pin.length() >= 4) {
      serial.print(SS+PN+String(pin).substring(0,3)+ES);
      this.pin = pin;
    }
    else
      println(PTS);
}

int Bluetooth::getPinCode()
{
  return this.pin;
}

void Bluetooth::setAutoConnect(bool activate)
{
  this.autoconnect = activate;
  if activate 
  {
    serial.print(SS+AC+"1"+ES);
  }
  else
  {
    serial.print(SS+AC+"0"+ES);
  }
}

void Bluetooth::setPermitConnection(bool activate)
{
  if activate 
  {
    serial.print(SS+PC+"1"+ES);
  }
  else
  {
    serial.print(SS+PC+"0"+ES);
  }
}

void Bluetooth::connectToDevice(String address)
{
  //TODO: Filter output of serial to return the state of the connection
  serial.print(SS+CD+address+ES);
}

void Bluetooth::inquire(bool activate)
{
  if activate 
  {
    serial.print(SS+IN+"1"+ES);
  }
  else
  {
    serial.print(SS+IN+"0"+ES);
  }
}

void Bluetooth::setAutoReconnect(bool activate)
{
  //TODO: If the device has been initialized as master
  // then we cannot activate autoreconnect
  if activate 
  {
    serial.print(SS+AR+"1"+ES);
  }
  else
  {
    serial.print(SS+AR+"0"+ES);
  }
}

void Bluetooth::inputPinCode(int pin)
{
  serial.print(SS+IP+String(pin)+ES);
}

void Bluetooth::readLocalAddress()
{
  serial.print(SS+RT+ES);
}

void Bluetooth::inquiry(activate)
{
  if activate
    serial.print(SS+INQ+"1"+ES);
  else
    serial.print(SS+INQ+"0"+ES);
}

void Bluetooth::connectToDevice(String address)
{
  serial.print(SS+CD+address+ES);
}
*/

