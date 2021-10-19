
#ifndef Tribots_TribotsUDPCommunication_h
#define Tribots_TribotsUDPCommunication_h

#include "NonspecificTaggedUDPCommunication.h"
 
#include "../Structures/FieldGeometry.h"
#include "../Structures/RobotStatusPackage.h"

#include <string>
#include <vector>


namespace Tribots {

  /** UDP-Verbindung, abgestimmt auf die Datenstrukturen die zwischen
      teamcontrol und robotcontrol zu verschicken sind */
  class TribotsUDPCommunication : public NonspecificTaggedUDPCommunication {
  public:
    /** Konstruktor */
    TribotsUDPCommunication () ;
    ~TribotsUDPCommunication () noexcept;
    /** 'Bye' senden und Verbindung schliessen */
    void close () noexcept;

    // Methoden zum lesen/schreiben/anfordern/Anforderung abfragen bestimmter Informationen
    // Return: true, wenn Information geschrieben werden konnte/empfangen wurde
    virtual bool putPing () ;
    virtual bool getPing () noexcept;
    virtual bool putPingRequest () ;
    virtual bool getPingRequest () noexcept;

    virtual bool putBye () ;
    virtual bool getBye () noexcept;

    virtual bool putExitRequest () ;
    virtual bool getExitRequest () noexcept;


    virtual bool putRobotID (unsigned int) ;
    virtual bool getRobotID (unsigned int&) noexcept;


    virtual bool putInGame (const bool&) ;
    virtual bool getInGame (bool&) noexcept;


    virtual bool putOwnHalf (const int&) ;
    virtual bool getOwnHalf (int&) noexcept;


    virtual bool putFieldGeometry (const FieldGeometry&) ;
    virtual bool getFieldGeometry (FieldGeometry&) noexcept;
    virtual bool putFieldGeometryRequest () ;
    virtual bool getFieldGeometryRequest () noexcept;

    virtual bool putRobotStatusPackage (const RobotStatusPackage&) ;
    virtual bool getRobotStatusPackage (RobotStatusPackage&) noexcept;
    virtual bool putRobotStatusPackageRequest () ;
    virtual bool getRobotStatusPackageRequest () noexcept;

    virtual bool putVcc (const float&) ;
    virtual bool getVcc (float&) noexcept;


    virtual bool putStandardRequest () ;  // anfragen nach: RobotLocation, BallLocation, ownHalf, inGame, RefereeState, PlayerType, PlayerRole, Vcc
    virtual bool getStandardRequest () noexcept;

    virtual bool putDebugImageRequest () ;
    virtual bool getDebugImageRequest () noexcept;

    virtual bool putScore (unsigned int, unsigned int, unsigned int) ;  // Spielstand eigeneTore, gegnerischeTore, Anzahl gelbe Karten (gelbe Karten noch nicht implementiert)
    virtual bool getScore (unsigned int&, unsigned int&, unsigned int&) noexcept;


  };

}

#endif
