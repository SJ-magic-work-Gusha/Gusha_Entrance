/************************************************************
************************************************************/
#include "th_fft.h"
#include "stdlib.h"

/************************************************************
************************************************************/

/******************************
******************************/
THREAD_FFT::THREAD_FFT()
: N(AUDIO_BUF_SIZE)
, LastInt(0)
{
	/********************
	********************/
	/* 窓関数 */
	fft_window.resize(N);
	for(int i = 0; i < N; i++)	fft_window[i] = 0.5 - 0.5 * cos(2 * PI * i / N);
	
	sintbl.resize(N + N/4);
	bitrev.resize(N);
	
	make_bitrev();
	make_sintbl();
	
	/********************
	********************/
	setup();
}

/******************************
******************************/
THREAD_FFT::~THREAD_FFT()
{
}

/******************************
******************************/
void THREAD_FFT::threadedFunction()
{
	while(isThreadRunning()) {
		lock();
		
		unlock();
		
		
		sleep(1);
	}
}

/******************************
******************************/
void THREAD_FFT::exit()
{
	this->lock();
	
	this->unlock();
}

/******************************
******************************/
void THREAD_FFT::setup()
{
	this->lock();
	for(int i = 0; i < AUDIO_BUF_SIZE; i++){
		Gain__Fast[i] = 0;
		Gain__Slow[i] = 0;
		
		Gain__SlowSmoothed[i] = 0;
		Gain__d_SlowSmoothed[i] = 0;
		
		ZeroCross__SlowSmoothedGain[i] = -1;
		Last_ZeroCross__SlowSmoothedGain[i] = -1;
	}
	this->unlock();
}

/******************************
******************************/
void THREAD_FFT::update()
{
	this->lock();
	
	this->unlock();
}


/******************************
******************************/
void THREAD_FFT::Log()
{
	/*
	for(int i = 0; i < AUDIO_BUF_SIZE/2; i++){
		fprintf(fp_Log, "%d,%f\n", i, ZeroCross__SlowSmoothedGain[i]);
	}
	*/
}

/******************************
******************************/
double THREAD_FFT::getBandLev(int BandId)
{
	double ret;
	
	this->lock();
		if(BandId == 0)							ret = get_ave_of_Gain(Gain__Fast, Gui_Global->AnalyzeRange__BandLev_FreqFrom[0], Gui_Global->AnalyzeRange__BandLev_FreqFrom[0] + Gui_Global->AnalyzeRange__BandLev_FreqWidth[0] - 1);
		else if(BandId < NUM_ANALYSIS_BANDS)	ret = get_ave_of_Gain(Gain__Slow, Gui_Global->AnalyzeRange__BandLev_FreqFrom[BandId], Gui_Global->AnalyzeRange__BandLev_FreqFrom[BandId] + Gui_Global->AnalyzeRange__BandLev_FreqWidth[BandId] - 1);
		else									ret = 0;
	this->unlock();
	
	return ret;
}

/******************************
return
	true	: Bad Range.
	false	: Good Range.
******************************/
bool THREAD_FFT::checkAndFix_AccessRangeOfGainArray(int& from, int& to)
{
	/********************
	********************/
	if(to < from)	return true;
	
	/********************
	********************/
	if(from < 0)					from = 0;
	if(AUDIO_BUF_SIZE/2 <= from)	from = AUDIO_BUF_SIZE/2 - 1;
	
	if(to < 0)					to = 0;
	if(AUDIO_BUF_SIZE/2 <= to)	to = AUDIO_BUF_SIZE/2 - 1;
	
	/********************
	********************/
	if(to < from)	return true;
	else			return false;
}

/******************************
description
	昇順
******************************/
int THREAD_FFT::double_sort( const void * a , const void * b )
{
	if(*(double*)a < *(double*)b){
		return -1;
	}else if(*(double*)a == *(double*)b){
		return 0;
	}else{
		return 1;
	}
}

/******************************
******************************/
double THREAD_FFT::get_CenterId_of_Gain(double* Gain, int from, int to)
{
	return get_SortAndGet_TargetId_of_Gain(Gain, from, to, 0.5);
}

/******************************
description
	昇順に並べた後、targetRatio(centerの場合、0.5)の位置に来る値を返す.
******************************/
double THREAD_FFT::get_SortAndGet_TargetId_of_Gain(double* Gain, int from, int to, double targetRatio)
{
	/********************
	********************/
	if(checkAndFix_AccessRangeOfGainArray(from, to)) return 0;
	
	/********************
	********************/
	double _Gain[AUDIO_BUF_SIZE];
	int id = 0;
	for(int i = from; i <= to; i++){
		_Gain[id] = Gain[i];
		id++;
	}
	
	if(id <= 0){
		return 0;
	}else{
		qsort(_Gain, id, sizeof( _Gain[0] ), double_sort);
		
		if(targetRatio < 0)			targetRatio = 0;
		else if(1 < targetRatio)	targetRatio = 1;
		int targetId = int((id - 1) * targetRatio);
		
		return _Gain[targetId];
	}
}

/******************************
******************************/
double THREAD_FFT::get_max_of_Gain(double* Gain, int from, int to)
{
	/********************
	********************/
	if(checkAndFix_AccessRangeOfGainArray(from, to)) return 0;
	
	/********************
	********************/
	double ret = Gain[from];
	for(int i = from + 1; i <= to; i++){
		if(ret < Gain[i]){
			ret = Gain[i];
		}
	}
	
	return ret;
}

/******************************
******************************/
double THREAD_FFT::get_ave_of_Gain(double* Gain, int from, int to)
{
	/********************
	********************/
	if(checkAndFix_AccessRangeOfGainArray(from, to)) return 0;
	
	/********************
	********************/
	double sum = 0;
	int num = 0;
	
	for(int i = from; i <= to; i++){
		sum += Gain[i];
		num++;
	}
	
	if(num == 0)	return 0;
	else			return sum / num;
}

/******************************
param
	GainType_id
		0	Gain__Fast
		1	Gain__Slow
		2	Gain__SlowSmoothed
		3	Gain__d_SlowSmoothed
******************************/
double THREAD_FFT::getArrayVal(int GainType_id, int id)
{
	if( (id < 0) || (AUDIO_BUF_SIZE/2 <= id) ) return 0;
	
	double ret = 0;
	this->lock();
	switch(GainType_id){
		case 0:
			ret = Gain__Fast[id];
			break;
			
		case 1:
			ret = Gain__Slow[id];
			break;
			
		case 2:
			ret = Gain__SlowSmoothed[id];
			break;
			
		case 3:
			ret = Gain__d_SlowSmoothed[id];
			break;
	}
	this->unlock();
	
	return ret;
}

/******************************
param
	GainType_id
		0	Gain__Fast
		1	Gain__Slow
		2	Gain__SlowSmoothed
		3	Gain__d_SlowSmoothed
******************************/
double THREAD_FFT::getArrayVal_x_DispGain(int GainType_id, int id, float Gui_DispGain)
{
	if( (id < 0) || (AUDIO_BUF_SIZE/2 <= id) ) return 0;
	
	double ret = 0;
	this->lock();
	switch(GainType_id){
		case 0:
			ret = ofMap(Gain__Fast[id], 0, Gui_DispGain, 0, 2 * ofGetHeight()/NUM_VSPLIT_DISP, true);
			break;
			
		case 1:
			ret = ofMap(Gain__Slow[id], 0, Gui_DispGain, 0, 2 * ofGetHeight()/NUM_VSPLIT_DISP, true);
			break;
			
		case 2:
			ret = ofMap(Gain__SlowSmoothed[id], 0, Gui_DispGain, 0, 2 * ofGetHeight()/NUM_VSPLIT_DISP, true);
			break;
			
		case 3:
			ret = ofMap(Gain__d_SlowSmoothed[id], -Gui_DispGain, Gui_DispGain, - ofGetHeight()/NUM_VSPLIT_DISP, ofGetHeight()/NUM_VSPLIT_DISP, true);
			break;
	}
	this->unlock();
	
	return ret;
}

/******************************
******************************/
double THREAD_FFT::getArrayVal__ZeroCross_of_SlowSmoothedGain(int id)
{
	if( (id < 0) || (AUDIO_BUF_SIZE/2 <= id) ) return -1;  // ないはずだが、一応、periodである"-1"を返しておく.
	
	double ret;
	this->lock();
	ret = ZeroCross__SlowSmoothedGain[id];
	this->unlock();
	
	return ret;
}

/******************************
******************************/
void THREAD_FFT::update__Gain(const vector<float> &AudioSample_L, const vector<float> &AudioSample_R)
{
	this->lock();
		/********************
		********************/
		float now = ofGetElapsedTimef();
		if(fp_Log_fft != NULL){
			if(now - t_MeasProcessTime < d_MeasProcessTime) fprintf(fp_Log_fft, "%f,", now);
		}
			
		/********************
		********************/
		AudioSample_fft_LPF_saveToArray(AudioSample_L, now - LastInt);
		LastInt = now;
			
		/********************
		********************/
		if(fp_Log_fft != NULL){
			if(now - t_MeasProcessTime < d_MeasProcessTime)	{ fprintf(fp_Log_fft, "%f\n", ofGetElapsedTimef()); }
			else											{ fclose(fp_Log_fft); fp_Log_fft = NULL; }
		}
	this->unlock();
}

/******************************
******************************/
void THREAD_FFT::AudioSample_fft_LPF_saveToArray(const vector<float> &AudioSample, float dt)
{
	/********************
	********************/
	if( AudioSample.size() != N ) { ERROR_MSG(); std::exit(1); }
	
	/********************
	********************/
	double x[N], y[N];
	
	for(int i = 0; i < N; i++){
		x[i] = AudioSample[i] * fft_window[i];
		y[i] = 0;
	}
	
	fft(x, y);

	/********************
	********************/
	Gain__Fast[0] = 0;
	Gain__Fast[N/2] = 0;
	Gain__Slow[0] = 0;
	Gain__Slow[N/2] = 0;
	for(int i = 1; i < N/2; i++){
		double GainTemp = 2 * sqrt(x[i] * x[i] + y[i] * y[i]);
		
		Gain__Fast[i] = LPF(Gain__Fast[i], GainTemp, Gui_Global->LPFAlpha_dt__FFTGain_Fast, dt);
		Gain__Fast[N - i] = Gain__Fast[i]; // 共役(yの正負反転)だが、Gainは同じ
		
		Gain__Slow[i] = LPF(Gain__Slow[i], GainTemp, Gui_Global->LPFAlpha_dt__FFTGain_Slow, dt);
		Gain__Slow[N - i] = Gain__Slow[i]; // 共役(yの正負反転)だが、Gainは同じ
	}
	
	/********************
	********************/
	cal_GainSlowSmoothed_dGainSlowSmoothed();
	cal__ZeroCross_of_SlowSmoothedGain();
}

/******************************
******************************/
void THREAD_FFT::cal_GainSlowSmoothed_dGainSlowSmoothed()
{
	/********************
	********************/
	Gain__SlowSmoothed[0] = Gain__Slow[0];
	Gain__d_SlowSmoothed[0] = 0;
	
	for(int i = 1; i < N/2; i++){
		Gain__SlowSmoothed[i] = LPF(Gain__SlowSmoothed[i - 1], Gain__Slow[i], Gui_Global->LPFAlpha__FFTGainSlow_SmoothInFreqSpace);
		Gain__d_SlowSmoothed[i] = Gain__SlowSmoothed[i] - Gain__SlowSmoothed[i - 1];
	}
	
	/********************
	********************/
	smooth_Array(Gain__d_SlowSmoothed, AUDIO_BUF_SIZE/2, Gui_Global->LPFAlpha__d1_FFTGainSlowSmoothed_SmoothInFreqSpace);
}

/******************************
******************************/
void THREAD_FFT::smooth_Array(double* Array, int size, double Alpha)
{
	for(int i = 1; i < size; i++){
		Array[i] = LPF(Array[i - 1], Array[i], Alpha);
	}
}

/******************************
******************************/
void THREAD_FFT::cal__ZeroCross_of_SlowSmoothedGain()
{
	/********************
	********************/
	for(int i = 0; i < AUDIO_BUF_SIZE; i++){
		Last_ZeroCross__SlowSmoothedGain[i] = ZeroCross__SlowSmoothedGain[i];
	}
	
	/********************
	********************/
	enum STATE { STATE_RISE, STATE_FALL, };
	STATE State;
	if(Gain__d_SlowSmoothed[0] <= 0)	State = STATE_FALL; // always here. ∵Gain__d_SlowSmoothed[0] == 0;
	else								State = STATE_RISE;
	
	int id = 0;
	int counter = 0;
	const int c_thresh = 2;
	
	for(int i = 1; i < AUDIO_BUF_SIZE/2; i++){
		switch(State){
			case STATE_RISE:
				if(Gain__d_SlowSmoothed[i] <= 0)	counter++;
				else								counter = 0;
				
				if(c_thresh <= counter){
					State = STATE_FALL;
					counter = 0;
					
					if(i < 3){
						/* skip. */
					}else if(Gui_Global->thresh__ValidGain_ofSlowSmoothed_onZeroCrossPoint < Gain__SlowSmoothed[i - 2]){
						ZeroCross__SlowSmoothedGain[id] = (i - 3) + ( 2 * abs(Gain__d_SlowSmoothed[i - 2]) / (abs(Gain__d_SlowSmoothed[i - 2]) + abs(Gain__d_SlowSmoothed[i - 1])) );
						id++;
					}else{
						/* nothing. */
					}
				}
				
				break;
				
			case STATE_FALL:
				if(0 < Gain__d_SlowSmoothed[i])		counter++;
				else								counter = 0;
				
				if(c_thresh <= counter){
					State = STATE_RISE;
					counter = 0;
				}
				
				break;
		}
	}
	
	ZeroCross__SlowSmoothedGain[id] = -1; // period.
}

/******************************
******************************/
int THREAD_FFT::fft(double x[], double y[], int IsReverse)
{
	/*****************
		bit反転
	*****************/
	int i, j;
	for(i = 0; i < N; i++){
		j = bitrev[i];
		if(i < j){
			double t;
			t = x[i]; x[i] = x[j]; x[j] = t;
			t = y[i]; y[i] = y[j]; y[j] = t;
		}
	}

	/*****************
		変換
	*****************/
	int n4 = N / 4;
	int k, ik, h, d, k2;
	double s, c, dx, dy;
	for(k = 1; k < N; k = k2){
		h = 0;
		k2 = k + k;
		d = N / k2;

		for(j = 0; j < k; j++){
			c = sintbl[h + n4];
			if(IsReverse)	s = -sintbl[h];
			else			s = sintbl[h];

			for(i = j; i < N; i += k2){
				ik = i + k;
				dx = s * y[ik] + c * x[ik];
				dy = c * y[ik] - s * x[ik];

				x[ik] = x[i] - dx;
				x[i] += dx;

				y[ik] = y[i] - dy;
				y[i] += dy;
			}
			h += d;
		}
	}

	/*****************
	*****************/
	if(!IsReverse){
		for(i = 0; i < N; i++){
			x[i] /= N;
			y[i] /= N;
		}
	}

	return 0;
}

/******************************
******************************/
void THREAD_FFT::make_bitrev(void)
{
	int i, j, k, n2;

	n2 = N / 2;
	i = j = 0;

	for(;;){
		bitrev[i] = j;
		if(++i >= N)	break;
		k = n2;
		while(k <= j)	{j -= k; k /= 2;}
		j += k;
	}
}

/******************************
******************************/
void THREAD_FFT::make_sintbl(void)
{
	for(int i = 0; i < N + N/4; i++){
		sintbl[i] = sin(2 * PI * i / N);
	}
}


