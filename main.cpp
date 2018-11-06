#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char** argv ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	/********************
	********************/
	int soundStream_Input_DeviceId;
	int soundStream_Output_DeviceId;
	
	bool b_EnableAudioOut;
	
	bool b_EnableProcess_CheckLightDesign;
	bool b_EnableProcess_Light;
	bool b_EnableProcess_vj;

	/********************
	********************/
	if(argc < 6){
		/********************
		********************/
		printf("> exe [AudioInput] [AudioOutput] [b_EnableAudioOut] [CheckLightDesign] [Light] [vj]\n");
		
		/********************
		********************/
		soundStream_Input_DeviceId = -1;
		soundStream_Output_DeviceId = -1;
		b_EnableAudioOut = false;
		b_EnableProcess_CheckLightDesign = false;
		b_EnableProcess_Light = false;
		b_EnableProcess_vj = false;
		
		ofRunApp(new ofApp(soundStream_Input_DeviceId, soundStream_Output_DeviceId, b_EnableAudioOut, b_EnableProcess_CheckLightDesign, b_EnableProcess_Light, b_EnableProcess_vj));
	
	}else{
		soundStream_Input_DeviceId = atoi(argv[1]);
		soundStream_Output_DeviceId = atoi(argv[2]);
		b_EnableAudioOut = atoi(argv[3]);
		b_EnableProcess_CheckLightDesign = atoi(argv[4]);
		b_EnableProcess_Light = atoi(argv[5]);
		b_EnableProcess_vj = atoi(argv[6]);

		ofRunApp(new ofApp(soundStream_Input_DeviceId, soundStream_Output_DeviceId,  b_EnableAudioOut, b_EnableProcess_CheckLightDesign, b_EnableProcess_Light, b_EnableProcess_vj));
		
	}
}
