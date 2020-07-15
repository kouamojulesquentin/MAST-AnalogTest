#ifndef CRYPTOPROC_H
#define CRYPTOPROC_H

#include <string>
#include <list>

#define u8 unsigned char
#include "endian.h"

class cryptoProc
{
    public:
        typedef enum{key_e,plain_e,encr_e}CryptoTabs;
        cryptoProc(unsigned endian);
        virtual ~cryptoProc();
        virtual unsigned keySize()const=0;
        virtual unsigned plainSize()const=0;
        virtual unsigned encrSize()const=0;
        virtual void encrypt()=0;


        void addLogStream(std::ostream& flux);
        void toLogStream(const std::string& txt);
        void fluxOut(std::ostream& oFlux)const;
        std::string bin2Hex(const u8* bin)const;
        void hex2bin(const std::string& hex,u8* bin)const;

        ///Setters & getters
        virtual void Setkey(const u8*  val);
        virtual void Setplain(const u8* val);
        bool isBigEndian()const{return m_endian!=LITTLE_ENDIAN;}
        const u8* Getencr()const{return m_encr;}
        std::string GetHexEncr()const;
        virtual void loadHexa(const std::string& val,CryptoTabs dest=plain_e);
    protected:
        void defaultTab();
        void toHex(std::ostream& oflux,const u8 *data)const;

        u8* m_key;
        u8* m_plain;
        u8* m_encr;

    private:
        const unsigned m_endian;
        std::list<std::ostream*> m_logOutputs;
};

#endif // CRYPTOPROC_H
