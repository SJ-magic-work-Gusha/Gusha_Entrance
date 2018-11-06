/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "sj_common.h"
#include "L_common.h"
#include "L_DesignFunc.h"

/************************************************************
************************************************************/
enum{
	MAX_CANDIDATES_PER_STATE = 30,
};

struct SEL__FUNC_GETLUM{
	int weight;
	pFUNC_LUM pFunc_Lum;
	pFUNC_FixIdOrder pFunc_FixIdOrder;
};

struct SEL__FUNC_GETPOS{
	int weight;
	pFUNC_POS pFunc_Pos;
	pFUNC_FixIdOrder pFunc_FixIdOrder;
};

struct SEL__COL{
	int weight;
	sjRGBW col_H;
	sjRGBW col_L;
};


class L_TABLE_UTIL{
private:

public:
	static int Count_NumCandidate(SEL__COL* Array, int NumMax);
	static int Count_NumCandidate(SEL__FUNC_GETLUM* Array, int NumMax);
	static int Count_NumCandidate(SEL__FUNC_GETPOS* Array, int NumMax);
};

/************************************************************
************************************************************/
extern int T_SameOrDiffDesignForEachBlock[NUM_STATES][2];
extern int T_NumBlocksOn[NUM_STATES];
extern int T_NumBlocksOn__Layer_Strobe[NUM_STATES];
extern double T_dInterval_Strobe[NUM_STATES];
extern double T_dLength_Strobe[NUM_STATES];
extern double T_dInterval_Flow[NUM_STATES];
extern double T_dLength_Flow[NUM_STATES];
extern double T_dPanTilt[NUM_STATES];
extern SEL__COL T_Color[COLOR_THEME::NUM_THEMES][NUM_STATES][MAX_CANDIDATES_PER_STATE];
extern SEL__FUNC_GETLUM T_SelectFunc_GetLum[NUM_STATES][MAX_CANDIDATES_PER_STATE];
extern SEL__FUNC_GETPOS T_SelectFunc_GetPos[NUM_STATES][MAX_CANDIDATES_PER_STATE];


