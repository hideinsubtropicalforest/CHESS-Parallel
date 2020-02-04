#include <stdio.h>
#include "functions.h"
#include <stdlib.h>
#include "chess.h"
#include <iostream>
using namespace std;
void out_patch_level_daily(
	struct patch_object* patch,
	struct CommandLineObject* ComLin,
	struct  SimulationInformation* SimInf,
	struct date current_date,
	struct OutputDateRange* OutDate,
	struct output_hydro_plant* DM_outfiles,
	struct OutArray_object *OutArray)
{
	int num_patches = SimInf->patch_num;
	struct OutputDateRange outdate = *OutDate;

	static int out_flag=0;

	if (current_date.year == outdate.first_year && current_date.month == outdate.first_month && current_date.day == outdate.first_day)	
		out_flag = 1;	

	//if(out_flag ==1 && ComLin->p !=NULL && (current_date.year>=1997 && current_date.year<=2005 )){//
	if(out_flag ==1 && ComLin->p !=NULL ){

		for(int pch=0; pch <num_patches; pch++){
			
			output_patch_daily_hydrology(&patch[pch], pch, OutArray->HydroMon,current_date,DM_outfiles->fPatchDailyHydro,ComLin);
			output_patch_daily_growth(&patch[pch], pch, OutArray->PlantMon, current_date, DM_outfiles->fPatchDailyPlant, ComLin);
		}
		//}
	}

    if(current_date.year == outdate.last_year && current_date.month == outdate.last_month && current_date.day == outdate.last_day)
		out_flag=0;

	return;
}