#include "cryptoProc.h"
#include <stdexcept>
#include "usefullFunc.h"
#include <ostream>
#include <sstream>


using namespace std;
cryptoProc::cryptoProc(unsigned endian):m_endian(endian)
{
    if(m_endian &(!0x1))throw logic_error("Endian type unknown");
}
void cryptoProc::defaultTab(){
    m_plain= new u8[plainSize()];
    m_key = new u8[keySize()];
    m_encr = new u8[encrSize()];
}

cryptoProc::~cryptoProc()
{
    delete[] m_plain;
    delete[] m_key;
    delete[] m_encr;
}



u8 hexChar(char lettre){
    if(lettre>='a'&&lettre<='f')
        return 10+lettre-'a';
    if(lettre>='A'&&lettre<='f')
        return 10+lettre-'A';
    if(lettre>='0'&&lettre<='9')
        return lettre-'0';
    throw range_error(l2s(lettre)+" is not a hexadecimal digit");
}
u8 hexPair(char msb,char lsb){
    msb=hexChar(msb);
    lsb=hexChar(lsb);
    return ((msb<<4)&0xf0)|lsb;
}
void cryptoProc::loadHexa(const string& val,cryptoProc::CryptoTabs dest)
{
    unsigned BLZ;
    u8* p_dest;
    switch(dest)
    {
        case plain_e:
            BLZ=plainSize();
            p_dest=m_plain;
            break;
        case key_e:
            BLZ=keySize();
            p_dest=m_key;
            break;
        case encr_e:
            BLZ=encrSize();
            throw logic_error("Can't load hexa into result memory");
            break;
	default: break; //Should this raise an error?    
    }
    if(val.size()!=BLZ/4)throw range_error("Hex vector '"+val+"' has "+i2s(val.size()*4)+" bits instead of "+i2s(BLZ));
    if(isBigEndian())
        for(unsigned i=0;i<BLZ/8;i++)
            p_dest[i]=hexPair(val[2*i],val[2*i+1]);
    else
        for(unsigned i=0;i<BLZ/8;i++)
            p_dest[BLZ/8-1-i]=hexPair(val[2*i],val[2*i+1]);
}


void cryptoProc::addLogStream(ostream& flux)
{
    m_logOutputs.push_back(&flux);
}

void cryptoProc::toLogStream(const string& logs)
{
    for(ostream* p_log: m_logOutputs)
        *p_log << logs;
}


void cryptoProc::toHex(ostream &flux,const u8* data)const
{
//    const char hexTab[17]="0123456789abcdef";
    for(unsigned i=0;i<16;i++)
    {
        unsigned idata;
        if(isBigEndian())
            idata=data[i];
        else
            idata=data[15-i];
        if(idata<=0xf)
            flux << "0";
        flux << hex << (unsigned)idata;
    }
}

string cryptoProc::GetHexEncr()const
{
    stringstream convRes;
    toHex(convRes,m_encr);
    return convRes.str();
}

string cryptoProc::bin2Hex(const u8* bin)const
{
    stringstream resConv;
    toHex(resConv,bin);
    return resConv.str();
}

void cryptoProc::fluxOut(ostream& flux)const
{
    flux << string("------Crypto-state------") << endl << "Plain : ";
    toHex(flux,m_plain);
    flux << endl << "Key : ";
    toHex(flux,m_key);
    flux << endl << "Result : ";
    toHex(flux,m_encr);
    flux << endl;
}

void cryptoProc::Setkey(const u8* val){
    if(!m_key)m_key=new u8[keySize()];
    for(unsigned i=0;i<keySize()/8;i++)
        m_key[i]=val[i];
}

void cryptoProc::Setplain(const u8* plain)
{
    if(!m_plain)m_plain=new u8[plainSize()];
    for(unsigned i=0;i<plainSize()/8;i++)
        m_plain[i]=plain[i];
}
