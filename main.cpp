#include "ofMain.h"
#include "ofApp.h"

//========================================================================
/******************************
******************************/
int main( int argc, char** argv ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	/********************
	********************/
	int soundStream_Input_DeviceId = -1;
	int soundStream_Output_DeviceId = -1;
	
	bool b_EnableProcess_CheckLightDesign = false;
	bool b_EnableProcess_Light = true;
	bool b_EnableProcess_vj = true;

	/********************
	********************/
	printf("> parameters\n");
	printf("\t-i:Audio in(-1)\n");
	printf("\t-o:Audio out(-1)\n");
	printf("\t-c:Check Design(false)\n");
	printf("\t-l:Enable Light(true)\n");
	printf("\t-v:Enable vj(true)\n");
	
	for(int i = 1; i < argc; i++){
		if( strcmp(argv[i], "-i") == 0 ){
			if(i+1 < argc) soundStream_Input_DeviceId = atoi(argv[i+1]);
		}else if( strcmp(argv[i], "-o") == 0 ){
			if(i+1 < argc) soundStream_Output_DeviceId = atoi(argv[i+1]);
		}else if( strcmp(argv[i], "-c") == 0 ){
			if(i+1 < argc) b_EnableProcess_CheckLightDesign = atoi(argv[i+1]);
		}else if( strcmp(argv[i], "-l") == 0 ){
			if(i+1 < argc) b_EnableProcess_Light = atoi(argv[i+1]);
		}else if( strcmp(argv[i], "-v") == 0 ){
			if(i+1 < argc) b_EnableProcess_vj = atoi(argv[i+1]);
		}
	}
	
	ofRunApp(new ofApp(soundStream_Input_DeviceId, soundStream_Output_DeviceId,  b_EnableProcess_CheckLightDesign, b_EnableProcess_Light, b_EnableProcess_vj));
}
