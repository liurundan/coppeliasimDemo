#include "RobotStatusPackage.h"
#include "../Fundamental/stringconvert.h"
#include <cmath>
#include <sstream>

using Tribots::RobotStatusPackage;
using namespace std;

RobotStatusPackage::RobotStatusPackage()
{
    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        jointPos[i] = jointVel[i] = cartPos[i] = cartVel[i] = 0.0;
        jointError[i] = 0;
    }

    cartFig = 0;
    errorCode = 0;
    digitalInput = 0;
    digitalOutput = 0;
    jogCoord = 0;
    robotState = 0;    // free, coordinate
    operationMode = 0;
}

RobotStatusPackage::RobotStatusPackage (const RobotStatusPackage& rsp) noexcept
{
    operator= (rsp);
}

const RobotStatusPackage& RobotStatusPackage::operator= (const RobotStatusPackage& rsp) noexcept
{
    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        jointPos[i] = rsp.jointPos[i];
        jointVel[i] = rsp.jointVel[i];
        cartPos[i] = rsp.cartPos[i];
        cartVel[i] = rsp.cartVel[i];
        jointError[i] = rsp.jointError[i];
    }

    cartFig = rsp.cartFig;
    errorCode = rsp.errorCode;
    digitalInput = rsp.digitalInput;
    digitalOutput = rsp.digitalOutput;
    jogCoord = rsp.jogCoord;
    robotState = rsp.robotState;    // free, coordinate
    operationMode = rsp.operationMode;
    return *this;
}

RobotStatusPackage::~RobotStatusPackage () noexcept
{

}

std::string RobotStatusPackage::serialize () const
{
    stringstream inout;
    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        inout << jointPos[i] << ' ';
    }
    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        inout << jointVel[i] << ' ';
    }
    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        inout << cartPos[i] << ' ';
    }
    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        inout << cartVel[i] << ' ';
    }
    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        inout << jointError[i] << ' ';
    }

    inout << cartFig << ' '
          << errorCode << ' '
          << digitalInput << ' '
          << digitalOutput << ' '
          << jogCoord << ' '
          << robotState << ' '
          << operationMode << ' ';

    inout << " |" << endl;

    string result;
    getline (inout, result);
    return result;
}

bool RobotStatusPackage::deserialize (const std::string& s) noexcept
{
    RobotStatusPackage fg;
    std::vector<std::string> parts;

    Tribots::split_string (parts, s); // size 53

    if (parts.size()<52)
      return false;

    std::vector<std::string>::iterator strpart_it = parts.begin();
    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        string2double ( fg.jointPos[i], *(strpart_it++) );
    }

    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        string2double ( fg.jointVel[i], *(strpart_it++) );
    }

    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        string2double ( fg.cartPos[i], *(strpart_it++) );
    }

    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        string2double ( fg.cartVel[i], *(strpart_it++) );
    }

    for(int i = 0; i < MAX_JOINTS_VAR; ++i)
    {
        string2uint ( fg.jointError[i], *(strpart_it++) );
    }

    string2uint( fg.cartFig,        *(strpart_it++) );
    string2uint( fg.errorCode,      *(strpart_it++) );
    string2uint( fg.digitalInput,   *(strpart_it++) );
    string2uint( fg.digitalOutput,  *(strpart_it++) );
    string2uint( fg.jogCoord,       *(strpart_it++) );
    string2uint( fg.robotState,     *(strpart_it++) );
    string2uint( fg.operationMode,  *(strpart_it++) );

    operator= (fg);

    return true;
}

//int main(int argc, char *argv[])
//{
//    RobotStatusPackage rp;
//    for( int i = 0; i < MAX_JOINTS_VAR ; ++i )
//    {
//        rp.jointPos[i] = rp.jointVel[i] = rp.cartPos[i] = rp.cartVel[i] = 123.0456;
//    }
//    rp.cartFig = 5;
//    rp.errorCode = 12;
//    rp.digitalInput = 909;
//    rp.digitalOutput = 123;
//    rp.jogCoord = 3;
//    rp.robotState = 4;
//    rp.operationMode = 7;
//    string result = rp.serialize();

//    RobotStatusPackage rerp;
//    rerp.deserialize(result);

//    return 0;
//}

