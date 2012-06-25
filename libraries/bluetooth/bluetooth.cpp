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

#include "Arduino.h"
#include "Bluetooth.h"

Bluetooth::Bluetooth(int RX, int TX, int BaudRate=38400)
{ 
  rx = RX;
  tx = TX;
  baudrate = BaudRate;
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);
  serial = SoftwareSerial(rx,tx);
  // TODO: Check valid baud rates
  serial.begin(baudrate);
  serial.flush();
}

void workingMode(String mode)
{
  // TODO: Check mode is 1 or 0
  serial.print(SS+WM+mode+ES);
}

void setName(String name)
{
  //TODO: Check max size for name
  serial.print(SS+SN+name+ES);
}

