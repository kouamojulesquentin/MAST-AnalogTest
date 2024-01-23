#ifndef SSAKPLUGIN_H
#define SSAKPLUGIN_H
#include "cryptoProc.h"

#include <vector>

#ifndef u8
#define u8 unsigned char
#endif // u8

#define SSAK_CONNECTION_WATCHDOG 5

class SSAKplugin
{
    public:
        SSAKplugin(cryptoProc& CP,unsigned nbS2IB,unsigned interfaceSize=0);
        virtual ~SSAKplugin();
//        void Setkey(u8* val) { m_key = val; }
        virtual bool select(const u8* config,const u8* key);
	virtual const u8* computeResponse(const u8* key,const u8* challenge);
        virtual void deselect();
//        virtual const u8* isSelected(){return m_Selected;}
        virtual bool isSelected()const; //TRUE if any S²IB is unlocked
        virtual bool isSelected(unsigned iS2IB)const; //True if the S²IB number iS²IB is unlocked

   unsigned getInterfaceSize() {return m_interfaceSize;}
   
    protected:
      void setSelected(const u8* configVector);
    private:
        std::vector<u8> m_Selected;

        unsigned m_interfaceSize;
        unsigned m_nbS2IB;

        cryptoProc& m_CP;

//        u8* m_key;
};

#endif // SSAKPLUGIN_H
