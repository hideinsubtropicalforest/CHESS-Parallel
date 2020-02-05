#include <stdio.h>

#include "chess.h"
#include <iostream>
using namespace std;

//struct output_hydro_plant construct_output_files(int start_year,int end_year,
void close_all_output_files(struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf,
	struct output_hydro_plant* DM_outfiles)
{
	if(ComLin->b!=NULL){
		if (DM_outfiles->fBasinDailyHydro != NULL) fclose(DM_outfiles->fBasinDailyHydro);
		if (DM_outfiles->fBasinDailyPlant != NULL) fclose(DM_outfiles->fBasinDailyPlant);
	}
	if (ComLin->gg != NULL) {
		for (int inx = 0; inx != SimInf->gauge_num; inx++) {
			if (DM_outfiles->fGaugeDailyHydro[inx] != NULL) fclose(DM_outfiles->fGaugeDailyHydro[inx]);
			if (DM_outfiles->fGaugeDailyPlant[inx] != NULL) fclose(DM_outfiles->fGaugeDailyPlant[inx]);
		}
	}
    return ;
}