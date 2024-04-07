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

#ifndef ATNTRIP_h
#define ATNTRIP_h

#include <Arduino.h>
#include "TR_SIM7000.h"

class AT_NTRIP
{
    public:
  
        // Constructor
        AT_NTRIP();
        
        // Initialize
        void init(int pwrkey, 
                  int reset_key,
                  char* apn_in,
                  char* host_in,
                  int &tcp_port_in,
                  char* mntpnt_in,
                  char* user_in,
                  char* psw_in,
                  char* info_in,
                  Stream &input_port);
        
        // Request mount points raw data
        bool establishTCPConnectionClient();
        
        // Request mount points raw data
        bool establishTCPConnectionServer();
        
        uint16_t readTCP(char *tcp_buffer,
                         int buffer_len,
                         int max_time);
                         
        bool setBaudRate(long rate);
        
        bool checkTCP();
        
        bool sendNTRIPCorrections(char* ntrip_correction, int data_length);
        
    private:
    
        TR_SIM7000 sim7000;
        
        // GPIO pin values
        int TX, RX, PWRKEY, RESET;
        
        char* apn;
        char* host;
        int tcp_port;
        char* mntpnt;
        char* user;
        char* psw;
        char* info;
        
        Stream *port;
        
};

#endif



