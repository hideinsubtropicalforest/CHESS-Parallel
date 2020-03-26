//---------------------------------------------------------------------------------------------------------------------
//					compute_subsurface_routing			
//											
//	Descripiton: route surface and subsurface flow and dissolved nutrients
//---------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <iostream>
#include "functions.h"
#include "chess.h"
#include "constants.h"



void  patch_lateral_flow(patch_object *patch,struct CommandLineObject *ComLin,
						 struct	date	current_date)
{
	//-----------------------------------------------------------------------------------------------------------------
	//	Local variable definition.				
	//-----------------------------------------------------------------------------------------------------------------
	double time_int=24;
	double water_balance=0;
	bool balance_index;
	//-----------------------------------------------------------------------------------------------------------------
	//	1. for roads, saturated throughflow beneath road cut is routed to downslope patches; saturated     
	//     throughflow above the cut and overland flow is routed to the stream                             
	//  2. for streams, no routing - all exported from basin	                                           
	//	3. regular land patches - route to downslope neighbours                                            
	//-----------------------------------------------------------------------------------------------------------------
	if (patch->drainage_type == STREAM){
		update_drainage_stream(patch, ComLin->routing_flag, time_int, ComLin->verbose_flag,
			ComLin->grow_flag);
	}
	else{
		update_drainage_land(patch, ComLin->routing_flag, time_int, ComLin->verbose_flag,
			ComLin->grow_flag);
	}

	//Update sat_deficit_z when soil saturation deficit (soil moisture)is changed
	patch->sat_deficit_z = compute_z_final(
		ComLin->verbose_flag,
		patch->soil_defaults->porosity_0,
		patch->soil_defaults->porosity_decay,
		patch->soil_defaults->soil_depth,
		0.0,
		-1.0 * patch->sat_deficit);


	// water balance
	balance_index = check_water_balance(patch);
	if (balance_index == false) {
		
		//expose all values in debug mode
		water_balance = patch->rain + patch->snow
			+ patch->preday_rain_stored - patch->rain_stored                     // rain stored by canopy
			+ patch->preday_snow_stored - patch->snow_stored                     // snow stored by canopy
			+ patch->litter.preday_rain_stored - patch->litter.rain_stored              // rain stored by litter
			+ patch->preday_snowpack - patch->snowpack.water_equivalent_depth // snowpack on the ground
			+ patch->detention_store_preday - patch->detention_store                 // water detened on soil surface
			+ patch->rz_storage_preday - patch->rz_storage                      // water stored in root zone layer
			+ patch->unsat_storage_preday - patch->unsat_storage                   // water stored in unsat zone layer
			- patch->sat_deficit_preday + patch->sat_deficit                     // water needed to saturate the whole soil
			- patch->AET                                                                // water evapotranspirated
			- patch->gw_drainage                                                     // groundwater drainage
			- patch->subsurface_Qout - patch->surface_Qout
			+ patch->subsurface_Qin + patch->surface_Qin;

		printf("time and id is %d %d %d %d %d \n", current_date.year, current_date.month, current_date.day, patch->ID, patch->drainage_type);
	}
		

return;
}
