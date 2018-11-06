/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "sj_common.h"
#include "L_common.h"


/************************************************************
************************************************************/

/**************************************************
**************************************************/
class LED_DESIGN_FUNC : private Noncopyable{
private:
	static sjRGBW Map_sjRGBW(double ratio, const sjRGBW& Col_L, const sjRGBW& Col_H);
	static double GetLum__Sawtooth(double now, int NumChs, int Ch_id, double d_LumInterval, double d_LumLength);
	
	static double GetPos__Triangle(double t, double T, double phase);
	static double Filter_GainSync_Lum(double Amp_GainSync);
	static double Filter_GainSync_Pos(double Amp_GainSync);
	
public:
	static sjRGBW Func_GetLum__Stop(double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow, double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name);
	static sjRGBW Func_GetLum__On(double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow, double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name);
	static sjRGBW Func_GetLum__On_FullW(double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow, double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name);
	static sjRGBW Func_GetLum__Flow(double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow, double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name);
	static sjRGBW Func_GetLum__Flash(double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow, double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name);
	static sjRGBW Func_GetLum__GainSync(double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow, double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name);
	static sjRGBW Func_GetLum__Strobe_1_W(double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow, double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name);
	static sjRGBW Func_GetLum__Strobe_1_C(double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow, double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name);

	static sjPanTilt Func_GetPos__Stop(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
	static sjPanTilt Func_GetPos__Center(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
	static sjPanTilt Func_GetPos__Circle_L(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
	static sjPanTilt Func_GetPos__Circle_L_PhaseDiff(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
	static sjPanTilt Func_GetPos__Circle_R(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
	static sjPanTilt Func_GetPos__Circle_R_PhaseDiff(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
	static sjPanTilt Func_GetPos__Pan(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
	static sjPanTilt Func_GetPos__Pan_PhaseDiff(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
	static sjPanTilt Func_GetPos__Tilt(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
	static sjPanTilt Func_GetPos__Tilt_PhaseDiff(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
	static sjPanTilt Func_GetPos__Pan_GainSync(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
	
	static void FixIdOrder_Forward(int* Array, int Num, string* Name);
	static void FixIdOrder_Reverse(int* Array, int Num, string* Name);
	static void FixIdOrder_Random(int* Array, int Num, string* Name);
	
};