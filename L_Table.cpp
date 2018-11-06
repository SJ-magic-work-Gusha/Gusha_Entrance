/************************************************************
************************************************************/
#include "L_Table.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
int T_SameOrDiffDesignForEachBlock[NUM_STATES][2] = {								//	[4]	[0]
//		same		diff						
/* 0 */	{	1	,	0	},		//	stop	x	x
/* 1 */	{	1	,	0	},		//	play	L	L
/* 2 */	{	1	,	1	},		//	play	L	H
/* 3 */	{	1	,	1	},		//	play	H	L
/* 4 */	{	1	,	1	},		//	play	H	H
};										

/**************************************************
**************************************************/
SEL__FUNC_GETLUM T_SelectFunc_GetLum[NUM_STATES][MAX_CANDIDATES_PER_STATE] = {								//		[4]	[0]	
{ // 0								//	stop	x	x	
	{	1	,	LED_DESIGN_FUNC::Func_GetLum__Stop	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__On	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Flow	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Flow	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Flash	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__GainSync	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Strobe_1_W	,	LED_DESIGN_FUNC::FixIdOrder_Random	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Strobe_1_C	,	LED_DESIGN_FUNC::FixIdOrder_Random	},					
	{	-1	,	NULL	,	NULL	},					
},												
{ // 1								//	play	L	L	
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Stop	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	1	,	LED_DESIGN_FUNC::Func_GetLum__On	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	1	,	LED_DESIGN_FUNC::Func_GetLum__Flow	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	1	,	LED_DESIGN_FUNC::Func_GetLum__Flow	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Flash	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__GainSync	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Strobe_1_W	,	LED_DESIGN_FUNC::FixIdOrder_Random	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Strobe_1_C	,	LED_DESIGN_FUNC::FixIdOrder_Random	},					
	{	-1	,	NULL	,	NULL	},					
},												
{ // 2								//	play	L	H	
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Stop	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	1	,	LED_DESIGN_FUNC::Func_GetLum__On	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Flow	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Flow	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},					
	{	2	,	LED_DESIGN_FUNC::Func_GetLum__Flash	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__GainSync	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Strobe_1_W	,	LED_DESIGN_FUNC::FixIdOrder_Random	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Strobe_1_C	,	LED_DESIGN_FUNC::FixIdOrder_Random	},					
	{	-1	,	NULL	,	NULL	},					
},												
{ // 3								//	play	H	L	
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Stop	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	1	,	LED_DESIGN_FUNC::Func_GetLum__On	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	1	,	LED_DESIGN_FUNC::Func_GetLum__Flow	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	1	,	LED_DESIGN_FUNC::Func_GetLum__Flow	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Flash	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__GainSync	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Strobe_1_W	,	LED_DESIGN_FUNC::FixIdOrder_Random	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Strobe_1_C	,	LED_DESIGN_FUNC::FixIdOrder_Random	},					
	{	-1	,	NULL	,	NULL	},					
},												
{ // 4								//	play	H	H	
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Stop	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__On	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	1	,	LED_DESIGN_FUNC::Func_GetLum__Flow	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	1	,	LED_DESIGN_FUNC::Func_GetLum__Flow	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},					
	{	4	,	LED_DESIGN_FUNC::Func_GetLum__Flash	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__GainSync	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Strobe_1_W	,	LED_DESIGN_FUNC::FixIdOrder_Random	},					
	{	0	,	LED_DESIGN_FUNC::Func_GetLum__Strobe_1_C	,	LED_DESIGN_FUNC::FixIdOrder_Random	},					
	{	-1	,	NULL	,	NULL	},					
},												
};												

/**************************************************
**************************************************/
SEL__FUNC_GETPOS T_SelectFunc_GetPos[NUM_STATES][MAX_CANDIDATES_PER_STATE] = {								//		[4]	[0]
{ // 0								//	stop	x	x
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Stop	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Center	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_GainSync	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	-1	,	NULL	,	NULL	},				
},											
{ // 1								//	play	L	L
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Stop	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Center	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_GainSync	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	-1	,	NULL	,	NULL	},				
},											
{ // 2								//	play	L	H
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Stop	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Center	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_GainSync	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	-1	,	NULL	,	NULL	},				
},											
{ // 3								//	play	H	L
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Stop	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Center	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Pan_GainSync	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	-1	,	NULL	,	NULL	},				
},											
{ // 4								//	play	H	H
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Stop	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Center	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	0	,	LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Pan	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Tilt	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Reverse	},				
	{	1	,	LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff	,	LED_DESIGN_FUNC::FixIdOrder_Random	},				
	{	3	,	LED_DESIGN_FUNC::Func_GetPos__Pan_GainSync	,	LED_DESIGN_FUNC::FixIdOrder_Forward	},				
	{	-1	,	NULL	,	NULL	},				
},											
};											

/**************************************************
**************************************************/
int T_NumBlocksOn[NUM_STATES] = {					//	[4]	[0]
/* 0 */	5	,		//	stop	x	x
/* 1 */	1	,		//	play	L	L
/* 2 */	3	,		//	play	L	H
/* 3 */	3	,		//	play	H	L
/* 4 */	5	,		//	play	H	H
};							
							
int T_NumBlocksOn__Layer_Strobe[NUM_STATES] = {					//	[4]	[0]
/* 0 */	0	,		//	stop	x	x
/* 1 */	0	,		//	play	L	L
/* 2 */	0	,		//	play	L	H
/* 3 */	3	,		//	play	H	L
/* 4 */	3	,		//	play	H	H
};							


/**************************************************
**************************************************/
double T_dInterval_Strobe[NUM_STATES] = {							//	[4]	[0]
/* 0 */	0.1	,				//	stop	x	x
/* 1 */	0.1	,				//	play	L	L
/* 2 */	0.1	,				//	play	L	H
/* 3 */	0.06	,				//	play	H	L
/* 4 */	0.06	,				//	play	H	H
};									
									
//	1.5								
double T_dLength_Strobe[NUM_STATES] = {							//	[4]	[0]
/* 0 */	0.15	,				//	stop	x	x
/* 1 */	0.15	,				//	play	L	L
/* 2 */	0.15	,				//	play	L	H
/* 3 */	0.09	,				//	play	H	L
/* 4 */	0.09	,				//	play	H	H
};									

/**************************************************
**************************************************/
double T_dInterval_Flow[NUM_STATES] = {							//	[4]	[0]	
/* 0 */	2	,				//	stop	x	x	
/* 1 */	2	,				//	play	L	L	
/* 2 */	1	,				//	play	L	H	
/* 3 */	1	,				//	play	H	L	
/* 4 */	0.5	,				//	play	H	H	
};										
										
//	2									
double T_dLength_Flow[NUM_STATES] = {							//	[4]	[0]	
/* 0 */	4	,				//	stop	x	x	
/* 1 */	4	,				//	play	L	L	
/* 2 */	2	,				//	play	L	H	
/* 3 */	2	,				//	play	H	L	
/* 4 */	1	,				//	play	H	H	
};										

/**************************************************
**************************************************/
double T_dPanTilt[NUM_STATES] = {				//	[4]	[0]
/* 0 */	3	,	//	stop	x	x
/* 1 */	3	,	//	play	L	L
/* 2 */	3	,	//	play	L	H
/* 3 */	3	,	//	play	H	L
/* 4 */	3	,	//	play	H	H
};						

/**************************************************
**************************************************/
SEL__COL T_Color[COLOR_THEME::NUM_THEMES][NUM_STATES][MAX_CANDIDATES_PER_STATE] = {																												//		[4]	[0]
{ // R																															
	{ // 0																											//	stop	x	x
		{	1	,	sjRGBW(	1	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 1																											//	play	L	L
		{	1	,	sjRGBW(	1	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 2																											//	play	L	H
		{	1	,	sjRGBW(	1	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 3																											//	play	H	L
		{	1	,	sjRGBW(	1	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 4																											//	play	H	H
		{	3	,	sjRGBW(	1	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	1	,	sjRGBW(	1	,	0.3	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	1	,	sjRGBW(	1	,	1	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
},																															
{ // G																															
	{ // 0																											//	stop	x	x
		{	1	,	sjRGBW(	0	,	1	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 1																											//	play	L	L
		{	1	,	sjRGBW(	0	,	1	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 2																											//	play	L	H
		{	1	,	sjRGBW(	0	,	1	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 3																											//	play	H	L
		{	1	,	sjRGBW(	0	,	1	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 4																											//	play	H	H
		{	3	,	sjRGBW(	0	,	1	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	1	,	sjRGBW(	1	,	1	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
},																															
{ // B																															
	{ // 0																											//	stop	x	x
		{	1	,	sjRGBW(	0	,	0	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 1																											//	play	L	L
		{	1	,	sjRGBW(	0	,	0	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 2																											//	play	L	H
		{	2	,	sjRGBW(	0	,	0	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	1	,	sjRGBW(	0	,	1	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 3																											//	play	H	L
		{	1	,	sjRGBW(	0	,	0	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 4																											//	play	H	H
		{	2	,	sjRGBW(	0	,	0	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	1	,	sjRGBW(	0	,	1	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
},																															
{ // Pu																															
	{ // 0																											//	stop	x	x
		{	1	,	sjRGBW(	0	,	0	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 1																											//	play	L	L
		{	1	,	sjRGBW(	0	,	0	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 2																											//	play	L	H
		{	1	,	sjRGBW(	0.4	,	0	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 3																											//	play	H	L
		{	1	,	sjRGBW(	0.4	,	0	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
	{ // 4																											//	play	H	H
		{	2	,	sjRGBW(	0.4	,	0	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	1	,	sjRGBW(	0	,	0	,	1	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
		{	-1	,	sjRGBW(	0	,	0	,	0	,	0	),	sjRGBW(	0	,	0	,	0	,	0	),	},								
	},																														
},																															
};																															

/************************************************************
************************************************************/

/******************************
******************************/
int L_TABLE_UTIL::Count_NumCandidate(SEL__COL* Array, int NumMax)
{
	int i;
	for(i = 0; i < NumMax; i++){
		if(Array[i].weight == -1) break;
	}
	
	if( (i == 0) || (i == NumMax) ) { ERROR_MSG(); std::exit(1); }
	
	return i;
}

/******************************
******************************/
int L_TABLE_UTIL::Count_NumCandidate(SEL__FUNC_GETLUM* Array, int NumMax)
{
	int i;
	for(i = 0; i < NumMax; i++){
		if(Array[i].weight == -1) break;
	}
	
	if( (i == 0) || (i == NumMax) ) { ERROR_MSG(); std::exit(1); }
	
	return i;
}

/******************************
******************************/
int L_TABLE_UTIL::Count_NumCandidate(SEL__FUNC_GETPOS* Array, int NumMax)
{
	int i;
	for(i = 0; i < NumMax; i++){
		if(Array[i].weight == -1) break;
	}
	
	if( (i == 0) || (i == NumMax) ) { ERROR_MSG(); std::exit(1); }
	
	return i;
}


