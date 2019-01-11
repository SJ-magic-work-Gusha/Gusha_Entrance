/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "stdio.h"

#include "ofMain.h"
#include "ofxGui.h"

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/

enum{
	// WINDOW_WIDTH = 1630,
	WINDOW_WIDTH = 1700,	// 切れの良い解像度でないと、ofSaveScreen()での画面保存が上手く行かなかった(真っ暗な画面が保存されるだけ).
	WINDOW_HEIGHT = 1000,
};

enum{
	NUM_VSPLIT_DISP = 10,
};

enum{
	BUF_SIZE = 500,
	LARGE_BUF_SIZE = 6000,
};

enum{
	AUDIO_BUF_SIZE = 512,
	
	AUDIO_BUFFERS = 2,
	AUDIO_SAMPLERATE = 44100,
};

enum{
	GRAPH_BAR_WIDTH__FFT_GAIN = 1,
	GRAPH_BAR_SPACE__FFT_GAIN = 2,
};

enum{
	NUM_ANALYSIS_BANDS = 5,
	MAINBAND_ID = NUM_ANALYSIS_BANDS - 1,
};


/************************************************************
************************************************************/

/**************************************************
Derivation
	class MyClass : private Noncopyable {
	private:
	public:
	};
**************************************************/
class Noncopyable{
protected:
	Noncopyable() {}
	~Noncopyable() {}

private:
	void operator =(const Noncopyable& src);
	Noncopyable(const Noncopyable& src);
};

/**************************************************
**************************************************/
class LIGHT;
typedef void (LIGHT::*pFUNC_LIGHT_voidvoid)();

/**************************************************
**************************************************/
class COLOR_THEME : Noncopyable{
private:
	int LastState;
	float t_LastChange;
	
	bool b_ColorChange_Jiwari;
	bool b_ColorChange_immediate;
	
public:
	enum THEME{
		THEME_RED,
		THEME_GREEN,
		THEME_BLUE,
		THEME_PURPLE,
		THEME_CANDY,
		THEME_PSYCHE,
		
		NUM_THEMES,
	};
	
private:
	THEME Theme;
	
public:
	COLOR_THEME()
	: LastState(0)
	, t_LastChange(0)
	, Theme(THEME_BLUE)
	, b_ColorChange_Jiwari(false)
	, b_ColorChange_immediate(false)
	{
	}
	
	void set_Jiwari()		{ b_ColorChange_Jiwari = true; }
	void set_immediate()	{ b_ColorChange_immediate = true; }
	
	void setup(int theme_id)
	{
		if(theme_id < NUM_THEMES) { Theme = THEME(theme_id); }
	}
	
	/******************************
	param
		pFunc_immediate:メンバ関数へのpointerなので、Base(= LIGHT& Light)が必要.
	******************************/
	void update_byGui(int Gui_theme_id, LIGHT& Light, pFUNC_LIGHT_voidvoid pFunc_Jiwari, pFUNC_LIGHT_voidvoid pFunc_immediate){
		if(b_ColorChange_Jiwari){
			if(Gui_theme_id < NUM_THEMES) { Theme = THEME(Gui_theme_id); }
			b_ColorChange_Jiwari = false;
			(Light.*pFunc_Jiwari)();
		}
		if(b_ColorChange_immediate){
			if(Gui_theme_id < NUM_THEMES) { Theme = THEME(Gui_theme_id); }
			b_ColorChange_immediate = false;
			(Light.*pFunc_immediate)();
		}
	}
	
	void update_Auto(bool b_Enable, float now, int _State){
		// if( b_Enable && (5.0 * 60 < now - t_LastChange) && (_State != LastState) ){
		if( b_Enable && (60 < now - t_LastChange) && (_State != LastState) ){
			int RandomNumber = (int)( ((double)rand() / ((double)RAND_MAX + 1)) * NUM_THEMES );
			Theme = THEME(RandomNumber);
			
			t_LastChange = now;
		}
		LastState = _State;
	}
	
	int getTheme()	{ return int(Theme);}
};

/**************************************************
**************************************************/
class GUI_GLOBAL{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	/****************************************
	****************************************/
	ofxPanel gui;
	
	ofxGuiGroup Group_FFT;
		ofxFloatSlider LPFAlpha_dt__FFTGain_Fast;
		ofxFloatSlider LPFAlpha_dt__FFTGain_Slow;
		
		ofxFloatSlider LPFAlpha__FFTGainSlow_SmoothInFreqSpace;
		ofxFloatSlider LPFAlpha__d1_FFTGainSlowSmoothed_SmoothInFreqSpace;
		
		ofxFloatSlider thresh__ValidGain_ofSlowSmoothed_onZeroCrossPoint;
	
		ofxFloatSlider Val_DispMax__FFTGain_Fast;
		ofxFloatSlider Val_DispMax__FFTGain_Slow;
		ofxFloatSlider Val_DispMax__FFTGain_SlowSmoothed;
		ofxFloatSlider Val_DispMax__FFTGain_d1_SlowSmoothed;
		
	ofxGuiGroup Group_BandLev[NUM_ANALYSIS_BANDS];
		/* */
		ofxIntSlider AnalyzeRange__BandLev_FreqFrom[NUM_ANALYSIS_BANDS];
		ofxIntSlider AnalyzeRange__BandLev_FreqWidth[NUM_ANALYSIS_BANDS];
	
		ofxFloatSlider LPFAlpha_dt__BandLev[NUM_ANALYSIS_BANDS];
		ofxFloatSlider LPFAlpha_dt__d1_BandLev[NUM_ANALYSIS_BANDS];
		ofxFloatSlider LPFAlpha_dt__d2_BandLev[NUM_ANALYSIS_BANDS];
		
		ofxFloatSlider thresh_BandLev__H[NUM_ANALYSIS_BANDS];
		ofxFloatSlider thresh_BandLev__H_ratio_from_MainBand[NUM_ANALYSIS_BANDS];
		ofxFloatSlider thresh_BandLev__ratio_HtoL[NUM_ANALYSIS_BANDS];
		
		ofxFloatSlider thresh_d2BandLev__H[NUM_ANALYSIS_BANDS];
		ofxFloatSlider thresh_d2BandLev__ratio_HtoL[NUM_ANALYSIS_BANDS];
		
		ofxFloatSlider Val_DispMax__BandLev[NUM_ANALYSIS_BANDS];
		ofxFloatSlider Val_d1_DispMax__BandLev[NUM_ANALYSIS_BANDS];
		ofxFloatSlider Val_d2_DispMax__BandLev[NUM_ANALYSIS_BANDS];
		
		/* for only MainBand (= [4]) */
		ofxFloatSlider thresh_MainBand_RapidDown;
		
	ofxGuiGroup Group_Graph;
		ofxFloatSlider LineWidth;
	
	ofxGuiGroup Group_LightDesign;
		ofxToggle BeatDetect_Rise;
		
		ofxToggle b_CheckDmx;
		ofxIntSlider Dmx_TestId;
		ofxColorSlider Dmx_TestColor;
		
		ofxFloatSlider Led_dimmer;
		ofxToggle PlayStart;
		ofxToggle FestMode;
		ofxToggle b_Enable_LayerON;
		
	ofxGuiGroup Group_ColorTheme;
		ofxFloatSlider ColorTheme_id; // ofxIntSliderだと操作しづらいので.
		
		ofxButton button_ColorTheme_Jiwari;
		ofxButton button_ColorTheme_immediate;
		
		ofxToggle b_AutoColorChange;
};

/************************************************************
************************************************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt);
double LPF(double LastVal, double CurrentVal, double Alpha);
double sj_max(double a, double b);

/************************************************************
************************************************************/
extern GUI_GLOBAL* Gui_Global;

extern bool b_EnableProcess_CheckLightDesign;
extern bool b_EnableProcess_Light;
extern bool b_EnableProcess_vj;

extern FILE* fp_Log;
extern FILE* fp_Log_main;
extern FILE* fp_Log_fft;

extern float t_MeasProcessTime;
extern const float d_MeasProcessTime;

extern int GPIO_0;
extern int GPIO_1;

extern COLOR_THEME ColorTheme;

/************************************************************
************************************************************/

