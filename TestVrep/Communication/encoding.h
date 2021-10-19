
#ifndef _Tribots_encoding_h_
#define _Tribots_encoding_h_


#include "../Structures/FieldGeometry.h"
#include "../Structures/RobotStatusPackage.h"

#include <vector>
#include <string>

namespace Tribots {

  // Funktionen, um Strukturen fuer die Kommunikation geeignet zu codieren.
  // bufferSizeXXX (x) liefert die Puffergroesse zurueck, die benoetigt wird, um x zu codieren
  // encodeXXX (buffer, x) schreibt x nach buffer. buffer muss vorher angelegt und ausreichend gross sein. Liefert die benoetigte Puffergroesse
  // decodeXXX (buffer, bufferlen, x) liest x aus buffer aus und liefert true bei Erfolg. bufferlen ist die Groesse von buffer


  unsigned int bufferSizeOwnHalf (const int&) throw ();
  unsigned int encodeOwnHalf (char*, const int&) throw ();
  bool decodeOwnHalf (const char*, unsigned int, int&) throw ();

  unsigned int bufferSizeStringList (const std::vector<std::string>&) throw ();
  unsigned int encodeStringList (char*, const std::vector<std::string>&) throw ();
  bool decodeStringList (const char*, unsigned int, std::vector<std::string>&) throw ();

  unsigned int bufferSizeFieldGeometry (const FieldGeometry&) throw ();
  unsigned int encodeFieldGeometry (char*, const FieldGeometry&) throw ();
  bool decodeFieldGeometry (const char*, unsigned int, FieldGeometry&) throw ();

//-----------------------------------------------
  unsigned int bufferSizeRobotStatusPackage (const RobotStatusPackage&) noexcept;
  unsigned int encodeRobotStatusPackage (char*, const RobotStatusPackage&) noexcept;
  bool decodeRobotStatusPackage (const char*, unsigned int, RobotStatusPackage&) noexcept;
//---------------------------------------------------------
  
  unsigned int bufferSizeVcc (const float&) throw ();
  unsigned int encodeVcc (char*, const float&) throw ();
  bool decodeVcc (const char*, unsigned int, float&) throw ();


  unsigned int bufferSizeScore (unsigned int, unsigned int, unsigned int) throw (); // own score, opponent score, yellow cards
  unsigned int encodeScore (char*, unsigned int, unsigned int, unsigned int) throw ();
  bool decodeScore (const char*, unsigned int, unsigned int&, unsigned int&, unsigned int&) throw ();

}


#endif
