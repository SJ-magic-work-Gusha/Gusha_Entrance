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
enum{
	SIZE_DMX_UNIVERSE = 512,
};

enum LED_DEVICE_TYPE{
	LED_DEVICE_TYPE_FIXED,
	LED_DEVICE_TYPE_MOVING,
};

/************************************************************
************************************************************/
class ODE{
private:
	char ch_IP[BUF_SIZE];
	
public:
	unsigned char universe[SIZE_DMX_UNIVERSE];
	
	ODE(const char* _ch_IP)
	{
		strcpy(ch_IP, _ch_IP);
	}
	const char* get_IP()
	{
		return ch_IP;
	}
};

struct LED_PARAM{
	sjRGBW rgbw;
	sjPanTilt PanTilt;
	
	void clear()
	{
		rgbw.clear();
		PanTilt.clear();
	}
};

struct LED_LIGHT{
	const int ODE_id;
	const int AddressFrom;
	const enum LED_DEVICE_TYPE LedDeviceType;
	const int Pan_min;
	const int Pan_max;
	const int Tilt_min;
	const int Tilt_max;
	
	LED_PARAM LedParam_Out;
	
	LED_LIGHT(int _ODE_id, int _AddressFrom, enum LED_DEVICE_TYPE _LedDeviceType, int _Pan_min, int _Pan_max, int _Tilt_min, int _Tilt_max)
	: ODE_id(_ODE_id), AddressFrom(_AddressFrom), LedDeviceType(_LedDeviceType), Pan_min(_Pan_min), Pan_max(_Pan_max), Tilt_min(_Tilt_min), Tilt_max(_Tilt_max)
	{
	}
	
	void AddLum(const sjRGBW& rgbw)
	{
		LedParam_Out.rgbw += rgbw;
	}
	
	void clear()
	{
		LedParam_Out.clear();
	}
};


