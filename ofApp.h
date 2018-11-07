/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxOsc_BiDirection.h"

#include <new> // placement new

#include "sj_common.h"
#include "th_fft.h"
#include "Light.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
struct AUDIO_SAMPLE : private Noncopyable{
	vector<float> Left;
	vector<float> Right;
	
	void resize(int size){
		Left.resize(size);
		Right.resize(size);
	}
};

/**************************************************
**************************************************/
class COLOR_DYNAMIC_ALPHA : private Noncopyable{
private:
	ofColor color;
	
public:
	COLOR_DYNAMIC_ALPHA(const ofColor& _color)
	:color(_color)
	{
	}
	
	ofColor GetColor(int Alpha){
		color.a = Alpha;
		return color;
	}
};

/**************************************************
**************************************************/
struct VBO_SET : private Noncopyable{
	ofVbo Vbo;
	vector<ofVec3f> VboVerts;
	vector<ofFloatColor> VboColor;
	
	void setup(int size){
		VboVerts.resize(size);
		VboColor.resize(size);
		
		Vbo.setVertexData(&VboVerts[0], VboVerts.size(), GL_DYNAMIC_DRAW);
		Vbo.setColorData(&VboColor[0], VboColor.size(), GL_DYNAMIC_DRAW);
	}
	
	void set_singleColor(const ofColor& color){
		for(int i = 0; i < VboColor.size(); i++) { VboColor[i].set( double(color.r)/255, double(color.g)/255, double(color.b)/255, double(color.a)/255); }
	}
	
	void update(){
		Vbo.updateVertexData(&VboVerts[0], VboVerts.size());
		Vbo.updateColorData(&VboColor[0], VboColor.size());
	}
	
	void draw(int drawMode){
		Vbo.draw(drawMode, 0, VboVerts.size());
	}
	
	void draw(int drawMode, int total){
		if(VboVerts.size() < total) total = VboVerts.size();
		Vbo.draw(drawMode, 0, total);
	}
};

/**************************************************
**************************************************/
class STATECHART_BANDGAIN : private Noncopyable
{
private:
	enum STATE{
		STATE__L,
		STATE__LtoH,
		STATE__H,
		STATE__HtoL,
	};
	STATE State;
	
	bool b_valid;
	float t_DisableFrom;
	float t_timer;
	
	float thresh_H;
	float thresh_L;
	
	float t_thresh_LtoH;
	float t_thresh_HtoL;
	
	float d2_Amp;
	float Lum_GainSync;
	float Lum_GainSync_whenOff;
	
	bool IsDisabled(float now)
	{
		if( !b_valid || (now - t_DisableFrom < 2.0) )	return true;
		else											return false;
	}
	
	void Reset(float now)
	{
		State = STATE__L;
		d2_Amp = 0;
		Lum_GainSync = 0;
		
		t_DisableFrom = now; // thresh操作後(GUI)、一定時間Disable.
	}
	
	void StateChart(float now, float BandGain)
	{
		switch(State){
			case STATE__L:
				if(thresh_H < BandGain){
					State = STATE__LtoH;
					t_timer = now;
				}
				break;
				
			case STATE__LtoH:
				if(t_thresh_LtoH < now - t_timer){
					State = STATE__H;
				}else if(BandGain < thresh_H){
					State = STATE__L;
				}
				break;
				
			case STATE__H:
				if(BandGain < thresh_L){
					State = STATE__HtoL;
					t_timer = now;
				}
				break;
				
			case STATE__HtoL:
				if(t_thresh_HtoL < now - t_timer){
					State = STATE__L;
					d2_Amp = 0;
					Lum_GainSync = 0;
				}else if(thresh_L < BandGain){
					State = STATE__H;
				}
				break;
		}
	}
	
	void Refresh_d2Amp(float d2_BandGain)
	{
		// d2_Amp *= 0.999;
		
		if( (State == STATE__H) || (State == STATE__HtoL) ){
			d2_BandGain = d2_BandGain * 0.87;
			
			if( (0 < d2_BandGain) && (d2_Amp < abs(d2_BandGain)) ){
				d2_Amp = abs(d2_BandGain);
			}
		}
	}
	
	void cal_LumGainSync(float d2_BandGain)
	{
		if( (State == STATE__L) || (State == STATE__LtoH) ){
			Lum_GainSync = Lum_GainSync_whenOff;
		}else{
			if(d2_Amp == 0)	Lum_GainSync = Lum_GainSync_whenOff;
			else			Lum_GainSync = ofMap(d2_BandGain, -d2_Amp, d2_Amp, 1, 0, true);
		}
	}
	
public:
	STATECHART_BANDGAIN()
	: State(STATE__L)
	, b_valid(false)
	, t_DisableFrom(0)
	, t_timer(0)
	, thresh_H(0)
	, thresh_L(0)
	, d2_Amp(0)
	, Lum_GainSync(0)
	, Lum_GainSync_whenOff(0.5)
	{
	}
	
	STATECHART_BANDGAIN(float _t_thresh_LtoH, float _t_thresh_HtoL)
	: State(STATE__L)
	, b_valid(false)
	, t_DisableFrom(0)
	, t_timer(0)
	, thresh_H(0)
	, thresh_L(0)
	, t_thresh_LtoH(_t_thresh_LtoH)
	, t_thresh_HtoL(_t_thresh_HtoL)
	, d2_Amp(0)
	, Lum_GainSync(0)
	, Lum_GainSync_whenOff(0.5)
	{
	}
	
	void OnGuiChanged(float now)
	{
		Reset(now);
	}
	
	void SetThresh(float now, float _thresh_H, float _thresh_L)
	{
		thresh_H = _thresh_H;
		thresh_L = _thresh_L;
		b_valid = true;
	}
	
	void update(float now, float BandGain, float d2_BandGain)
	{
		if(IsDisabled(now)) return;
		
		StateChart(now, BandGain);
		Refresh_d2Amp(d2_BandGain);
		cal_LumGainSync(d2_BandGain);
	}
	
	float Get_d2Amp()
	{
		return d2_Amp;
	}
	
	float GetLum_GainSync(float now)
	{
		if(IsDisabled(now)) return 0;
		
		return Lum_GainSync;
	}
	
	/******************************
	return
		0: L
		1: H
	******************************/
	int GetState(float now)
	{
		if(IsDisabled(now)) return 0;
		
		if( (State == STATE__L) || (State == STATE__LtoH) )	return 0;
		else												return 1;
	}
};

/**************************************************
**************************************************/
class STATECHART_D1_BANDGAIN : private Noncopyable
{
private:
	enum INTERNAL_STATE{
		INTERNAL_STATE_RISE,
		INTERNAL_STATE_FALL,
	};
	
	INTERNAL_STATE InternalState;
	double Lev_RiseGround;
	double RiseHeight;
	double CurrentHeight_Ratio; // just for Disp : debug.
	
	const double thresh_DownRatio_min;
	const double thresh_DownRatio_max;
	double thresh_DownRatio;
	float t_EnterRise;
	const float d_Fade_DownRatio;
	
	void StateChart_internal(float now, double d1, double raw)
	{
		switch(InternalState){
			case INTERNAL_STATE_RISE:
				{
					double CurrentHeight = raw - Lev_RiseGround;
					if(RiseHeight < CurrentHeight) RiseHeight = CurrentHeight;
					CurrentHeight_Ratio = CurrentHeight / RiseHeight; // Disp : just fot debug.
					
					if(d_Fade_DownRatio < now - t_EnterRise)	{ thresh_DownRatio = thresh_DownRatio_max; }
					else										{ thresh_DownRatio = thresh_DownRatio_min + (now - t_EnterRise) * (thresh_DownRatio_max - thresh_DownRatio_min) / d_Fade_DownRatio; }
					
					if(d1 < 0){
						if( CurrentHeight < RiseHeight * thresh_DownRatio ){
							InternalState = INTERNAL_STATE_FALL;
							
							 // no need actually. just for debug.
							thresh_DownRatio = 0;
							CurrentHeight_Ratio = 0;
						}else{
							// keep.
						}
					}
				}
				break;
				
			case INTERNAL_STATE_FALL:
				if(0 <= d1){
					InternalState = INTERNAL_STATE_RISE;
					Lev_RiseGround = raw;
					RiseHeight = 0;
					thresh_DownRatio = thresh_DownRatio_min;
					t_EnterRise = now;
				}
				break;
		}
	}
	
public:
	// init val will be overwrited.
	STATECHART_D1_BANDGAIN()
	: InternalState(INTERNAL_STATE_FALL)
	, thresh_DownRatio_min(0.55)
	, thresh_DownRatio_max(0.87)
	, d_Fade_DownRatio(15)
	, thresh_DownRatio(0.6)
	, State(STATE__L)
	{
	}
	
	STATECHART_D1_BANDGAIN(float _d_echo, float _t_thresh_LtoH, float _t_thresh_HtoL)
	: InternalState(INTERNAL_STATE_FALL)
	, thresh_DownRatio_min(0.55)
	, thresh_DownRatio_max(0.87)
	, d_Fade_DownRatio(15)
	, thresh_DownRatio(0.6)
	, State(STATE__L)
	, t_thresh_LtoH(_t_thresh_LtoH)
	, d_echo(_d_echo)
	, t_thresh_HtoL(_t_thresh_HtoL)
	{
	}
	
	void update(float now, double d1, double raw){
		StateChart_internal(now, d1, raw);
		StateChart(now);
	}
	
	/******************************
	return
		0: L
		1: H
	******************************/
	int GetState()
	{
		if( (State == STATE__L) || (State == STATE__LtoH) )	return 0;
		else												return 1;
	}
	
	double Get_threshDownRatio()		{ return thresh_DownRatio; }
	double Get_CurrentHeight_Ratio()	{ return CurrentHeight_Ratio; }
	
private:
	enum STATE{
		STATE__L,
		STATE__LtoH,
		STATE__H_echo,
		STATE__H,
		STATE__HtoL,
	};
	STATE State;
	
	float t_timer;
	
	float t_thresh_LtoH;
	float d_echo;
	float t_thresh_HtoL;
	

	
	void StateChart(float now)
	{
		switch(State){
			case STATE__L:
				if(InternalState == INTERNAL_STATE_RISE){
					State = STATE__LtoH;
					t_timer = now;
				}
				break;
				
			case STATE__LtoH:
				if(InternalState == INTERNAL_STATE_FALL){
					State = STATE__L;
				}else if(t_thresh_LtoH < now - t_timer){
					State = STATE__H_echo;
					t_timer = now;
				}
				break;
				
			case STATE__H_echo:
				if(d_echo < now - t_timer){
					State = STATE__H;
				}
				break;
				
			case STATE__H:
				if(InternalState == INTERNAL_STATE_FALL){
					State = STATE__HtoL;
					t_timer = now;
				}
				break;
				
			case STATE__HtoL:
				if(InternalState == INTERNAL_STATE_RISE){
					State = STATE__H;
				}else if(t_thresh_HtoL < now - t_timer){
					State = STATE__L;
				}
				break;
		}
	}
};

/**************************************************
**************************************************/
class STATECHART_D2_BANDGAIN : private Noncopyable
{
private:
	enum STATE{
		STATE__L,
		STATE__H,
	};
	STATE State;
	
	bool b_valid;
	float t_DisableFrom;
	
	float thresh_H;
	float thresh_L;
	
	bool IsDisabled(float now)
	{
		if( !b_valid || (now - t_DisableFrom < 2.0) )	return true;
		else											return false;
	}
	
	void Reset(float now)
	{
		State = STATE__L;
		t_DisableFrom = now; // thresh操作後(GUI)、一定時間Disable.
	}
	
	void StateChart(float now, float d2_BandGain)
	{
		switch(State){
			case STATE__L:
				if(thresh_H < d2_BandGain){
					State = STATE__H;
				}
				break;
			case STATE__H:
				if(d2_BandGain < thresh_L){
					State = STATE__L;
				}
				break;
		}
	}
	
public:
	STATECHART_D2_BANDGAIN()
	: State(STATE__L)
	, b_valid(false)
	, t_DisableFrom(0)
	{
	}
	
	void SetThresh(float now, float _thresh_H, float _thresh_L)
	{
		if( !b_valid || (_thresh_H != thresh_H) || (_thresh_L != thresh_L) ){
			thresh_H = _thresh_H;
			thresh_L = _thresh_L;
			
			Reset(now);
		}
		
		b_valid = true;
	}
	
	void update(float now, float d2_BandGain)
	{
		if(IsDisabled(now)) return;
		StateChart(now, d2_BandGain);
	}
	
	/******************************
	return
		0: L
		1: H
	******************************/
	int GetState(float now)
	{
		if(IsDisabled(now)) return 0;
		
		if( State == STATE__L )	return 0;
		else					return 1;
	}
};

/**************************************************
**************************************************/
class STATECHART_BEATLOCK : private Noncopyable
{
private:
	int Last_State_BandGain;
	int Last_State_d2BandGain;
	
	double Diff_percent;
	
	bool b_lock;
	double Last_BeatInterval;
	double BeatInterval;
	float t_LastBeat;
	int count = 0;
	
	const int c_thresh_lock;
	
	void Process_H(float now, int State_d2BandGain){
		if( (Last_State_d2BandGain == 1) && (State_d2BandGain == 0) ){
			if(t_LastBeat != -1){
				Last_BeatInterval = BeatInterval;
				BeatInterval = now - t_LastBeat;
				
				if(Last_BeatInterval == 0){
					count = 0;
				}else{
					Diff_percent = 100 * abs(BeatInterval - Last_BeatInterval) / Last_BeatInterval;
					if(Diff_percent < 20)	{ count++; }
					else					{ count = 0; }
				}
				
				if(c_thresh_lock < count)	b_lock = true;
				else						b_lock = false;
			}
			
			t_LastBeat = now;
		}
	}
	
public:
	STATECHART_BEATLOCK()
	: Last_State_BandGain(-1)
	, Last_State_d2BandGain(-1)
	, Diff_percent(0)
	, b_lock(false)
	, Last_BeatInterval(0.5)
	, BeatInterval(0.5)
	, t_LastBeat(-1)
	, c_thresh_lock(4)
	{
	}
	
	void update(float now, int State_BandGain, int State_d2BandGain){
		/********************
		********************/
		if( (Last_State_BandGain == -1) || (Last_State_d2BandGain == -1) ){
			Last_State_BandGain = State_BandGain;
			Last_State_d2BandGain = State_d2BandGain;
			return;
		}
		
		/********************
		********************/
		if( (Last_State_BandGain == 0) && (State_BandGain == 1) ){
			BeatInterval = 0.5; // temp.
			t_LastBeat = -1;
			count = 0;
			b_lock = false;
			
		}else if( (Last_State_BandGain == 1) && (State_BandGain == 0) ){
			b_lock = false;
		}
		
		/********************
		********************/
		if(State_BandGain == 1) Process_H(now, State_d2BandGain);
		
		/********************
		********************/
		Last_State_BandGain = State_BandGain;
		Last_State_d2BandGain = State_d2BandGain;
	}
	
	bool IsLock()				{ return b_lock; }
	int get_count()				{ return count; }
	double get_BeatInterval()	{ return BeatInterval; }
	double get__t_LastBeat()	{ return t_LastBeat; }
	double get__Diff_percent()	{ return Diff_percent; }
	
};

/**************************************************
**************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
		NUM_TIME_POINTS = 370,
	};
	
	enum{
		WIDTH__GRAPH_TIMEBASED = NUM_TIME_POINTS,
		WIDTH__GRAPH_FREQBASED = 520,
	};
	
	enum{
		FONT_S,
		FONT_M,
		FONT_L,
		
		NUM_FONT_SIZE,
	};
	
	enum GRAPH_TIMEBASED{ // per Band(There are 5 Bands in this app).
		GRAPH_TIMEBASED__BANDLEV__RAW,
		GRAPH_TIMEBASED__BANDLEV__d1,
		GRAPH_TIMEBASED__BANDLEV__d2,
		
		GRAPH_TIMEBASED__RAW_DYNAMIC_THRESH__H,
		GRAPH_TIMEBASED__RAW_DYNAMIC_THRESH__L,
		
		GRAPH_TIMEBASED__LH__RAW,
		GRAPH_TIMEBASED__LH__d1,
		GRAPH_TIMEBASED__LH__d2,
		
		GRAPH_TIMEBASED__d2AMP_H,
		GRAPH_TIMEBASED__d2AMP_L,
		
		NUM_GRAPH_TIMEBASED,
	};
	
	enum GRAPH_FREQBASED{
		GRAPH_FREQBASED__FFTGAIN_FAST,
		GRAPH_FREQBASED__FFTGAIN_SLOW,
		GRAPH_FREQBASED__FFTGAIN_SLOW_SMOOTHED,
		GRAPH_FREQBASED__d1_FFTGAIN_SLOW_SMOOTHED,
		GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED,
		
		NUM_GRAPH_FREQBASED,
	};
	
	/****************************************
	parameter
	****************************************/
	/********************
	********************/
	ofxSyphonServer mainOutputSyphonServer;
	
	/********************
	********************/
	int soundStream_Input_DeviceId;
	int soundStream_Output_DeviceId;
	ofSoundStream soundStream;
	
	AUDIO_SAMPLE AudioSample;
	bool b_EnableAudioOut;
	
	// OSC_TARGET Osc;
	
	/********************
	********************/
	float now;
	float LastInt;
	
	bool b_DispGui;
	bool b_PauseGraph;
	bool b_ShowFrameRate;
	
	THREAD_FFT* fft_thread;
	
	ofTrueTypeFont font[NUM_FONT_SIZE];
	
	/********************
	********************/
	int ofs_x_ReadCursor;
	
	/********************
	Graph
	********************/
	double GraphTimeBased_Log_Raw[NUM_ANALYSIS_BANDS][NUM_GRAPH_TIMEBASED][NUM_TIME_POINTS];
	
	VBO_SET Vboset_FreqBased[NUM_GRAPH_FREQBASED];
	VBO_SET Vboset_TimeBased[NUM_ANALYSIS_BANDS][NUM_GRAPH_TIMEBASED];
	
	STATECHART_BANDGAIN StateChart_BandGain[NUM_ANALYSIS_BANDS];
	STATECHART_D1_BANDGAIN StateChart_d1_BandGain[NUM_ANALYSIS_BANDS];
	STATECHART_D2_BANDGAIN StateChart_d2_BandGain[NUM_ANALYSIS_BANDS];
	
	STATECHART_BEATLOCK StateChart_BeatLock[NUM_ANALYSIS_BANDS];
	
	int png_id;
	
	ofColor col_Back_0;
	ofColor col_Back_1;
	
	float t_Beat_Band3;
	bool b_Beat_Band3;
	
	/********************
	********************/
	LIGHT* Light;
	
	
	/****************************************
	method
	****************************************/
	/********************
	gui
	********************/
	void setup_Gui();
	void Guis_LoadSetting();
	void remove_xml();
	bool checkif_FileExist(const char* FileName);
	
	/********************
	********************/
	void audioIn(float *input, int bufferSize, int nChannels);
	void audioOut(float *output, int bufferSize, int nChannels);
	
	
	/********************
	Graph
	********************/
	void RefreshVerts();
	void Refresh_BarColor();
	void TimaBasedGraph__get__Data_and_Graph_Range(int BandId, GRAPH_TIMEBASED TimeBasedGraph_Id, float& dataFrom, float& dataTo, float& GraphFrom, float& GraphTo);
	void TimaBasedGraph_getVal_ReverseFromVbo(int BandId, GRAPH_TIMEBASED TimeBasedGraph_Id, int ArrayId, char* buf);
	bool IsHigh_TimeBasedGraph_ReverseFromVbo(int BandId, GRAPH_TIMEBASED TimeBasedGraph_Id, int ArrayId);
	void FreqBasedGraph_getVal_ReverseFromVbo(GRAPH_FREQBASED FreqBasedGraph_Id, float mouse_x, char* buf);
	bool IsZeroCross_ReverseFromVboGraph(float mouse_x, char* buf = NULL);
	void RefreshData__GraphTimeBased_Log(double* Array_Log, double NewData, double dt_of_Alpha);
	void RefreshData__GraphTimeBased_Log(double* Array_Log, double NewData);
	
	void draw_ColorTheme();
	void draw_TimeBasedGraph(int BandId, GRAPH_TIMEBASED GraphTimeBased_id, const ofColor& col_Back, const ofRectangle& rect_Back, const ofPoint& Coord_zero, float Screen_y_max, float Val_Disp_y_Max, float Val_Disp_y_Min);
	void draw_FreqBasedGraph(GRAPH_FREQBASED GraphFreqBased_id, const ofColor& col_Back, const ofRectangle& rect_Back, const ofPoint& Coord_zero, float Screen_y_max, float Val_Disp_y_Max);
	void draw_AnalyzeRange_and_Level(const ofColor& col_Line, const ofColor& col_Rect, int x_from, int x_to, float BandLev, float y_DrawRangeLine, float Screen_y_max, float Screen_x_max, float Val__Disp_y_max);
	void drawGuis();
	void draw_CursorAndValue_TimeBasedGraph(int BandId, GRAPH_TIMEBASED TimeBasedGraph_Id, float Screen_y_max);
	void draw_CursorAndValue_FreqBasedGraph(GRAPH_FREQBASED FreqBasedGraph_Id, float Screen_y_max);
	void draw_ZeroCrossPointer(float Screen_y_max);
	void draw_DigitalPointer_TimeBasedGraph(int BandId, GRAPH_TIMEBASED TimeBasedGraph_Id, double PointerPos_y);
	void draw__thresh_BandGain(int BandId, float Screen_y_max);
	void draw__thresh_d2_BandGain(int BandId, float Screen_y_max);
	void draw__thresh__d_MainBand_RadipDown(int BandId, float Screen_y_max);
	void draw__thresh_ValidGain_ofSlowSmoothed_onZeroCrossPoint(float Screen_y_max);
	
	bool TimeBasedGraph_isPlusMinus(GRAPH_TIMEBASED TimeBasedGraph_Id);
	bool FreqBasedGraph_isPlusMinus(GRAPH_FREQBASED FreqBasedGraph_Id);
	
	/********************
	********************/
	void update_StateLH(int BandId);
	bool IsBoost();
	bool Is_LayerON_Go();
	int GetState_BandGain();
		
public:
	/****************************************
	****************************************/
	ofApp(int _soundStream_Input_DeviceId, int _soundStream_Output_DeviceId, bool _b_EnableProcess_CheckLightDesign, bool _b_EnableProcess_Light, bool _b_EnableProcess_vj);
	~ofApp();
	
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
};
