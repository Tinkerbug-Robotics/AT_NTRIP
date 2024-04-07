/**********************************************************************
*
* Copyright (c) 2023 Tinkerbug Robotics
*
* This program is free software: you can redistribute it and/or modify it under the terms
* of the GNU General Public License as published by the Free Software Foundation, either
* version 3 of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
* PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this 
* program. If not, see <https://www.gnu.org/licenses/>.
* 
* Authors: 
* Christian Pedersen; tinkerbug@tinkerbugrobotics.com
* 
**********************************************************************/

#include <AT_NTRIP.h>
#include "ArduinoLog.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


// Constructors
AT_NTRIP::AT_NTRIP(){}


// Public Methods

// Reset, power cycle, and allow the SIM7000 to boot
void AT_NTRIP::init(int pwrkey, 
                          int reset_key,
                          char* apn_in,
                          char* host_in,
                          int &tcp_port_in,
                          char* mntpnt_in,
                          char* user_in,
                          char* psw_in,
                          char* info_in,
                          Stream &input_port)
{
    
    // Initialize with log level and log output. 
    Log.begin(LOG_LEVEL_VERBOSE, &Serial, false);
    
    port = &input_port;

    PWRKEY = pwrkey;
    RESET = reset_key;
    
    apn = apn_in;
    host = host_in;
    tcp_port = tcp_port_in;
    mntpnt = mntpnt_in;
    user = user_in;
    psw = psw_in;
    info = info_in;
    
    sim7000.init(PWRKEY, 
                 RESET, 
                 apn,
                 host,
                 tcp_port,
                 mntpnt,
                 user,
                 psw,
                 info_in,
                 input_port);
    
    sim7000.connect();
    
}

bool AT_NTRIP::setBaudRate(long rate)
{
    // Establish the connection
    if(!sim7000.setBaudRate(rate))
    {
        return false;
    }
    
    return true;
}

bool AT_NTRIP::establishTCPConnectionClient()
{
    
    // Establish the connection
    if(!sim7000.establishTCPConnectionClient())
    {
        return false;
    }
    
    return true;
}

bool AT_NTRIP::establishTCPConnectionServer()
{
    
    // Establish the connection
    if(!sim7000.establishTCPConnectionServer())
    {
        return false;
    }
    
    return true;
}

bool AT_NTRIP::checkTCP()
{
    return sim7000.checkTCP();
}

uint16_t AT_NTRIP::readTCP(char *tcp_buffer,
                                int max_length,
                                int max_time)
{
    
    uint16_t i = 0;
    uint64_t time_start = millis();
    uint64_t time_read = 0;
    bool read = false;
    int time_out = 50;
    
    // While nothing has been read or it has been less than 
    // a given time since the last read
    while((!read || millis()-time_read < time_out) &&
          i < max_length &&
          millis()-time_start < max_time)
    {
        if (port->available())
        {
            // Read data
            tcp_buffer[i++] =(char)port->read();
            time_read = millis();
            read = true;
        }
    }
    //Serial.print("Read time = ");Serial.println(millis()-time_start);
    //Serial.print("buffer = ");Serial.println(tcp_buffer);
    //Serial.print("i = ");Serial.println(i);
    return i;
     
}

bool AT_NTRIP::sendNTRIPCorrections(char* ntrip_correction, int data_length)
{
    unsigned long start_time = millis();
    port->print(F("AT+CIPSEND="));
    port->println(data_length);
    delay(10);
    for(int i=0; i<data_length; i++)
    {
        port->write(ntrip_correction[i]);
        //Serial.print(ntrip_correction[i], HEX);
    }
    Serial.println("");

    Log.trace("Send length = %d; Send time = %d\n",data_length, millis()-start_time);
    return true;
}