#ifndef MICROAES_H
#define MICROAES_H
#include <string>
#include <list>
#include <ostream>

#ifndef u8
#define u8 unsigned char
#endif // u8

#define BLSZ 128
#include "cryptoProc.h"
#include "endian.h"

#define DEFAULT_ENDIAN BIG_ENDIAN
class microAES : public cryptoProc
{
    public:
        microAES(unsigned endian=DEFAULT_ENDIAN);
        virtual ~microAES();

        const u8* Getencr()const;
        std::string GetHexEncr()const;
        void encrypt();

        virtual unsigned plainSize()const{return 128;}
        virtual unsigned keySize()const{return 128;}
        virtual unsigned encrSize()const{return 128;}

        //void loadHexa(const std::string& val,CryptoTabs dest=plain_e);
//        void fluxOut(std::ostream& oFlux)const;
 //       std::string bin2Hex(const u8* bin)const;
  //      void hex2bin(const std::string& hex,u8* bin)const;
//        bool isBigEndian()const{return m_endian!=LITTLE_ENDIAN;}
    protected:
        void toHex(std::ostream& oflux,const u8 *data)const;

    private:
//        u8* m_key;
//        u8* m_plain;
//        u8* m_encr;
 //       const unsigned m_endian;
 //       std::list<std::ostream*> m_logOutputs;
};

//std::ostream& operator<<(const microAES& obj,std::ostream &flux){obj.fluxOut(flux);return flux;}
#endif // MICROAES_H
