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
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


// Statics
Bluetooth *Bluetooth::active = 0;

#if defined(TIMER1_COMPA_vect)
ISR(TIMER1_COMPA_vect)
{
  Bluetooth::handle_interrupt();
}
#endif

char* command(char *subcommands, ... )
{
  char command[MAX_COMMAND_BUFFER];
  va_list arguments;
  char *value;
  int i, mc;

  strcpy(command, subcommands);
  va_start(arguments, subcommands);
 
  for (i=0; subcommands[i] != '\0'; ++i)
  {
    value = va_arg(arguments, char *);
    strcat(command, value); 
  }

  return command;
}


// Static function for the interrupt vector
inline void Bluetooth::handle_interrupt()
{ 
  if (active)
    active->reply();
}

Bluetooth::Bluetooth(uint8_t RX, uint8_t TX, int BaudRate=38400) : serial(SoftwareSerial(RX, TX))
{
  // Code for set an interrupt timer in order to print 
  // serial data available in concrete intervals in an async form.
  // initialize Timer1
  cli();             // disable global interrupts
  TCCR1A = 0;        // set entire TCCR1A register to 0
  TCCR1B = 0;
  // set compare match register to desired timer count:
  OCR1A = 15624;
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A); 
  sei();
  
  rx = RX;
  tx = TX;
  baudrate = BaudRate;
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);
  // TODO: Check valid baud rates
  serial.begin(baudrate);
  serial.flush();
  active = this;
}

void Bluetooth::reply()
{
//  char recvChar;
//  if(serial.available())
//  {
//    recvChar = serial.read();
//    lastreply = command(lastreply, recvChar);
//  }
//  Serial.println(lastreply);
  Serial.println("En interrupcion");
  delay(100);
}

void Bluetooth::setWorkingMode(bool mode)
{
  master = mode;
  if (this->master)
    lastcommand = command(SS,WM,"1",ES);
  else
    lastcommand = command(SS,WM,"0",ES);

  if (DEBUG)
    Serial.println(lastcommand);
  
  serial.print(lastcommand);
}

const char* Bluetooth::getWorkingMode()
{
  if (this->master)
    return "Master";
  else
    return "Slave";
}

void Bluetooth::setName(const char* name)
{
  this->name = name;
  lastcommand = command(SS,SN,name,ES);

  //TODO: Check max size for name
  if (DEBUG)
    Serial.println(lastcommand);
  serial.print(lastcommand);
}

const char* Bluetooth::getName()
{
  return this->name;
}

void Bluetooth::setPinCode(const char* pin)
{
  lastcommand = command(SS,PN,pin,ES);
  if (strlen(pin) >= 4) {
    if (DEBUG)
      Serial.println(lastcommand);
    this->pin = pin;
    serial.print(lastcommand);
 }
}

const char* Bluetooth::getPinCode()
{
  return this->pin;
}

void Bluetooth::setAutoConnect(bool activate)
{
  this->autoconnect = activate;
  if (activate)
    lastcommand = command(SS,AC,"1",ES);
  else
    lastcommand = command(SS,AC,"0",ES);
  if (DEBUG)
    Serial.println(lastcommand);
  serial.print(lastcommand);
  //TODO: Check when we need to insert delays
}


void Bluetooth::setPermitConnect(bool activate)
{
  this->permitconnect = activate;
  if (activate)
    lastcommand = command(SS,PC,"1",ES);
  else
    lastcommand = command(SS,PC,"0",ES);
 
  if (DEBUG)
    Serial.println(lastcommand); 
  serial.print(lastcommand);
}

void Bluetooth::connectToDevice(String address)
{
  lastcommand = command(SS,CD,address,ES);
  if (DEBUG)
    Serial.println(lastcommand); 
  //TODO: Filter output of serial to return the state of the connection
  serial.print(lastcommand);
}

void Bluetooth::inquire(bool activate)
{
  this->inquiring = activate;
  if (activate)
    lastcommand = command(SS,IN,"1",ES);
  else
    lastcommand = command(SS,IN,"0",ES);
  if (DEBUG)
    Serial.println(lastcommand); 
  serial.print(lastcommand);
}

void Bluetooth::setAutoReconnect(bool activate)
{
  if (activate)
    if (!master)
    {
      this->autoconnect = activate;
      lastcommand = command(SS,AR,"1",ES);
    }
  else
    lastcommand = command(SS,AR,"0",ES);
  if (DEBUG)
    Serial.println(lastcommand);
  serial.print(lastcommand);
}

void Bluetooth::inputPinCode(int pin)
{
  lastcommand = command(SS,IP,String(pin),ES);
  if (DEBUG)
    Serial.println(lastcommand);
  serial.print(lastcommand);
}

void Bluetooth::readLocalAddress()
{
  lastcommand = command(SS, RT, ES);
  if (DEBUG)
    Serial.println(lastcommand);
  serial.print(lastcommand);
}

char* Bluetooth::lastCommand()
{
  return lastcommand;
}

