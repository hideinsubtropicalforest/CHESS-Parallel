#include <stdio.h>

#include <stdlib.h>
#include "chess.h"


void out_basin_level_daily(
				struct patch_object *patch,
				struct CommandLineObject* ComLin, 
				struct  SimulationInformation* SimInf,
			    struct date current_date,
				struct OutputDateRange *OutDate,
			    struct output_hydro_plant *DM_outfiles
			    )
{
	int num_patches = SimInf->patch_num;
	struct OutputDateRange outdate = *OutDate;

	//local functions
	void	output_basin_daily_hydrology(int,struct patch_object *,struct date ,FILE *,struct CommandLineObject *);
	void	output_basin_daily_growth(int, struct patch_object *, struct date, FILE *, struct CommandLineObject *);
	static int outflag=0;

	if(current_date.year == outdate.first_year && current_date.month ==outdate.first_month && current_date.day == outdate.first_day)
		outflag=1;
	//printf("outflag is %d \n",outflag);

	if(outflag ==1 && ComLin->b !=NULL){
		output_basin_daily_hydrology(num_patches,patch,current_date,DM_outfiles->fBasinDailyHydro,ComLin);
		output_basin_daily_growth(num_patches, patch, current_date, DM_outfiles->fBasinDailyPlant, ComLin);
	}

    if(current_date.year == outdate.last_year && current_date.month == outdate.last_month && current_date.day == outdate.last_day)
		outflag=0;

	return;
}