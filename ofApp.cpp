/************************************************************
************************************************************/
#include "ofApp.h"
#include <time.h>

/************************************************************
************************************************************/
static COLOR_DYNAMIC_ALPHA col_white(ofColor(255, 255, 255, 255));
static COLOR_DYNAMIC_ALPHA col_gray(ofColor(80, 80, 80, 80));
static COLOR_DYNAMIC_ALPHA col_red(ofColor(255, 0, 0, 255));
static COLOR_DYNAMIC_ALPHA col_green(ofColor(0, 255, 0, 255));
static COLOR_DYNAMIC_ALPHA col_blue(ofColor(0, 80, 255, 255));
static COLOR_DYNAMIC_ALPHA col_cian(ofColor(0, 255, 255, 255));
static COLOR_DYNAMIC_ALPHA col_pink(ofColor(255, 0, 255, 255));
static COLOR_DYNAMIC_ALPHA col_purple(ofColor(100, 0, 255, 255));
static COLOR_DYNAMIC_ALPHA col_yellow(ofColor(255, 255, 0, 255));
static COLOR_DYNAMIC_ALPHA col_orange(ofColor(255, 120, 0, 255));

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _soundStream_Input_DeviceId, int _soundStream_Output_DeviceId, bool _b_EnableProcess_CheckLightDesign, bool _b_EnableProcess_Light, bool _b_EnableProcess_vj)
: soundStream_Input_DeviceId(_soundStream_Input_DeviceId)
, soundStream_Output_DeviceId(_soundStream_Output_DeviceId)
, b_DispGui(true)
, b_ShowFrameRate(false)
, fft_thread(THREAD_FFT::getInstance())
, png_id(0)
, b_PauseGraph(false)
, LastInt(0)
, ofs_x_ReadCursor(0)
, col_Back_0(0, 0, 0, 0)
, col_Back_1(10, 10, 10, 0)
, t_Beat_Band3(-1)
, b_Beat_Band3(false)
, Light(LIGHT::getInstance())
, b_EnableAudioOut(false)
{
	/********************
	placement new
	********************/
	new( StateChart_BandGain + 0 ) STATECHART_BANDGAIN(0, 1.0);
	for ( int i = 1; i < NUM_ANALYSIS_BANDS; i++ ) {
		new( StateChart_BandGain + i ) STATECHART_BANDGAIN(1.0, 1.0);
	}
	
	for ( int i = 0; i < NUM_ANALYSIS_BANDS; i++ ) {
		// new( StateChart_d1_BandGain + i ) STATECHART_D1_BANDGAIN(2.0, 1.5, 0.1); // float _d_echo, float _t_thresh_LtoH, float _t_thresh_HtoL
		new( StateChart_d1_BandGain + i ) STATECHART_D1_BANDGAIN(0.0, 1.5, 0.1); // float _d_echo, float _t_thresh_LtoH, float _t_thresh_HtoL
	}
	
	/********************
	********************/
	b_EnableProcess_CheckLightDesign = _b_EnableProcess_CheckLightDesign;
	b_EnableProcess_Light = _b_EnableProcess_Light;
	b_EnableProcess_vj = _b_EnableProcess_vj;
	if(b_EnableProcess_vj) b_EnableProcess_Light = true; // LIGHT classからUdp送るので.Dmxは、相手がいなくても動作に問題なし.

	
	fp_Log = fopen("../../../data/Log.csv", "w");
	
	/********************
	********************/
	srand((unsigned) time(NULL));
	
	/********************
	********************/
	font[FONT_S].load("RictyDiminished-Regular.ttf", 8, true, true, true);
	font[FONT_M].load("RictyDiminished-Regular.ttf", 15, true, true, true);
	font[FONT_L].load("RictyDiminished-Regular.ttf", 25, true, true, true);
	
	/********************
	********************/
	for(int i = 0; i < NUM_GRAPH_FREQBASED; i++) { Vboset_FreqBased[i].setup(AUDIO_BUF_SIZE/2 * 4); /* square */ }
	
	for(int i = 0; i < NUM_ANALYSIS_BANDS; i++){
		for(int j = 0; j < NUM_GRAPH_TIMEBASED; j++) { Vboset_TimeBased[i][j].setup(NUM_TIME_POINTS); }
	}
	
	/********************
	FreqBased Graphは、都度 全ての値を算出するが、
	TimeBased Graphは、slideしながら表示なので、初期化が必要.
	********************/
	for(int i = 0; i < NUM_ANALYSIS_BANDS; i++){
		for(int j = 0; j < NUM_GRAPH_TIMEBASED; j++){
			for(int k = 0; k < NUM_TIME_POINTS; k++){
				Vboset_TimeBased[i][j].VboVerts[k].set(k, 0);
				GraphTimeBased_Log_Raw[i][j][k] = 0;
			}
		}
	}
}

/******************************
******************************/
ofApp::~ofApp()
{
	if(fp_Log)		fclose(fp_Log);
	if(fp_Log_main)	fclose(fp_Log_main);
	if(fp_Log_fft)	fclose(fp_Log_fft);
}

/******************************
******************************/
void ofApp::exit()
{
	/********************
	ofAppとaudioが別threadなので、ここで止めておくのが安全.
	********************/
	soundStream.stop();
	soundStream.close();
	
	/********************
	********************/
	fft_thread->exit();
	try{
		/********************
		stop済みのthreadをさらにstopすると、Errorが出るようだ。
		********************/
		while(fft_thread->isThreadRunning()){
			fft_thread->waitForThread(true);
		}
		
	}catch(...){
		printf("Thread exiting Error\n");
	}
	
	/********************
	********************/
	if(b_EnableProcess_Light || b_EnableProcess_vj) Light->exit();
	
	/********************
	********************/
	printf("\n> Good bye\n");
}


//--------------------------------------------------------------
void ofApp::setup(){
	/********************
	********************/
	if(b_EnableProcess_CheckLightDesign){
		Light->test();
		ofExit();
		return;
	}
	
	/********************
	********************/
	ofSetBackgroundAuto(true);
	
	ofSetWindowTitle("Gusha:Entrance");
	// ofSetVerticalSync(false);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	/********************
	********************/
	mainOutputSyphonServer.setName("Gusha_Entrance_ScreenOutput"); // server name
	
	/********************
	********************/
	setup_Gui();
	Guis_LoadSetting();
	
	/********************
	********************/
	// soundStream.setDeviceID(soundStream_DeviceId);
	/* set in & out respectively. */
	vector<ofSoundDevice> SoundStreamLists = soundStream.listDevices();
	if( soundStream_Input_DeviceId == -1 ){
		ofExit();
		return;
		
	}else{
		soundStream.setInDeviceID(soundStream_Input_DeviceId);
		
		if( soundStream_Output_DeviceId != -1 ){
			if(SoundStreamLists[soundStream_Output_DeviceId].name == "Apple Inc.: Built-in Output"){
				printf("!!!!! prohibited to use [%s] for output ... by SJ for safety !!!!!\n", SoundStreamLists[soundStream_Output_DeviceId].name.c_str());
			}else{
				soundStream.setOutDeviceID(soundStream_Output_DeviceId);
				b_EnableAudioOut = true;
			}
		}
	}
	
	AudioSample.resize(AUDIO_BUF_SIZE);
	
	/********************
	********************/
	fft_thread->setup();
	
	/********************
	********************/
	RefreshVerts();
	Refresh_BarColor();
	
	/********************
	soundStream.setup()の位置に注意:最後
		audioIn()/audioOut()がstartする.
		これらのmethodは、fft_threadにaccessするので、start前にReStart()によって、fft_threadが初期化されていないと、不正accessが発生してしまう.
		
	out/in chs
		今回は、audioOut()から出力する音はないので、検討時に誤ってLoopハウリングを起こすなどの危険性に対する安全も考慮し、out ch = 0.とした.
	********************/
	if(b_EnableAudioOut)	soundStream.setup(this, 2/* out */, 2/* in */, AUDIO_SAMPLERATE, AUDIO_BUF_SIZE, AUDIO_BUFFERS);
	else					soundStream.setup(this, 0/* out */, 2/* in */, AUDIO_SAMPLERATE, AUDIO_BUF_SIZE, AUDIO_BUFFERS);
	
	/********************
	********************/
	if(b_EnableProcess_Light || b_EnableProcess_vj){
		Light->setup(GetState_BandGain(), GraphTimeBased_Log_Raw[3][GRAPH_TIMEBASED__LH__d2][NUM_TIME_POINTS - 1]);
	}
}

/******************************
******************************/
int ofApp::GetState_BandGain()
{
	if(!Gui_Global->PlayStart){
		return 0;
	}else{
		return (int(GraphTimeBased_Log_Raw[4][GRAPH_TIMEBASED__LH__RAW][NUM_TIME_POINTS - 1]) << 1) + (int(GraphTimeBased_Log_Raw[0][GRAPH_TIMEBASED__LH__RAW][NUM_TIME_POINTS - 1]) << 0) + 1;
	}
}

/******************************
******************************/
void ofApp::RefreshVerts()
{
	/********************
	********************/
	if(b_PauseGraph) return;

	/********************
	********************/
	float BarWidth = GRAPH_BAR_WIDTH__FFT_GAIN;
	float BarSpace = GRAPH_BAR_SPACE__FFT_GAIN;
	
	/********************
	********************/
	for(int i = 0; i < GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED; i++){
		float Gui_DispGain;
		switch(i){
			case GRAPH_FREQBASED__FFTGAIN_FAST:
				Gui_DispGain = Gui_Global->Val_DispMax__FFTGain_Fast;
				break;
			case GRAPH_FREQBASED__FFTGAIN_SLOW:
				Gui_DispGain = Gui_Global->Val_DispMax__FFTGain_Slow;
				break;
			case GRAPH_FREQBASED__FFTGAIN_SLOW_SMOOTHED:
				Gui_DispGain = Gui_Global->Val_DispMax__FFTGain_SlowSmoothed;
				break;
			case GRAPH_FREQBASED__d1_FFTGAIN_SLOW_SMOOTHED:
				Gui_DispGain = Gui_Global->Val_DispMax__FFTGain_d1_SlowSmoothed;
				break;
		}
		
		for(int j = 0; j < AUDIO_BUF_SIZE/2; j++){
			Vboset_FreqBased[i].VboVerts[j * 4 + 0].set( BarSpace * j            , 0 );
			Vboset_FreqBased[i].VboVerts[j * 4 + 1].set( BarSpace * j            , fft_thread->getArrayVal_x_DispGain(i, j, Gui_DispGain) );
			Vboset_FreqBased[i].VboVerts[j * 4 + 2].set( BarSpace * j  + BarWidth, fft_thread->getArrayVal_x_DispGain(i, j, Gui_DispGain) );
			Vboset_FreqBased[i].VboVerts[j * 4 + 3].set( BarSpace * j  + BarWidth, 0 );
		}
	}
	
	/********************
	********************/
	{
		int id = 0;
		for(int i = 0; i < AUDIO_BUF_SIZE/2 - 1; i++){
			
			if( (i <= fft_thread->getArrayVal__ZeroCross_of_SlowSmoothedGain(id)) && (fft_thread->getArrayVal__ZeroCross_of_SlowSmoothedGain(id) < i + 1) ){
				Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[i * 4 + 0].set( BarSpace * fft_thread->getArrayVal__ZeroCross_of_SlowSmoothedGain(id)           , 0 );
				Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[i * 4 + 1].set( BarSpace * fft_thread->getArrayVal__ZeroCross_of_SlowSmoothedGain(id)           , ofGetHeight()/NUM_VSPLIT_DISP/2 );
				Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[i * 4 + 2].set( BarSpace * fft_thread->getArrayVal__ZeroCross_of_SlowSmoothedGain(id) + BarWidth, ofGetHeight()/NUM_VSPLIT_DISP/2 );
				Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[i * 4 + 3].set( BarSpace * fft_thread->getArrayVal__ZeroCross_of_SlowSmoothedGain(id) + BarWidth, 0 );
				
			}else{
				Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[i * 4 + 0].set( BarSpace * i           , 0 );
				Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[i * 4 + 1].set( BarSpace * i           , 0 );
				Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[i * 4 + 2].set( BarSpace * i + BarWidth, 0 );
				Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[i * 4 + 3].set( BarSpace * i + BarWidth, 0 );
			}
			
			while( (fft_thread->getArrayVal__ZeroCross_of_SlowSmoothedGain(id) != -1) && (fft_thread->getArrayVal__ZeroCross_of_SlowSmoothedGain(id) < (i + 1)) ) id++;
		}
		
		Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[(AUDIO_BUF_SIZE/2 - 1) * 4 + 0].set( BarSpace * (AUDIO_BUF_SIZE/2 - 1)           , 0 );
		Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[(AUDIO_BUF_SIZE/2 - 1) * 4 + 1].set( BarSpace * (AUDIO_BUF_SIZE/2 - 1)           , 0 );
		Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[(AUDIO_BUF_SIZE/2 - 1) * 4 + 2].set( BarSpace * (AUDIO_BUF_SIZE/2 - 1) + BarWidth, 0 );
		Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[(AUDIO_BUF_SIZE/2 - 1) * 4 + 3].set( BarSpace * (AUDIO_BUF_SIZE/2 - 1) + BarWidth, 0 );
	}
	
	/********************
	********************/
	for(int i = 0; i < NUM_ANALYSIS_BANDS; i++){
		for(int j = 0; j < NUM_GRAPH_TIMEBASED; j++){
			float dataFrom, dataTo, GraphFrom, GraphTo;
			TimaBasedGraph__get__Data_and_Graph_Range(i, GRAPH_TIMEBASED(j), dataFrom, dataTo, GraphFrom, GraphTo);
			
			for(int k = 0; k < NUM_TIME_POINTS; k++){
				Vboset_TimeBased[i][j].VboVerts[k].set(k, ofMap(GraphTimeBased_Log_Raw[i][j][k], dataFrom, dataTo, GraphFrom, GraphTo, true));
			}
		}
	}
}

/******************************
******************************/
void ofApp::TimaBasedGraph__get__Data_and_Graph_Range(int BandId, GRAPH_TIMEBASED TimeBasedGraph_Id, float& dataFrom, float& dataTo, float& GraphFrom, float& GraphTo)
{
	switch(TimeBasedGraph_Id){
		case GRAPH_TIMEBASED__BANDLEV__RAW:
		case GRAPH_TIMEBASED__RAW_DYNAMIC_THRESH__H:
		case GRAPH_TIMEBASED__RAW_DYNAMIC_THRESH__L:
			dataFrom	= 0;
			dataTo		= Gui_Global->Val_DispMax__BandLev[BandId];
			GraphFrom	= 0;
			GraphTo		= 2 * ofGetHeight()/NUM_VSPLIT_DISP;
			break;
			
		case GRAPH_TIMEBASED__BANDLEV__d1:
			dataFrom	= -Gui_Global->Val_d1_DispMax__BandLev[BandId];
			dataTo		= Gui_Global->Val_d1_DispMax__BandLev[BandId];
			GraphFrom	= -ofGetHeight()/NUM_VSPLIT_DISP;
			GraphTo		= ofGetHeight()/NUM_VSPLIT_DISP;
			break;
			
		case GRAPH_TIMEBASED__BANDLEV__d2:
			dataFrom	= -Gui_Global->Val_d2_DispMax__BandLev[BandId];
			dataTo		= Gui_Global->Val_d2_DispMax__BandLev[BandId];
			GraphFrom	= -ofGetHeight()/NUM_VSPLIT_DISP;
			GraphTo		= ofGetHeight()/NUM_VSPLIT_DISP;
			break;
		
		case GRAPH_TIMEBASED__LH__RAW:
			dataFrom	= 0;
			dataTo		= 6; // "0 - dataTo" のRangeに "1" をplot.
			GraphFrom	= 0;
			GraphTo		= ofGetHeight()/NUM_VSPLIT_DISP;
			break;
			
		case GRAPH_TIMEBASED__LH__d1:
			dataFrom	= 0;
			dataTo		= 6;
			GraphFrom	= -ofGetHeight()/NUM_VSPLIT_DISP;
			GraphTo		= 0;
			break;
			
		case GRAPH_TIMEBASED__LH__d2:
			dataFrom	= 0;
			dataTo		= 6;
			GraphFrom	= -ofGetHeight()/NUM_VSPLIT_DISP;
			GraphTo		= 0;
			break;
			
		case GRAPH_TIMEBASED__d2AMP_H:
			dataFrom	= -Gui_Global->Val_d2_DispMax__BandLev[BandId];
			dataTo		= Gui_Global->Val_d2_DispMax__BandLev[BandId];
			GraphFrom	= -ofGetHeight()/NUM_VSPLIT_DISP;
			GraphTo		= ofGetHeight()/NUM_VSPLIT_DISP;
			break;
			
		case GRAPH_TIMEBASED__d2AMP_L:
			dataFrom	= -Gui_Global->Val_d2_DispMax__BandLev[BandId];
			dataTo		= Gui_Global->Val_d2_DispMax__BandLev[BandId];
			GraphFrom	= -ofGetHeight()/NUM_VSPLIT_DISP;
			GraphTo		= ofGetHeight()/NUM_VSPLIT_DISP;
			break;
			
		default:
			ERROR_MSG();
			std::exit(1);
			break;
	}
}

/******************************
******************************/
void ofApp::TimaBasedGraph_getVal_ReverseFromVbo(int BandId, GRAPH_TIMEBASED TimeBasedGraph_Id, int ArrayId, char* buf)
{
	/********************
	********************/
	if(GRAPH_TIMEBASED__LH__RAW <= TimeBasedGraph_Id)	return;

	/********************
	********************/
	if( (ArrayId < 0) || (Vboset_TimeBased[BandId][TimeBasedGraph_Id].VboVerts.size() <= ArrayId) ){
		sprintf(buf, "---");
	}else{
		ofVec3f VboVal = Vboset_TimeBased[BandId][TimeBasedGraph_Id].VboVerts[ArrayId];

		float dataFrom, dataTo, GraphFrom, GraphTo;
		TimaBasedGraph__get__Data_and_Graph_Range(BandId, GRAPH_TIMEBASED(TimeBasedGraph_Id), dataFrom, dataTo, GraphFrom, GraphTo);
		
		double val = ofMap(VboVal.y, GraphFrom, GraphTo, dataFrom, dataTo);
		sprintf(buf, "%4d, %e", ArrayId, val);
	}
}

/******************************
******************************/
bool ofApp::IsHigh_TimeBasedGraph_ReverseFromVbo(int BandId, GRAPH_TIMEBASED TimeBasedGraph_Id, int ArrayId)
{
	/********************
	********************/
	if(TimeBasedGraph_Id < GRAPH_TIMEBASED__LH__RAW)	return false;

	/********************
	********************/
	if( (ArrayId < 0) || (Vboset_TimeBased[BandId][TimeBasedGraph_Id].VboVerts.size() <= ArrayId) ){
		return false;
	}else{
		ofVec3f VboVal = Vboset_TimeBased[BandId][TimeBasedGraph_Id].VboVerts[ArrayId];
		
		float dataFrom, dataTo, GraphFrom, GraphTo;
		TimaBasedGraph__get__Data_and_Graph_Range(BandId, GRAPH_TIMEBASED(TimeBasedGraph_Id), dataFrom, dataTo, GraphFrom, GraphTo);
		
		double val = ofMap(VboVal.y, GraphFrom, GraphTo, dataFrom, dataTo);
		
		if(val != 0)	return true;
		else			return false;
	}
}

/******************************
******************************/
void ofApp::FreqBasedGraph_getVal_ReverseFromVbo(GRAPH_FREQBASED FreqBasedGraph_Id, float mouse_x, char* buf)
{
	/********************
	********************/
	if((FreqBasedGraph_Id < 0) || (GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED <= FreqBasedGraph_Id)){
		ERROR_MSG(); std::exit(1);
	}

	/********************
	********************/
	const float BarSpace = GRAPH_BAR_SPACE__FFT_GAIN;
	const int FreqId = int(mouse_x/BarSpace);
	
	if((FreqId < 0) || (AUDIO_BUF_SIZE/2 <= FreqId)){
		sprintf(buf, "---");
	}else{
		ofVec3f VboVal = Vboset_FreqBased[FreqBasedGraph_Id].VboVerts[FreqId * 4 + 1];
		
		double val;
		switch(FreqBasedGraph_Id){
			case GRAPH_FREQBASED__FFTGAIN_FAST:
				val = ofMap(VboVal.y, 0, 2 * ofGetHeight()/NUM_VSPLIT_DISP, 0, Gui_Global->Val_DispMax__FFTGain_Fast);
				break;
				
			case GRAPH_FREQBASED__FFTGAIN_SLOW:
				val = ofMap(VboVal.y, 0, 2 * ofGetHeight()/NUM_VSPLIT_DISP, 0, Gui_Global->Val_DispMax__FFTGain_Slow);
				break;
				
			case GRAPH_FREQBASED__FFTGAIN_SLOW_SMOOTHED:
				val = ofMap(VboVal.y, 0, 2 * ofGetHeight()/NUM_VSPLIT_DISP, 0, Gui_Global->Val_DispMax__FFTGain_SlowSmoothed);
				break;
				
			case GRAPH_FREQBASED__d1_FFTGAIN_SLOW_SMOOTHED:
				val = ofMap(VboVal.y, -ofGetHeight()/NUM_VSPLIT_DISP, ofGetHeight()/NUM_VSPLIT_DISP, -Gui_Global->Val_DispMax__FFTGain_d1_SlowSmoothed, Gui_Global->Val_DispMax__FFTGain_d1_SlowSmoothed);
				break;
				
			default:
				ERROR_MSG();
				std::exit(1);
				break;
		}
		
		sprintf(buf, "%3d, %e", FreqId, val);
	}
}

/******************************
******************************/
bool ofApp::IsZeroCross_ReverseFromVboGraph(float mouse_x, char* buf)
{
	/********************
	********************/
	const float BarSpace = GRAPH_BAR_SPACE__FFT_GAIN;
	const int FreqId = int(mouse_x/BarSpace);
	
	if((FreqId < 0) || (AUDIO_BUF_SIZE/2 <= FreqId)){
		return false;
	}else{
		ofVec3f VboVal = Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].VboVerts[FreqId * 4 + 1];
		
		
		if(0 < VboVal.y){
			if(buf != NULL) sprintf(buf, "%f", VboVal.x/double(GRAPH_BAR_SPACE__FFT_GAIN));
			return true;
		}else{
			return false;
		}
	}
}

/******************************
******************************/
void ofApp::Refresh_BarColor()
{
	/********************
	********************/
	Vboset_FreqBased[GRAPH_FREQBASED__FFTGAIN_FAST].set_singleColor(col_white.GetColor(100));
	Vboset_FreqBased[GRAPH_FREQBASED__FFTGAIN_SLOW].set_singleColor(col_white.GetColor(100));
	Vboset_FreqBased[GRAPH_FREQBASED__FFTGAIN_SLOW_SMOOTHED].set_singleColor(col_white.GetColor(100));
	Vboset_FreqBased[GRAPH_FREQBASED__d1_FFTGAIN_SLOW_SMOOTHED].set_singleColor(col_white.GetColor(100));
	Vboset_FreqBased[GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED].set_singleColor(col_purple.GetColor(255));
	
	/********************
	********************/
	for(int i = 0; i < NUM_ANALYSIS_BANDS; i++){
		for(int j = 0; j < NUM_GRAPH_TIMEBASED; j++){
			if( (j == GRAPH_TIMEBASED__LH__RAW) || (j == GRAPH_TIMEBASED__LH__d1) || (j == GRAPH_TIMEBASED__LH__d2) ){
				Vboset_TimeBased[i][j].set_singleColor(col_purple.GetColor(255));
			}else if( (j == GRAPH_TIMEBASED__d2AMP_H) || (j == GRAPH_TIMEBASED__d2AMP_L) ){
				Vboset_TimeBased[i][j].set_singleColor(col_red.GetColor(150));
			}else if( j == GRAPH_TIMEBASED__RAW_DYNAMIC_THRESH__H ){
				Vboset_TimeBased[i][j].set_singleColor(col_green.GetColor(200));
			}else if( j == GRAPH_TIMEBASED__RAW_DYNAMIC_THRESH__L ){
				Vboset_TimeBased[i][j].set_singleColor(col_yellow.GetColor(200));
			}else{
				switch(i){
					case 0:
						Vboset_TimeBased[i][j].set_singleColor(col_green.GetColor(255));
						break;
					case 1:
						Vboset_TimeBased[i][j].set_singleColor(col_blue.GetColor(255));
						break;
					case 2:
						Vboset_TimeBased[i][j].set_singleColor(col_cian.GetColor(255));
						break;
					case 3:
						Vboset_TimeBased[i][j].set_singleColor(col_orange.GetColor(255));
						break;
					case 4:
						Vboset_TimeBased[i][j].set_singleColor(col_pink.GetColor(255));
						break;
				}
			}
		}
	}
}

/******************************
description
	memoryを確保は、app start後にしないと、
	segmentation faultになってしまった。
******************************/
void ofApp::setup_Gui()
{
	/********************
	********************/
	Gui_Global = new GUI_GLOBAL;
	Gui_Global->setup("Gusha:Entrance", "gui.xml", 1000, 10);
}

/******************************
******************************/
void ofApp::Guis_LoadSetting()
{
	/********************
	********************/
	printf("\n> Gui Load setting\n");
	
	/********************
	********************/
	string FileName;
	
	/********************
	********************/
	FileName = "gui.xml";
	if(checkif_FileExist(FileName.c_str())) Gui_Global->gui.loadFromFile(FileName.c_str());
	
	printf("\n");
}

/******************************
******************************/
void ofApp::remove_xml()
{
	ofFile::removeFile("gui.xml");
}

/******************************
******************************/
bool ofApp::checkif_FileExist(const char* FileName)
{
	if(ofFile::doesFileExist(FileName)){
		printf("loaded file of %s\n", FileName);
		return true;
		
	}else{
		printf("%s not exist\n", FileName);
		return false;
	}
}

/******************************
******************************/
void ofApp::update_StateLH(int BandId){
	/********************
	********************/
	static float Last__thresh_BandLev__H[NUM_ANALYSIS_BANDS];
	static float Last__thresh_BandLev__H_ratio_from_MainBand[NUM_ANALYSIS_BANDS];
	static float Last__thresh_BandLev__ratio_HtoL[NUM_ANALYSIS_BANDS];
	
	/********************
	********************/
	if(	(Last__thresh_BandLev__H[BandId] != Gui_Global->thresh_BandLev__H[BandId]) || 
		(Last__thresh_BandLev__H_ratio_from_MainBand[BandId] != Gui_Global->thresh_BandLev__H_ratio_from_MainBand[BandId]) ||
		(Last__thresh_BandLev__ratio_HtoL[BandId] != Gui_Global->thresh_BandLev__ratio_HtoL[BandId]) ){
		StateChart_BandGain[BandId].OnGuiChanged(now);
	}
	Last__thresh_BandLev__H[BandId] = Gui_Global->thresh_BandLev__H[BandId];
	Last__thresh_BandLev__H_ratio_from_MainBand[BandId] = Gui_Global->thresh_BandLev__H_ratio_from_MainBand[BandId];
	Last__thresh_BandLev__ratio_HtoL[BandId] = Gui_Global->thresh_BandLev__ratio_HtoL[BandId];
	
	/********************
	********************/
	float thresh_BandLev__H, thresh_BandLev__L;
	
	if(BandId == MAINBAND_ID){
		thresh_BandLev__H = Gui_Global->thresh_BandLev__H[BandId];
	}else{
		thresh_BandLev__H = sj_max(GraphTimeBased_Log_Raw[MAINBAND_ID][GRAPH_TIMEBASED__BANDLEV__RAW][NUM_TIME_POINTS - 1], Gui_Global->thresh_BandLev__H[MAINBAND_ID]) * Gui_Global->thresh_BandLev__H_ratio_from_MainBand[BandId];
	}
	thresh_BandLev__L = thresh_BandLev__H * Gui_Global->thresh_BandLev__ratio_HtoL[BandId];
	RefreshData__GraphTimeBased_Log( GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__RAW_DYNAMIC_THRESH__H], thresh_BandLev__H );
	RefreshData__GraphTimeBased_Log( GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__RAW_DYNAMIC_THRESH__L], thresh_BandLev__L );
	
	StateChart_BandGain[BandId].SetThresh(now, thresh_BandLev__H, thresh_BandLev__L);
	StateChart_BandGain[BandId].update(now, GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__BANDLEV__RAW][NUM_TIME_POINTS - 1], GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__BANDLEV__d2][NUM_TIME_POINTS - 1]);
	
	if(StateChart_BandGain[BandId].GetState(now))	RefreshData__GraphTimeBased_Log(GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__LH__RAW], 1);
	else											RefreshData__GraphTimeBased_Log(GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__LH__RAW], 0);
	
	/********************
	********************/
	StateChart_d1_BandGain[BandId].update(now, GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__BANDLEV__d1][NUM_TIME_POINTS - 1], GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__BANDLEV__RAW][NUM_TIME_POINTS - 1]);
	if(StateChart_d1_BandGain[BandId].GetState())	RefreshData__GraphTimeBased_Log(GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__LH__d1], 1);
	else											RefreshData__GraphTimeBased_Log(GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__LH__d1], 0);
	
	/********************
	********************/
	float thresh_d2BandLev__L = Gui_Global->thresh_d2BandLev__H[BandId] * Gui_Global->thresh_d2BandLev__ratio_HtoL[BandId];
	StateChart_d2_BandGain[BandId].SetThresh(now, Gui_Global->thresh_d2BandLev__H[BandId], -thresh_d2BandLev__L); // 注) L側はマイナスに反転する.
	StateChart_d2_BandGain[BandId].update(now, GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__BANDLEV__d2][NUM_TIME_POINTS - 1]);
	if(StateChart_d2_BandGain[BandId].GetState(now))	RefreshData__GraphTimeBased_Log(GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__LH__d2], 1);
	else												RefreshData__GraphTimeBased_Log(GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__LH__d2], 0);
	
	RefreshData__GraphTimeBased_Log( GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__d2AMP_H], StateChart_BandGain[BandId].Get_d2Amp() );
	RefreshData__GraphTimeBased_Log( GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__d2AMP_L], -StateChart_BandGain[BandId].Get_d2Amp() );
}

/******************************
******************************/
void ofApp::update(){
	/********************
	********************/
	now = ofGetElapsedTimef();
	
	if(fp_Log_main != NULL){
		if(now - t_MeasProcessTime < d_MeasProcessTime) fprintf(fp_Log_main, "%f,", now);
	}
	
	/********************
	********************/
	fft_thread->update();
	
	/********************
	********************/
	for(int i = 0; i < NUM_ANALYSIS_BANDS; i++){
		RefreshData__GraphTimeBased_Log( GraphTimeBased_Log_Raw[i][GRAPH_TIMEBASED__BANDLEV__RAW], fft_thread->getBandLev(i), Gui_Global->LPFAlpha_dt__BandLev[i] );
		if((LastInt < now) && (now - LastInt < 0.1)){
			RefreshData__GraphTimeBased_Log(	GraphTimeBased_Log_Raw[i][GRAPH_TIMEBASED__BANDLEV__d1], 
												(GraphTimeBased_Log_Raw[i][GRAPH_TIMEBASED__BANDLEV__RAW][NUM_TIME_POINTS - 1] - GraphTimeBased_Log_Raw[i][GRAPH_TIMEBASED__BANDLEV__RAW][NUM_TIME_POINTS - 2]) / (now - LastInt),
												Gui_Global->LPFAlpha_dt__d1_BandLev[i]);
			RefreshData__GraphTimeBased_Log(	GraphTimeBased_Log_Raw[i][GRAPH_TIMEBASED__BANDLEV__d2], 
												(GraphTimeBased_Log_Raw[i][GRAPH_TIMEBASED__BANDLEV__d1][NUM_TIME_POINTS - 1] - GraphTimeBased_Log_Raw[i][GRAPH_TIMEBASED__BANDLEV__d1][NUM_TIME_POINTS - 2]) / (now - LastInt),
												Gui_Global->LPFAlpha_dt__d2_BandLev[i]);
		}
	}
	
	/********************
	********************/
	for(int i = 0; i < NUM_ANALYSIS_BANDS; i++){
		update_StateLH(i);
	}
	
	/********************
	********************/
	for(int i = 0; i < NUM_ANALYSIS_BANDS; i++){
		StateChart_BeatLock[i].update(now, GraphTimeBased_Log_Raw[i][GRAPH_TIMEBASED__LH__RAW][NUM_TIME_POINTS - 1], GraphTimeBased_Log_Raw[i][GRAPH_TIMEBASED__LH__d2][NUM_TIME_POINTS - 1]);
	}

	/********************
	parameter for Design of Lighting.
	********************/
	if(b_EnableProcess_Light || b_EnableProcess_vj){
		Light->update(	GetState_BandGain(), IsBoost(), Is_LayerON_Go(), GraphTimeBased_Log_Raw[3][GRAPH_TIMEBASED__LH__d2][NUM_TIME_POINTS - 1], 
						StateChart_BandGain[0].GetLum_GainSync(now), StateChart_BeatLock[0].IsLock(), StateChart_BeatLock[0].get_BeatInterval(), StateChart_BeatLock[0].get__t_LastBeat(),
						b_Beat_Band3
						);
	}
	
	/********************
	********************/
	LastInt = now;
	
	if(fp_Log_main != NULL){
		if(now - t_MeasProcessTime < d_MeasProcessTime) fprintf(fp_Log_main, "%f,", ofGetElapsedTimef());
	}
}

/******************************
******************************/
void ofApp::RefreshData__GraphTimeBased_Log(double* Array_Log, double NewData, double dt_of_Alpha){
	for(int i = 0; i < NUM_TIME_POINTS - 1; i++){
		Array_Log[i] = Array_Log[i + 1];
	}
	
	Array_Log[NUM_TIME_POINTS - 1] = LPF(Array_Log[NUM_TIME_POINTS - 2], NewData, dt_of_Alpha, now - LastInt);
}

/******************************
******************************/
void ofApp::RefreshData__GraphTimeBased_Log(double* Array_Log, double NewData){
	for(int i = 0; i < NUM_TIME_POINTS - 1; i++){
		Array_Log[i] = Array_Log[i + 1];
	}
	
	Array_Log[NUM_TIME_POINTS - 1] = NewData;
}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	if(fp_Log_main != NULL){
		if(now - t_MeasProcessTime < d_MeasProcessTime) fprintf(fp_Log_main, "%f,", ofGetElapsedTimef());
	}
	
	/********************
	********************/
	RefreshVerts();
	Refresh_BarColor();
	
	/********************
	以下は、audioOutからの呼び出しだと segmentation fault となってしまった.
	********************/
	for(int i = 0; i < NUM_GRAPH_FREQBASED; i++) { Vboset_FreqBased[i].update(); }
	for(int i = 0; i < NUM_ANALYSIS_BANDS; i++){
		for(int j = 0; j < NUM_GRAPH_TIMEBASED; j++){
			Vboset_TimeBased[i][j].update();
		}
	}
 	
	/********************
	********************/
	// Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ofBackground(0);
	
	/********************
	********************/
	ofRectangle rect_Back;
	ofPoint Coord_zero;
	float y_max;
	
	rect_Back.x = 0;	rect_Back.y = ofGetHeight()*0/NUM_VSPLIT_DISP;	rect_Back.width = WIDTH__GRAPH_FREQBASED - 1;	rect_Back.height = ofGetHeight() * 2/NUM_VSPLIT_DISP;
	Coord_zero.x = 0;	Coord_zero.y = ofGetHeight()*2/NUM_VSPLIT_DISP;
	y_max = 2 * ofGetHeight()/NUM_VSPLIT_DISP;
	draw_FreqBasedGraph(GRAPH_FREQBASED__FFTGAIN_FAST, col_Back_0, rect_Back, Coord_zero, y_max, Gui_Global->Val_DispMax__FFTGain_Fast);
	
	rect_Back.x = 0;	rect_Back.y = ofGetHeight()*2/NUM_VSPLIT_DISP;	rect_Back.width = WIDTH__GRAPH_FREQBASED - 1;	rect_Back.height = ofGetHeight() * 2/NUM_VSPLIT_DISP;
	Coord_zero.x = 0;	Coord_zero.y = ofGetHeight()*4/NUM_VSPLIT_DISP;
	y_max = 2 * ofGetHeight()/NUM_VSPLIT_DISP;
	draw_FreqBasedGraph(GRAPH_FREQBASED__FFTGAIN_SLOW, col_Back_1, rect_Back, Coord_zero, y_max, Gui_Global->Val_DispMax__FFTGain_Slow);
	
	rect_Back.x = 0;	rect_Back.y = ofGetHeight()*4/NUM_VSPLIT_DISP;	rect_Back.width = WIDTH__GRAPH_FREQBASED - 1;	rect_Back.height = ofGetHeight() * 2/NUM_VSPLIT_DISP;
	Coord_zero.x = 0;	Coord_zero.y = ofGetHeight()*6/NUM_VSPLIT_DISP;
	y_max = 2 * ofGetHeight()/NUM_VSPLIT_DISP;
	draw_FreqBasedGraph(GRAPH_FREQBASED__FFTGAIN_SLOW_SMOOTHED, col_Back_0, rect_Back, Coord_zero, y_max, Gui_Global->Val_DispMax__FFTGain_SlowSmoothed);
	
	rect_Back.x = 0;	rect_Back.y = ofGetHeight()*6/NUM_VSPLIT_DISP;	rect_Back.width = WIDTH__GRAPH_FREQBASED - 1;	rect_Back.height = ofGetHeight() * 2/NUM_VSPLIT_DISP;
	Coord_zero.x = 0;	Coord_zero.y = ofGetHeight()*7/NUM_VSPLIT_DISP;
	y_max = 1 * ofGetHeight()/NUM_VSPLIT_DISP;
	draw_FreqBasedGraph(GRAPH_FREQBASED__d1_FFTGAIN_SLOW_SMOOTHED, col_Back_1, rect_Back, Coord_zero, y_max, Gui_Global->Val_DispMax__FFTGain_d1_SlowSmoothed);
	
	rect_Back.x = 0;	rect_Back.y = ofGetHeight()*8/NUM_VSPLIT_DISP;	rect_Back.width = WIDTH__GRAPH_FREQBASED - 1;	rect_Back.height = ofGetHeight() * 2/NUM_VSPLIT_DISP;
	Coord_zero.x = 0;	Coord_zero.y = ofGetHeight()*10/NUM_VSPLIT_DISP;
	y_max = 2 * ofGetHeight()/NUM_VSPLIT_DISP;
	draw_FreqBasedGraph(GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED, col_Back_0, rect_Back, Coord_zero, y_max, 0);
	
	/********************
	********************/
	int counter = 0;
	for(int BandId = 0; BandId < NUM_ANALYSIS_BANDS; BandId++){
		/* */
		ofColor col_Back;
		if(counter % 2 == 0) 	col_Back = col_Back_1;
		else					col_Back = col_Back_0;
		counter++;
		
		rect_Back.x = WIDTH__GRAPH_FREQBASED + WIDTH__GRAPH_TIMEBASED * 0;	rect_Back.y = ofGetHeight()*(BandId * 2)/NUM_VSPLIT_DISP;	rect_Back.width = WIDTH__GRAPH_TIMEBASED - 1;	rect_Back.height = ofGetHeight() * 2/NUM_VSPLIT_DISP;
		Coord_zero.x = WIDTH__GRAPH_FREQBASED + WIDTH__GRAPH_TIMEBASED * 0;	Coord_zero.y = ofGetHeight() * (2 * BandId + 2)/NUM_VSPLIT_DISP;
		y_max = 2 * ofGetHeight()/NUM_VSPLIT_DISP;
		draw_TimeBasedGraph(BandId, GRAPH_TIMEBASED__BANDLEV__RAW, col_Back, rect_Back, Coord_zero, y_max, Gui_Global->Val_DispMax__BandLev[BandId], 0);
		
		/* */
		if(counter % 2 == 0) 	col_Back = col_Back_1;
		else					col_Back = col_Back_0;
		counter++;
		
		rect_Back.x = WIDTH__GRAPH_FREQBASED + WIDTH__GRAPH_TIMEBASED * 1;	rect_Back.y = ofGetHeight()*(BandId * 2)/NUM_VSPLIT_DISP;	rect_Back.width = WIDTH__GRAPH_TIMEBASED - 1;	rect_Back.height = ofGetHeight() * 2/NUM_VSPLIT_DISP;
		Coord_zero.x = WIDTH__GRAPH_FREQBASED + WIDTH__GRAPH_TIMEBASED * 1;	Coord_zero.y = ofGetHeight() * (2 * BandId + 1)/NUM_VSPLIT_DISP;
		y_max = 1 * ofGetHeight()/NUM_VSPLIT_DISP;
		draw_TimeBasedGraph(BandId, GRAPH_TIMEBASED__BANDLEV__d1, col_Back, rect_Back, Coord_zero, y_max, Gui_Global->Val_d1_DispMax__BandLev[BandId], 0);
		
		/* */
		if(counter % 2 == 0) 	col_Back = col_Back_1;
		else					col_Back = col_Back_0;
		counter++;
		
		rect_Back.x = WIDTH__GRAPH_FREQBASED + WIDTH__GRAPH_TIMEBASED * 2;	rect_Back.y = ofGetHeight()*(BandId * 2)/NUM_VSPLIT_DISP;	rect_Back.width = WIDTH__GRAPH_TIMEBASED - 1;	rect_Back.height = ofGetHeight() * 2/NUM_VSPLIT_DISP;
		Coord_zero.x = WIDTH__GRAPH_FREQBASED + WIDTH__GRAPH_TIMEBASED * 2;	Coord_zero.y = ofGetHeight()* (2 * BandId + 1)/NUM_VSPLIT_DISP;
		y_max = 1 * ofGetHeight()/NUM_VSPLIT_DISP;
		draw_TimeBasedGraph(BandId, GRAPH_TIMEBASED__BANDLEV__d2, col_Back, rect_Back, Coord_zero, y_max, Gui_Global->Val_d2_DispMax__BandLev[BandId], 0);
	}

	/********************
	********************/
	if(!Gui_Global->b_CheckDmx) draw_ColorTheme();
	
	/********************
	********************/
	if(b_EnableProcess_Light || b_EnableProcess_vj) Light->draw();
	
	/********************
	********************/
	if(b_EnableProcess_vj){
		static int c_syphon = 0;
		c_syphon++;
		if(c_syphon % 2 == 0){
			c_syphon = 0;
			mainOutputSyphonServer.publishScreen();
		}
	}
	
	/********************
	********************/
	drawGuis();
	
	if(b_ShowFrameRate){
		char buf[BUF_SIZE];
		sprintf(buf, "%7.2f", ofGetFrameRate());
		ofSetColor(col_white.GetColor(100));
		// font[FONT_M].drawString(buf, 5, 830);
		font[FONT_M].drawString(buf, 420, 830);
	}
	
	/********************
	checked time cost:Top of update - End of draw.
	Result < 3ms
	********************/
	if(fp_Log_main != NULL){
		if(now - t_MeasProcessTime < d_MeasProcessTime)	{ fprintf(fp_Log_main, "%f\n", ofGetElapsedTimef()); }
		else											{ printf("End Log\n"); fflush(stdout); fclose(fp_Log_main); fp_Log_main = NULL; }
	}
}

/******************************
******************************/
void ofApp::draw_ColorTheme()
{
	char buf[BUF_SIZE];
	
	switch(int(Gui_Global->ColorTheme_id)){
		case COLOR_THEME::THEME_RED:
			ofSetColor(col_red.GetColor(200));
			sprintf(buf, "Gui:Red");
			break;
		case COLOR_THEME::THEME_GREEN:
			ofSetColor(col_green.GetColor(200));
			sprintf(buf, "Gui:Green");
			break;
		case COLOR_THEME::THEME_BLUE:
			ofSetColor(col_blue.GetColor(200));
			sprintf(buf, "Gui:Blue");
			break;
		case COLOR_THEME::THEME_PURPLE:
			ofSetColor(col_purple.GetColor(200));
			sprintf(buf, "Gui:Purple");
			break;
		case COLOR_THEME::THEME_CANDY:
			ofSetColor(col_pink.GetColor(200));
			sprintf(buf, "Gui:Candy");
			break;
		case COLOR_THEME::THEME_PSYCHE:
			ofSetColor(col_cian.GetColor(200));
			sprintf(buf, "Gui:Psychedelic");
			break;
	}
	ofDrawCircle(20, 825, 9);
	
	ofSetColor(col_white.GetColor(200));
	font[FONT_M].drawString(buf, 18, 832);
	
	
	switch(ColorTheme.getTheme()){
		case COLOR_THEME::THEME_RED:
			ofSetColor(col_red.GetColor(200));
			break;
		case COLOR_THEME::THEME_GREEN:
			ofSetColor(col_green.GetColor(200));
			break;
		case COLOR_THEME::THEME_BLUE:
			ofSetColor(col_blue.GetColor(200));
			break;
		case COLOR_THEME::THEME_PURPLE:
			ofSetColor(col_purple.GetColor(200));
			break;
		case COLOR_THEME::THEME_CANDY:
			ofSetColor(col_pink.GetColor(200));
			break;
		case COLOR_THEME::THEME_PSYCHE:
			ofSetColor(col_cian.GetColor(200));
			break;
	}
	ofDrawCircle(20, 850, 9);
}

/******************************
******************************/
void ofApp::draw_TimeBasedGraph(int BandId, GRAPH_TIMEBASED GraphTimeBased_id, const ofColor& col_Back, const ofRectangle& rect_Back, const ofPoint& Coord_zero, float Screen_y_max, float Val_Disp_y_Max, float Val_Disp_y_Min)
{
	/********************
	********************/
	if(Val_Disp_y_Max <= Val_Disp_y_Min) return;
	
	/********************
	********************/
	ofEnableAlphaBlending();
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		
	/********************
	back color.
	********************/
	ofSetColor(col_Back);
	ofDrawRectangle(rect_Back);
	
	/********************
	********************/
	ofPushStyle();
	ofPushMatrix();
		/********************
		********************/
		ofTranslate(Coord_zero);
		ofScale(1, -1, 1);
		
		/********************
		t目盛り
		********************/
		ofSetColor(col_gray.GetColor(100));
		ofSetLineWidth(1);
		
		const double x_step = 60; // 1sec
		for(int i = 0; i * x_step <= WIDTH__GRAPH_TIMEBASED - 1; i++){
			int x = int(i * x_step + 0.5);
			ofDrawLine(x, 0, x, Screen_y_max);
		}
		
		/********************
		y目盛り
		********************/
		const int num_lines = 5;
		const double y_step = Screen_y_max/num_lines;
		for(int i = 0; i < num_lines; i++){
			int y = int(i * y_step + 0.5);
			
			ofSetColor(col_gray.GetColor(255));
			ofSetLineWidth(1);
			ofDrawLine(0, y, WIDTH__GRAPH_TIMEBASED - 1, y);
			if(TimeBasedGraph_isPlusMinus(GraphTimeBased_id)) ofDrawLine(0, -y, WIDTH__GRAPH_TIMEBASED - 1, -y);

			/********************
			********************/
			char buf[BUF_SIZE];
			sprintf(buf, "%g", Val_Disp_y_Min + (Val_Disp_y_Max - Val_Disp_y_Min)/num_lines * i);
			
			ofSetColor(col_gray.GetColor(255));
			ofScale(1, -1, 1); // 文字が上下逆さまになってしまうので.
			font[FONT_S].drawString(buf, WIDTH__GRAPH_TIMEBASED - 1 - font[FONT_S].stringWidth(buf) - 10, -y); // y posはマイナス
			if( TimeBasedGraph_isPlusMinus(GraphTimeBased_id) && (y != 0) ) font[FONT_S].drawString(buf, WIDTH__GRAPH_TIMEBASED - 1 - font[FONT_S].stringWidth(buf) - 10, y);
			ofScale(1, -1, 1); // 戻す.
		}
		
		/********************
		********************/
		ofSetColor(255);
		glPointSize(1.0);
		// glLineWidth(1);
		glLineWidth(Gui_Global->LineWidth);
		
		Vboset_TimeBased[BandId][GraphTimeBased_id].draw(GL_LINE_STRIP);
		draw_CursorAndValue_TimeBasedGraph(BandId, GraphTimeBased_id, Screen_y_max);
		
		/********************
		********************/
		glLineWidth(Gui_Global->LineWidth); // draw_CursorAndValue_TimeBasedGraph()で glLineWidth(1); されているので.
		
		if(GraphTimeBased_id == GRAPH_TIMEBASED__BANDLEV__RAW){
			/********************
			********************/
			Vboset_TimeBased[BandId][GRAPH_TIMEBASED__LH__RAW].draw(GL_LINE_STRIP);
			draw_DigitalPointer_TimeBasedGraph(BandId, GRAPH_TIMEBASED__LH__RAW, Screen_y_max * (1.0/4.0));
			
			// draw__thresh_BandGain(BandId, Screen_y_max);
			glLineWidth(1);
			Vboset_TimeBased[BandId][GRAPH_TIMEBASED__RAW_DYNAMIC_THRESH__H].draw(GL_LINE_STRIP);
			Vboset_TimeBased[BandId][GRAPH_TIMEBASED__RAW_DYNAMIC_THRESH__L].draw(GL_LINE_STRIP);
			
			/********************
			L/H 文字で表示
			********************/
			ofScale(1, -1, 1); // 文字が上下逆さまになってしまうので.
			if(GraphTimeBased_Log_Raw[BandId][GRAPH_TIMEBASED__LH__RAW][NUM_TIME_POINTS - 1]){
				ofSetColor(col_cian.GetColor(100));
				font[FONT_L].drawString("H", 10, -(Screen_y_max - 30));
			}else{
				ofSetColor(col_white.GetColor(100));
				font[FONT_L].drawString("L", 10, -(Screen_y_max - 30));
			}
			ofScale(1, -1, 1); //戻す

			
		}else if(GraphTimeBased_id == GRAPH_TIMEBASED__BANDLEV__d1){
			/********************
			********************/
			Vboset_TimeBased[BandId][GRAPH_TIMEBASED__LH__d1].draw(GL_LINE_STRIP);
			draw_DigitalPointer_TimeBasedGraph(BandId, GRAPH_TIMEBASED__LH__d1, Screen_y_max * (-1.0/2.0));
			
			/********************
			Disp : Strobe on
			********************/
			if( BandId == 4 ){
				/* */
				if(	IsBoost() ){
					/* */
					ofSetColor(col_white.GetColor(100));
					ofDrawCircle(20, Screen_y_max - 20, 10);
					
					/* */
					ofSetColor(col_white.GetColor(100));
					
					char buf[BUF_SIZE];
					sprintf(buf, "%5.2f\n%5.2f", StateChart_d1_BandGain[4].Get_CurrentHeight_Ratio(), StateChart_d1_BandGain[4].Get_threshDownRatio());
					
					ofScale(1, -1, 1); // 文字が上下逆さまになってしまうので.
					font[FONT_M].drawString(buf, 30, -(Screen_y_max - 25));
					ofScale(1, -1, 1); //戻す
				}
				
				/* */
				draw__thresh__d_MainBand_RadipDown(BandId, Screen_y_max);
				
				/* */
				if(	Is_LayerON_Go() ){
					ofSetColor(col_white.GetColor(100));
					ofDrawCircle(20, -10, 10);
				}
			}
			
		}else if(GraphTimeBased_id == GRAPH_TIMEBASED__BANDLEV__d2){
			/********************
			********************/
			Vboset_TimeBased[BandId][GRAPH_TIMEBASED__LH__d2].draw(GL_LINE_STRIP);
			draw_DigitalPointer_TimeBasedGraph(BandId, GRAPH_TIMEBASED__LH__d2, Screen_y_max * (-1.0/2.0));
			
			if(BandId == 0){
				Vboset_TimeBased[BandId][GRAPH_TIMEBASED__d2AMP_H].draw(GL_LINE_STRIP);
				Vboset_TimeBased[BandId][GRAPH_TIMEBASED__d2AMP_L].draw(GL_LINE_STRIP);
			}
			
			draw__thresh_d2_BandGain(BandId, Screen_y_max);
			
			/********************
			********************/
			if(BandId == 3){
				if(Gui_Global->b_CheckDmx){ // 「この時は、Beat Detectしないよ」と言うのを忘れがちなので、表示しておく.
					ofSetColor(col_red.GetColor(100));
					
					ofScale(1, -1, 1); // 文字が上下逆さまになってしまうので.
					font[FONT_M].drawString("DMX", 0, -(Screen_y_max - 23));
					ofScale(1, -1, 1); //戻す
					
				}else{
					if(b_Beat_Band3) t_Beat_Band3 = now;
					
					if( (t_Beat_Band3 != -1) && (now - t_Beat_Band3 < 0.2) ){
						ofSetColor(col_cian.GetColor(100));
						ofDrawCircle(20, Screen_y_max - 20, 10);
					}
				}
			}
			
			/********************
			********************/
			{
				char buf[BUF_SIZE];
				if(StateChart_BeatLock[BandId].IsLock())	ofSetColor(col_yellow.GetColor(100));
				else										ofSetColor(col_cian.GetColor(100));
				sprintf(buf, "%7.1fms/ %5.1f%%, %3d", StateChart_BeatLock[BandId].get_BeatInterval() * 1000, StateChart_BeatLock[BandId].get__Diff_percent(), StateChart_BeatLock[BandId].get_count());
				
				ofScale(1, -1, 1); // 文字が上下逆さまになってしまうので.
				font[FONT_M].drawString(buf, 30, -(Screen_y_max - 25));
				ofScale(1, -1, 1); //戻す
			}
		}
		
	ofPopMatrix();
	ofPopStyle();
}

/******************************
******************************/
bool ofApp::IsBoost()
{
	if(	(GraphTimeBased_Log_Raw[MAINBAND_ID][GRAPH_TIMEBASED__LH__d1][NUM_TIME_POINTS - 1] == 1)	&& 
		(GraphTimeBased_Log_Raw[MAINBAND_ID][GRAPH_TIMEBASED__LH__RAW][NUM_TIME_POINTS - 1] == 1)	&&
		(GraphTimeBased_Log_Raw[2][GRAPH_TIMEBASED__LH__RAW][NUM_TIME_POINTS - 1] == 1)				&&
		(GraphTimeBased_Log_Raw[3][GRAPH_TIMEBASED__LH__RAW][NUM_TIME_POINTS - 1] == 1)
	){
		return true;
	}else{
		return false;
	}
}

/******************************
******************************/
bool ofApp::Is_LayerON_Go()
{
	if(!Gui_Global->b_Enable_LayerON){
		return false;
	}else{
		if(	GraphTimeBased_Log_Raw[MAINBAND_ID][GRAPH_TIMEBASED__BANDLEV__d1][NUM_TIME_POINTS - 1] < -Gui_Global->thresh_MainBand_RapidDown ){
			return true;
		}else{
			return false;
		}
	}
}

/******************************
******************************/
void ofApp::draw_FreqBasedGraph(GRAPH_FREQBASED GraphFreqBased_id, const ofColor& col_Back, const ofRectangle& rect_Back, const ofPoint& Coord_zero, float Screen_y_max, float Val_Disp_y_Max){
	/********************
	********************/
	ofEnableAlphaBlending();
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	/********************
	back color.
	********************/
	ofSetColor(col_Back);
	ofDrawRectangle(rect_Back);
		
	/********************
	********************/
	ofPushStyle();
	ofPushMatrix();
		/********************
		********************/
		ofTranslate(Coord_zero);
		ofScale(1, -1, 1);
		
		/********************
		y目盛り
		********************/
		if(0 < Val_Disp_y_Max){
			const int num_lines = 5;
			const double y_step = Screen_y_max/num_lines;
			for(int i = 0; i < num_lines; i++){
				int y = int(i * y_step + 0.5);
				
				ofSetColor(col_gray.GetColor(255));
				ofSetLineWidth(1);
				ofDrawLine(0, y, WIDTH__GRAPH_FREQBASED - 1, y);
	
				/********************
				********************/
				char buf[BUF_SIZE];
				sprintf(buf, "%7.7f", Val_Disp_y_Max/num_lines * i);
				
				ofSetColor(col_gray.GetColor(255));
				ofScale(1, -1, 1); // 文字が上下逆さまになってしまうので.
				font[FONT_S].drawString(buf, WIDTH__GRAPH_FREQBASED - 1 - font[FONT_S].stringWidth(buf) - 10, -y); // y posはマイナス
				if( FreqBasedGraph_isPlusMinus(GraphFreqBased_id) && (y != 0) ) font[FONT_S].drawString(buf, WIDTH__GRAPH_FREQBASED - 1 - font[FONT_S].stringWidth(buf) - 10, y);
				ofScale(1, -1, 1); // 戻す.
			}
		}
		
		/********************
		********************/
		switch(GraphFreqBased_id){
			case GRAPH_FREQBASED__FFTGAIN_FAST:
			case GRAPH_FREQBASED__FFTGAIN_SLOW:
				{
					ofColor col_Line[NUM_ANALYSIS_BANDS] = {col_green.GetColor(200), col_blue.GetColor(200), col_cian.GetColor(200), col_orange.GetColor(200), col_pink.GetColor(200),};
					ofColor col_Rect[NUM_ANALYSIS_BANDS] = {col_green.GetColor(90), col_blue.GetColor(110), col_cian.GetColor(90), col_orange.GetColor(90), col_pink.GetColor(50),};
					const double ofs_y = 5;
					
					int startBand;
					if(GraphFreqBased_id == GRAPH_FREQBASED__FFTGAIN_FAST)	startBand = 0;
					else													startBand = 1;
					
					for(int i = startBand; i < NUM_ANALYSIS_BANDS; i++){
						draw_AnalyzeRange_and_Level(	col_Line[i], col_Rect[i],
														Gui_Global->AnalyzeRange__BandLev_FreqFrom[i] * GRAPH_BAR_SPACE__FFT_GAIN,
														(Gui_Global->AnalyzeRange__BandLev_FreqFrom[i] + Gui_Global->AnalyzeRange__BandLev_FreqWidth[i] - 1) * GRAPH_BAR_SPACE__FFT_GAIN + GRAPH_BAR_WIDTH__FFT_GAIN,
														GraphTimeBased_Log_Raw[i][GRAPH_TIMEBASED__BANDLEV__RAW][NUM_TIME_POINTS - 1],
														Screen_y_max * 0.95 - i * ofs_y, Screen_y_max, WIDTH__GRAPH_FREQBASED - 1, Val_Disp_y_Max
													);
													
					}
				}
				break;
				
			case GRAPH_FREQBASED__FFTGAIN_SLOW_SMOOTHED:
			case GRAPH_FREQBASED__d1_FFTGAIN_SLOW_SMOOTHED:
			case GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED:
				// nothing.
				break;
				
			default:
				ERROR_MSG();
				std::exit(1);
				break;
		}
		
		/********************
		********************/
		ofSetColor(255);
		glPointSize(1.0);
		glLineWidth(1);
		
		Vboset_FreqBased[GraphFreqBased_id].draw(GL_QUADS);
		
		/********************
		********************/
		if(GraphFreqBased_id < GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED)	draw_CursorAndValue_FreqBasedGraph(GraphFreqBased_id, Screen_y_max);
		else																	draw_ZeroCrossPointer(Screen_y_max);
		
		/********************
		********************/
		if(GraphFreqBased_id == GRAPH_FREQBASED__FFTGAIN_SLOW_SMOOTHED){
			draw__thresh_ValidGain_ofSlowSmoothed_onZeroCrossPoint(Screen_y_max);
		}
		
	ofPopMatrix();
	ofPopStyle();
}

/******************************
******************************/
void ofApp::draw_AnalyzeRange_and_Level(const ofColor& col_Line, const ofColor& col_Rect, int x_from, int x_to, float BandLev, float y_DrawRangeLine, float Screen_y_max, float Screen_x_max, float Val__Disp_y_max)
{
	if(x_to < x_from) x_to = x_from;
	if(Screen_x_max < x_from)	x_from = Screen_x_max;
	if(Screen_x_max < x_to) 	x_to = Screen_x_max;
	
	ofSetColor(col_Line);
	ofSetLineWidth(1);
	ofDrawLine(x_from, y_DrawRangeLine, x_to, y_DrawRangeLine);
	
	ofFill();
	float radius = 2.0;
	ofDrawCircle(x_from, y_DrawRangeLine, radius);
	ofDrawCircle(x_to, y_DrawRangeLine, radius);
	
	if( (!b_PauseGraph) && (0 < BandLev) ){
		int height = ofMap(BandLev, 0, Val__Disp_y_max, 0, Screen_y_max, true);
		ofSetColor(col_Rect);
		ofDrawRectangle(x_from, 0, x_to - x_from + 1, height);
	}
}

/******************************
******************************/
void ofApp::draw__thresh_BandGain(int BandId, float Screen_y_max)
{
	/********************
	********************/
	ofSetLineWidth(1);
	
	ofSetColor(col_green.GetColor(200));
	int height = ofMap(Gui_Global->thresh_BandLev__H[BandId], 0, Gui_Global->Val_DispMax__BandLev[BandId], 0, Screen_y_max);
	ofDrawLine(0, height, WIDTH__GRAPH_TIMEBASED - 1, height);
	
	float thresh_BandLev__L = Gui_Global->thresh_BandLev__H[BandId] * Gui_Global->thresh_BandLev__ratio_HtoL[BandId];
	ofSetColor(col_yellow.GetColor(200));
	height = ofMap(thresh_BandLev__L, 0, Gui_Global->Val_DispMax__BandLev[BandId], 0, Screen_y_max);
	ofDrawLine(0, height, WIDTH__GRAPH_TIMEBASED - 1, height);
}

/******************************
******************************/
void ofApp::draw__thresh__d_MainBand_RadipDown(int BandId, float Screen_y_max)
{
	ofSetLineWidth(1);
	
	ofSetColor(col_yellow.GetColor(200));
	int height = -ofMap(Gui_Global->thresh_MainBand_RapidDown, 0, Gui_Global->Val_d1_DispMax__BandLev[BandId], 0, Screen_y_max);
	ofDrawLine(0, height, WIDTH__GRAPH_TIMEBASED - 1, height);
}

/******************************
******************************/
void ofApp::draw__thresh_d2_BandGain(int BandId, float Screen_y_max)
{
	/********************
	********************/
	ofSetLineWidth(1);
	
	ofSetColor(col_green.GetColor(200));
	int height = ofMap(Gui_Global->thresh_d2BandLev__H[BandId], 0, Gui_Global->Val_d2_DispMax__BandLev[BandId], 0, Screen_y_max);
	ofDrawLine(0, height, WIDTH__GRAPH_TIMEBASED - 1, height);
	
	float thresh_d2BandLev__L = Gui_Global->thresh_d2BandLev__H[BandId] * Gui_Global->thresh_d2BandLev__ratio_HtoL[BandId];
	ofSetColor(col_yellow.GetColor(200));
	height = -ofMap(thresh_d2BandLev__L, 0, Gui_Global->Val_d2_DispMax__BandLev[BandId], 0, Screen_y_max); // 注) L側はマイナスに反転.
	ofDrawLine(0, height, WIDTH__GRAPH_TIMEBASED - 1, height);
}

/******************************
******************************/
void ofApp::draw__thresh_ValidGain_ofSlowSmoothed_onZeroCrossPoint(float Screen_y_max)
{
	ofSetColor(col_red.GetColor(200));
	
	int height = ofMap(Gui_Global->thresh__ValidGain_ofSlowSmoothed_onZeroCrossPoint, 0, Gui_Global->Val_DispMax__FFTGain_SlowSmoothed, 0, Screen_y_max);
	ofSetLineWidth(1);
	ofDrawLine(0, height, WIDTH__GRAPH_FREQBASED, height);
}

/******************************
******************************/
bool ofApp::TimeBasedGraph_isPlusMinus(GRAPH_TIMEBASED TimeBasedGraph_Id)
{
	switch(TimeBasedGraph_Id){
		case GRAPH_TIMEBASED__BANDLEV__RAW:
			return false;
			
		case GRAPH_TIMEBASED__BANDLEV__d1:
		case GRAPH_TIMEBASED__BANDLEV__d2:
			return true;
			
		default:
			return false;
	}
}

/******************************
******************************/
bool ofApp::FreqBasedGraph_isPlusMinus(GRAPH_FREQBASED FreqBasedGraph_Id)
{
	switch(FreqBasedGraph_Id){
		case GRAPH_FREQBASED__FFTGAIN_FAST:
		case GRAPH_FREQBASED__FFTGAIN_SLOW:
		case GRAPH_FREQBASED__FFTGAIN_SLOW_SMOOTHED:
		case GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED:
			return false;
		
		case GRAPH_FREQBASED__d1_FFTGAIN_SLOW_SMOOTHED:
			return true;
			
		default:
			return false;
	}
}

/******************************
******************************/
void ofApp::draw_CursorAndValue_TimeBasedGraph(int BandId, GRAPH_TIMEBASED TimeBasedGraph_Id, float Screen_y_max)
{
	/********************
	********************/
	if((mouseX < WIDTH__GRAPH_FREQBASED) || (ofGetWidth() <= mouseX)) return;
	
	/********************
	********************/
	int Cursor_x = (mouseX - WIDTH__GRAPH_FREQBASED) % WIDTH__GRAPH_TIMEBASED + ofs_x_ReadCursor;
	int Vbo_id = Cursor_x;

	/********************
	********************/
	if( (Vbo_id < 0) || (NUM_TIME_POINTS <= Vbo_id) ) return;
	
	/********************
	********************/
	bool b_PlusMinus = TimeBasedGraph_isPlusMinus(TimeBasedGraph_Id);
	 
	ofSetColor(col_red.GetColor(255));
	ofSetLineWidth(1);
	if(b_PlusMinus)	ofDrawLine(Cursor_x, -Screen_y_max, Cursor_x, Screen_y_max);
	else			ofDrawLine(Cursor_x, 0, Cursor_x, Screen_y_max);
	
	/********************
	********************/
	ofSetColor(col_red.GetColor(255));
	int ofs_x = 5;
	int ofs_y = 10;
	char buf[BUF_SIZE];
	
	TimaBasedGraph_getVal_ReverseFromVbo(BandId, TimeBasedGraph_Id, Vbo_id, buf);
	ofScale(1, -1, 1); // 文字が上下逆さまになってしまうので.
		if(b_PlusMinus){
			if(WIDTH__GRAPH_TIMEBASED/2 < Cursor_x)	font[FONT_S].drawString(buf, Cursor_x - font[FONT_S].stringWidth(buf) - ofs_x, Screen_y_max - ofs_y);
			else									font[FONT_S].drawString(buf, Cursor_x + ofs_x, Screen_y_max - ofs_y);
		}else{
			if(WIDTH__GRAPH_TIMEBASED/2 < Cursor_x)	font[FONT_S].drawString(buf, Cursor_x - font[FONT_S].stringWidth(buf) - ofs_x, 0 - ofs_y);
			else									font[FONT_S].drawString(buf, Cursor_x + ofs_x, 0 - ofs_y);
		}
	ofScale(1, -1, 1); // 戻す.
}

/******************************
******************************/
void ofApp::draw_DigitalPointer_TimeBasedGraph(int BandId, GRAPH_TIMEBASED TimeBasedGraph_Id, double PointerPos_y)
{
	/********************
	********************/
	if((mouseX < WIDTH__GRAPH_FREQBASED) || (ofGetWidth() <= mouseX)) return;
	
	/********************
	********************/
	int Cursor_x = (mouseX - WIDTH__GRAPH_FREQBASED) % WIDTH__GRAPH_TIMEBASED + ofs_x_ReadCursor;
	int Vbo_id = Cursor_x;

	/********************
	********************/
	if( (Vbo_id < 0) || (NUM_TIME_POINTS <= Vbo_id) ) return;
	
	/********************
	********************/
	{
		ofSetColor(col_red.GetColor(255));
		ofSetLineWidth(1);
		ofNoFill();
		float radius = 2.0;
		
		if(IsHigh_TimeBasedGraph_ReverseFromVbo(BandId, TimeBasedGraph_Id, Vbo_id)){
			ofDrawCircle(Cursor_x, PointerPos_y, radius);
		}else{
			// no pointer.
		}
		
		ofFill();
	}
}

/******************************
******************************/
void ofApp::draw_CursorAndValue_FreqBasedGraph(GRAPH_FREQBASED FreqBasedGraph_Id, float Screen_y_max)
{
	/********************
	********************/
	if(GRAPH_FREQBASED__ZEROCROSS_FFTGAIN_SLOW_SMOOTED <= FreqBasedGraph_Id) return;
	
	/********************
	********************/
	if((mouseX < 0) || (WIDTH__GRAPH_FREQBASED <= mouseX)) return;
	
	/********************
	********************/
	int Cursor_x = mouseX + ofs_x_ReadCursor;

	/********************
	********************/
	if( (Cursor_x < 0) || (WIDTH__GRAPH_FREQBASED <= Cursor_x) ) return;
	
	/********************
	********************/
	bool b_PlusMinus = FreqBasedGraph_isPlusMinus(FreqBasedGraph_Id);
	 
	ofSetColor(col_red.GetColor(255));
	ofSetLineWidth(1);
	if(b_PlusMinus)	ofDrawLine(Cursor_x, -Screen_y_max, Cursor_x, Screen_y_max);
	else			ofDrawLine(Cursor_x, 0, Cursor_x, Screen_y_max);
	
	/********************
	********************/
	ofSetColor(col_red.GetColor(200));
	int ofs_x = 5;
	int ofs_y = 10;
	char buf[BUF_SIZE];
	
	FreqBasedGraph_getVal_ReverseFromVbo(FreqBasedGraph_Id, Cursor_x, buf);
	ofScale(1, -1, 1); // 文字が上下逆さまになってしまうので.
		if(b_PlusMinus){
			if(WIDTH__GRAPH_FREQBASED/2 < Cursor_x)	font[FONT_S].drawString(buf, Cursor_x - font[FONT_S].stringWidth(buf) - ofs_x, Screen_y_max - ofs_y);
			else									font[FONT_S].drawString(buf, Cursor_x + ofs_x, Screen_y_max - ofs_y);
		}else{
			if(WIDTH__GRAPH_FREQBASED/2 < Cursor_x)	font[FONT_S].drawString(buf, Cursor_x - font[FONT_S].stringWidth(buf) - ofs_x, 0 - ofs_y);
			else									font[FONT_S].drawString(buf, Cursor_x + ofs_x, 0 - ofs_y);
		}
	ofScale(1, -1, 1); // 戻す.
}

/******************************
******************************/
void ofApp::draw_ZeroCrossPointer(float Screen_y_max)
{
	/********************
	********************/
	if((mouseX < 0) || (WIDTH__GRAPH_FREQBASED <= mouseX)) return;
	
	/********************
	********************/
	int Cursor_x = mouseX + ofs_x_ReadCursor;

	/********************
	********************/
	if( (Cursor_x < 0) || (WIDTH__GRAPH_FREQBASED <= Cursor_x) ) return;
	
	/********************
	********************/
	ofSetColor(col_red.GetColor(255));
	ofSetLineWidth(1);
	ofDrawLine(Cursor_x, 0, Cursor_x, Screen_y_max);
	
	/********************
	********************/
	{
		int ofs_x = 5;
		int ofs_y = 10;
		char buf[BUF_SIZE];
	
		ofSetColor(col_red.GetColor(255));
		ofSetLineWidth(1);
		ofNoFill();
		float radius = 2.0;
		
		if(IsZeroCross_ReverseFromVboGraph(Cursor_x, buf)){
			ofDrawCircle(Cursor_x, Screen_y_max/2, radius);
			
			ofScale(1, -1, 1); // 文字が上下逆さまになってしまうので.
				font[FONT_S].drawString(buf, Cursor_x + ofs_x, 0 - ofs_y);
			ofScale(1, -1, 1); // 戻す.
		}else{
			ofDrawCircle(Cursor_x, 0, radius);
		}
	}
}

/******************************
******************************/
void ofApp::drawGuis()
{
	if(!b_DispGui) return;
	
	Gui_Global->gui.draw();
}

/******************************
audioIn/ audioOut
	同じthreadで動いている様子。
	また、audioInとaudioOutは、同時に呼ばれることはない(多分)。
	つまり、ofAppからaccessがない限り、変数にaccessする際にlock/unlock する必要はない。
	ofApp側からaccessする時は、threadを立てて、安全にpassする仕組みが必要
******************************/
void ofApp::audioIn(float *input, int bufferSize, int nChannels)
{
    for (int i = 0; i < bufferSize; i++) {
        AudioSample.Left[i] = input[2*i];
		AudioSample.Right[i] = input[2*i+1];
    }
	
	/********************
	FFT Filtering
	1 process / block.
	********************/
	fft_thread->update__Gain(AudioSample.Left, AudioSample.Right);
}  

/******************************
******************************/
void ofApp::audioOut(float *output, int bufferSize, int nChannels)
{
	/********************
	x	:input -> output
	o	:No output.
	********************/
    for (int i = 0; i < bufferSize; i++) {
		if(b_EnableAudioOut){
			output[2*i  ] = AudioSample.Left[i];
			output[2*i+1] = AudioSample.Right[i];
		}else{
			output[2*i  ] = 0; // L
			output[2*i+1] = 0; // R
		}
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case 'd':
			b_DispGui = !b_DispGui;
			break;
			
		case 'f':
			b_ShowFrameRate = !b_ShowFrameRate;
			break;
			
		case 'p':
			b_PauseGraph = !b_PauseGraph;
			break;
			
		case ' ':
			{
				char buf[BUF_SIZE];
				
				sprintf(buf, "image_%d.png", png_id);
				ofSaveScreen(buf);
				// ofSaveFrame();
				printf("> %s saved\n", buf);
				
				png_id++;
			}
			break;
			
		case 'l':
			fft_thread->Log();
			printf("fft log\n");
			break;
			
		case 'm':
			Gui_Global->gui.minimizeAll();
			break;
			
		case 't':
			if( (fp_Log_main == NULL) && (fp_Log_fft == NULL) ){
				fp_Log_main	= fopen("../../../data/Log_main.csv", "w");
				fp_Log_fft	= fopen("../../../data/Log_fft.csv", "w");
				t_MeasProcessTime = ofGetElapsedTimef();
				printf("Start Log\n");
				fflush(stdout);
			}
			break;
			
		case OF_KEY_RIGHT:
			ofs_x_ReadCursor++;
			break;
			
		case OF_KEY_LEFT:
			ofs_x_ReadCursor--;
			break;
			
		case OF_KEY_UP:
			Gui_Global->Dmx_TestId = Gui_Global->Dmx_TestId + 1;
			break;
			
		case OF_KEY_DOWN:
			// -1 : No Led Lighting.
			if(0 <= Gui_Global->Dmx_TestId) Gui_Global->Dmx_TestId = Gui_Global->Dmx_TestId - 1;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	ofs_x_ReadCursor = 0;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
