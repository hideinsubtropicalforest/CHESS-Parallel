#include <stdio.h>

#include <stdlib.h>
#include "chess.h"
#include "functions.h"
#include <iostream>

using namespace std;
void out_gauge_level_daily(
	struct patch_object* patch,
	struct CommandLineObject* ComLin,
	struct  SimulationInformation* SimInf,
	struct date current_date,
	struct OutputDateRange* OutDate,
	struct output_hydro_plant* DM_outfiles
)
{
	int num_patches = SimInf->patch_num;
	struct OutputDateRange outdate = *OutDate;
	int* gauge_list = SimInf->gauge_list;
	double cellsize = SimInf->cell_size;

	int i;

	static int out_flag = 0;

	if (current_date.year == outdate.first_year && current_date.month == outdate.first_month && current_date.day == outdate.first_day)
		out_flag = 1;


	if (out_flag == 1 && ComLin->gg != NULL) {

		for (int gauge_inx = 0; gauge_inx != GAUGE_NUM; gauge_inx++) {
			output_gauge_daily_hydrology(&patch[(gauge_list[gauge_inx])], current_date, DM_outfiles->fGaugeDailyHydro[gauge_inx], ComLin,cellsize);
			output_gauge_daily_growth(&patch[(gauge_list[gauge_inx])], current_date, DM_outfiles->fGaugeDailyPlant[gauge_inx], ComLin, cellsize);
		}
	}

	if (current_date.year == outdate.last_year && current_date.month == outdate.last_month && current_date.day == outdate.last_day)
		out_flag = 0;

	return;
}