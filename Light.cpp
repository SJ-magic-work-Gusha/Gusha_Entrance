/************************************************************
************************************************************/
#include <new> // placement new

#include "Light.h"

/************************************************************
************************************************************/
/********************
********************/
static ODE ode[] = {
	ODE("10.7.162.176"),
	ODE("10.7.153.16"),
	ODE("10.7.164.56"),
};
static const int NUM_ODES = sizeof(ode) / sizeof(ode[0]);

/********************
********************/
static LED_LIGHT LedLight[] = {
//				ODE id		AddressFrom				Pan_min		Pan_max		Tilt_min		Tilt_max			Hardware(Start Address) setting 
/*	0	*/	LED_LIGHT(	0	,	0	,	LED_DEVICE_TYPE_MOVING	,	36408	,	50970	,	0	,	28086	),	//	1
/*	1	*/	LED_LIGHT(	0	,	13	,	LED_DEVICE_TYPE_MOVING	,	36408	,	50970	,	0	,	28086	),	//	14
/*	2	*/	LED_LIGHT(	0	,	26	,	LED_DEVICE_TYPE_MOVING	,	36408	,	50970	,	0	,	28086	),	//	27
/*	3	*/	LED_LIGHT(	0	,	39	,	LED_DEVICE_TYPE_MOVING	,	36408	,	50970	,	0	,	28086	),	//	40
/*	4	*/	LED_LIGHT(	0	,	52	,	LED_DEVICE_TYPE_MOVING	,	36408	,	50970	,	0	,	28086	),	//	53
/*	5	*/	LED_LIGHT(	0	,	65	,	LED_DEVICE_TYPE_MOVING	,	36408	,	50970	,	0	,	28086	),	//	66
/*	6	*/	LED_LIGHT(	1	,	0	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	1
/*	7	*/	LED_LIGHT(	1	,	6	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	7
/*	8	*/	LED_LIGHT(	2	,	0	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	1
/*	9	*/	LED_LIGHT(	2	,	6	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	7
/*	10	*/	LED_LIGHT(	1	,	12	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	13
/*	11	*/	LED_LIGHT(	1	,	18	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	19
/*	12	*/	LED_LIGHT(	1	,	24	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	25
/*	13	*/	LED_LIGHT(	1	,	30	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	31
/*	14	*/	LED_LIGHT(	1	,	36	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	37
/*	15	*/	LED_LIGHT(	1	,	42	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	43
/*	16	*/	LED_LIGHT(	1	,	48	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	49
/*	17	*/	LED_LIGHT(	1	,	54	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	55
/*	18	*/	LED_LIGHT(	2	,	12	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	13
/*	19	*/	LED_LIGHT(	2	,	18	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	19
/*	20	*/	LED_LIGHT(	2	,	24	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	25
/*	21	*/	LED_LIGHT(	2	,	30	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	31
/*	22	*/	LED_LIGHT(	2	,	36	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	37
/*	23	*/	LED_LIGHT(	2	,	42	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	43
/*	24	*/	LED_LIGHT(	2	,	48	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	49
/*	25	*/	LED_LIGHT(	2	,	54	,	LED_DEVICE_TYPE_FIXED	,	-1	,	1	,	-1	,	1	),	//	55
};

static const int NUM_LEDS = sizeof(LedLight) / sizeof(LedLight[0]);

/********************
********************/
static LED_BLOCK LedBlock[] = {				
	{ // block[0]			
		{ // LogicalCh		
			{0, 	-1},
			{1, 	-1},
			{2, 	-1},
			{	-1},
		},		
	},			
	{ // block[1]			
		{ // LogicalCh		
			{3, 	-1},
			{4, 	-1},
			{5, 	-1},
			{	-1},
		},		
	},			
	{ // block[2]			
		{ // LogicalCh		
			{14, 18,  	-1},
			{13, 19,  	-1},
			{12, 20, 	-1},
			{11, 21, 	-1},
			{10, 22, 	-1},
			{	-1},
		},		
	},			
	{ // block[3]			
		{ // LogicalCh		
			{17, 23,  	-1},
			{16, 24,  	-1},
			{15, 25,	-1},
			{	-1},
		},		
	},			
	{ // block[4]			
		{ // LogicalCh		
			{6, 	-1},
			{7, 	-1},
			{8, 	-1},
			{9, 	-1},
			{	-1},
		},		
	},			
};				

/********************
********************/
static const int NumBlocks = int(sizeof(LedBlock) / sizeof(LedBlock[0]));


/************************************************************
LIGHT
************************************************************/
/****************************************
param
****************************************/
// float LIGHT::d_MinLimit_ValidBlockChange = 0.4;
float LIGHT::d_MinLimit_ValidBlockChange = 2.0;

/****************************************
func
****************************************/

/******************************
******************************/
LIGHT::LIGHT()
: now(0)
, t_LastInt(0)
{
	Vector_Weight.resize(MAX_CANDIDATES_PER_STATE);
}

/******************************
******************************/
LIGHT::~LIGHT()
{
	/********************
	何故か、exit()で以下を記述すると、dmx commandが上手く送られず、照明が光ったまま停止してしまった。
	こちらに持ってくると所望の動作となったので、デストラクタで処理する。
	********************/
	Send_AllZero_to_AllOde();
}

/******************************
******************************/
void LIGHT::exit()
{
}

/******************************
execute like below.
	$ ./Gusha_Entrance 1 2 1 > Log.txt
******************************/
void LIGHT::test()
{
	/********************
	********************/
	printf("> NUM_ODES = %d\n", NUM_ODES);
	
	/********************
	********************/
	printf("> NUM_LEDS = %d\n", NUM_LEDS);
	printf("> check ODE_id\n");
	for(int i = 0; i < NUM_LEDS; i++){
		if(NUM_ODES <= LedLight[i].ODE_id) { ERROR_MSG(); std::exit(1); }
	}
	printf("ok\n");
	
	/********************
	********************/
	printf("> NumBlocks = %d\n", NumBlocks);
	
	/********************
	********************/
	printf("> check LedBlock\n");
	for(int i = 0; i < NumBlocks; i++){
		int NumLogicalChs = Count_NumLogicalChs(LedBlock[i]);
		printf("----- Block[%3d] -----\n", i);
		for(int j = 0; j < NumLogicalChs; j++){
			int NumPhysicalChs = Count_NumPhysicalChs(LedBlock[i].LogicalCh[j]);
			printf("[%3d] : ", j);
			for(int k = 0; k < NumPhysicalChs; k++){
				printf("%d, ", LedBlock[i].LogicalCh[j].PhysicalCh[k] );
			}
			printf("\n");
		}
		printf("\n");
	}
	printf("ok\n");
	
	/********************
	********************/
	check_DesignFunc_Lum();
	check_DesignFunc_Pos();
	
	/********************
	********************/
	check_DesignTable();
	
	/********************
	********************/
	printf("> All passed. Check xls also.\n");
}

/******************************
******************************/
void LIGHT::check_DesignFunc_Lum()
{
	/********************
	********************/
	pFUNC_LUM pFunc_Lum[] = {
		LED_DESIGN_FUNC::Func_GetLum__Stop,
		LED_DESIGN_FUNC::Func_GetLum__On,
		LED_DESIGN_FUNC::Func_GetLum__On_FullW,
		LED_DESIGN_FUNC::Func_GetLum__Flow,
		LED_DESIGN_FUNC::Func_GetLum__Flash,
		LED_DESIGN_FUNC::Func_GetLum__Strobe_1_W,
		LED_DESIGN_FUNC::Func_GetLum__Strobe_1_C,
	};
	int NumFuncs_ToCheck = sizeof(pFunc_Lum) / sizeof(pFunc_Lum[0]);
	
	/* */
	int NumChs[] = {1, 3, 5};
	
	/********************
	********************/
	double d_LumInterval_Strobe = 0.1;
	// double d_LumInterval_Strobe = 0.06;
	double d_LumLength_Strobe = d_LumInterval_Strobe * 1.5;
	double d_LumInterval_Flow = 1.0;
	double d_LumLength_Flow = d_LumInterval_Flow * 2;
	double BeatInterval = 0.5;
	double t_LastBeat = 1.0;
	
	sjRGBW Col_L(0, 0, 0, 0);
	sjRGBW Col_H(1, 1, 1, 1);
	sjRGBW ret_rgbw;
	
	for(int i = 0; i < NumFuncs_ToCheck; i++){
		char FileName[BUF_SIZE];
		sprintf(FileName, "../../../Lum_%d.csv", i);
		FILE* fp = fopen(FileName, "w");
		
		fprintf(fp, ",ch1_0,ch3_0,ch3_1,ch3_2,ch5_0,ch5_1,ch5_2,ch5_3,ch5_4\n");
		
		for(double now = 0; now < 5.0; now += 0.016){
			fprintf(fp, "%f,", now);
			
			for(int j = 0; j < sizeof(NumChs)/sizeof(NumChs[0]); j++){
				for(int k = 0; k < NumChs[j]; k++){
					ret_rgbw = pFunc_Lum[i](now, NumChs[j], k, d_LumInterval_Strobe, d_LumLength_Strobe, d_LumInterval_Flow, d_LumLength_Flow, 1.0, true, BeatInterval, t_LastBeat, Col_L, Col_H, NULL);
					fprintf(fp, "%f,", ret_rgbw.w); // Strobe系は、関数側でcolor = wに自動変換するので、W成分でLevelをoutput. -> Excelでcheck.
				}
			}
			fprintf(fp, "\n");
		}
		
		fclose(fp);
	}
}

/******************************
******************************/
void LIGHT::check_DesignFunc_Pos()
{
	/********************
	********************/
	pFUNC_POS pFunc_Pos[] = {
		LED_DESIGN_FUNC::Func_GetPos__Stop,
		LED_DESIGN_FUNC::Func_GetPos__Center,
		LED_DESIGN_FUNC::Func_GetPos__Circle_L,
		LED_DESIGN_FUNC::Func_GetPos__Circle_L_PhaseDiff,
		LED_DESIGN_FUNC::Func_GetPos__Circle_R,
		LED_DESIGN_FUNC::Func_GetPos__Circle_R_PhaseDiff,
		LED_DESIGN_FUNC::Func_GetPos__Pan,
		LED_DESIGN_FUNC::Func_GetPos__Pan_PhaseDiff,
		LED_DESIGN_FUNC::Func_GetPos__Tilt,
		LED_DESIGN_FUNC::Func_GetPos__Tilt_PhaseDiff,
	};
	int NumFuncs_ToCheck = sizeof(pFunc_Pos) / sizeof(pFunc_Pos[0]);
	
	/* */
	int NumChs[] = {1, 3, 5};
	
	/********************
	********************/
	double d_Interval = 2.0;
	
	sjPanTilt ret_PanTilt;
	
	for(int i = 0; i < NumFuncs_ToCheck; i++){
		char FileName[BUF_SIZE];
		sprintf(FileName, "../../../Pos_%d.csv", i);
		FILE* fp = fopen(FileName, "w");
		
		fprintf(fp, ",ch1_0_Pan,ch1_0_Tilt,ch3_0_Pan,ch3_0_Tilt,ch3_1_Pan,ch3_1_Tilt,ch3_2_Pan,ch3_2_Tilt,ch5_0_Pan,ch5_0_Tilt,ch5_1_Pan,ch5_1_Tilt,ch5_2_Pan,ch5_2_Tilt,ch5_3_Pan,ch5_3_Tilt,ch5_4_Pan,ch5_4_Tilt\n");
		
		for(double now = 0; now < 6.0; now += 0.016){
			fprintf(fp, "%f,", now);
			
			for(int j = 0; j < sizeof(NumChs)/sizeof(NumChs[0]); j++){
				for(int k = 0; k < NumChs[j]; k++){
					ret_PanTilt = pFunc_Pos[i](now, NumChs[j], k, d_Interval, 1.0, NULL);
					fprintf(fp, "%f,%f,", ret_PanTilt.Pan, ret_PanTilt.Tilt);
				}
			}
			fprintf(fp, "\n");
		}
		
		fclose(fp);
	}
}

/******************************
******************************/
void LIGHT::check_DesignTable()
{
	/********************
	********************/
	printf("> NumBlocksOn\n");
	for(int i = 0; i < NUM_STATES; i++){
		if(NumBlocks < T_NumBlocksOn[i]){
			printf("%d, %d\n", i, T_NumBlocksOn[i]);
			ERROR_MSG(); std::exit(1);
		}
	}
	printf("ok\n");
	
	/********************
	********************/
	printf("> NumBlocksOn__Layer_Strobe\n");
	for(int i = 0; i < NUM_STATES; i++){
		if(NumBlocks < T_NumBlocksOn__Layer_Strobe[i]){
			printf("%d, %d\n", i, T_NumBlocksOn__Layer_Strobe[i]);
			ERROR_MSG(); std::exit(1);
		}
	}
	printf("ok\n");
	
	/********************
	********************/
	printf("> Table select func = GetLum\n");
	for(int i = 0; i < NUM_STATES; i++){
		printf("\t----- State[%d] -----\n", i);
		
		int NumCandidates = L_TABLE_UTIL::Count_NumCandidate(T_SelectFunc_GetLum[i], MAX_CANDIDATES_PER_STATE);
		double TotalWeight = 0;
		for(int j = 0; j < NumCandidates; j++){
			TotalWeight += T_SelectFunc_GetLum[i][j].weight;
		}
		for(int j = 0; j < NumCandidates; j++){
			if(0 < T_SelectFunc_GetLum[i][j].weight){
				string Name_Func;
				string Name_FixIdOrder;
				
				(void)T_SelectFunc_GetLum[i][j].pFunc_Lum(0, 0, 0, 0, 0, 0, 0, 0, true, 0, 0, sjRGBW(0, 0, 0, 0), sjRGBW(0, 0, 0, 0), &Name_Func);
				(void)T_SelectFunc_GetLum[i][j].pFunc_FixIdOrder(NULL, 0, &Name_FixIdOrder);
				printf("\t%5.1f : %-40s, %-40s\n", 100 * T_SelectFunc_GetLum[i][j].weight / TotalWeight, Name_Func.c_str(), Name_FixIdOrder.c_str());
			}
		}
		printf("\n");
	}
	
	/********************
	********************/
	printf("> Table select func = GetPos\n");
	for(int i = 0; i < NUM_STATES; i++){
		printf("\t----- State[%d] -----\n", i);
		
		int NumCandidates = L_TABLE_UTIL::Count_NumCandidate(T_SelectFunc_GetPos[i], MAX_CANDIDATES_PER_STATE);
		double TotalWeight = 0;
		for(int j = 0; j < NumCandidates; j++){
			TotalWeight += T_SelectFunc_GetPos[i][j].weight;
		}
		for(int j = 0; j < NumCandidates; j++){
			if(0 < T_SelectFunc_GetPos[i][j].weight){
				string Name_Func;
				string Name_FixIdOrder;
				
				(void)T_SelectFunc_GetPos[i][j].pFunc_Pos(0, 0, 0, 0, 0, &Name_Func);
				(void)T_SelectFunc_GetPos[i][j].pFunc_FixIdOrder(NULL, 0, &Name_FixIdOrder);
				
				printf("\t%5.1f : %-40s, %-40s\n", 100 * T_SelectFunc_GetPos[i][j].weight / TotalWeight, Name_Func.c_str(), Name_FixIdOrder.c_str());
			}
		}
		printf("\n");
	}
	
	/********************
	********************/
	printf("> number of Color Themes\n");
	if( COLOR_THEME::NUM_THEMES != int(sizeof(T_Color) / sizeof(T_Color[0])) ){
			ERROR_MSG(); std::exit(1);
	}
	printf("ok\n");
	
	/********************
	********************/
	printf("> Table select color\n");
	for(int ColTheme = 0; ColTheme < COLOR_THEME::NUM_THEMES; ColTheme++){
		switch(ColTheme){
			case COLOR_THEME::THEME_RED:
				printf("---------- RED ----------\n");
				break;
			case COLOR_THEME::THEME_GREEN:
				printf("---------- GREEN ----------\n");
				break;
			case COLOR_THEME::THEME_BLUE:
				printf("---------- BLUE ----------\n");
				break;
		}
		
		for(int i = 0; i < NUM_STATES; i++){
			printf("\t----- State[%d] -----\n", i);
			
			int NumCandidates = L_TABLE_UTIL::Count_NumCandidate(T_Color[ColTheme][i], MAX_CANDIDATES_PER_STATE);
			double TotalWeight = 0;
			for(int j = 0; j < NumCandidates; j++){
				TotalWeight += T_Color[ColTheme][i][j].weight;
			}
			for(int j = 0; j < NumCandidates; j++){
				if(0 < T_Color[ColTheme][i][j].weight){
					sjRGBW col_H = T_Color[ColTheme][i][j].col_H;
					sjRGBW col_L = T_Color[ColTheme][i][j].col_L;
					
					printf("\t%5.1f : (%5.2f, %5.2f, %5.2f, %5.2f), (%5.2f, %5.2f, %5.2f, %5.2f)\n", 100 * T_Color[ColTheme][i][j].weight / TotalWeight, col_H.r, col_H.g, col_H.b, col_H.w, col_L.r, col_L.g, col_L.b, col_L.w);
				}
			}
			printf("\n");
		}
	}
}

/******************************
******************************/
int LIGHT::Count_NumLogicalChs(const LED_BLOCK& LedBlock)
{
	int i;
	for(i = 0; i < MAX_LOGICAL_CHS_IN_A_BLOCK; i++){
		if(LedBlock.LogicalCh[i].PhysicalCh[0] == -1) break;
	}
	if(i == MAX_LOGICAL_CHS_IN_A_BLOCK){
		ERROR_MSG(); std::exit(1);
	}
	
	return i;
}

/******************************
******************************/
int LIGHT::Count_NumPhysicalChs(const PHYSICAL_CH_SET& LogicalCh)
{
	int i;
	for(i = 0; i < PHYSICAL_CH_SET::MAX_CHS_IN_1LOGICAL_CH; i++){
		if(LogicalCh.PhysicalCh[i] == -1) break;
	}
	if(i == PHYSICAL_CH_SET::MAX_CHS_IN_1LOGICAL_CH){
		ERROR_MSG(); std::exit(1);
	}
	
	return i;
}

/******************************
description
	本Func内で、強制的にPlayStart = stop. にするので、State_BandGainは、zero(stop)になる。
	よって、引数として、State_BandGainは不要.
******************************/
void LIGHT::setup(int State_BandGain, int State_d2_BandGain3)
{
	/********************
	********************/
    //at first you must specify the Ip address of this machine
    artnet.setup("10.0.0.5"); //make sure the firewall is deactivated at this point
	
	/********************
	********************/
    udp_SendTo_VjUnity.Create();
	udp_SendTo_VjUnity.Connect("127.0.0.1",12348);
	udp_SendTo_VjUnity.SetNonBlocking(true);
	
	/********************
	********************/
	for(int i = 0; i < NUM_STATES; i++) { ParamForLedControl__Layer_States[i].Set_SizeOfBlocks(NumBlocks); }
	ParamForLedControl__Layer_Strobe.Set_SizeOfBlocks(NumBlocks);
	ParamForLedControl__Layer_ON.Set_SizeOfBlocks(NumBlocks);
	
	/********************
	void LAYER_ALPHA::setup(float now, int _condition, double RiseTime, double FallTime);
	********************/
	float _now = ofGetElapsedTimef();
	LayerAlpha[0].setup(_now, 1.0, 1.0); // stop.
	LayerAlpha[1].setup(_now, 0.5, 1.0); // [4], [0] = LL
	LayerAlpha[2].setup(_now, 0.0, 1.0); // [4], [0] = LH
	LayerAlpha[3].setup(_now, 0.5, 1.0); // [4], [0] = HL
	LayerAlpha[4].setup(_now, 0.0, 1.0); // [4], [0] = HH
	LayerAlpha_Strobe.setup(_now, 0.5, 2.0);
	LayerAlpha_ON.setup(_now, 0.0, 2.0);
	
	/********************
	********************/
	ColorTheme.setup(int(Gui_Global->ColorTheme_id));
	
	Reset(_now);
	
	/********************
	********************/
	if(Gui_Global == NULL) {ERROR_MSG(); std::exit(1);}
	Last_ParamFromSignalProcess.set(Gui_Global->b_CheckDmx, Gui_Global->PlayStart, State_BandGain, State_d2_BandGain3);
	ParamFromSignalProcess = Last_ParamFromSignalProcess;
}

/******************************
******************************/
void LIGHT::Reset(float now)
{
	if(Gui_Global == NULL) { ERROR_MSG(); std::exit(1); }
	
	Gui_Global->PlayStart = false;

	for(int i = 0; i < NUM_STATES; i++) { LayerAlpha[i].Reset(now);	}
	LayerAlpha_Strobe.Reset(now);
	LayerAlpha_ON.Reset(now);
	
	Init_Design_AllLayer();
}

/******************************
******************************/
void LIGHT::Init_Design_AllLayer()
{
	/********************
	********************/
	for(int State = 0; State < NUM_STATES; State++){
		SetDesign_OneLayer(State);
	}
	
	// 引数 : State はtemporary.
	SetDesign_Layer_Strobe(0);
	SetDesign_Layer_ON(0);
}

/******************************
******************************/
void LIGHT::SetDesign_AllLayer()
{
	for(int i = 0; i < NUM_STATES; i++){
		SetDesign_OneLayer(i);
	}
	
	/********************
	以下は、b_JumToOn のtimingでsetされるので、急いでsetしない.
	********************/
	/*
	SetDesign_Layer_Strobe(ParamFromSignalProcess.State_BandGain);
	SetDesign_Layer_ON(ParamFromSignalProcess.State_BandGain);
	*/
}

/******************************
******************************/
void LIGHT::SetDesign_All_BackLayer()
{
	for(int i = 0; i < NUM_STATES; i++){
		if(i != ParamFromSignalProcess.State_BandGain) SetDesign_OneLayer(i);
	}
	
	/********************
	以下は、b_JumToOn のtimingでsetされるので、急いでsetしない.
	********************/
	/*
	SetDesign_Layer_Strobe(ParamFromSignalProcess.State_BandGain);
	SetDesign_Layer_ON(ParamFromSignalProcess.State_BandGain);
	*/
}

/******************************
******************************/
void LIGHT::SetDesign_OneLayer(int State)
{
	/********************
	********************/
	for(int DesignId = 0; DesignId < NUM_DESIGNS_PER_BLOCK; DesignId++){
		bool b_IsSamaDesignForAllBlock = Dice_IsSameDesignForAllBlock(State);
		
		// まず、block[0]
		pFUNC_FixIdOrder pFunc_FixIdOrder = Dice_FuncLum(0, State, DesignId);
		Dice_Color_LH(0, State, DesignId);
		
		// block[1] -
		for(int BlockId = 1; BlockId < NumBlocks; BlockId++){
			if(b_IsSamaDesignForAllBlock){
				LedBlock[BlockId].LedDesign_Lum[State][DesignId] = LedBlock[0].LedDesign_Lum[State][DesignId];
				
				int Num_LogicalChs = Count_NumLogicalChs(LedBlock[BlockId]);
				pFunc_FixIdOrder(LedBlock[BlockId].LedDesign_Lum[State][DesignId].id_order, Num_LogicalChs, NULL);
			}else{
				Dice_FuncLum(BlockId, State, DesignId);
				Dice_Color_LH(BlockId, State, DesignId);
			}
		}
	}
	
	/********************
	********************/
	{
		bool b_IsSamaDesignForAllBlock = Dice_IsSameDesignForAllBlock(State);
		
		// まず、block[0]
		pFUNC_FixIdOrder pFunc_FixIdOrder = Dice_FuncPos(0, State);
		
		// block[1] -
		for(int BlockId = 1; BlockId < NumBlocks; BlockId++){
			if(b_IsSamaDesignForAllBlock){
				LedBlock[BlockId].LedDesign_Pos[State] = LedBlock[0].LedDesign_Pos[State];
				
				int Num_LogicalChs = Count_NumLogicalChs(LedBlock[BlockId]);
				pFunc_FixIdOrder(LedBlock[BlockId].LedDesign_Pos[State].id_order, Num_LogicalChs, NULL);
			}else{
				Dice_FuncPos(BlockId, State);
			}
		}
	}
	
	/********************
	********************/
	Fix__ParamForLedControl__Layer_States(State);
}

/******************************
******************************/
void LIGHT::SetDesign_Layer_Strobe(int State)
{
	for(int i = 0; i < NumBlocks; i++){
		Set_FuncLum__LayerStrobe(i);
		Set_Color_LH__Strobe(i);
	}
	
	Fix__ParamForLedControl__Layer_Strobe(State);
}

/******************************
******************************/
void LIGHT::SetDesign_Layer_ON(int State)
{
	for(int i = 0; i < NumBlocks; i++){
		Set_FuncLum__LayerON(i);
		Set_Color_LH__ON(i, State);
	}
	
	Fix__ParamForLedControl__Layer_ON();
}

/******************************
******************************/
void LIGHT::setparam_LedLight_for_DmxTest()
{
	for(int i = 0; i < NUM_LEDS; i++){
		LedLight[i].LedParam_Out.clear();
	}
	
	int id = Gui_Global->Dmx_TestId;
	if(id < 0) 				return;
	else if(NUM_LEDS <= id)	id = NUM_LEDS - 1;
	
	ofColor color = Gui_Global->Dmx_TestColor;
	LedLight[id].LedParam_Out.rgbw.r = double(color.r) / 255;
	LedLight[id].LedParam_Out.rgbw.g = double(color.g) / 255;
	LedLight[id].LedParam_Out.rgbw.b = double(color.b) / 255;
	LedLight[id].LedParam_Out.rgbw.w = double(color.a) / 255;
}

/******************************
******************************/
bool LIGHT::Dice_IsSameDesignForAllBlock(int State_BandGain)
{
	int NumCandidates = 2;
	
	if(Vector_Weight.size() < NumCandidates) Vector_Weight.resize(NumCandidates);
	
	for(int i = 0; i < NumCandidates; i++){
		Vector_Weight[i] = T_SameOrDiffDesignForEachBlock[State_BandGain][i];
	}
	
	if(L_UTIL::Dice_index(Vector_Weight, NumCandidates) == 0)	return true;
	else														return false;
}

/******************************
******************************/
pFUNC_FixIdOrder LIGHT::Dice_FuncLum(int BlockId, int State_BandGain, int DesignId)
{
	/********************
	candidate Arrayは、Stateによって一意に決まる.
	********************/
	int NumCandidates = L_TABLE_UTIL::Count_NumCandidate(T_SelectFunc_GetLum[State_BandGain], MAX_CANDIDATES_PER_STATE);
	
	if(Vector_Weight.size() < NumCandidates) Vector_Weight.resize(NumCandidates);
	
	for(int i = 0; i < NumCandidates; i++){
		Vector_Weight[i] = T_SelectFunc_GetLum[State_BandGain][i].weight;
	}
	
	/********************
	candidateの中で、どれを選択するかは、blockごとにDice.
	********************/
	int id = L_UTIL::Dice_index(Vector_Weight, NumCandidates);
	LedBlock[BlockId].LedDesign_Lum[State_BandGain][DesignId].pFunc = T_SelectFunc_GetLum[State_BandGain][id].pFunc_Lum;
	
	int Num_LogicalChs = Count_NumLogicalChs(LedBlock[BlockId]);
	T_SelectFunc_GetLum[State_BandGain][id].pFunc_FixIdOrder(LedBlock[BlockId].LedDesign_Lum[State_BandGain][DesignId].id_order, Num_LogicalChs, NULL);
	
	/********************
	********************/
	return T_SelectFunc_GetLum[State_BandGain][id].pFunc_FixIdOrder;
}

/******************************
******************************/
void LIGHT::Dice_Color_LH(int BlockId, int State_BandGain, int DesignId)
{
	/********************
	candidate Arrayは、State_BandGainによって一意に決まる.
	********************/
	int NumCandidates = L_TABLE_UTIL::Count_NumCandidate(T_Color[ColorTheme.getTheme()][State_BandGain], MAX_CANDIDATES_PER_STATE);
	
	if(Vector_Weight.size() < NumCandidates) Vector_Weight.resize(NumCandidates);
	
	for(int i = 0; i < NumCandidates; i++){
		Vector_Weight[i] = T_Color[ColorTheme.getTheme()][State_BandGain][i].weight;
	}
	
	/********************
	candidateの中で、どれを選択するかは、blockごとにDice.
	********************/
	int id = L_UTIL::Dice_index(Vector_Weight, NumCandidates);
	LedBlock[BlockId].LedDesign_Lum[State_BandGain][DesignId].color_L = T_Color[ColorTheme.getTheme()][State_BandGain][id].col_L;
	LedBlock[BlockId].LedDesign_Lum[State_BandGain][DesignId].color_H = T_Color[ColorTheme.getTheme()][State_BandGain][id].col_H;
}

/******************************
******************************/
pFUNC_FixIdOrder LIGHT::Dice_FuncPos(int BlockId, int State_BandGain)
{
	/********************
	candidate Arrayは、State_BandGainによって一意に決まる.
	********************/
	int NumCandidates = L_TABLE_UTIL::Count_NumCandidate(T_SelectFunc_GetPos[State_BandGain], MAX_CANDIDATES_PER_STATE);
	
	if(Vector_Weight.size() < NumCandidates) Vector_Weight.resize(NumCandidates);
	
	for(int i = 0; i < NumCandidates; i++){
		Vector_Weight[i] = T_SelectFunc_GetPos[State_BandGain][i].weight;
	}
	
	/********************
	candidateの中で、どれを選択するかは、blockごとにDice.
	********************/
	int id = L_UTIL::Dice_index(Vector_Weight, NumCandidates);
	LedBlock[BlockId].LedDesign_Pos[State_BandGain].pFunc = T_SelectFunc_GetPos[State_BandGain][id].pFunc_Pos;
	
	int Num_LogicalChs = Count_NumLogicalChs(LedBlock[BlockId]);
	T_SelectFunc_GetPos[State_BandGain][id].pFunc_FixIdOrder(LedBlock[BlockId].LedDesign_Pos[State_BandGain].id_order, Num_LogicalChs, NULL);
	
	/********************
	********************/
	return T_SelectFunc_GetPos[State_BandGain][id].pFunc_FixIdOrder;
}

/******************************
******************************/
pFUNC_FixIdOrder LIGHT::Set_FuncLum__LayerStrobe(int BlockId)
{
	/********************
	********************/
	LedBlock[BlockId].LedDesign_Lum__StrobeW.pFunc = LED_DESIGN_FUNC::Func_GetLum__Strobe_1_W;
	
	int Num_LogicalChs = Count_NumLogicalChs(LedBlock[BlockId]);
	LED_DESIGN_FUNC::FixIdOrder_Random(LedBlock[BlockId].LedDesign_Lum__StrobeW.id_order, Num_LogicalChs, NULL);
	
	/********************
	********************/
	return LED_DESIGN_FUNC::FixIdOrder_Random;
}

/******************************
******************************/
pFUNC_FixIdOrder LIGHT::Set_FuncLum__LayerON(int BlockId)
{
	/********************
	********************/
	LedBlock[BlockId].LedDesign_Lum__On.pFunc = LED_DESIGN_FUNC::Func_GetLum__On_FullW;
	
	int Num_LogicalChs = Count_NumLogicalChs(LedBlock[BlockId]);
	LED_DESIGN_FUNC::FixIdOrder_Forward(LedBlock[BlockId].LedDesign_Lum__On.id_order, Num_LogicalChs, NULL);
	
	/********************
	********************/
	return LED_DESIGN_FUNC::FixIdOrder_Forward;
}

/******************************
******************************/
void LIGHT::Set_Color_LH__Strobe(int BlockId)
{
	sjRGBW Col_Black(0, 0, 0, 0);
	sjRGBW Col_White(0, 0, 0, 1.0);

	LedBlock[BlockId].LedDesign_Lum__StrobeW.color_L = Col_Black;
	LedBlock[BlockId].LedDesign_Lum__StrobeW.color_H = Col_White;
}

/******************************
******************************/
void LIGHT::Set_Color_LH__ON(int BlockId, int State)
{
	int Design_id = ParamForLedControl__Layer_States[State].Design_id;
	
	LedBlock[BlockId].LedDesign_Lum__On.color_L = LedBlock[BlockId].LedDesign_Lum[State][Design_id].color_L;
	LedBlock[BlockId].LedDesign_Lum__On.color_H = LedBlock[BlockId].LedDesign_Lum[State][Design_id].color_H;
}

/******************************
******************************/
void LIGHT::Fix__ParamForLedControl__Layer_States(int State_BandGain)
{
	ParamForLedControl__Layer_States[State_BandGain].set(T_NumBlocksOn[State_BandGain], L_UTIL::FisherYates, T_dInterval_Strobe[State_BandGain], T_dLength_Strobe[State_BandGain], T_dInterval_Flow[State_BandGain], T_dLength_Flow[State_BandGain], T_dPanTilt[State_BandGain]);
}

/******************************
******************************/
void LIGHT::Fix__ParamForLedControl__Layer_Strobe(int State_BandGain)
{
	ParamForLedControl__Layer_Strobe.set(T_NumBlocksOn__Layer_Strobe[State_BandGain], L_UTIL::FisherYates, T_dInterval_Strobe[State_BandGain], T_dLength_Strobe[State_BandGain], T_dInterval_Flow[State_BandGain], T_dLength_Flow[State_BandGain], T_dPanTilt[State_BandGain]);
}

/******************************
******************************/
void LIGHT::Fix__ParamForLedControl__Layer_ON()
{
	ParamForLedControl__Layer_ON.set(NumBlocks, L_UTIL::Array_Forward, 1, 1, 1, 1, 1); // dInterval_, dLength, については、使わないので、zeroでいいが、Intervalなどがzeroと言うのも気持ち悪いので、1にしておく.
}

/******************************
******************************/
void LIGHT::update(int State_BandGain, bool b_Layer_strobe, bool b_Layer_ON, int State_d2_BandGain3, double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat, bool& b_Beat_Band3)
{
	/********************
	********************/
	now = ofGetElapsedTimef();
	
	/********************
	Receive Param.
	********************/
	Last_ParamFromSignalProcess = ParamFromSignalProcess;
	if(Gui_Global == NULL) {ERROR_MSG(); std::exit(1);}
	ParamFromSignalProcess.set(Gui_Global->b_CheckDmx, Gui_Global->PlayStart, State_BandGain, State_d2_BandGain3);
	
	/********************
	StateChart : gui__b_CheckDmx
	********************/
	if( Last_ParamFromSignalProcess.gui__b_CheckDmx && !ParamFromSignalProcess.gui__b_CheckDmx )		{ Reset(now); }
	else if( !Last_ParamFromSignalProcess.gui__b_CheckDmx && ParamFromSignalProcess.gui__b_CheckDmx)	{ /* nothing. */ }
	
	/********************
	Dmx test mode -> test & return.
	********************/
	if(ParamFromSignalProcess.gui__b_CheckDmx) { setparam_LedLight_for_DmxTest(); goto EXIT_FUNC; }
	
	/********************
	********************/
	// for(int i = 0; i < NUM_STATES; i++) { LayerAlpha[i].update(now, State_BandGain == i); }
	LayerAlpha[0].update(now, State_BandGain == 0);
	LayerAlpha[1].update(now, State_BandGain == 1);
	LayerAlpha[2].update(now, State_BandGain == 2);
	LayerAlpha[3].update(now, (State_BandGain == 3) && (!b_Layer_strobe));
	LayerAlpha[4].update(now, State_BandGain == 4);
	
	LayerAlpha_Strobe.update(now, b_Layer_strobe);
	LayerAlpha_ON.update(now, b_Layer_ON);
	
	/********************
	int Gui_theme_id, LIGHT& Light, pFUNC_LIGHT_voidvoid pFunc_immediate
	void update_Auto(bool b_Enable, float now, int _State){
	********************/
	ColorTheme.update_byGui(int(Gui_Global->ColorTheme_id), *this, &LIGHT::SetDesign_All_BackLayer, &LIGHT::SetDesign_AllLayer);
	ColorTheme.update_Auto(Gui_Global->b_AutoColorChange, now, State_BandGain);
	
	/********************
	each Layer : change Design.
	********************/
	for(int i = 0; i < NUM_STATES; i++){
		if(LayerAlpha[i].IsStable_at_Off(now)) SetDesign_OneLayer(i);
	}
	
	if(LayerAlpha_Strobe.IsState_JumpTo_ON())	SetDesign_Layer_Strobe(State_BandGain);
	if(LayerAlpha_ON.IsState_JumpTo_ON())		SetDesign_Layer_ON(State_BandGain);

	/********************
	注)
	-	b_Beat_Band3は、gui__b_CheckDmxを外すまで更新されない(Graphにも出てこない).
	-	"mode == BOOTMODE__NO_DMX"の時も、Lightのmethodが呼ばれないので、更新されない.
	********************/
	b_Beat_Band3 = false;
	if(Gui_Global->BeatDetect_Rise){
		if( (Last_ParamFromSignalProcess.State_d2_BandGain3 == 0) && (ParamFromSignalProcess.State_d2_BandGain3 == 1) ) b_Beat_Band3 = true;
	}else{
		if( (Last_ParamFromSignalProcess.State_d2_BandGain3 == 1) && (ParamFromSignalProcess.State_d2_BandGain3 == 0) ) b_Beat_Band3 = true;
	}
	if( b_Beat_Band3 && (d_MinLimit_ValidBlockChange < now - CommonParamForLedControl.t_DesignId_ValidBlock_change) ){
		ParamForLedControl__Layer_States[State_BandGain].Design_id++;
		if(NUM_DESIGNS_PER_BLOCK <= ParamForLedControl__Layer_States[State_BandGain].Design_id) ParamForLedControl__Layer_States[State_BandGain].Design_id = 0;
		
		ParamForLedControl__Layer_States[State_BandGain].inc_IdFrom();
		ParamForLedControl__Layer_Strobe.inc_IdFrom();
		ParamForLedControl__Layer_ON.inc_IdFrom();
		
		CommonParamForLedControl.t_DesignId_ValidBlock_change = now;
	}else{
		b_Beat_Band3 = false; // for Graph. BeatDetectも、timeでskipされた時.
	}

	/********************
	********************/
	for(int i = 0; i < NUM_LEDS; i++) { LedLight[i].clear(); }
	
	update_LedParam_Lum(Amp_GainSync, b_BeatLock, BeatInterval, t_LastBeat);
	update_LedParam_PanTilt(State_BandGain, Amp_GainSync);
	
	
	EXIT_FUNC:
	/********************
	udp
	********************/
	if(b_EnableProcess_vj) SendUdp_Vj_Unity(b_Beat_Band3);
	
	/********************
	********************/
	t_LastInt = now;
}

/******************************
******************************/
void LIGHT::SendUdp_Vj_Unity(bool b_Beat_Band3)
{
	/********************
	********************/
	char buf[BUF_SIZE];
	string message="";
	
	message += "/FromEntrance<p>";
	
	/********************
	********************/
	if(b_Beat_Band3)	message += "1<p>";
	else				message += "0<p>";
	
	/********************
	********************/
	for(int i = 0; i < NUM_LEDS; i++){
		if(i == NUM_LEDS - 1)	sprintf(buf, "%f,%f,%f,%f", LedLight[i].LedParam_Out.rgbw.r, LedLight[i].LedParam_Out.rgbw.g, LedLight[i].LedParam_Out.rgbw.b, LedLight[i].LedParam_Out.rgbw.w);
		else					sprintf(buf, "%f,%f,%f,%f|", LedLight[i].LedParam_Out.rgbw.r, LedLight[i].LedParam_Out.rgbw.g, LedLight[i].LedParam_Out.rgbw.b, LedLight[i].LedParam_Out.rgbw.w);
		
		message += buf;
	}

	/********************
	********************/
	udp_SendTo_VjUnity.Send(message.c_str(),message.length());
}

/******************************
******************************/
void LIGHT::inc_ValidBlocks_BlockId(int& id)
{
	id++;
	if(NumBlocks <= id) id = 0;
}

/******************************
******************************/
void LIGHT::update_LedParam_Lum(double Amp_GainSync, bool b_BeatLock, double BeatInterval, double t_LastBeat)
{
	/********************
	********************/
	int count = 0;
	for(int Layer = 0; Layer < NUM_STATES; Layer++){
		int DesignId = ParamForLedControl__Layer_States[Layer].Design_id;
		
		count = 0;
		for(int BlockId = ParamForLedControl__Layer_States[Layer].id_from; count < ParamForLedControl__Layer_States[Layer].NumBlocks_on; count++, inc_ValidBlocks_BlockId(BlockId)){
			int NumLogicalChs = Count_NumLogicalChs(LedBlock[BlockId]);
			
			for(int i = 0; i < NumLogicalChs; i++){
				sjRGBW color = LedBlock[BlockId].LedDesign_Lum[Layer][DesignId].pFunc(now, NumLogicalChs, i, 
														ParamForLedControl__Layer_States[Layer].d_LumInterval_Strobe, ParamForLedControl__Layer_States[Layer].d_LumLength_Strobe, 
														ParamForLedControl__Layer_States[Layer].d_LumInterval_Flow, ParamForLedControl__Layer_States[Layer].d_LumLength_Flow,
														Amp_GainSync, b_BeatLock, BeatInterval, t_LastBeat, 
														LedBlock[BlockId].LedDesign_Lum[Layer][DesignId].color_L, LedBlock[BlockId].LedDesign_Lum[Layer][DesignId].color_H, NULL );
				
				color *= LayerAlpha[Layer].get_alpha();
				
				int LogicalCh_id = LedBlock[BlockId].LedDesign_Lum[Layer][DesignId].id_order[i];
				int NumPhysicalChs = Count_NumPhysicalChs(LedBlock[BlockId].LogicalCh[LogicalCh_id]);
				for(int j = 0; j < NumPhysicalChs; j++){
					int PhysicalCh_id = LedBlock[BlockId].LogicalCh[LogicalCh_id].PhysicalCh[j];
					LedLight[PhysicalCh_id].AddLum(color);
				}
			}
		}
	}
	
	/********************
	********************/
	count = 0;
	for(int BlockId = ParamForLedControl__Layer_Strobe.id_from; count < ParamForLedControl__Layer_Strobe.NumBlocks_on; count++, inc_ValidBlocks_BlockId(BlockId)){
		int NumLogicalChs = Count_NumLogicalChs(LedBlock[BlockId]);
		
		for(int i = 0; i < NumLogicalChs; i++){
			sjRGBW color = LedBlock[BlockId].LedDesign_Lum__StrobeW.pFunc(now, NumLogicalChs, i, 
													ParamForLedControl__Layer_Strobe.d_LumInterval_Strobe, ParamForLedControl__Layer_Strobe.d_LumLength_Strobe, 
													ParamForLedControl__Layer_Strobe.d_LumInterval_Flow, ParamForLedControl__Layer_Strobe.d_LumLength_Flow,
													Amp_GainSync, b_BeatLock, BeatInterval, t_LastBeat, 
													LedBlock[BlockId].LedDesign_Lum__StrobeW.color_L, LedBlock[BlockId].LedDesign_Lum__StrobeW.color_H, NULL );
			
			color *= LayerAlpha_Strobe.get_alpha();
			
			int LogicalCh_id = LedBlock[BlockId].LedDesign_Lum__StrobeW.id_order[i];
			int NumPhysicalChs = Count_NumPhysicalChs(LedBlock[BlockId].LogicalCh[LogicalCh_id]);
			for(int j = 0; j < NumPhysicalChs; j++){
				int PhysicalCh_id = LedBlock[BlockId].LogicalCh[LogicalCh_id].PhysicalCh[j];
				LedLight[PhysicalCh_id].AddLum(color);
			}
		}
	}
	
	/********************
	********************/
	count = 0;
	for(int BlockId = ParamForLedControl__Layer_ON.id_from; count < ParamForLedControl__Layer_ON.NumBlocks_on; count++, inc_ValidBlocks_BlockId(BlockId)){
		int NumLogicalChs = Count_NumLogicalChs(LedBlock[BlockId]);
		
		for(int i = 0; i < NumLogicalChs; i++){
			sjRGBW color = LedBlock[BlockId].LedDesign_Lum__On.pFunc(now, NumLogicalChs, i, 
													ParamForLedControl__Layer_ON.d_LumInterval_Strobe, ParamForLedControl__Layer_ON.d_LumLength_Strobe, 
													ParamForLedControl__Layer_ON.d_LumInterval_Flow, ParamForLedControl__Layer_ON.d_LumLength_Flow,
													Amp_GainSync, b_BeatLock, BeatInterval, t_LastBeat, 
													LedBlock[BlockId].LedDesign_Lum__On.color_L, LedBlock[BlockId].LedDesign_Lum__On.color_H, NULL );
			
			color *= LayerAlpha_ON.get_alpha();
			
			int LogicalCh_id = LedBlock[BlockId].LedDesign_Lum__On.id_order[i];
			int NumPhysicalChs = Count_NumPhysicalChs(LedBlock[BlockId].LogicalCh[LogicalCh_id]);
			for(int j = 0; j < NumPhysicalChs; j++){
				int PhysicalCh_id = LedBlock[BlockId].LogicalCh[LogicalCh_id].PhysicalCh[j];
				LedLight[PhysicalCh_id].AddLum(color);
			}
		}
	}
}

/******************************
******************************/
void LIGHT::update_LedParam_PanTilt(int State_BandGain, double Amp_GainSync)
{
	for(int BlockId = 0; BlockId < NumBlocks; BlockId++){
		int NumLogicalChs = Count_NumLogicalChs(LedBlock[BlockId]);
		
		for(int i = 0; i < NumLogicalChs; i++){
			sjPanTilt PanTilt = LedBlock[BlockId].LedDesign_Pos[State_BandGain].pFunc(now, NumLogicalChs, i, ParamForLedControl__Layer_States[State_BandGain].d_PanTilt, Amp_GainSync, NULL);
			
			int LogicalCh_id = LedBlock[BlockId].LedDesign_Pos[State_BandGain].id_order[i];
			int NumPhysicalChs = Count_NumPhysicalChs(LedBlock[BlockId].LogicalCh[LogicalCh_id]);
			for(int j = 0; j < NumPhysicalChs; j++){
				int PhysicalCh_id = LedBlock[BlockId].LogicalCh[LogicalCh_id].PhysicalCh[j];
				
				LedLight[PhysicalCh_id].LedParam_Out.PanTilt = PanTilt;
			}
		}
	}
}

/******************************
******************************/
void LIGHT::draw()
{
	sendDmx();
}

/******************************
******************************/
void LIGHT::Send_AllZero_to_AllOde()
{
	for(int i = 0; i < NUM_ODES; i++){
		for(int j = 0; j < SIZE_DMX_UNIVERSE; j++){
			ode[i].universe[j] = 0;
		}
		
		artnet.sendDmx(ode[i].get_IP(), ode[i].universe, SIZE_DMX_UNIVERSE);
	}
}

/******************************
******************************/
void LIGHT::sendDmx()
{
	/********************
	********************/
	for(int i = 0; i < NUM_LEDS; i++){
		switch(LedLight[i].LedDeviceType){
			case LED_DEVICE_TYPE_FIXED:
				// ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 0 ] = 255; // dimmer
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 0 ] = int(255 * Gui_Global->Led_dimmer); // dimmer
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 1 ] = ofMap(LedLight[i].LedParam_Out.rgbw.r, 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 2 ] = ofMap(LedLight[i].LedParam_Out.rgbw.g, 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 3 ] = ofMap(LedLight[i].LedParam_Out.rgbw.b, 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 4 ] = ofMap(LedLight[i].LedParam_Out.rgbw.w, 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 5 ] = 1; // Strobe = open.
				
				break;
				
			case LED_DEVICE_TYPE_MOVING:
			{
				int Pan		= ofMap(LedLight[i].LedParam_Out.PanTilt.Pan, -1, 1, LedLight[i].Pan_min, LedLight[i].Pan_max, true);
				int Tilt	= ofMap(LedLight[i].LedParam_Out.PanTilt.Tilt, -1, 1, LedLight[i].Tilt_min, LedLight[i].Tilt_max, true);
				
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom +  0 ] = (unsigned char)((Pan >> 8) & 0xFF);	// H
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom +  1 ] = (unsigned char)((Pan >> 0) & 0xFF);	// L
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom +  2 ] = (unsigned char)((Tilt >> 8) & 0xFF);	// H
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom +  3 ] = (unsigned char)((Tilt >> 0) & 0xFF);	// L
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom +  4 ] = ofMap(LedLight[i].LedParam_Out.rgbw.r, 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom +  5 ] = ofMap(LedLight[i].LedParam_Out.rgbw.g, 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom +  6 ] = ofMap(LedLight[i].LedParam_Out.rgbw.b, 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom +  7 ] = ofMap(LedLight[i].LedParam_Out.rgbw.w, 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom +  8 ] = 255;	// shutter open
				
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom +  9 ] = 255; // dimmer
				
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 10 ] = 0; // Pan/Tilt speed : [0 : fast] <---> [slow : 255]
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 11 ] = 0; // Blackout reset
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 12 ] = 0; // dimmer curve = standard
			}
				
				break;
		}
	}
	
	/********************
	********************/
	for(int i = 0; i < NUM_ODES; i++){
		artnet.sendDmx(ode[i].get_IP(), ode[i].universe, SIZE_DMX_UNIVERSE);
	}
}


