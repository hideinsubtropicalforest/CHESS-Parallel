//------------------------------------------------------------------------------------------------------------------------------
//	A parallel way for computing daily F
//	Designed by Xu.
//------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include "CHESS.h"
#include "Functions.h"
#include "Constants.h"
#include <iostream>

using namespace std;


void	init_parallel_environment(struct patch_object *patch,struct parallel_object *parallel)
{

	//in environment 
	parallel->layer_num = 0;
	for (int inx = 0; inx < parallel->patch_num; inx++) {
		if (patch[inx].clayer >= parallel->layer_num)
			parallel->layer_num = patch[inx].clayer;
	}


	//new memory for arrays
	parallel->land_pch = new int* [parallel->thread_num] {};
	for (int inx = 0; inx != parallel->thread_num; inx++)
		parallel->land_pch[inx] = new int[parallel->patch_num] {};

	parallel->channel_pch = new int** [parallel->layer_num] {};
	for (int inx = 0; inx != parallel->layer_num; inx++) {
		parallel->channel_pch[inx] = new int*[parallel->thread_num] {};
		for (int iny = 0; iny != parallel->thread_num; iny++)
			parallel->channel_pch[inx][iny] = new int[parallel->patch_num] {};
	}
	parallel->land_thread_patch_num = new int[parallel->thread_num] {};
	parallel->channel_layer_thread_patch_num = new int*[parallel->layer_num] {};
	for (int inx = 0; inx != parallel->layer_num; inx++) {
		parallel->channel_layer_thread_patch_num[inx] = new int[parallel->thread_num] {};
	}


	int thread_inx=0;
	int layer_inx = 0;
	for (int inx = 0; inx < parallel->patch_num; inx++) {
		
		//start from 0
		thread_inx = patch[inx].sthread -1;

		parallel->land_pch[thread_inx][(parallel->land_thread_patch_num[thread_inx])] = inx;
		
		parallel->land_thread_patch_num[thread_inx]++;

		if (patch[inx].clayer > 0) {
			thread_inx = patch[inx].cthread - 1;//start from 0
			layer_inx = patch[inx].clayer - 1;
			parallel->channel_pch[layer_inx][thread_inx][(parallel->channel_layer_thread_patch_num[layer_inx][thread_inx])] = inx;
			parallel->channel_layer_thread_patch_num[layer_inx][thread_inx]++;
		}
	
	}//end of all patches

}//end of thread_line