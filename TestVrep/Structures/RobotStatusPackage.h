#ifndef ROBOTSTATUSPACKAGE_H
#define ROBOTSTATUSPACKAGE_H

#include <string>
#define MAX_JOINTS_VAR 9 // Reference to emcmotcfg.h EMCMOT_MAX_AXIS
namespace Tribots {

class RobotStatusPackage
{
public:
    RobotStatusPackage();
    RobotStatusPackage (const RobotStatusPackage&) noexcept;
    const RobotStatusPackage& operator= (const RobotStatusPackage&) noexcept;
    ~RobotStatusPackage () noexcept;

    std::string serialize () const ;
    bool deserialize (const std::string&) noexcept;

    double  jointPos[MAX_JOINTS_VAR];
    double  cartPos[MAX_JOINTS_VAR];
    double  jointVel[MAX_JOINTS_VAR];
    double  cartVel[MAX_JOINTS_VAR];
    unsigned int   jointError[MAX_JOINTS_VAR];
    unsigned int   cartFig;
    unsigned int   errorCode;
    unsigned int   digitalInput;
    unsigned int   digitalOutput;
    unsigned int   jogCoord;
    unsigned int   robotState;    // free, coordinate
    unsigned int   operationMode;


};

}
#endif // ROBOTSTATUSPACKAGE_H
