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

//const std::string LICENSE_PATH="MAST_LICENSE_PATH";

typedef struct {
    unsigned char mac_address[6];
    std::time_t expiration;
    std::time_t last_run;
 } mast_license_type;

#endif
