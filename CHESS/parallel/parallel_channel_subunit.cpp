#include <stdio.h>
#include <math.h>
#include "chess.h"
#include "functions.h"
#include "constants.h"
#include <iostream>

void  parallel_channel_subunit(patch_object* patch, struct CommandLineObject* ComLin,
	struct  SimulationInformation* SimInf,
	struct	date current_date,
	struct  daily_clim_object* daily_clim,
	int layer_inx,
	int thread_inx
)
{
	// moves from upstream to downstream grids
	// differences in channel_layer_thread_patch_num, which refers to the grid series
	for (int patch_inx = 0; patch_inx != SimInf->channel_layer_thread_patch_num[layer_inx][thread_inx]; patch_inx++) {

		// local vars
		struct patch_object* patch_current = &patch[(SimInf->channel_pch[layer_inx][thread_inx][patch_inx])];
		int climate_inx = patch_current->climatetype - 1;//since it start from 1

		// 1.1. initialization before eco-hydrological simulation
		zero_patch_object(patch_current);

		// 1.2 Initializing Climate Forcing
		patch_climate_initial(patch_current, daily_clim[climate_inx], ComLin, current_date);

		// 1.3 Initializing Land (Hydro-Eco) Pool State
		patch_land_initial(patch_current, ComLin, current_date);

		// 2... eco-hydrological simulation within a patch
		patch_daily_final(patch_current, ComLin, current_date);

		// 3... lateral flow routing and nutrient transport to neighbor patches
		patch_lateral_flow(patch_current, ComLin, current_date);
	}
}