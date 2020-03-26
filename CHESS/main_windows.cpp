//=======================================================================================================================
//  A brief introduction to the "R-RHESSys" model, which is short for rasterized RHESSys model::
//
//  The "R-RHESSys" is developed based on the Regional Hydroecological Simulation Systems (RHESSys, Tague and Band, 2004)
//  but excluded basin, hillslope and zone related hierarchical framework/structure in its predecessor. It also excluded
//  the TOP-MODEL embedded in its predecessor. It is a strictly cell-by-cell-based model and uses spatially explicit
//  routing algorithm to simulate spatial connectivity and movement of water and dissolved nutrients among simulated cells.
//  Since vegetation growth is dynamically simulated, a spin-up simulation control for vegetation growth and associated
//  carbon dynamics was added. This enables users to model vegetation and cycling of C and N in terrestrial ecosystems.
//  In addition, land surface water balance in each cell will be checked before routing water and solutes. The model codes
//  were checked/debugged and greatly simplified compared to the original RHESSys model. In author's personal view, it is
//  easier to handle than its predecessor while most subroutines are kept identical as their counterparts in its
//  predecessor.
//
//  Running R-RHESSys, a potential alternative to the RHESSys, requires the following data:
//
//  First, DEM data and DEM-derived slope, aspect, streams, roads raster data in ArcInfor ASCII format;
//  Second, meteorological data: daily minimum and maximum temperature (in Celsius degree), and precipitation (in meter);
//  Third, annual time-series atmospheric CO2 concentration (in ppm);
//  Fourth, rasterized land-cover, land use or vegetation data to predefine land cover, land use or vegetation types;
//  Fifth, rasterized soil texture class data to predefine soil types for the study region;
//  Finally, a flow table file describes the spatial connectivity of water and nutrients movement among simulated patches.

//  Besides, four default files of patch, soil, vegetation and land-use are provided. These files include parameters
//  relevant to soil, vegetation and land-use that are required to parameterize before running R-RHESSys.

//  As its predecessor, R-RHESSys is able to simulate::
//
//  (i) Land surface hydrology, including river stream flow, base flow, ET, plant transpiration, soil moisture, and etc.
//  (ii) Ecosystem dynamics: vegetation growth and productivity, cycling of carbon and nitrogen (e.g., soil autotrophic
//       and heterotrophic respiration) in terrestrial ecosystems, and etc.
//  (iii) Biogeochemical dynamics:: soil nitrification and denitrification, river nitrate, dissolved DOC and DON,
//       soil mineralization, and etc.
//
//  Note: For simulating land surface hydrology, a least of 20-year spin-up simulations are required for leaf area index
//  to reach stable state. For simulating soil and vegetation carbon, spin-up simulations more than 150 years are required.
//
//  Users are welcome to employ R-RHESSys to regions of their own interests. Examples of model forcing data can be
//  provided for reference or learning. For other questions, please contact the author::

//  Dr. Guoping Tang
//  Division of Earth and Ecosystem Sciences
//  Desert Research Institute
//  Reno, NV 89512
//  Email: gtang@dri.edu or tangg2010@gmail.com
//  Phone: 775-673-7938 (o)

//  Date created: 08-31-2011
//=======================================================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <direct.h>
#include <omp.h>
#include "chess.h"
#include "functions.h"
#include "constants.h"

using namespace std;

//=======================================================================================================================
// THE MAIN INTERFACE TO CONTROL MODEL SIMULATION
//================================================s=======================================================================
int	main(int main_argc, char **main_argv)
{

	// Variables
	// tunable
	struct  CommandLineObject* ComLin = new struct CommandLineObject;
	struct  SimulationInformation* SimInf = new struct SimulationInformation;
	struct  SimulationDateRange * SimDate = new struct SimulationDateRange;
	struct  OutputDateRange *OutDate = new struct  OutputDateRange;
	struct  SpinInformation* SpinInf = new struct SpinInformation;
	
	/*//xf_ws
	*SimInf = { "xf_ws",//basin name, the prefix in file systems
			1,//threads in parallel simulations
			1,//involved climate gauge
			1 //number of output gauge
	};
	*SimDate = { 1960, //start_year
			1960, //end_year
			1, //start_month
			1, //end_month
			1, //start_day
			31, //end_day
			1,  //start_hour
			24 //end_hour
	};
	//Output Date Range
	*OutDate = { 1960, 1960, 1, 1, 1, 31, 1, 24 };
	//Spin Up Informations
	*SpinInf = { 0,//spin_years: years required for vegetation and soil carbon to reach the stable state with long-term balance
				10,//spin_interval: the period of input climate data used for spin-up simulations
				true//spin_flag
	};*/





	//lxh_sk
	
	//Simulation Information (Basic)
	*SimInf = { "lxh_sk",//basin name, the prefix in file systems
				24,//threads in parallel simulations
				1,//involved climate gauge
				1 //number of output gauge
	};
	//Simulation Date Range
	*SimDate = {1985, //start_year
				2018, //end_year
				1, //start_month
				12, //end_month
				1, //start_day
				31, //end_day
				1,  //start_hour
				24 //end_hour
	};
	//Output Date Range
	*OutDate = { 1985, 2018, 1, 12, 1, 31, 1, 24 };
	//Spin Up Informations
	*SpinInf = {40,//spin_years: years required for vegetation and soil carbon to reach the stable state with long-term balance
				10,//spin_interval: the period of input climate data used for spin-up simulations
				true//spin_flag
	};
	
	
	// default
	struct  date current_date {};
	struct  input_Clim_Files   inClimFiles {};
	struct  InputGridData *GridData = new struct InputGridData;
	struct  InFilePath *InFilePath = new struct  InFilePath;
	struct  OutArray_object *OutArray = new struct OutArray_object;
	struct  output_hydro_plant* DM_outfiles = new struct output_hydro_plant;
	struct  patch_object *patch = nullptr;//allocate momeries in Fuc. construct_routing_topography
	struct  daily_clim_object* daily_clim = new struct daily_clim_object[SimInf->climate_num]{};
	//others
	int     out_flag = 0,endyear = 0, spin_yrs = 0, firstmonth, lastmonth, firstday, lastday;//local parameters
	double  time1=0, time2=0;//record time

	//=======================================================================================================================
	//xu. BUILD AND INITIAL THE ENDVIRONMENT FOR SIMULATION
	//=======================================================================================================================
	printf("Starting initialization:: \n"); time1 = omp_get_wtime();

	//construct and assign command line arguments
	construct_command_line(main_argc, main_argv, ComLin);

	//construct input file paths
	construct_infile_path(SimInf,InFilePath);

	//xu. I sugguest should 1\ flow table then  2\construct patch and read images
	patch=construct_routing_topology(patch,SimInf,InFilePath);

	//reading GRASS- or ArcInfo-based input images such as DEM,slope,aspect....stream,roads, gauge_lists
	read_geo_images(patch, ComLin, SimInf, InFilePath, GridData);

	//Initialize the default values of patch fields/members
	construct_patch(patch, ComLin, SimInf, InFilePath, GridData);

	//open input climate files (daily precipitation, minimum temperature and maximum temperature)
	inClimFiles = open_Clim_Files(SimInf,InFilePath,daily_clim);

	//distribute parallel threads of each
	init_parallel_environment(patch, SimInf);


	//time spent in initialization
	time2 = omp_get_wtime(); printf("Finishing initialization:: %lf seconds \n",time2-time1);

	//=======================================================================================================================
	//xu. SPIN UP and CHESS SIMULATION
	//=======================================================================================================================
	printf("\nStarting simulation:: \n"); 
	do {

		if (spin_yrs < SpinInf->spin_years) { //&& ComLin->grow_flag>0
			endyear = SimDate->start_year + SpinInf->spin_interval - 1;
			SpinInf->spin_flag = true;

			//SpinUp needs routing_flag now for channel flow
			ComLin->routing_flag = 1;
		}
		else {
			endyear = SimDate->end_year;
			SpinInf->spin_flag = false;
			ComLin->routing_flag = 1;
		}

		//contruct daily output files
		if (!SpinInf->spin_flag && out_flag == 0) {

			//basin-level
			if (!SpinInf->spin_flag && ComLin->b != NULL) {
				construct_basin_output_files(ComLin,InFilePath, DM_outfiles);
			}
			//xu. gauge-level
			if (!SpinInf->spin_flag && ComLin->gg != NULL) {
				construct_gauge_output_files(patch, ComLin, SimInf, InFilePath, DM_outfiles);
			}
			out_flag = 1;
		}

		//=======================================================================================================================
		//CHESS SIMULATION by YEAR, MON and DAY
		//=======================================================================================================================
		for (current_date.year = SimDate->start_year; current_date.year <= endyear; current_date.year++) {

			//record system time when annual simulation starts
			time1= omp_get_wtime();

			if (current_date.year == SimDate->start_year)
				firstmonth = SimDate->start_month;
			else
				firstmonth = 1;

			if (current_date.year == SimDate->end_year)
				lastmonth = SimDate->end_month;
			else
				lastmonth = 12;

			init_phenology_object(patch, SimInf);

			for (current_date.month = firstmonth; current_date.month <= lastmonth; current_date.month++) {

				//construct patch-level daily output files for a month
				if (!SpinInf->spin_flag && ComLin->p != NULL) {
					construct_patch_output_files(ComLin,SimInf,InFilePath,OutDate, DM_outfiles, OutArray, current_date);
				}

				if (current_date.year == SimDate->start_year && current_date.month == SimDate->start_month)
					firstday = SimDate->start_day;
				else
					firstday = 1;

				if (current_date.year == SimDate->end_year && current_date.month == SimDate->end_month)
					lastday = SimDate->end_day;
				else
					lastday = end_day_norm_leap(current_date.year, current_date.month - 1);

				//determine if the year is leap year and thus the end day of February can be different
				//end_day=end_day_norm_leap(current_date.year,=current_date.month-1);

			
				for (current_date.day = firstday; current_date.day <= lastday; current_date.day++) {

					//=======================================================================================================================
					//xu. DAILY SIMULATION (THREE KEY FUNCTIONS)
					//=======================================================================================================================
					
					//CLIMATE.. Read input climate data Precipitation, Tmin, Tmax and annual CO2
					chess_climate_daily(patch,ComLin,SimInf, current_date, inClimFiles, daily_clim);

					//PATCH.. Initial and run parallel CHESS daily Ecohydrological process and Transport of water and nutrients
					chess_patch_daily(patch, ComLin, SimInf,current_date, daily_clim);//daily_clim is a pointer now, the changes through out chess land daily

					//CHANNEL.. Channel flow routing process the route out the water
					chess_channel_daily(patch, ComLin, SimInf, current_date, daily_clim);

					//END OF DAILY SIMULATION

					//DAILY OUTPUT daily-step variables
					if (!SpinInf->spin_flag) {
						if (ComLin->b != NULL)
							out_basin_level_daily( patch, ComLin,SimInf,current_date, OutDate, DM_outfiles);
						if (ComLin->p != NULL)
							out_patch_level_daily(patch, ComLin, SimInf, current_date, OutDate, DM_outfiles, OutArray);
						if (ComLin->gg != NULL)
							out_gauge_level_daily(patch, ComLin, SimInf, current_date, OutDate, DM_outfiles);
					}

				}//END OF A MONTH

				//close patch_level_output_files
				if (!SpinInf->spin_flag && ComLin->p == true)
					close_patch_output_files(DM_outfiles, OutArray);

			} // END OF A YEAR


			time2 = omp_get_wtime();

			if (!SpinInf->spin_flag)
				printf("year, month, day and execution time:: %d %d %d %lf seconds\n", current_date.year, current_date.month - 1, current_date.day - 1,time2-time1);

			spin_yrs += 1;
			if (spin_yrs == SpinInf->spin_years && SpinInf->spin_flag) break;
		}//END OF A TIME SERIES (SPIN INTERVAL OR SIMULATION PERIOD)



		if (SpinInf->spin_flag) {
			rewind_daily_clim(inClimFiles);
			//printf("Spin-up to %d years takes %f minutes \n", spin_yrs, ((float)(clock() - t) / CLOCKS_PER_SEC / 60.));
		}

	} while (SpinInf->spin_flag);

	//CLOSE basin output files
	close_all_output_files(ComLin, SimInf, DM_outfiles);

	//free memories
	delete SimInf;delete SimDate;delete OutDate;delete SpinInf;delete GridData;
	delete[] patch;
	//delete daily_clim;
	//int a;
	//=======================================================================================================================
	//END OF SIMULATION
	//=======================================================================================================================

	return (0);
} //END OF CHESS