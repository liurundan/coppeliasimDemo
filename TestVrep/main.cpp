#include <stdio.h>
#include <fstream>
#include <iostream>
#include "Communication/TribotsUDPCommunication.h"
#include <Windows.h>
#include <conio.h>
#define _USE_MATH_DEFINES
#include <math.h>

extern "C" {
# include "extApi.h"
# include "extApiPlatform.h"
}

#define DEG2RAD(t)  (t*(M_PI/180))

using namespace std;
using namespace Tribots;
#pragma warning(disable:4996)

 
int main()
{
	//////////////////////////////////////////////////////////////////////////
	TribotsUDPCommunication comm;
	RobotStatusPackage robotStatus;
	bool b = comm.init_as_server(17000);
	if (!b)
	{
		printf("init simulate data receive server failed.\n");
		return 0;
	}	

	int Port = 19999;
	simxInt ret = 0;
	const simxChar* Adresse = "127.0.0.1";
 
	double poscmd[6] = { 0 };
	
	int joint_handle[6] = { 0 };

	int clientID = simxStart(Adresse, Port, true, true, 2000, 5);
	simxFloat fpos;
	if (clientID != -1)
	{
		printf("V-rep connected.");
		extApi_sleepMs(300);

		char strJointName[32];
		for (int i = 0; i < 6; i++)
		{
			sprintf(strJointName, "%s%d", "triv_joint", i);
			simxGetObjectHandle(clientID, strJointName, &joint_handle[i], simx_opmode_oneshot_wait);
		}
		char cKey;
		float fstep = 0.02;
		while (simxGetConnectionId(clientID) != -1)
		{
			// IRB140_manipulationSphere ABB IRB 360
		//	simxGetObjectHandle(clientID , "IRB140_manipulationSphere", &PositionControlHandle, simx_opmode_oneshot);
		//	simxGetObjectPosition(clientID, PositionControlHandle, -1, position, simx_opmode_oneshot);
		//	printf("(%f,%f,%f)\r\n", position[0], position[1], position[2]);
			if ( _kbhit() ) 
			{
				cKey = _getch();
				simxPauseCommunication(clientID, 1);
				int index = cKey - '1';
				if ( index < 0 || index > 7 )
				{
					continue;
				}
				int axisindex = (int)(index/2);
				int dir       = (index % 2) == 1 ? -1 : 1 ;

				simxGetJointPosition(clientID, joint_handle[axisindex], (simxFloat*)(&robotStatus.jointPos[axisindex]), simx_opmode_streaming);
				if ( axisindex < 3 )
				{
					robotStatus.jointPos[axisindex] += dir * fstep;
				}
				else
				{			 
					robotStatus.jointPos[3] = (dir == 1) ? -M_PI / 2 : 0;
				}
				simxSetJointPosition(clientID, joint_handle[axisindex], robotStatus.jointPos[axisindex], simx_opmode_streaming);
				simxPauseCommunication(clientID, 0);

				printf("jointpos = %.3f %.3f %.3f %.3f\n",
					robotStatus.jointPos[0], robotStatus.jointPos[1], robotStatus.jointPos[2], robotStatus.jointPos[3]);
 
			}
			else
			{
				comm.receive();
				if (comm.getRobotStatusPackage(robotStatus))
				{
					printf("jointpos = %.3f %.3f %.3f %.3f\n",
						robotStatus.jointPos[0], robotStatus.jointPos[1], robotStatus.jointPos[2], robotStatus.jointPos[3]);

					simxPauseCommunication(clientID, 1);
					for (int i = 0; i < 4; i++)
					{
						if (i == 3)
						{
							ret = simxSetJointPosition(clientID, joint_handle[i], robotStatus.jointPos[i], simx_opmode_streaming);
						}
						else
						{
							ret = simxSetJointPosition(clientID, joint_handle[i], (robotStatus.jointPos[i] * 0.001), simx_opmode_streaming);
						}

					}
					simxPauseCommunication(clientID, 0);
				}

				comm.send();
				Sleep(100);
			}


			
		}

		simxFinish(clientID);

		comm.close();

	}
	else 
	{
		printf("V-rep can't be connected.");
		extApi_sleepMs(300);
	}
	system("pause");

	return 0;
}