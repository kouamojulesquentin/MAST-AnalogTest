#include "microAES.h"
#include <stdexcept>
#include "usefullFunc.h"
#include <ostream>
#include <sstream>
#include "aes.h"

using namespace std;

microAES::microAES(unsigned endian):cryptoProc(endian)
{

//    if(m_endian &(!0x1))throw logic_error("Endian type unknown");
    m_key=new u8[BLSZ/8];
    m_plain=new u8[BLSZ/8];
    m_encr=new u8[BLSZ/8];
}

microAES::~microAES()
{
    delete[] m_key;
    delete[] m_plain;
    delete[] m_encr;
}


const u8* microAES::Getencr()const{
    if(!m_encr)throw logic_error("Resultat non calculé");
    return m_encr;
}

/*
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
void microAES::loadHexa(const string& val,microAES::AesTabs dest)
{
    if(val.size()!=BLSZ/4)throw range_error("Hex vector '"+val+"' has "+i2s(val.size()*4)+" bits instead of "+i2s(BLSZ));
    u8* p_dest;
    switch(dest)
    {
        case plain_e:
            p_dest=m_plain;
            break;
        case key_e:
            p_dest=m_key;
            break;
        case encr_e:
            throw logic_error("Can't load hexa into result memory");
    }
    if(isBigEndian())
        for(unsigned i=0;i<BLSZ/8;i++)
            p_dest[i]=hexPair(val[2*i],val[2*i+1]);
    else
        for(unsigned i=0;i<BLSZ/8;i++)
            p_dest[BLSZ/8-1-i]=hexPair(val[2*i],val[2*i+1]);
}

void microAES::fluxOut(ostream& flux)const
{
    flux << string("------AES-state------") << endl << "Plain : ";
    toHex(flux,m_plain);
    flux << endl << "Key : ";
    toHex(flux,m_key);
    flux << endl << "Result : ";
    toHex(flux,m_encr);
    flux << endl;
}*/

void microAES::encrypt()
{
    stringstream logs;
    AES_struct aes_state;
    AES_init_ctx(&aes_state,m_key);
    for(unsigned i=0;i<16;i++)
        m_encr[i]=m_plain[i];
    AES_ECB_encrypt(&aes_state,m_encr);

    logs << "-----------AES--LOG-----------"<< endl << "Plain\t" << bin2Hex(m_plain);
    logs << endl << "Key \t" << bin2Hex(m_key) << endl << "Encr\t" << bin2Hex(m_encr);
    logs << endl << "---------END-AES-Log----------" << endl << endl;
    toLogStream(logs.str());
}


