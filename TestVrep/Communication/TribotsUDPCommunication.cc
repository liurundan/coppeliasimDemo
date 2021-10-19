
#include "TribotsUDPCommunication.h"
#include "encoding.h"
#include <cstring>

using namespace Tribots;
using namespace std;

// Tags sind wiefolgt belegt (gerade Zahl=Wert, ungerade Zahl=Request).
// Prioritaeten fuer Werte in Klammern; Requests haben stets Prioritaet 0
//
// 0/1: Ping (0)
// 2: RobotLocation (0)
// 4: BallLocation (0)
// 6/7: ObstacleLocation (1)
// 8/9: VisibleObjectList (1)
// 10/11: RobotData (1)
// 12: GameState (0)
// 14: InGame (0)
// 16: RefereeState (0)
// 18: OwnHalf (0)
// 20: PlayerType (0)
// 22: PlayerRole (0)
// 24/25: PlayerTypeList (0)
// 26/27: PlayerRoleList (0)
// 28/29: FieldGeometry (0)
// 30: SLHint (0)
// 32/33: MessageBoard (0)
// 34: Vcc (0)
// 37: StandardRequest (0)
// 38: TeammateLocation (0)
// 40: RobotID (0)
// 42: TacticsBoard (0)
// 45: DebugImageRequest (0)
// 46: RobotLocation & TeammateOccupancyGrid (0)
// 48: Spielstand (0)
// 50: DriveVector (0)
// 52: SL-Spiegelungshinweis (0)
// 53/54: RegionList (0)
// 56: TacticsBoardForce (0)
// 250: ExitRequest (0)
// 252: Synchronisationssignal (0)
// 254: Bye (0)


namespace {
  // um diese beiden Variablen nicht in jeder einzelnen Methode erneut deklarieren zu muessen, hier:
  const char* retbuf;
  unsigned int retlen;
  const char magic_bytes [] = { 15, -6 };
  const char exit_code [] = { 93, -18, -109, -54, 70 };
  const unsigned int exit_code_length = 5;
}


TribotsUDPCommunication::TribotsUDPCommunication ()  : NonspecificTaggedUDPCommunication (magic_bytes) {;}

TribotsUDPCommunication::~TribotsUDPCommunication () noexcept { 
  close(); 
}

void TribotsUDPCommunication::close () noexcept {
//  clear_send_buffer();
//  putBye ();
//  send ();
  NonspecificTaggedUDPCommunication::close ();
}



bool TribotsUDPCommunication::putPing ()  {
  return socket.put (0,NULL,0);
}
bool TribotsUDPCommunication::getPing () noexcept {
  return socket.get (0,retbuf,retlen);
}
bool TribotsUDPCommunication::putPingRequest ()  {
  return socket.put (1,NULL,0);
}
bool TribotsUDPCommunication::getPingRequest () noexcept {
  return socket.get (1,retbuf,retlen);
}

bool TribotsUDPCommunication::putExitRequest ()  {
  return socket.put (250,exit_code,exit_code_length);
}
bool TribotsUDPCommunication::getExitRequest () noexcept {
  bool okay = (socket.get (250,retbuf,retlen)) && (retlen==exit_code_length);
  if (!okay)
    return false;
  for (unsigned int i=0; i<exit_code_length; i++) {
    okay &= (retbuf[i]==exit_code[i]);
  }
  return okay;
}

bool TribotsUDPCommunication::putBye ()  {
  return socket.put (254,NULL,0);
}
bool TribotsUDPCommunication::getBye () noexcept {
  return socket.get (254,retbuf,retlen);
}
 
 

bool TribotsUDPCommunication::putRobotID (unsigned int id)  {
  char buffer[1];
  buffer[0] = static_cast<char>(id);
  return socket.put (40, buffer, 1);
}

bool TribotsUDPCommunication::getRobotID (unsigned int& id) noexcept {
  if (socket.get (40,retbuf,retlen) && retlen>=1) {
    id = static_cast<unsigned int>(retbuf[0]);
    return true;
  }
  return false;
}

 
 

bool TribotsUDPCommunication::putInGame (const bool& src)  {
  char buffer[1];
  buffer[0]=src ? 1 : 0;
  return socket.put (14,buffer,1);
}
bool TribotsUDPCommunication::getInGame (bool& dest) noexcept {
  // 2 Moeglichkeiten: direkt oder im GameState uebertragen
  if (socket.get (14,retbuf,retlen) && retlen>=1) {
    dest = (retbuf[0]==1);
    return true;
  }
  if (socket.get (12,retbuf,retlen) && retlen>=2) {
    dest = (retbuf[1]==1);
    return true;
  }
  return false;
}

 
 

bool TribotsUDPCommunication::putOwnHalf (const int& src)  {
  char buffer [1];
  unsigned int len = encodeOwnHalf (buffer, src);
  return socket.put (18,buffer,len);
}
bool TribotsUDPCommunication::getOwnHalf (int& dest) noexcept {
  return socket.get (18,retbuf,retlen) && decodeOwnHalf (retbuf, retlen, dest);
}

 

bool TribotsUDPCommunication::putFieldGeometry (const FieldGeometry& src)  {
	int bufsize = bufferSizeFieldGeometry(src);
	char *buffer = new char[bufsize];
 /* char buffer [bufferSizeFieldGeometry (src)];*/
  unsigned int len = encodeFieldGeometry (buffer, src);
  bool ret = socket.put(28, buffer, len);
  delete[] buffer;
  return ret;
}
bool TribotsUDPCommunication::getFieldGeometry (FieldGeometry& dest) noexcept {
  return socket.get (28,retbuf,retlen) && decodeFieldGeometry (retbuf, retlen, dest);
}
bool TribotsUDPCommunication::putFieldGeometryRequest ()  {
  return socket.put (29,NULL,0);
}
bool TribotsUDPCommunication::getFieldGeometryRequest () noexcept {
  return socket.get (29,retbuf,retlen);
}

//----------------------------------------------
bool TribotsUDPCommunication::putRobotStatusPackage (const RobotStatusPackage& src)
{
	int bufsize = bufferSizeRobotStatusPackage(src);
	char *buffer = new char[bufsize];
	unsigned int len = encodeRobotStatusPackage(buffer, src);
	bool ret = socket.put(30, buffer, len);
	delete[] buffer;
	return ret;
 
}

bool TribotsUDPCommunication::getRobotStatusPackage (RobotStatusPackage& dest) noexcept
{
   return socket.get (30,retbuf,retlen) && decodeRobotStatusPackage (retbuf, retlen, dest);
}

bool TribotsUDPCommunication::putRobotStatusPackageRequest ()
{
   return socket.put (31,NULL,0);
}

bool TribotsUDPCommunication::getRobotStatusPackageRequest () noexcept
{
    return socket.get (31,retbuf,retlen);
}

bool TribotsUDPCommunication::putVcc (const float& src)  {
  char buffer [1];
  unsigned int len = encodeVcc (buffer, src);
  return socket.put (34,buffer,len);
}
bool TribotsUDPCommunication::getVcc (float& dest) noexcept {
  return socket.get (34,retbuf,retlen) && decodeVcc(retbuf, retlen, dest);
}

bool TribotsUDPCommunication::putStandardRequest ()  {
  return socket.put (37,NULL,0);
}
bool TribotsUDPCommunication::getStandardRequest () noexcept {
  return socket.get (37,retbuf,retlen);
}
 
 

bool TribotsUDPCommunication::putDebugImageRequest ()  {
  return socket.put (45,NULL,0);
}
bool TribotsUDPCommunication::getDebugImageRequest () noexcept {
  return socket.get (45,retbuf,retlen);
}

bool TribotsUDPCommunication::putScore (unsigned int own, unsigned int opponent, unsigned int yellow)  {
  char buffer [2];
  unsigned int len=encodeScore(buffer,own,opponent,yellow);
  return socket.put (48,buffer,len);
}
bool TribotsUDPCommunication::getScore (unsigned int& own, unsigned int& opponent, unsigned int& yellow) noexcept {
  return socket.get (48,retbuf,retlen) && decodeScore (retbuf,retlen,own,opponent,yellow);
}

 
 

 
 


 
