
#include "encoding.h"
#include "NonspecificTaggedUDPCommunication.h"
#include "cstring"
using namespace Tribots;
using namespace std;

 

unsigned int Tribots::bufferSizeOwnHalf (const int&) throw () {
  return 1;
}
unsigned int Tribots::encodeOwnHalf (char* buffer, const int& src) throw () {
  buffer[0]=src;
  return 1;
}
bool Tribots::decodeOwnHalf (const char* retbuf, unsigned int retlen, int& dest) throw () {
  if (retlen<1)
    return false;
  dest = retbuf[0];
  return true;
}

unsigned int Tribots::bufferSizeStringList (const std::vector<std::string>& src) throw () {
  unsigned int bufsize = 0;
  for (unsigned int i=0; i<src.size(); i++)
    bufsize+=src[i].length()+1;
  return bufsize;
}
unsigned int Tribots::encodeStringList (char* buffer, const std::vector<std::string>& src) throw () {
  unsigned int len=0;
  char* pt = buffer;
  for (unsigned int i=0; i<src.size(); i++) {
    memcpy (pt, src[i].c_str(), src[i].length());
    pt+=src[i].length();
    (*pt) = '\n';
    pt++;
    len+=src[i].length()+1;
  }
  return len;
}
bool Tribots::decodeStringList (const char* retbuf, unsigned int retlen, std::vector<std::string>& dest) throw () {
  const char* pt=retbuf;
  const char* pt2=retbuf;
  while (pt2<retbuf+retlen) {
    if ((*pt2)=='\n') {
      string newstring (pt, pt2-pt);
      dest.push_back (newstring);
      pt=pt2=pt2+1;
    } else
      pt2++;
  }
  return true;
}

unsigned int Tribots::bufferSizeFieldGeometry (const FieldGeometry& src) throw () {
  return src.serialize().length();
}
unsigned int Tribots::encodeFieldGeometry (char* buffer, const FieldGeometry& src) throw () {
  string s = src.serialize();
  std::strncpy (buffer, s.c_str(), s.length());
  return s.length();
}
bool Tribots::decodeFieldGeometry (const char* retbuf, unsigned int retlen, FieldGeometry& dest) throw () {
  string s (retbuf, retlen);
  return dest.deserialize (s);
}

//--------------------------------------
unsigned int Tribots::bufferSizeRobotStatusPackage (const RobotStatusPackage& rsp) noexcept
{
    return rsp.serialize().length();
}

unsigned int Tribots::encodeRobotStatusPackage (char* buffer, const RobotStatusPackage& src) noexcept
{
    string s = src.serialize();
    std::strncpy (buffer, s.c_str(), s.length());
    return s.length();
}

bool Tribots::decodeRobotStatusPackage (const char* retbuf, unsigned int retlen, RobotStatusPackage& dest) noexcept
{
    string s (retbuf, retlen);
    return dest.deserialize (s);
}

//---------------------------------------
unsigned int Tribots::bufferSizeVcc (const float&) throw () {
  return 1;
}
unsigned int Tribots::encodeVcc (char* buffer, const float& src) throw () {
  double src2=src;
  buffer[0]=static_cast<char>(static_cast<unsigned char>(5*src2));
  return 1;
}
bool Tribots::decodeVcc (const char* retbuf, unsigned int retlen, float& dest) throw () {
  if (retlen<1)
    return false;
  dest = 0.2*static_cast<unsigned char>(retbuf[0]);
  return true;
}

unsigned int Tribots::bufferSizeScore (unsigned int, unsigned int, unsigned int) throw () {
  return 2;
}
unsigned int Tribots::encodeScore (char* buffer, unsigned int own, unsigned int opponent, unsigned int yellow) throw () {
  if (own>31) own=31;
  if (opponent>31) opponent=31;
  if (yellow>7) yellow=7;
  unsigned int msg = (own<<11)+(opponent<<6)+(yellow<<3);  // Bit 11..15: eigeneTore, Bit 6..10: gegnerTore, Bit 3..5: gelbe Karten, Bit 0..2: frei
  buffer[0] = static_cast<char>(static_cast<unsigned char>(msg>>8));
  buffer[1] = static_cast<char>(static_cast<unsigned char>(msg&255));
  return 2;
}
bool Tribots::decodeScore (const char* retbuf, unsigned int retlen, unsigned int& own, unsigned int& opponent, unsigned int& yellow) throw () {
  if (retlen<2)
    return false;
  unsigned int msg = (static_cast<unsigned short int>(static_cast<unsigned char>(retbuf[0]))<<8)+static_cast<unsigned short int>(static_cast<unsigned char>(retbuf[1]));
  own = (msg>>11);
  opponent = ((msg>>6)&31);
  yellow = ((msg>>3)&7);
  return true;
}
