/************************************************************
useful little functions
	http://www.iquilezles.org/www/articles/functions/functions.htm
************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/************************************************************
************************************************************/
float gain(float x, float k);

/************************************************************
************************************************************/

/******************************
******************************/
int main(int argc, char** argv)
{
	if(argc < 2){
		printf("exe [k: (0,1)]\n");
		return -1;
	}
	
	FILE* fp;
	fp = fopen("Log.csv", "w");
	if(fp == NULL){
		printf("please close Log.csv\n");
		return -1;
	}
	
	float k = atof(argv[1]);
	
	for(float x = 0; x < 1.0; x += 0.01){
		float y = gain(x, k);
		fprintf(fp, "%f,%f\n", x, y);
	}
	
	printf("Good-bye\n");
	
	return 0;
}

/******************************
******************************/
float gain(float x, float k) 
{
    float a = 0.5*pow(2.0*((x<0.5)?x:1.0-x), k);
    return (x<0.5)?a:1.0-a;
}


