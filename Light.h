/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxArtnet.h"
#include "ofxNetwork.h"

#include "sj_common.h"

#include "L_common.h"
#include "L_Device.h"
#include "L_Block.h"
#include "L_Table.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class LAYER_ALPHA{
private:
	/****************************************
	****************************************/
	class VAL_IN_RANGE{
	private:
		double val_min;
		double val_max;
		double val;
		
		double t_min;
		double t_max;
		
	public:
		VAL_IN_RANGE()
		: t_min(2.0), t_max(10.0), val_min(1.0/2.0), val_max(1.0/1.0)
		{
			val = val_min;
		}
		
		void setup(double _min, double _max, double _init) { val_min = _min; val_max = _max; val = _init; }
		void update_val(double delta_t) { val = ofMap(delta_t, t_min, t_max, val_max, val_min, true); }
		double get_val() { return val; }
	};
	
	/****************************************
	****************************************/
	enum STATE{
		STATE_OFF,
		STATE_ON,
	};
	
	STATE State;
	
	double alpha;
	
	VAL_IN_RANGE RiseSpeed;
	VAL_IN_RANGE FallSpeed;
	
	float t_ChangeState;
	bool b_Res_ImStable;
	bool b_JumToOn;
	
	float t_StrobeOff;
	
	float t_LastInt;
	
public:
	LAYER_ALPHA()
	: State(STATE_OFF), t_LastInt(0)
	{
		Reset(0);
	}
	
	void setup(float now, double RiseTime_min, double RiseTime_max, double FallTime_min, double FallTime_max)
	{
		Reset(now);
		
		if(RiseTime_min <= 0)	RiseTime_min = 0.001; // 1ms means immediate.
		if(RiseTime_max <= 0)	RiseTime_max = 0.001; // 1ms means immediate.
		if(RiseTime_max < RiseTime_min) RiseTime_max = RiseTime_min;
		RiseSpeed.setup(1.0/RiseTime_max, 1.0/RiseTime_min, 1.0/RiseTime_max); // RiseTime_max gives minimum Rise speed.
		
		if(FallTime_min <= 0)	FallTime_min = 0.001;
		if(FallTime_max <= 0)	FallTime_max = 0.001;
		if(FallTime_max < FallTime_min) FallTime_max = FallTime_min;
		FallSpeed.setup(1.0/FallTime_max, 1.0/FallTime_min, 1.0/FallTime_max); // FallTime_max gives minimum Rise speed.
	}
	
	void Reset(float now)
	{
		State = STATE_OFF;
		alpha = 0;
		t_ChangeState = now;
		b_Res_ImStable = false;
		b_JumToOn = false;
		t_StrobeOff = now;
		t_LastInt = now;
	}
	
	void update(float now, bool b_condition)
	{
		if(now < t_LastInt) { t_LastInt = now; return; }
		
		switch(State){
			case STATE_OFF:
				alpha -= FallSpeed.get_val() * (now - t_LastInt);
				if(alpha < 0) alpha = 0;
				if(1 < alpha) alpha = 1;
				
				if(b_condition){
					State = STATE_ON;
					if(alpha == 0) b_JumToOn = true;
					
					RiseSpeed.update_val(now - t_StrobeOff);
					FallSpeed.update_val(now - t_StrobeOff);
				}
				break;
				
			case STATE_ON:
				alpha += RiseSpeed.get_val() * (now - t_LastInt);
				if(alpha < 0) alpha = 0;
				if(1 < alpha) alpha = 1;
				
				if(!b_condition){
					State = STATE_OFF;
					t_ChangeState = now;
					b_Res_ImStable = false;
					b_JumToOn = false;
					t_StrobeOff = now;
				}
				break;
		}
		
		t_LastInt = now;
	}
	
	bool IsStable_at_Off(float now)
	{
		if( (State == STATE_OFF) && (alpha == 0) && (3.0 < now - t_ChangeState) && (!b_Res_ImStable) ){
			b_Res_ImStable = true; // 1 time.
			return true;
		}else{
			return false;
		}
	}
	
	bool IsState_JumpTo_ON()
	{
		if(b_JumToOn){
			b_JumToOn = false; // 1 time.
			return true;
		}else{
			return false;
		}
	}
	
	double get_alpha() { return alpha; }
};

/**************************************************
**************************************************/
class LIGHT{
private:
	/****************************************
	****************************************/
	struct PARAM_FROM_SIGNALPROCESS{
		bool gui__b_CheckDmx;
		bool gui__PlayStart;
		
		int State_BandGain;
 		int State_d2_BandGain3;
		
		void set(bool _gui__b_CheckDmx, bool _gui__PlayStart, int _State_BandGain, int _State_d2_BandGain3)
		{
			gui__b_CheckDmx = _gui__b_CheckDmx;
			gui__PlayStart = _gui__PlayStart;
			
			State_BandGain = _State_BandGain;
			State_d2_BandGain3 = _State_d2_BandGain3;
		}
	};
	
	/****************************************
	****************************************/
	ofxArtnet artnet;
	ofxUDPManager udp_SendTo_VjMaterial;
	ofxUDPManager udp_SendTo_VjUnity;
	
	PARAM_FROM_SIGNALPROCESS Last_ParamFromSignalProcess;
	PARAM_FROM_SIGNALPROCESS ParamFromSignalProcess;
	
	LAYER_ALPHA LayerAlpha[NUM_STATES];
	LAYER_ALPHA LayerAlpha_Strobe;
	LAYER_ALPHA LayerAlpha_ON;
	
	PARAM_FOR_LED_CONTROL ParamForLedControl__Layer_States[NUM_STATES];
	PARAM_FOR_LED_CONTROL ParamForLedControl__Layer_Strobe;
	PARAM_FOR_LED_CONTROL ParamForLedControl__Layer_ON;
	PARAM_FOR_LED_CONTROL_COMMON CommonParamForLedControl;
	
	double now;
	double t_LastInt;
	
	vector<int> Vector_Weight;
	

	/****************************************
	****************************************/
	
	/********************
	singleton
	********************/
	LIGHT();
	~LIGHT();
	LIGHT(const LIGHT&); // Copy constructor. no define.
	LIGHT& operator=(const LIGHT&); // コピー代入演算子. no define.
	
	/********************
 	********************/
	void check_DesignFunc_Lum();
	void check_DesignFunc_Pos();
	void check_DesignTable();
	
	/********************
 	********************/
	int Count_NumLogicalChs(const LED_BLOCK& LedBlock);
	int Count_NumPhysicalChs(const PHYSICAL_CH_SET& LogicalCh);
	
	void sendDmx();
	void Send_AllZero_to_AllOde();
	void Reset(float now);
	
	void Init_Design_AllLayer();
	void SetDesign_AllLayer();
	void SetDesign_All_BackLayer();
	void SetDesign_OneLayer(int State);
	void SetDesign_Layer_Strobe(int State);
	void SetDesign_Layer_ON(int State);
	
	void setparam_LedLight_for_DmxTest();
	
	bool Dice_IsSameDesignForAllBlock(int State_BandGain);
	pFUNC_FixIdOrder Dice_FuncLum(int BlockId, int State_BandGain, int DesignId);
	void Dice_Color_LH(int BlockId, int State_BandGain, int DesignId);
	pFUNC_FixIdOrder Dice_FuncPos(int BlockId, int State_BandGain);
	pFUNC_FixIdOrder Set_FuncLum__LayerON(int BlockId);
	pFUNC_FixIdOrder Set_FuncLum__LayerStrobe(int BlockId);
	void Set_Color_LH__Strobe(int BlockId);
	void Set_Color_LH__ON(int BlockId, int State);
	
	void Fix__ParamForLedControl__Layer_States(int State_BandGain);
	void Fix__ParamForLedControl__Layer_Strobe(int State_BandGain);
	void Fix__ParamForLedControl__Layer_ON();
	
	void update_LedParam_Lum(double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat);
	void update_LedParam_PanTilt(int State_BandGain, double Amp_GainSync);
	
	void SendUdp_Vj_Unity(bool b_Beat_Band3);
	
	void inc_ValidBlocks_BlockId(int& id);
	
public:
	/****************************************
	****************************************/
	static float d_MinLimit_ValidBlockChange;
	
	/****************************************
	****************************************/
	/********************
	********************/
	static LIGHT* getInstance(){
		static LIGHT inst;
		return &inst;
	}
	
	void setup(int State_BandGain, int State_d2_BandGain3);
	void update(int State_BandGain, bool b_Layer_strobe, bool b_Layer_ON, int State_d2_BandGain3, double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, bool& b_Beat_Band3);
	void draw();
	void exit();
	
	void test();
};
