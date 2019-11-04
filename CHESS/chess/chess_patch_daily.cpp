//---------------------------------------------------------------------------------------------------------------------------
//xu. LAND.. PARALLEL COMPUTATION.. Initial and run parallel CHESS daily Ecohydrological process and Transport of water and nutrients
//---------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>

#include <math.h>
#include "chess.h"
#include "functions.h"
#include "constants.h"
#include <iostream>
#include <thread>
#include <omp.h>

//using std::thread;

void  chess_patch_daily(struct patch_object *patch,
	struct 	command_line_object *command_line,
	struct	date 			current_date,
	struct  daily_clim_object *daily_clim,
	struct  parallel_object *parallel
)
{
	//---------------------------------------------------------------------------------------------------------------------------
	//xu. The patch_daily process are processed in separated sub-basins by ASP(Automatic Sub-basin Partition)
	//---------------------------------------------------------------------------------------------------------------------------
	/*
	//creat thread
	std::thread *thd=new std::thread [parallel->thread_num];
	//init and run all thread
	for (int thread_inx = 0; thread_inx != parallel->thread_num; thread_inx++) {

		//parallel patch daily is served to be a elementary function of patch daily process
		thd[thread_inx] = thread(parallel_patch_daily, patch, command_line, current_date, daily_clim,parallel, thread_inx);
	}
	//wait till all threads terminate
	for (int thread_inx = 0; thread_inx != parallel->thread_num; thread_inx++) {
		thd[thread_inx].join();
	}
	delete[] thd;
	*/
	///*
	//init and run all thread
	int thread_num = parallel->thread_num;
	//#pragma omp parallel for num_threads(thread_num)
	for (int thread_inx = 0; thread_inx < thread_num; thread_inx++) {

		//parallel patch daily is served to be a elementary function of patch daily process

		parallel_patch_daily(patch, command_line, current_date, daily_clim, parallel, thread_inx);
		//printf("%d\n", omp_get_num_threads());
	}//*/

	
	return;
}//__END OF DAILY PATCH