//
//Developed from code by Nickel Liang
//Eric Liang	1/21/2018
//Format are kept close to gimbal code created by Ma Yuan
//

#ifndef IRM2018_LAUNCHER_H
#define IRM2018_LAUNCHER_H

#include "pid.h"
#include "bsp_can.h"

static int launcherRpm;	//Speed of the launcher
static bool launcherReady;	//Whether launcher is ready for steady shots
static bool clockwiseflag;	//Determines if the poke motor spins in clockwise direction or not

void LauncherPIDSetMode(bool clockwise);
void LauncherPIDSet(int rpm);
void LauncherPIDsetwithoutswitch(int rpm);
bool LauncherShoot(void);
bool LauncherReady(void);

#endif	//IRM2018_LAUNCHER_H
