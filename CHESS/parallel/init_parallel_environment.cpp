//------------------------------------------------------------------------------------------------------------------------------
//	A parallel way for computing daily F
//	Designed by Xu.
//------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include "chess.h"
#include "functions.h"
#include "constants.h"
#include <iostream>

using namespace std;


void	init_parallel_environment(struct patch_object *patch, struct  SimulationInformation* SimInf)
{

	//compute number of layers
	SimInf->layer_num = 0;
	for (int inx = 0; inx < SimInf->patch_num; inx++) {
		if (patch[inx].clayer >= SimInf->layer_num)
			SimInf->layer_num = patch[inx].clayer;
	}


	//new memory for arrays
	SimInf->land_pch = new int* [SimInf->thread_num] {};
	for (int inx = 0; inx != SimInf->thread_num; inx++)
		SimInf->land_pch[inx] = new int[SimInf->patch_num] {};

	SimInf->channel_pch = new int** [SimInf->layer_num] {};
	for (int inx = 0; inx != SimInf->layer_num; inx++) {
		SimInf->channel_pch[inx] = new int*[SimInf->thread_num] {};
		for (int iny = 0; iny != SimInf->thread_num; iny++)
			SimInf->channel_pch[inx][iny] = new int[SimInf->patch_num] {};
	}
	SimInf->land_thread_patch_num = new int[SimInf->thread_num] {};
	SimInf->channel_layer_thread_patch_num = new int*[SimInf->layer_num] {};
	for (int inx = 0; inx != SimInf->layer_num; inx++) {
		SimInf->channel_layer_thread_patch_num[inx] = new int[SimInf->thread_num] {};
	}


	int thread_inx=0;
	int layer_inx = 0;
	for (int inx = 0; inx < SimInf->patch_num; inx++) {
		
		//start from 0
		if(patch[inx].sthread > 0){
			
			thread_inx = patch[inx].sthread -1;
			SimInf->land_pch[thread_inx][(SimInf->land_thread_patch_num[thread_inx])] = inx;
			SimInf->land_thread_patch_num[thread_inx]++;
		}

		if (patch[inx].clayer > 0) {
			thread_inx = patch[inx].cthread - 1;//start from 0
			layer_inx = patch[inx].clayer - 1;
			SimInf->channel_pch[layer_inx][thread_inx][(SimInf->channel_layer_thread_patch_num[layer_inx][thread_inx])] = inx;
			SimInf->channel_layer_thread_patch_num[layer_inx][thread_inx]++;
		}
	
	}//end of all patches

}//end of thread_line