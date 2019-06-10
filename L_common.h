/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "sj_common.h"

/************************************************************
************************************************************/
enum{
	NUM_STATES = 5,
};

/************************************************************
************************************************************/

struct sjRGBW{
	double r;
	double g;
	double b;
	double w;
	
	sjRGBW()
	: r(0), g(0), b(0), w(0)
	{
	}
	
	sjRGBW(double _r, double _g, double _b, double _w)
	: r(_r), g(_g), b(_b), w(_w)
	{
	}
	
	void clear(){
		r = 0; g = 0; b = 0; w = 0;
	}
	
	void checkLimit()
	{
		if(r < 0)	r = 0;
		if(1.0 < r) r = 1.0;

		if(g < 0)	g = 0;
		if(1.0 < g) g = 1.0;

		if(b < 0)	b = 0;
		if(1.0 < b) b = 1.0;

		if(w < 0)	w = 0;
		if(1.0 < w) w = 1.0;
	}
	
	sjRGBW& operator+=(const sjRGBW& val)
	{
		r += val.r;
		g += val.g;
		b += val.b;
		w += val.w;
		
		checkLimit();
		
		return *this;
	}
	
	sjRGBW& operator*=(double val)
	{
		r *= val;
		g *= val;
		b *= val;
		w *= val;
		
		checkLimit();
		
		return *this;
	}
 };

struct sjPanTilt{
	double Pan;		// -1 - 1
	double Tilt;
	
	sjPanTilt()
	: Pan(0), Tilt(0)
	{
	}
	
	sjPanTilt(double _Pan, double _Tilt)
	: Pan(_Pan), Tilt(_Tilt)
	{
	}
	
	void clear(){
		Pan = 0; Tilt = 0;
	}
};

class L_UTIL{
private:

public:
	static void Array_Forward(int* Array, int Num);
	static void Array_Forward(vector<int>& Array);
	static void Array_Reverse(int* Array, int Num);
	static void Array_Reverse(vector<int>& Array);
	static void FisherYates(int* Array, int Num);
	static void FisherYates(vector<int>& Array);
	
	static int Dice_index(int *Weight, int NUM);
	static int Dice_index(vector<int>& Weight, int NUM);
};

/************************************************************
************************************************************/
typedef sjRGBW (*pFUNC_LUM)(double now, int NumChs, int Ch_id, double d_LumInterval_Strobe, double d_LumLength_Strobe, double d_LumInterval_Flow, double d_LumLength_Flow, double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, const sjRGBW& Col_L, const sjRGBW& Col_H, string* Name);
typedef sjPanTilt (*pFUNC_POS)(double now, int NumChs, int Ch_id, double d_interval, double Amp_GainSync, string* Name);
typedef void (*pFUNC_FixIdOrder)(int* Array, int Num, string* Name);
typedef void (*pFUNC_VectorSort)(vector<int>& Array);
