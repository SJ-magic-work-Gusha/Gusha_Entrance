/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "sj_common.h"
#include "L_common.h"


/************************************************************
************************************************************/

/**************************************************
**************************************************/
struct PHYSICAL_CH_SET{
	enum{
		MAX_CHS_IN_1LOGICAL_CH = 50,
	};
	int PhysicalCh[MAX_CHS_IN_1LOGICAL_CH + 1];
};

/**************************************************
**************************************************/
enum{
	MAX_LOGICAL_CHS_IN_A_BLOCK = 20,
};

/**************************************************
**************************************************/
struct LED_DESIGN_LUM{
	pFUNC_LUM pFunc;
	sjRGBW color_L;
	sjRGBW color_H;
	
	int id_order[MAX_LOGICAL_CHS_IN_A_BLOCK];
	
	void set(pFUNC_LUM _pFunc, const sjRGBW& _col_L, const sjRGBW& _col_H, pFUNC_FixIdOrder pFunc_FixIdOrder, int NumLogicalChs){
		pFunc = _pFunc;
		
		color_L = _col_L;
		color_H = _col_H;
		
		pFunc = _pFunc;
		
		pFunc_FixIdOrder(id_order, NumLogicalChs, NULL);
	}
};

/**************************************************
**************************************************/
struct LED_DESIGN_POS{
	pFUNC_POS pFunc;
	
	int id_order[MAX_LOGICAL_CHS_IN_A_BLOCK];
	
	void set(pFUNC_POS _pFunc, pFUNC_FixIdOrder pFunc_FixIdOrder, int NumLogicalChs){
		pFunc = _pFunc;
		
		pFunc_FixIdOrder(id_order, NumLogicalChs, NULL);
	}
};

/**************************************************
**************************************************/
enum{
	NUM_DESIGNS_PER_BLOCK = 4,
};

/**************************************************
**************************************************/
struct LED_BLOCK{
	/********************
	********************/
	PHYSICAL_CH_SET LogicalCh[MAX_LOGICAL_CHS_IN_A_BLOCK + 1];
	
	/********************
	********************/
	LED_DESIGN_POS LedDesign_Pos[NUM_STATES];
	LED_DESIGN_LUM LedDesign_Lum[NUM_STATES][NUM_DESIGNS_PER_BLOCK];
	
	LED_DESIGN_LUM LedDesign_Lum__StrobeW;
	LED_DESIGN_LUM LedDesign_Lum__On;
};

/**************************************************
**************************************************/
struct PARAM_FOR_LED_CONTROL : private Noncopyable{
	int NumBlocks_on;
	vector<int> Order_of_ValidBlocks;
	int id_from;
	int Design_id;
	
	double d_LumInterval_Strobe;
	double d_LumLength_Strobe;
	double d_LumInterval_Flow;
	double d_LumLength_Flow;
	double d_PanTilt;
	
	PARAM_FOR_LED_CONTROL()
	{
	}
	
	void Set_SizeOfBlocks(int NumBlocks)
	{
		Order_of_ValidBlocks.resize(NumBlocks);
	}
	
	void inc_IdFrom()
	{
		id_from++;
		if(Order_of_ValidBlocks.size() <= id_from) id_from = 0;
	}
	
	void set(int _NumBlocks_on, pFUNC_VectorSort pFunc_VectorSort, double _d_LumInterval_Strobe, double _d_LumLength_Strobe, double _d_LumInterval_Flow, double _d_LumLength_Flow, double _d_PanTilt){
		NumBlocks_on = _NumBlocks_on;
		
		if(pFunc_VectorSort != NULL){
			pFunc_VectorSort(Order_of_ValidBlocks);
			id_from = 0;
		}
		
		Design_id = 0;
		
		d_LumInterval_Strobe = _d_LumInterval_Strobe;
		d_LumLength_Strobe = _d_LumLength_Strobe;
		d_LumInterval_Flow = _d_LumInterval_Flow;
		d_LumLength_Flow = _d_LumLength_Flow;
		d_PanTilt = _d_PanTilt;
	}
	
	void Copy(PARAM_FOR_LED_CONTROL& org){
		if(Order_of_ValidBlocks.size() == org.Order_of_ValidBlocks.size()){
			NumBlocks_on = org.NumBlocks_on;
			copy(Order_of_ValidBlocks.begin(), Order_of_ValidBlocks.end(), org.Order_of_ValidBlocks.begin()); // readonly not assignable.
			id_from = org.id_from;
			
			d_LumInterval_Strobe = org.d_LumInterval_Strobe;
			d_LumLength_Strobe = org.d_LumLength_Strobe;
			d_LumInterval_Flow = org.d_LumInterval_Flow;
			d_LumLength_Flow = org.d_LumLength_Flow;
			d_PanTilt = org.d_PanTilt;
		}
	}
};

/**************************************************
**************************************************/
struct PARAM_FOR_LED_CONTROL_COMMON : private Noncopyable{
	double t_DesignId_ValidBlock_change;
	
	PARAM_FOR_LED_CONTROL_COMMON()
	: t_DesignId_ValidBlock_change(0)
	{
	}
};

