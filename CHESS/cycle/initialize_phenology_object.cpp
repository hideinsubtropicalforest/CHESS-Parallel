#include <stdio.h>

#include "chess.h"
#include <iostream>
using namespace std;

void init_phenology_object(patch_object *patch,  struct  SimulationInformation* SimInf)
{

	for (int i = 0; i < SimInf->patch_num; i++){
		patch[i].leaf_expand_flag = -999;
		patch[i].litfall_stop_flag = -999;
		patch[i].preyear_litfall_stopday = patch[i].canopy_strata->phen.litfall_stopday;
	}

	return;
}