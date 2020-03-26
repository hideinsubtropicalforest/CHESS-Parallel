#include <stdio.h>
#include <iostream>
#include "functions.h"
#include "chess.h"
#include "constants.h"

bool check_water_balance(struct patch_object *patch) {

	double water_balance = patch->rain + patch->snow
		+ patch->preday_rain_stored - patch->rain_stored                     // rain stored by canopy
		+ patch->preday_snow_stored - patch->snow_stored                     // snow stored by canopy
		+ patch->litter.preday_rain_stored - patch->litter.rain_stored              // rain stored by litter
		+ patch->preday_snowpack - patch->snowpack.water_equivalent_depth // snowpack on the ground
		+ patch->detention_store_preday - patch->detention_store                 // water detened on soil surface
		+ patch->rz_storage_preday - patch->rz_storage                      // water stored in root zone layer
		+ patch->unsat_storage_preday - patch->unsat_storage                   // water stored in unsat zone layer
		- patch->sat_deficit_preday + patch->sat_deficit                     // water needed to saturate the whole soil
		- patch->AET                                                                // water evapotranspirated
		- patch->gw_drainage                                                        // groundwater drainage
		- patch->subsurface_Qout - patch->surface_Qout
		+ patch->subsurface_Qin + patch->surface_Qin; 

	if ((water_balance > 0.000000001) || (water_balance < -0.000001))
		return 0;
	else return 1;

}