/************************************************************
************************************************************/
#include "sj_common.h"

/************************************************************
************************************************************/
/********************
********************/
int GPIO_0 = 0;
int GPIO_1 = 0;

/********************
********************/
GUI_GLOBAL* Gui_Global = NULL;

bool b_EnableProcess_CheckLightDesign = false;
bool b_EnableProcess_Light = false;
bool b_EnableProcess_vj = false;

FILE* fp_Log = NULL;
FILE* fp_Log_main = NULL;
FILE* fp_Log_fft = NULL;

float t_MeasProcessTime = -1;
const float d_MeasProcessTime = 10;

COLOR_THEME ColorTheme;

/************************************************************
func
************************************************************/

/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt)
{
	double Alpha;
	if((Alpha_dt <= 0) || (Alpha_dt < dt))	Alpha = 1;
	else									Alpha = 1/Alpha_dt * dt;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha)
{
	if(Alpha < 0)		Alpha = 0;
	else if(1 < Alpha)	Alpha = 1;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double sj_max(double a, double b)
{
	if(a < b)	return b;
	else		return a;
}

/************************************************************
class
************************************************************/

/******************************
******************************/
void GUI_GLOBAL::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	Group_FFT.setup("FFT");
		Group_FFT.add(LPFAlpha_dt__FFTGain_Fast.setup(">LPF_dt Fast", 0.15, 0, 2.0));
		Group_FFT.add(LPFAlpha_dt__FFTGain_Slow.setup("Slow", 0.2, 0, 2.0));
		
		Group_FFT.add(LPFAlpha__FFTGainSlow_SmoothInFreqSpace.setup(">LPF Slow", 1.0, 0.01, 1.0));
		Group_FFT.add(LPFAlpha__d1_FFTGainSlowSmoothed_SmoothInFreqSpace.setup("d1_SlowSmoothed", 1.0, 0.01, 1.0));
		
		Group_FFT.add(thresh__ValidGain_ofSlowSmoothed_onZeroCrossPoint.setup(">G on 0X", 5e-5, 0, 1e-4));
		
		Group_FFT.add(Val_DispMax__FFTGain_Fast.setup(">ValDisp FFT_Fast", 0.02, 0, 0.1));
		Group_FFT.add(Val_DispMax__FFTGain_Slow.setup("FFT_Slow", 4e-3, 0, 1e-2));
		Group_FFT.add(Val_DispMax__FFTGain_SlowSmoothed.setup("SlowSmoothed", 4e-3, 0, 1e-2));
		Group_FFT.add(Val_DispMax__FFTGain_d1_SlowSmoothed.setup("d1_SlowSmoothed", 1e-4, 0, 1e-3));
	gui.add(&Group_FFT);
	
	Group_BandLev[0].setup("Band[0]");
		Group_BandLev[0].add(AnalyzeRange__BandLev_FreqFrom[0].setup(">Analyze From", 1, 0, 255));
		Group_BandLev[0].add(AnalyzeRange__BandLev_FreqWidth[0].setup("Width", 1, 1, 256));
		
		Group_BandLev[0].add(LPFAlpha_dt__BandLev[0].setup(">LPF_dt Lev", 0.1, 0, 2.0));
		Group_BandLev[0].add(LPFAlpha_dt__d1_BandLev[0].setup("d1", 0.04, 0, 2.0));
		Group_BandLev[0].add(LPFAlpha_dt__d2_BandLev[0].setup("d2", 0.04, 0, 2.0));
		
		Group_BandLev[0].add(thresh_BandLev__H[0].setup(">thresh Band_H", 0.0125, 0, 0.1)); // no use
		Group_BandLev[0].add(thresh_BandLev__H_ratio_from_MainBand[0].setup("H:x Band[wide]", 13, 0.01, 20));
		Group_BandLev[0].add(thresh_BandLev__ratio_HtoL[0].setup("ratio:HtoL", 0.85, 0, 1.0));
		
		Group_BandLev[0].add(thresh_d2BandLev__H[0].setup("d2Band_H", 0.2, 0, 1.0));
		Group_BandLev[0].add(thresh_d2BandLev__ratio_HtoL[0].setup("ratio:HtoL", 0.1, 0, 1.0));
		
		Group_BandLev[0].add(Val_DispMax__BandLev[0].setup(">ValDisp BandLev", 0.05, 0, 0.1));
		Group_BandLev[0].add(Val_d1_DispMax__BandLev[0].setup("d1", 0.6, 0, 1.0));
		Group_BandLev[0].add(Val_d2_DispMax__BandLev[0].setup("d2", 1.6, 0, 3.0));
	gui.add(&Group_BandLev[0]);
	
	Group_BandLev[1].setup("Band[1]");
		Group_BandLev[1].add(AnalyzeRange__BandLev_FreqFrom[1].setup(">Analyze From", 2, 0, 255));
		Group_BandLev[1].add(AnalyzeRange__BandLev_FreqWidth[1].setup("Width", 23, 1, 256));
		
		Group_BandLev[1].add(LPFAlpha_dt__BandLev[1].setup(">LPF_dt Lev", 0.15, 0, 2.0));
		Group_BandLev[1].add(LPFAlpha_dt__d1_BandLev[1].setup("d1", 0.25, 0, 2.0));
		Group_BandLev[1].add(LPFAlpha_dt__d2_BandLev[1].setup("d2", 0.25, 0, 2.0));
		
		Group_BandLev[1].add(thresh_BandLev__H[1].setup(">thresh Band_H", 15e-4, 0, 1e-2));  // no use
		Group_BandLev[1].add(thresh_BandLev__H_ratio_from_MainBand[1].setup("H:x Band[wide]", 3, 0.01, 10));
		Group_BandLev[1].add(thresh_BandLev__ratio_HtoL[1].setup("ratio:HtoL", 0.85, 0, 1.0));
		
		Group_BandLev[1].add(thresh_d2BandLev__H[1].setup("d2Band_H", 0.0008, 0, 0.01));
		Group_BandLev[1].add(thresh_d2BandLev__ratio_HtoL[1].setup("ratio:HtoL", 0.1, 0, 1.0));
		
		Group_BandLev[1].add(Val_DispMax__BandLev[1].setup(">Disp BandLev", 1e-2, 0, 2e-2));
		Group_BandLev[1].add(Val_d1_DispMax__BandLev[1].setup("d1", 0.003, 0, 0.01));
		Group_BandLev[1].add(Val_d2_DispMax__BandLev[1].setup("d2", 0.003, 0, 0.01));
	gui.add(&Group_BandLev[1]);
	
	Group_BandLev[2].setup("Band[2]");
		Group_BandLev[2].add(AnalyzeRange__BandLev_FreqFrom[2].setup(">Analyze From", 24, 0, 255));
		Group_BandLev[2].add(AnalyzeRange__BandLev_FreqWidth[2].setup("Width", 24, 1, 256));
		
		Group_BandLev[2].add(LPFAlpha_dt__BandLev[2].setup(">LPF_dt Lev", 0.15, 0, 2.0));
		Group_BandLev[2].add(LPFAlpha_dt__d1_BandLev[2].setup("d1", 0.25, 0, 2.0));
		Group_BandLev[2].add(LPFAlpha_dt__d2_BandLev[2].setup("d2", 0.25, 0, 2.0));
		
		Group_BandLev[2].add(thresh_BandLev__H[2].setup(">thresh Band_H", 1e-3, 0, 1e-2)); // no use
		Group_BandLev[2].add(thresh_BandLev__H_ratio_from_MainBand[2].setup("H:x Band[wide]", 0.95, 0.01, 10));
		Group_BandLev[2].add(thresh_BandLev__ratio_HtoL[2].setup("ratio:HtoL", 0.8, 0, 1.0));
		
		Group_BandLev[2].add(thresh_d2BandLev__H[2].setup("d2Band_H", 0.0008, 0, 0.01));
		Group_BandLev[2].add(thresh_d2BandLev__ratio_HtoL[2].setup("ratio:HtoL", 0.1, 0, 1.0));
		
		Group_BandLev[2].add(Val_DispMax__BandLev[2].setup(">Disp BandLev", 5e-3, 0, 1e-2));
		Group_BandLev[2].add(Val_d1_DispMax__BandLev[2].setup("d1", 0.003, 0, 0.01));
		Group_BandLev[2].add(Val_d2_DispMax__BandLev[2].setup("d2", 0.003, 0, 0.01));
	gui.add(&Group_BandLev[2]);
	
	Group_BandLev[3].setup("Band[3]");
		Group_BandLev[3].add(AnalyzeRange__BandLev_FreqFrom[3].setup(">Analyze From", 47, 0, 255));
		Group_BandLev[3].add(AnalyzeRange__BandLev_FreqWidth[3].setup("Width", 71, 1, 256));
		
		Group_BandLev[3].add(LPFAlpha_dt__BandLev[3].setup(">LPF_dt Lev", 0.15, 0, 2.0));
		Group_BandLev[3].add(LPFAlpha_dt__d1_BandLev[3].setup("d1", 0.35, 0, 2.0));
		Group_BandLev[3].add(LPFAlpha_dt__d2_BandLev[3].setup("d2", 0.5, 0, 2.0));
		
		Group_BandLev[3].add(thresh_BandLev__H[3].setup(">thresh Band_H", 35e-5, 0, 1e-2)); // no use
		Group_BandLev[3].add(thresh_BandLev__H_ratio_from_MainBand[3].setup("H:x Band[wide]", 0.5, 0.01, 2.0));
		Group_BandLev[3].add(thresh_BandLev__ratio_HtoL[3].setup("ratio:HtoL", 0.8, 0, 1.0));
		
		Group_BandLev[3].add(thresh_d2BandLev__H[3].setup("d2Band_H", 6e-5, 0, 1e-3));
		Group_BandLev[3].add(thresh_d2BandLev__ratio_HtoL[3].setup("ratio:HtoL", 0.5, 0, 1.0));
		
		Group_BandLev[3].add(Val_DispMax__BandLev[3].setup(">Disp BandLev", 1.5e-3, 0, 1e-2));
		Group_BandLev[3].add(Val_d1_DispMax__BandLev[3].setup("d1", 1e-3, 0, 1e-2));
		Group_BandLev[3].add(Val_d2_DispMax__BandLev[3].setup("d2", 1e-3, 0, 1e-2));
	gui.add(&Group_BandLev[3]);
	
	Group_BandLev[4].setup("Band[4]");
	/*
		Group_BandLev[4].add(AnalyzeRange__BandLev_FreqFrom[4].setup(">Analyze From", 2, 0, 255));
		Group_BandLev[4].add(AnalyzeRange__BandLev_FreqWidth[4].setup("Width", 174, 1, 256));
	*/
		Group_BandLev[4].add(AnalyzeRange__BandLev_FreqFrom[4].setup(">Analyze From", 10, 0, 255));
		Group_BandLev[4].add(AnalyzeRange__BandLev_FreqWidth[4].setup("Width", 166, 1, 256));
		
		Group_BandLev[4].add(LPFAlpha_dt__BandLev[4].setup(">LPF_dt Lev", 1.2, 0, 2.0));
		Group_BandLev[4].add(LPFAlpha_dt__d1_BandLev[4].setup("d1", 0.25, 0, 2.0));
		Group_BandLev[4].add(LPFAlpha_dt__d2_BandLev[4].setup("d2", 0.25, 0, 2.0));
		
		Group_BandLev[4].add(thresh_BandLev__H[4].setup(">thresh Band_H", 0.00075, 0, 0.003));
		Group_BandLev[4].add(thresh_BandLev__H_ratio_from_MainBand[4].setup("H:x Band[wide]", 0.35, 0.01, 1.0)); // no use
		Group_BandLev[4].add(thresh_BandLev__ratio_HtoL[4].setup("ratio:HtoL", 0.8, 0, 1.0));
		
		Group_BandLev[4].add(thresh_d2BandLev__H[4].setup("d2Band_H", 5e-5, 0, 10e-5));
		Group_BandLev[4].add(thresh_d2BandLev__ratio_HtoL[4].setup("ratio:HtoL", 0.1, 0, 1.0));
		
		Group_BandLev[4].add(Val_DispMax__BandLev[4].setup(">Disp BandLev", 25e-4, 0, 1e-2));
		Group_BandLev[4].add(Val_d1_DispMax__BandLev[4].setup("d1", 0.0005, 0, 0.002));
		Group_BandLev[4].add(Val_d2_DispMax__BandLev[4].setup("d2", 0.0005, 0, 0.002));
		
		/* */
		Group_BandLev[4].add(thresh_MainBand_RapidDown.setup(">LayerON:thresh", 0.0003, 0, 0.001));
	gui.add(&Group_BandLev[4]);

	Group_Graph.setup("GraphParam");
		Group_Graph.add(LineWidth.setup("LineWidth", 2, 1, 3));
	gui.add(&Group_Graph);
	
	Group_LightDesign.setup("LightDesign");
		Group_LightDesign.add(BeatDetect_Rise.setup("Beat_Rise", false));
		
		Group_LightDesign.add(b_CheckDmx.setup("b_CheckDmx", true));
		Group_LightDesign.add(Dmx_TestId.setup("id(DmxTest)", -1, -1, 100));
		{
			ofColor initColor = ofColor(255, 255, 255, 255);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			Group_LightDesign.add(Dmx_TestColor.setup("test color", initColor, minColor, maxColor));
		}
		
		Group_LightDesign.add(Led_dimmer.setup("dimmer", 1.0, 0, 1.0));
		Group_LightDesign.add(PlayStart.setup("PlayStart", false));
		Group_LightDesign.add(FestMode.setup("FestMode", false));
		Group_LightDesign.add(b_Enable_LayerON.setup("Enable:LayerON", false));
		Group_LightDesign.add(b_Enable_GainSync_CG_MovingLight.setup("GainSync CG moving", true));

	gui.add(&Group_LightDesign);
	
	Group_ColorTheme.setup("ColorTheme");
		Group_ColorTheme.add(ColorTheme_id.setup("Theme", COLOR_THEME::THEME_BLUE, 0, COLOR_THEME::NUM_THEMES - 1));
		
		button_ColorTheme_Jiwari.addListener(&ColorTheme, &COLOR_THEME::set_Jiwari);
		Group_ColorTheme.add(button_ColorTheme_Jiwari.setup("Button_Jiwari"));
		
		button_ColorTheme_immediate.addListener(&ColorTheme, &COLOR_THEME::set_immediate);
		Group_ColorTheme.add(button_ColorTheme_immediate.setup("Button_immediate"));
		
		Group_ColorTheme.add(b_AutoColorChange.setup("b_Auto", false));
	gui.add(&Group_ColorTheme);
		
	
	/********************
	********************/
	gui.minimizeAll();
}

