#include "usefullFunc.h"
#include <sstream>
#include <cmath>

using namespace std;

int s2i(const string& txt)
{
    stringstream conv;
    int nb;
    conv << txt;
    conv >> nb;
    return nb;
}

string i2s(int nb)
{
    stringstream conv;
    conv << nb;
    return conv.str();
}

std::string lcase(const std::string& str)
{
    string minuscules=str;
    for(char& c :minuscules)
        if(c>='A' && c<='Z') c+=32;
    return minuscules;
}

bool inStr(const string& entree,const string& liste)
{
    if(liste.find(entree)==string::npos)return false;
    return true;
}


string toBin(unsigned value,int nbBit)
{
    string binVal;
    if(-1==nbBit)nbBit=ceil(log2(value));
    for(int i=0;i<nbBit;i++)
        if(value&(1<<i)) binVal="1"+binVal;
        else binVal="0"+binVal;
    return binVal;
}


string translateIclVector(const string& expr)
{
    string nbBits;
    char vecType='\0';
    string strVecType;
    string value;
    unsigned mode=0;
    for(unsigned i=0;i<expr.size();i++)
    {
        if(mode==2)value+=expr[i];
        if(mode==1)
        {
            vecType=expr[i];
            mode++;
        }
        if(expr[i]=='\'')mode++;
        if(mode==0)nbBits+=expr[i];
    }
    strVecType=vecType;
    unsigned unbits=s2i(nbBits);

    switch(vecType)
    {
        default:
            throw string("No conversion for ")+strVecType;
            break;
        case 'b':
        case 'B':
            while(value.size()>unbits)value="0"+value;
            return "\""+value+"\"";
            break;
    }

    return string("");
}

string repetString(const string& str,unsigned nb)
{
    string retVal;
    for(unsigned i=0;i<nb;i++)
        retVal+=str;
    return retVal;
}

string toHex(char octet)
{
    static const string hex("0123456789abcdef");
    unsigned lsb,msb;
    lsb=octet&0xf;
    msb=(octet>>4)&0xf;
    return hex.substr(msb,1)+hex.substr(lsb,1);
}

bool inStr(char letter,const string &liste)
{
    for(const char &  c : liste)
        if(c==letter)
            return true;
    return false;
}
