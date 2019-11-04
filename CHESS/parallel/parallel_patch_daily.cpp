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


void		parallel_patch_daily(struct 	patch_object *patch,
	struct 	command_line_object *command_line,
	struct	date 			current_date,
	struct  daily_clim_object *daily_clim,
	struct  parallel_object *parallel,
	int		thread_inx
)
{

	for (int patch_inx = 0; patch_inx != parallel->land_thread_patch_num[thread_inx]; patch_inx++) {
	
		
		int pch = parallel->land_pch[thread_inx][patch_inx];
		int climate_inx = patch[pch].climatetype - 1;//since it start from 1
		struct patch_object *patch_current= &patch[pch];

		//=======================================================================================================================
		// 1... INITIALIZATION BEFORE ECO-HYDROLOGICAL SIMULATION
		//=======================================================================================================================
		
		if (patch[pch].drainage_type != STREAM){
		// 1.1 Initializing ecohydrological Fluxes as 0 
		zero_patch_object(patch_current);

		// 1.2 Initializing Climate Forcing
		patch_climate_initial(patch_current, daily_clim[climate_inx], command_line, current_date);

		// 1.3 Initializing Land (Hydro-Eco) Pool State
		patch_land_initial(patch_current, command_line, current_date);

		//=======================================================================================================================
		// 2... ECO-HYDROLOGICAL SIMULATION WITHIN A PATCH
		//=======================================================================================================================
		patch_daily_final(patch_current, command_line, current_date);

		//=======================================================================================================================
		// 3... LATERAL FLOW ROUTING AND NUTRIENT TRANSPORT to neighbor patches
		//=======================================================================================================================
		patch_lateral_flow(patch_current, command_line,  current_date);
		
		}
	}
	
	return;
	//END OF THIS THREAD
}