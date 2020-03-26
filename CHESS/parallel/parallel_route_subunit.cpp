//---------------------------------------------------------------------------------------------------------------------------
//xu. CHANNEL.. Channel flow routing process the route out the water
//A similar process as in Wigmosta et al.(2002)
//A static or dynamic "k" are used
//---------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>

#include <iostream>
#include <math.h>
#include "chess.h"
#include "constants.h"
#include "functions.h"

using std::cout;
using std::endl;

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

void  parallel_route_subunit(patch_object* patch, struct CommandLineObject* ComLin,
	struct  SimulationInformation* SimInf,
	struct	date current_date,
	struct  daily_clim_object* daily_clim,
	int layer_inx,
	int thread_inx
)
{

	for (int patch_inx = 0; patch_inx != SimInf->channel_layer_thread_patch_num[layer_inx][thread_inx]; patch_inx++) {


		// local vars
		struct patch_object* neigh;
		int i, j, k = 0, kk = 0;
		static double route_to_streamflow = 0, route_to_baseflow = 0;
		double sumstreamflow = 0, sumbaseflow = 0;
		static double volume = 0;
		static bool startDischarge = false;
		double Qout = 0., water = 0.;
		double surface_Qout = 0., return_flow = 0.;
		bool balance_index;
		double water_balance;

		//xu.
		double V1 = 0, V2 = 0, OUT_all = 0, INOUT_ratio = 0;
		struct patch_object* patch_current = &patch[(SimInf->channel_pch[layer_inx][thread_inx][patch_inx])];


		// 1. 
		// For river channel, soil should be mostly saturated. So, we assume one-tenth of
		// surface and subsurface flow are used to reduce soil saturation deficit in channel

		double partial_surface = patch_current->surface_Qout * 1. / 100.; //20
		double partial_subsurface = patch_current->subsurface_Qout * 1. / 100.;//20

		if (patch_current->sat_deficit > 0.) {
			double min_surface = min(partial_surface, patch_current->sat_deficit);
			patch_current->sat_deficit -= min_surface;
			patch_current->surface_Qout -= min_surface;
		}

		if (patch_current->sat_deficit > 0.) {
			double min_subsurface = min(partial_subsurface, patch_current->sat_deficit);
			patch_current->sat_deficit -= min_subsurface;
			patch_current->subsurface_Qout -= min_subsurface;
		}


		patch_current->sat_deficit_z = compute_z_final(
			ComLin->verbose_flag,
			patch_current->soil_defaults->porosity_0,
			patch_current->soil_defaults->porosity_decay,
			patch_current->soil_defaults->soil_depth,
			0.0,
			-1.0 * patch_current->sat_deficit);


		//===============================================================================================================================
		// 2. water body simulation	
		//===============================================================================================================================
		for (j = 0; j < patch_current->num_neighbours; j++) {
			
			neigh = patch_current->neighbours[j].patch;

			if (neigh->drainage_type == STREAM) { // note water routes through channel grids

				//---------------------------------------------------------------------------------------------------------------------------
				//2.1. For large basins, the channel routing and reservoirs
				//---------------------------------------------------------------------------------------------------------------------------
				if (ComLin->cf == TRUE) {

					//Q_IN
					patch_current->channel->Q_in = patch_current->surface_Qout + patch_current->subsurface_Qout + patch_current->gw.Qout;// m/day

					if (patch_current->channel->Q_in > 0.0) {
						patch_current->channel->surface_ratio = patch_current->surface_Qout / patch_current->channel->Q_in;
						patch_current->channel->subsurface_ratio = patch_current->subsurface_Qout / patch_current->channel->Q_in;
						patch_current->channel->gw_ratio = patch_current->gw.Qout / patch_current->channel->Q_in;
					}
					else patch_current->channel->surface_ratio = patch_current->channel->subsurface_ratio = patch_current->channel->gw_ratio = 1 / 3.0;

					//CHANGE UNIT
					patch_current->channel->Q_in *= pow(SimInf->cell_size, 2);//QOUT m^3/day


					//---------------------------------------------------------------------------------------------------------------------------
					//2.1.1. For Reservoirs (Target Release)
					//---------------------------------------------------------------------------------------------------------------------------
					if (ComLin->re == TRUE && patch_current->channel->ID == RESERVOIR && current_date.year >= patch_current->channel->reservoir->StartYear)
					{
						//STORAGE
						patch_current->channel->storage += patch_current->channel->Q_in;

						//i30 input
						if (patch_current->channel->reservoir->i30_days < 30) {
							patch_current->channel->reservoir->i30[patch_current->channel->reservoir->i30_days] = patch_current->channel->Q_in;
							patch_current->channel->reservoir->i30_days += 1;
						}
						else {
							patch_current->channel->reservoir->i30[29] = patch_current->channel->Q_in;//add to the last one 
						}

						//i30 mean
						patch_current->channel->reservoir->i30_all = 0;
						for (int inx = 0; inx != patch_current->channel->reservoir->i30_days; inx++) {
							patch_current->channel->reservoir->i30_all += patch_current->channel->reservoir->i30[inx];
						}
						patch_current->channel->reservoir->i30_mean = patch_current->channel->reservoir->i30_all / patch_current->channel->reservoir->i30_days;
						//i30 std
						patch_current->channel->reservoir->i30_std = 0;
						if (patch_current->channel->reservoir->i30_days == 1) {
							patch_current->channel->reservoir->i30_std = 2000000;// not sure how to set this value
						}
						else
							for (int inx = 0; inx != patch_current->channel->reservoir->i30_days; inx++) {
								patch_current->channel->reservoir->i30_std += pow(patch_current->channel->reservoir->i30[inx] - patch_current->channel->reservoir->i30_mean, 2);
							}
						patch_current->channel->reservoir->i30_std /= patch_current->channel->reservoir->i30_days;
						patch_current->channel->reservoir->i30_std = pow(patch_current->channel->reservoir->i30_std, 1 / 2.0);

						//move i30 to next
						if (patch_current->channel->reservoir->i30_days == 30)
							for (int inx = 0; inx != 29; inx++) {//0-28 , 29 in total days to be reserved
								patch_current->channel->reservoir->i30[inx] = patch_current->channel->reservoir->i30[inx + 1];
							}


						//modificaion of power¡¢supply¡¢impound
						//note that month start from 0
						patch_current->channel->reservoir->npow = (patch_current->channel->storage - patch_current->channel->reservoir->Vc[current_date.month - 1]) /
							max(patch_current->channel->reservoir->Vp[current_date.month - 1] - patch_current->channel->reservoir->Vc[current_date.month - 1],
								patch_current->channel->reservoir->Vc[current_date.month - 1] - patch_current->channel->reservoir->Vd);

						int year_day = yearday(current_date) - 1;//start from 0
						patch_current->channel->reservoir->nsup = (patch_current->channel->reservoir->longterm_i30[year_day] - patch_current->channel->reservoir->i30_mean) *
							(patch_current->channel->storage - patch_current->channel->reservoir->Vd) /
							(patch_current->channel->reservoir->Vp[current_date.month - 1] - patch_current->channel->reservoir->Vd) /
							patch_current->channel->reservoir->i30_std;

						patch_current->channel->reservoir->nimp = -(patch_current->channel->reservoir->Vp[current_date.month - 1] - patch_current->channel->storage) /
							(patch_current->channel->reservoir->Vp[current_date.month - 1] - patch_current->channel->reservoir->Vd);


						//final eqation
						int year_inx = current_date.year - patch_current->channel->reservoir->StartYear;
						patch_current->channel->Q_out = (1 +
							(patch_current->channel->reservoir->npow * patch_current->channel->reservoir->alpha[year_inx] +
								patch_current->channel->reservoir->nsup * patch_current->channel->reservoir->beta[year_inx] +
								patch_current->channel->reservoir->nimp * patch_current->channel->reservoir->gamma[year_inx]
								) * patch_current->channel->reservoir->kmon[current_date.month - 1]) *
							patch_current->channel->reservoir->longterm_qout[year_day];


						//mass balance and limitation
						if (patch_current->channel->storage < patch_current->channel->reservoir->Vd)
							patch_current->channel->Q_out = 0;
						//if(patch_current->channel->storage > patch_current->channel->reservoir->Ve)
							//patch_current->channel->Q_out = min(patch_current->channel->Q_out, (patch_current->channel->storage- patch_current->channel->reservoir->Ve)/5.0);

						patch_current->channel->Q_out = max(patch_current->channel->Q_out, 0);


						//RENEW STORAGE
						patch_current->channel->storage -= patch_current->channel->Q_out;

					}
					//---------------------------------------------------------------------------------------------------------------------------
					// 2.1.2 for original channel grids (not reservoir)
					// in this part we use a linear storage algorithm (based on Manning's equation)
					//---------------------------------------------------------------------------------------------------------------------------
					else {

						// initial the parameters for preparing parameters of k
						// length
						if (pow(patch_current->x - neigh->x, 2) < 0.01 || pow(patch_current->y - neigh->y, 2) < 0.01)
							patch_current->channel->channel_length = SimInf->cell_size;
						else patch_current->channel->channel_length = SimInf->cell_size * pow(2, 1 / 2.0);//a problem with sqrt 2 and 1;

						//slope
						patch_current->channel->channel_slope = (patch_current->z - neigh->z) / patch_current->channel->channel_length;
						patch_current->channel->channel_slope = max(patch_current->channel->channel_slope, 0.0001);

						//h (in current stage we assume that the channel_width are static)
						patch_current->channel->crosssectional_area = patch_current->channel->storage / patch_current->channel->channel_length;

						//(B*B-4AC)^1/2 -B/2A
						patch_current->channel->h = (pow(patch_current->channel->channel_width * patch_current->channel->channel_width + 4 * patch_current->channel->crosssectional_area * patch_current->channel->inverse_side_slope, 1 / 2.0)
							- patch_current->channel->channel_width)
							/ (2.0 * patch_current->channel->inverse_side_slope);

						patch_current->channel->wetted_parameter = patch_current->channel->channel_width + 2 * patch_current->channel->h * pow(1 + pow(patch_current->channel->inverse_side_slope, 2), 1.0 / 2);

						//Rr
						patch_current->channel->Rr = patch_current->channel->crosssectional_area / patch_current->channel->wetted_parameter;

						//k
						patch_current->channel->k = pow(patch_current->channel->Rr, 2.0 / 3) * pow(patch_current->channel->channel_slope, 1 / 2.0)
							/ (patch_current->channel->hydraulic_roughness * patch_current->channel->channel_length);

						//V1
						V1 = patch_current->channel->storage;

						//V2 a key function for computing channel storage change
						if (patch_current->channel->k > 0.0)
							V2 = patch_current->channel->Q_in / (seconds_per_day * patch_current->channel->k) +
							(V1 - patch_current->channel->Q_in / (patch_current->channel->k * seconds_per_day)) * exp(-patch_current->channel->k * seconds_per_day);
						else V2 = V1 + patch_current->channel->Q_in;


						//CHECK BALANCE
						if (V2 < 0.0) V2 = 0;
						else if (V2 > V1 + patch_current->channel->Q_in) V2 = V1 + patch_current->channel->Q_in;

						//Q_OUT
						patch_current->channel->Q_out = V1 + patch_current->channel->Q_in - V2;


						//RENEW STORAGE
						patch_current->channel->storage = V2;

					}


					// change units and pass values
					OUT_all = patch_current->channel->Q_out / pow(SimInf->cell_size, 2);// m/day
					patch_current->surface_Qout = OUT_all * patch_current->channel->surface_ratio;
					patch_current->subsurface_Qout = OUT_all * patch_current->channel->subsurface_ratio;
					patch_current->gw.Qout = OUT_all * patch_current->channel->gw_ratio;

					// C & N
					if (patch_current->channel->Q_in > 0.0)
						INOUT_ratio = patch_current->channel->Q_out / patch_current->channel->Q_in;
					else INOUT_ratio = 0;
					patch_current->DON_loss *= INOUT_ratio;
					patch_current->DOC_loss *= INOUT_ratio;
					patch_current->surface_leach_NH4_out *= INOUT_ratio;
					patch_current->subsurface_leach_NH4_out *= INOUT_ratio;
					patch_current->surface_leach_NO3_out *= INOUT_ratio;
					patch_current->subsurface_leach_NO3_out *= INOUT_ratio;


					// route waters to neighbors
					neigh->subsurface_Qout += patch_current->neighbours[j].gamma * patch_current->subsurface_Qout;
					neigh->gw.Qout += patch_current->neighbours[j].gamma * patch_current->gw.Qout;
					neigh->DON_loss += patch_current->neighbours[j].gamma * patch_current->DON_loss;
					neigh->DOC_loss += patch_current->neighbours[j].gamma * patch_current->DOC_loss;
					neigh->subsurface_leach_NH4_out += patch_current->neighbours[j].gamma * patch_current->subsurface_leach_NH4_out;
					neigh->surface_leach_NH4_out += patch_current->neighbours[j].gamma * patch_current->surface_leach_NH4_out;
					neigh->subsurface_leach_NO3_out += patch_current->neighbours[j].gamma * patch_current->subsurface_leach_NO3_out;
					neigh->surface_leach_NO3_out += patch_current->neighbours[j].gamma * patch_current->surface_leach_NO3_out;
				}// end of 2.1
				//---------------------------------------------------------------------------------------------------------------------------
				//2.2. for small basins, we route all water out 
				//---------------------------------------------------------------------------------------------------------------------------
				else {

					neigh->surface_Qout += patch_current->neighbours[j].gamma * patch_current->surface_Qout;
					neigh->subsurface_Qout += patch_current->neighbours[j].gamma * patch_current->subsurface_Qout;
					neigh->gw.Qout += patch_current->neighbours[j].gamma * patch_current->gw.Qout;
					neigh->DON_loss += patch_current->neighbours[j].gamma * patch_current->DON_loss;
					neigh->DOC_loss += patch_current->neighbours[j].gamma * patch_current->DOC_loss;
					neigh->subsurface_leach_NH4_out += patch_current->neighbours[j].gamma * patch_current->subsurface_leach_NH4_out;
					neigh->surface_leach_NH4_out += patch_current->neighbours[j].gamma * patch_current->surface_leach_NH4_out;
					neigh->subsurface_leach_NO3_out += patch_current->neighbours[j].gamma * patch_current->subsurface_leach_NO3_out;
					neigh->surface_leach_NO3_out += patch_current->neighbours[j].gamma * patch_current->surface_leach_NO3_out;

				}

			}
		}
		//xu.end of a stream patch


		/*
		balance_index = check_water_balance(patch_current);
		if (balance_index == false){

			water_balance = patch_current->rain + patch_current->snow
				+ patch_current->preday_rain_stored - patch_current->rain_stored                     // rain stored by canopy
				+ patch_current->preday_snow_stored - patch_current->snow_stored                     // snow stored by canopy
				+ patch_current->litter.preday_rain_stored - patch_current->litter.rain_stored              // rain stored by litter
				+ patch_current->preday_snowpack - patch_current->snowpack.water_equivalent_depth // snowpack on the ground
				+ patch_current->detention_store_preday - patch_current->detention_store                 // water detened on soil surface
				+ patch_current->rz_storage_preday - patch_current->rz_storage                      // water stored in root zone layer
				+ patch_current->unsat_storage_preday - patch_current->unsat_storage                   // water stored in unsat zone layer
				- patch_current->sat_deficit_preday + patch_current->sat_deficit                     // water needed to saturate the whole soil
				- patch_current->AET                                                                // water evapotranspirated
				- patch_current->gw_drainage                                                        // groundwater drainage
				- patch_current->subsurface_Qout - patch_current->surface_Qout
				+ patch_current->subsurface_Qin + patch_current->surface_Qin;

			printf("time and id is %d %d %d %d %d \n", current_date.year, current_date.month, current_date.day, patch_current->ID, patch_current->drainage_type);

		}*/


	}//end of all channels in this thread

	return;
}