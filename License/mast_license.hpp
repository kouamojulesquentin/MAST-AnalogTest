#ifndef H_MAST_LICENSE_H
#define H_MAST_LICENSE_H


#define LICENSE_FILE "license.mast"
#define LICENSE_PATH "MAST_LICENSE_PATH"

#include <ctime>

#define MINUTE 		60
#define HOUR 		MINUTE*60
#define DAY		HOUR*24
#define MONTH		DAY*30
#define YEAR		DAY*365

enum class LicenseOptions
{
  None                   = 0,
  Loopback_protocol      = 1 << 0,  //!< Only Loopback protocol are enabled
  Simulation_protocol    = 1 << 1,  //!< Only Simulation protocols enabled
  Hardware_protocol      = 1 << 2,  //!< Only hardware-interfaces (ex: openOCD) protocols enabled

  Default           = Loopback_protocol,
  Simulation        = Loopback_protocol | Simulation_protocol,
  Prototype         = Hardware_protocol | Loopback_protocol | Simulation_protocol,
  All               = Prototype,
};

struct mast_license_type {
    unsigned char mac_address[6];
    std::time_t expiration;
    std::time_t last_run;
    LicenseOptions options=LicenseOptions::Prototype;
 } ;

#endif
