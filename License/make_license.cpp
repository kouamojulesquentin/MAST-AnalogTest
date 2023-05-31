#include <sys/ioctl.h>
#include <net/if.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
using namespace std;

#include "mast_license.hpp"
#include "aes.hpp"

int main()
{
  unsigned int value;
  char ignore;
  aes_context ctx;
  unsigned char buf[32];

  mast_license_type new_license;
  
    int success = 0;

 istringstream iss(istringstream::in);

  cout << "Please write MAC address string: ";
  string line;
  getline(cin,line);

  iss.str(line);

 iss >> hex;

  for(int i=0;i<5;i++) {
    iss >> value >> ignore;
    new_license.mac_address[i]=value;
  }
  iss >> value;
  new_license.mac_address[5]=value;

  // validate

  for(unsigned int i=0;i<sizeof(new_license.mac_address)/sizeof(new_license.mac_address[0]);i++)
    cout << hex << static_cast<unsigned int>(new_license.mac_address[i]) << " ";

  cout << endl;
  
      std::time_t result = std::time(NULL);
    std::time_t expiration;
    std::cout << std::ctime(&result);
    expiration = result+MONTH+MONTH;
   std::cout << std::ctime(&expiration);

  new_license.expiration = expiration;
  new_license.last_run = result;

   if (expiration > result) std::cout << "License Valid until " << std::ctime(&expiration) <<"\n";

   success = 1;

    if (success) 
      {
      memset( buf, 0, 32 );
      aes_set_key( &ctx, MAST_master_key, 256 );
      memcpy(buf,(char *)&new_license,sizeof(mast_license_type));
      aes_encrypt( &ctx, buf, buf );
      aes_encrypt( &ctx, buf+16, buf+16 );
     
     ofstream file (LICENSE_FILE, ios::binary);
//     file.write ((char *)&new_license, sizeof(mast_license_type));
     file.write ((char *)buf, 32);
     file.close (); 
     std::cout << "Wrote license file of size 0x" << 32 << "\n"; 
    }
}
