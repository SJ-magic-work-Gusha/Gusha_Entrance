/************************************************************
************************************************************/
#include "L_DesignFunc.h"

/************************************************************
************************************************************/

/************************************************************
************************************************************/

/******************************
******************************/
sjRGBW LED_DESIGN_FUNC::Func_GetLum__Flash(	double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow,
											double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetLum__Flash"; return Col_L;}
	
	double Lum = 0;
	if( b_BeatLock && (BeatInterval != 0) ){
		if( (now - t_LastBeat < 0) || (BeatInterval < now - t_LastBeat) ){
			Lum = 0;
		}else{
			double tan = -1/BeatInterval;
			Lum = tan * (now - t_LastBeat) + 1;
		}
		
		sjRGBW rgbw = Map_sjRGBW(Lum, Col_L, Col_H);
		return rgbw;
		
	}else{
		return Func_GetLum__GainSync(now, NumChs, Ch_id, d_LumInterval_Strobe, d_LumLength_Strobe, d_LumInterval_Flow, d_LumLength_Flow, Amp_GainSync, b_BeatLock, BeatInterval, t_LastBeat, Col_L, Col_H, Name);
	}
}

/******************************
******************************/
sjRGBW LED_DESIGN_FUNC::Func_GetLum__On(	double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow,
											double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetLum__On"; return Col_L;}
	return Col_H;
}

/******************************
******************************/
sjRGBW LED_DESIGN_FUNC::Func_GetLum__On_FullW(	double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow,
												double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetLum__OnW"; return Col_L;}
	
	// sjRGBW Col_FullWhite(1.0, 1.0, 1.0, 1.0);
	sjRGBW Col_FullWhite(0.0, 0.0, 0.0, 1.0);
	
	return Col_FullWhite;
}

/******************************
******************************/
sjRGBW LED_DESIGN_FUNC::Func_GetLum__Stop(	double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow,
											double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetLum__Stop"; return sjRGBW(0, 0, 0, 0);}
	
	sjRGBW rgbw = Map_sjRGBW(0.5, Col_L, Col_H);
	return rgbw;
}

/******************************
private
******************************/
double LED_DESIGN_FUNC::GetLum__Sawtooth(double now, int NumChs, int Ch_id, double d_LumInterval, double d_LumLength)
{
	double d = NumChs * d_LumInterval;
	if(d <= 0) {ERROR_MSG(); std::exit(1);}
	
	double t = fmod(now, d); // double t = now - d * int(now / d);
	double tan = -1.0 / d_LumLength;
	
	double y[2];
	y[0] = tan * (t     - Ch_id * d_LumInterval) + 1.0;
	y[1] = tan * (t + d - Ch_id * d_LumInterval) + 1.0;
	
	for(int i = 0; i < 2; i++){
		if(1 < y[i])		y[i] = 0;
		else if(y[i] < 0)	y[i] = 0;
	}
	
	double y_ret = max(y[0], y[1]);
	
	return y_ret;
}

/******************************
private
******************************/
sjRGBW LED_DESIGN_FUNC::Map_sjRGBW(double ratio, const sjRGBW& Col_L, const sjRGBW& Col_H)
{
	sjRGBW rgbw;
	
	rgbw.r = ofMap(ratio, 0, 1.0, Col_L.r, Col_H.r, true);
	rgbw.g = ofMap(ratio, 0, 1.0, Col_L.g, Col_H.g, true);
	rgbw.b = ofMap(ratio, 0, 1.0, Col_L.b, Col_H.b, true);
	rgbw.w = ofMap(ratio, 0, 1.0, Col_L.w, Col_H.w, true);
	
	return rgbw;
}

/******************************
******************************/
sjRGBW LED_DESIGN_FUNC::Func_GetLum__Flow(	double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow,
											double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetLum__Flow"; return Col_L;}
	
	double Lum = GetLum__Sawtooth(now, NumChs, Ch_id, d_LumInterval_Flow, d_LumLength_Flow);
	sjRGBW rgbw = Map_sjRGBW(Lum, Col_L, Col_H);
	
	return rgbw;
}

/******************************
******************************/
sjRGBW LED_DESIGN_FUNC::Func_GetLum__Strobe_1_W(	double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow,
													double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name)
{
	/********************
	********************/
	if(Name != NULL) {*Name = "Func_GetLum__Strobe_1_W"; return Col_L;}
	
	/********************
	********************/
	if(!b_EnableProcess_CheckLightDesign){
		if(!Gui_Global->FestMode)	return Col_L;
	}
	
	/********************
	********************/
	 double Lum = GetLum__Sawtooth(now, NumChs, Ch_id, d_LumInterval_Strobe, d_LumLength_Strobe);
	// double Lum = GetLum__Sawtooth(now, 1, 0, d_LumInterval_Strobe, d_LumLength_Strobe); // for All ch Strobe.
	
	sjRGBW Col_Black(0, 0, 0, 0);
	sjRGBW Col_White(0, 0, 0, 1.0);
	
	sjRGBW rgbw = Map_sjRGBW(Lum, Col_Black, Col_White);
	
	return rgbw;
}

/******************************
******************************/
sjRGBW LED_DESIGN_FUNC::Func_GetLum__Strobe_1_C(	double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow,
													double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name)
{
	/********************
	********************/
	if(Name != NULL) {*Name = "Func_GetLum__Strobe_1_C"; return Col_L;}
	
	/********************
	********************/
	if(!b_EnableProcess_CheckLightDesign){
		if(!Gui_Global->FestMode)	return Col_L;
	}
	
	/********************
	********************/
	 double Lum = GetLum__Sawtooth(now, NumChs, Ch_id, d_LumInterval_Strobe, d_LumLength_Strobe);
	// double Lum = GetLum__Sawtooth(now, 1, 0, d_LumInterval_Strobe, d_LumLength_Strobe); // for All ch Strobe.
	
	sjRGBW rgbw = Map_sjRGBW(Lum, Col_L, Col_H);
	
	return rgbw;
}

/******************************
******************************/
sjRGBW LED_DESIGN_FUNC::Func_GetLum__GainSync(	double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow,
												double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetLum__GainSync"; return Col_L;}
	
	
	double _Amp_GainSync = Filter_GainSync_Lum(Amp_GainSync);
	sjRGBW rgbw = Map_sjRGBW(_Amp_GainSync, Col_L, Col_H);
	
	return rgbw;
}

/******************************
private
******************************/
double LED_DESIGN_FUNC::Filter_GainSync_Lum(double Amp_GainSync)
{
	/********************
	parabora curve
		http://www.iquilezles.org/www/articles/functions/functions.htm
	********************/
	double _Amp_GainSync;
	double k = 0.15; // small:高域で緩やか.
	
	if(Gui_Global->FestMode)	k = 0.3;
	else						k = 0.15;
	
	_Amp_GainSync = pow( 4.0 * (Amp_GainSync/2) * (1.0 - (Amp_GainSync/2)), k );
	
	return _Amp_GainSync;
	
	/********************
	********************/
	/*
	const double p = 0.15;
	const double a = 1.0/(p * p);
	
	double _Amp_GainSync;
	
	if( (0.5 - p < Amp_GainSync) && (Amp_GainSync < 0.5 + p) )	{ _Amp_GainSync = a * pow((Amp_GainSync - 0.5), 3) + 0.5; }
	else														{ _Amp_GainSync = Amp_GainSync; }
	
	return _Amp_GainSync;
	*/
}

/******************************
private
******************************/
double LED_DESIGN_FUNC::Filter_GainSync_Pos(double Amp_GainSync)
{
	/********************
	********************/
	const double p = 0.15;
	const double a = 1.0/(p * p);
	
	double _Amp_GainSync;
	
	if( (0.5 - p < Amp_GainSync) && (Amp_GainSync < 0.5 + p) )	{ _Amp_GainSync = a * pow((Amp_GainSync - 0.5), 3) + 0.5; }
	else														{ _Amp_GainSync = Amp_GainSync; }
	
	return _Amp_GainSync;
}

/******************************
private
******************************/
double LED_DESIGN_FUNC::GetPos__Triangle(double t, double T, double phase)
{
	double Amp = 1.0;
	double ret =	Amp * 8 / pow(PI, 2) * 
					(
						1.0			* sin(2 * PI * (t - phase) / T)
					-	1.0 / 9		* sin(2 * PI * 3 * (t - phase) / T)
					+	1.0 / 25	* sin(2 * PI * 5 * (t - phase) / T)
					-	1.0 / 49	* sin(2 * PI * 7 * (t - phase) / T)
					+	1.0 / 81	* sin(2 * PI * 9 * (t - phase) / T)
					-	1.0 / 121	* sin(2 * PI * 11 * (t - phase) / T)
					);
	
	return ret;
}

/******************************
******************************/
sjPanTilt LED_DESIGN_FUNC::Func_GetPos__Center(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetPos__Center"; return sjPanTilt(0, 0);}
	
	sjPanTilt PanTilt;
	PanTilt.Pan		= 0;
	PanTilt.Tilt	= 0;
	
	return PanTilt;
}

/******************************
******************************/
sjPanTilt LED_DESIGN_FUNC::Func_GetPos__Stop(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetPos__Stop"; return sjPanTilt(0, 0);}
	
	sjPanTilt PanTilt;
	PanTilt.Pan		= 0;
	PanTilt.Tilt	= 0;
	
	return PanTilt;
}

/******************************
******************************/
sjPanTilt LED_DESIGN_FUNC::Func_GetPos__Pan_GainSync(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetPos__GainSync"; return sjPanTilt(0, 0);}
	
	double _Amp_GainSync = Filter_GainSync_Pos(Amp_GainSync);
	
	sjPanTilt PanTilt;
	PanTilt.Pan		= ofMap(_Amp_GainSync, 0, 1, -1, 1, true);
	PanTilt.Tilt	= 0;
	
	return PanTilt;
}

/******************************
******************************/
sjPanTilt LED_DESIGN_FUNC::Func_GetPos__Circle_L(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetPos__Circle_L"; return sjPanTilt(0, 0);}
	
	double T = d_interval;
	double t = fmod(now, T);
	double phase_step = double(360) / double(NumChs);
	double phase = T * phase_step / 360 * Ch_id;
	double phase_90 = T * 90 / 360;
	
	sjPanTilt PanTilt;
	PanTilt.Pan		= GetPos__Triangle(t, T, 0);
	PanTilt.Tilt	= GetPos__Triangle(t, T, phase_90);
	
	return PanTilt;
}

/******************************
******************************/
sjPanTilt LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetPos__Circle_L_PhaseDiff"; return sjPanTilt(0, 0);}
	
	double T = d_interval;
	double t = fmod(now, T);
	double phase_step = double(360) / double(NumChs);
	double phase = T * phase_step / 360 * Ch_id;
	double phase_90 = T * 90 / 360;
	
	sjPanTilt PanTilt;
	PanTilt.Pan		= GetPos__Triangle(t, T, phase);
	PanTilt.Tilt	= GetPos__Triangle(t, T, phase + phase_90);
	
	return PanTilt;
}

/******************************
******************************/
sjPanTilt LED_DESIGN_FUNC::Func_GetPos__Circle_R(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetPos__Circle_R"; return sjPanTilt(0, 0);}
	
	double T = d_interval;
	double t = fmod(now, T);
	double phase_step = double(360) / double(NumChs);
	double phase = T * phase_step / 360 * Ch_id;
	double phase_90 = T * 90 / 360;
	
	sjPanTilt PanTilt;
	PanTilt.Pan		= GetPos__Triangle(t, T, 0);
	PanTilt.Tilt	= -GetPos__Triangle(t, T, phase_90);
	
	return PanTilt;
}

/******************************
******************************/
sjPanTilt LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetPos__Circle_R_PhaseDiff"; return sjPanTilt(0, 0);}
	
	double T = d_interval;
	double t = fmod(now, T);
	double phase_step = double(360) / double(NumChs);
	double phase = T * phase_step / 360 * Ch_id;
	double phase_90 = T * 90 / 360;
	
	sjPanTilt PanTilt;
	PanTilt.Pan		= GetPos__Triangle(t, T, phase);
	PanTilt.Tilt	= -GetPos__Triangle(t, T, phase + phase_90);
	
	return PanTilt;
}

/******************************
******************************/
sjPanTilt LED_DESIGN_FUNC::Func_GetPos__Pan(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetPos__Pan"; return sjPanTilt(0, 0);}
	
	double T = d_interval;
	double t = fmod(now, T);
	double phase_step = double(360) / double(NumChs);
	double phase = T * phase_step / 360 * Ch_id;
	double phase_90 = T * 90 / 360;
	
	sjPanTilt PanTilt;
	PanTilt.Pan		= GetPos__Triangle(t, T, 0);
	PanTilt.Tilt	= 0;
	
	return PanTilt;
}

/******************************
******************************/
sjPanTilt LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetPos__Pan_PhaseDiff"; return sjPanTilt(0, 0);}
	
	double T = d_interval;
	double t = fmod(now, T);
	double phase_step = double(360) / double(NumChs);
	double phase = T * phase_step / 360 * Ch_id;
	double phase_90 = T * 90 / 360;
	
	sjPanTilt PanTilt;
	PanTilt.Pan		= GetPos__Triangle(t, T, phase);
	PanTilt.Tilt	= 0;
	
	return PanTilt;
}

/******************************
******************************/
sjPanTilt LED_DESIGN_FUNC::Func_GetPos__Tilt(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetPos__Tilt"; return sjPanTilt(0, 0);}
	
	double T = d_interval;
	double t = fmod(now, T);
	double phase_step = double(360) / double(NumChs);
	double phase = T * phase_step / 360 * Ch_id;
	double phase_90 = T * 90 / 360;
	
	sjPanTilt PanTilt;
	PanTilt.Pan		= 0;
	PanTilt.Tilt	= GetPos__Triangle(t, T, phase_90);
	
	return PanTilt;
}

/******************************
******************************/
sjPanTilt LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name)
{
	if(Name != NULL) {*Name = "Func_GetPos__Tilt_PhaseDiff"; return sjPanTilt(0, 0);}
	
	double T = d_interval;
	double t = fmod(now, T);
	double phase_step = double(360) / double(NumChs);
	double phase = T * phase_step / 360 * Ch_id;
	double phase_90 = T * 90 / 360;
	
	sjPanTilt PanTilt;
	PanTilt.Pan		= 0;
	PanTilt.Tilt	= GetPos__Triangle(t, T, phase + phase_90);
	
	return PanTilt;
}

/******************************
******************************/
void LED_DESIGN_FUNC::FixIdOrder_Forward(int* Array, int Num, string* Name)
{
	if(Name != NULL) {*Name = "FixIdOrder_Forward"; return;}
	
	L_UTIL::Array_Forward(Array, Num);
}

/******************************
******************************/
void LED_DESIGN_FUNC::FixIdOrder_Reverse(int* Array, int Num, string* Name)
{
	if(Name != NULL) {*Name = "FixIdOrder_Reverse"; return;}
	
	L_UTIL::Array_Reverse(Array, Num);
}

/******************************
******************************/
void LED_DESIGN_FUNC::FixIdOrder_Random(int* Array, int Num, string* Name)
{
	/********************
	********************/
	if(Name != NULL) {*Name = "FixIdOrder_Random"; return;}
	
	L_UTIL::FisherYates(Array, Num);
}
