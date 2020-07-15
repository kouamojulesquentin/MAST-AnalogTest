#include <iostream>
#include "microAES.h"
#include "SSAKplugin.h"

using namespace std;

/* Example Senario:
  Crypto-processeir : AES128
  Amount of Secure SIB : 13;
*/
#define NUM_OF_S2IB 0x0D // 13
const u8 SSAK_bits[16]={0,0,0,0,0,0,0,0,0,0,0,0x7C,0x10,0xD4,0xDB};
const u8 config[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x1};
int main()
{
  cryptoProc* CryptoProcesseur= (cryptoProc*) new microAES();
  try{
    SSAKplugin SSAKdriver(*CryptoProcesseur,NUM_OF_S2IB);

    //Connexion
    if(SSAKdriver.select(config,SSAK_bits))
    {
      cout << "Succes" << endl;

      /// Test in protected addLogStream
      SSAKdriver.deselect();
    }
    else
      cout << "Failed" << endl;
  }
  catch(exception& e)
  {
    cerr << e.what();
    return 1;
  }
  return 0;
}
