#include "SSAKplugin.h"
#include "usefullFunc.h"
#include <stdexcept>
#include "g3log/g3log.hpp"

using namespace std;
unsigned nbCharFor(unsigned i)
{
    unsigned res=i/8;
    if(res*8<i)
        res++;
    return res;
}
SSAKplugin::SSAKplugin(cryptoProc& CP,unsigned nbS2IB,unsigned interfaceSize):m_CP(CP)
{
    m_nbS2IB=nbS2IB;
    unsigned nbCharForS2IBStates=nbCharFor(m_nbS2IB);
    if(interfaceSize==0)
        m_interfaceSize=CP.encrSize();
    else
        m_interfaceSize=interfaceSize;
    if(m_nbS2IB>m_interfaceSize)throw range_error("Number of S2IB("+i2s(m_nbS2IB)+") must be smaller than interface size("+i2s(m_interfaceSize)+")");
    m_Selected.resize(nbCharForS2IBStates);
    for(unsigned i=0;i<nbCharForS2IBStates;i++)
        m_Selected[i]=0;
}

SSAKplugin::~SSAKplugin()
{
    //dtor
}

const u8* SSAKplugin::computeResponse(const u8* key,const u8* challenge){
  const u8* response;

  m_CP.Setkey(key);
  m_CP.Setplain(challenge);
  m_CP.encrypt();
  response = m_CP.Getencr();

 return response;
}
// const u8* config : from right, bit set to 1 ask for sib unlocking 
// [00000 ... |S2IB_3| S2IB_2| S2IB_1| S2IB_0 ]
// const u8* key : credentials (user secret key)

bool SSAKplugin::select(const u8* config, const u8* key){
  u8* challenge;
  const u8* response;
  unsigned vectorSize=nbCharFor(m_interfaceSize);


  challenge = new u8[vectorSize];
//  response = new u8[vectorSize];

  /*SEND config & read challenge through MAST  */

 // envoi config et lis dans variable challenge
 //config est un registre de taille key: m_interfaceSize  bits
 
  /*end SEND Config*/
  
  m_CP.Setkey(key);
  m_CP.Setplain(challenge);
  m_CP.encrypt();
  response = m_CP.Getencr();

  //SEND response
  
   // envoi la variable response
   
  /*end SEND response*/

  // Authentication verification
  u8* nullVec=new u8[vectorSize];
  for(unsigned i=0;i<vectorSize;i++)
    nullVec[i]=0;
  for(unsigned i=0;i<SSAK_CONNECTION_WATCHDOG;i++)
  {
    ///SEND nullVec & READ challenge
      //lis et ecrit dans variable challenge
     ///end READ challenge
   
    if(challenge[0]==1)
    {
      delete[] challenge;
      delete[] nullVec;
      setSelected(config);
      //Now S2SIB can be used as a normal SIB
      return true;
    }
    if(challenge[0]==2){
      delete[] challenge;
      delete[] nullVec;
      //ERROR, challenge echouee
      return false;
    }
  }
  delete[] challenge;
  delete[] nullVec;
  return false;
}

void SSAKplugin::deselect()
{
  unsigned nbCharForInterface=nbCharFor(m_interfaceSize);
  u8* configVector=new u8[nbCharForInterface];
  for(unsigned i=0;i<nbCharForInterface;i++)
    configVector[i]=0;

  // SEND Configuration

 // envoi un vecteur de zero et ferme TOUTES lis S2SIB
 
  /*end SEND Config*/

  setSelected(configVector);
  delete[] configVector;
}

void SSAKplugin::setSelected(const u8* configVector){
  unsigned mask=0;
  unsigned nbByte=nbCharFor(m_nbS2IB);
  unsigned nbBitsLastByte=m_nbS2IB%8;
  for(unsigned i=0;i<nbBitsLastByte;i++)
    mask|=1<<i;
  for(unsigned i=0;i+1<nbByte;i++)
    m_Selected[i]=configVector[i];
  m_Selected[nbByte-1]=configVector[nbByte-1]&mask;
}

bool SSAKplugin::isSelected()const
{
  unsigned nbByteOfSelVector=nbCharFor(m_nbS2IB);
  for(unsigned i=0;i<nbByteOfSelVector;i++)
    if(m_Selected[i])
      return true;
  return false;
}
bool SSAKplugin::isSelected(unsigned iS2IB)const
{
  if(iS2IB>=m_nbS2IB)
    throw range_error("S²IB number '"+i2s(iS2IB)+"' is out of range (func bool SSAKplugin::isSelected(unsigned))");
  return m_Selected[iS2IB/8] & (1<<(iS2IB%8));
}
