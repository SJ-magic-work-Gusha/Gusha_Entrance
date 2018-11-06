/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class THREAD_FFT : public ofThread, private Noncopyable{
private:
	/****************************************
	****************************************/
	/********************
	********************/
	double Gain__Fast[AUDIO_BUF_SIZE];
	double Gain__Slow[AUDIO_BUF_SIZE];
	double Gain__SlowSmoothed[AUDIO_BUF_SIZE];
	double Gain__d_SlowSmoothed[AUDIO_BUF_SIZE];
	double ZeroCross__SlowSmoothedGain[AUDIO_BUF_SIZE];
	double Last_ZeroCross__SlowSmoothedGain[AUDIO_BUF_SIZE];
	
	const int N;
	vector<float> fft_window;
	vector<double> sintbl;
	vector<int> bitrev;
	
	float LastInt;
	
	/****************************************
	function
	****************************************/
	/********************
	singleton
	********************/
	THREAD_FFT();
	~THREAD_FFT();
	THREAD_FFT(const THREAD_FFT&); // Copy constructor. no define.
	THREAD_FFT& operator=(const THREAD_FFT&); // コピー代入演算子. no define.
	
	/********************
	********************/
	void threadedFunction();
	
	int fft(double x[], double y[], int IsReverse = false);
	void make_bitrev(void);
	void make_sintbl(void);
	
	static int double_sort( const void * a , const void * b );
	double get_CenterId_of_Gain(double* Gain, int from, int to);
	double get_SortAndGet_TargetId_of_Gain(double* Gain, int from, int to, double targetRatio);
	double get_max_of_Gain(double* Gain, int from, int to);
	double get_ave_of_Gain(double* Gain, int from, int to);
	void AudioSample_fft_LPF_saveToArray(const vector<float> &AudioSample, float dt);
	bool checkAndFix_AccessRangeOfGainArray(int& from, int& to);
	void cal_GainSlowSmoothed_dGainSlowSmoothed();
	void cal__ZeroCross_of_SlowSmoothedGain();
	void smooth_Array(double* Array, int size, double Alpha);
	
public:
	/****************************************
	****************************************/
	/********************
	********************/
	static THREAD_FFT* getInstance(){
		static THREAD_FFT inst;
		return &inst;
	}
	
	void exit();
	void setup();
	void update();
	
	void update__Gain(const vector<float> &AudioSample_L, const vector<float> &AudioSample_R);
	
	
	double getBandLev(int BandId);
	
	double getArrayVal(int GainType_id, int id);
	double getArrayVal_x_DispGain(int GainType_id, int id, float Gui_DispGain);
	double getArrayVal__ZeroCross_of_SlowSmoothedGain(int id);
	
	void Log();
};



