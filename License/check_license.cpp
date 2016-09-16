//===========================================================================
//                           check_license.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file check_license.cpp
//!
//! Implements license check 
//!
//===========================================================================
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include  <cstdlib>
using namespace std;
using std::pair;

#include "mast_license.hpp"
#include "aes.hpp"

bool get_mac_address(unsigned char *mac_address);

#ifndef _WIN32
#include <sys/ioctl.h>
#include <net/if.h> 
#include <netinet/in.h>

bool get_mac_address(unsigned char *mac_address)
{
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[1024];
    bool success = false;
   
     int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1) { /* handle error*/ };

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { /* handle error */ }

    struct ifreq* it = ifc.ifc_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                    success = true;
                    break;
                }
            }
        }
        else { /* handle error */ }
    }

    if (success) 
        memcpy(mac_address, ifr.ifr_hwaddr.sa_data, 6);

 return success;
}
#else
bool get_mac_address(unsigned char *mac_address)
{
 return true;
}
#endif

pair<bool,LicenseOptions> check_license();

pair<bool,LicenseOptions> check_license()
{
    unsigned char mac_address[6];
    bool success = false;
    aes_context ctx;
    unsigned char buf[32];
    char name_with_path[200]={""};
    char *lpath;
    pair<bool,LicenseOptions> retvalue;
    
    mast_license_type *mast_license;

    retvalue.first = false;
    retvalue.second = LicenseOptions::None;

   memset( buf, 0, 32 );
    aes_set_key( &ctx, MAST_master_key, 256 );
   
   lpath = std::getenv("MAST_LICENSE_PATH");
   if (lpath != nullptr)
     {
     strcpy(name_with_path,lpath);
     strcat( name_with_path,"/");
     }
   strcat( name_with_path,LICENSE_FILE);

    ifstream file (name_with_path, ios::binary);

 if (!file.good())
  {
   std::cerr << "Could not find MAST license file " << name_with_path << "\n";
   return retvalue;
  }
    file.seekg (0, ios::beg);
    file.read ((char *)buf, 32);
    file.close();

   aes_decrypt( &ctx, buf, buf );
   aes_decrypt( &ctx, &buf[16], &buf[16] );
 
mast_license = (mast_license_type *)buf;

    if (get_mac_address(mac_address)==true) 
      {
 
      if (memcmp(mac_address,mast_license->mac_address,6)==0) 
        {
	success = true;
	}
      else 	
        {
	std::cout << "Error: MAC Addresses not in license file\n";
	return retvalue;
	}

      std::time_t now = std::time(NULL);
      
      if (now > mast_license->expiration)
        {
        std::cout << "Error: License expired on " << std::ctime(&mast_license->expiration) <<" \n";
	 success = false;
	}
	
      if (now < mast_license->last_run) 
         {
        std::cout << "Error: System clock as been rolled back since last usage \n";
	 success = false;
	}

    if (success)
      {
        mast_license->last_run = now;
      memset( buf, 0, 32 );
      aes_set_key( &ctx, MAST_master_key, 256 );
      memcpy(buf,(char *)mast_license,sizeof(mast_license_type));
      aes_encrypt( &ctx, buf, buf );
      aes_encrypt( &ctx, buf+16, buf+16 );
     
     ofstream file (LICENSE_FILE, ios::binary);
     file.write ((char *)buf, 32);
     file.close (); 
      }

    }
  retvalue.first = success;
  
 return retvalue;
}

#ifdef TEST_LICENSE
int main()
{
 auto res = check_license();
 if (res.first==true)
       std::cout << "MAST license check OK\n";
 else
       std::cout << "MAST license check FAILES\n";
 
 return 0;
}
#endif
