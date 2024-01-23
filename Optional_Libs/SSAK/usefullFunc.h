#ifndef USEFULLFUNC_H
#define USEFULLFUNC_H

#include <string>

#define l2s(a) std::string({a})
std::string i2s(int nb);
int s2i(const std::string &txt);
std::string lcase(const std::string& str);
bool inStr(const std::string& entree,const std::string& liste);
bool inStr(char letter,const std::string &liste);
std::string toBin(unsigned value,int nbBit=-1);
std::string toHex(char octet);
std::string translateIclVector(const std::string& expr);
std::string repetString(const std::string& str,unsigned nb);
#endif // USEFULLFUNC_H
