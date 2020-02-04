#include <stdio.h>
#include "functions.h"
#include "chess.h"
#include "constants.h"

struct patch_object *construct_routing_topology(struct patch_object* patch,struct  SimulationInformation* SimInf,
	struct InFilePath *InFilePath)
{

	/*--------------------------------------------------------------*/
	/*	Local variable definition.									*/
	/*--------------------------------------------------------------*/
	int		i, j;
	int		num_neighbours;
	int		patch_ID;
	int		drainage_type;
	double	x, y, z, area, gamma, width;
	FILE	*routing_file;
	struct	patch_object	*stream;


	//=======================================================================================================================
	//xu. 1. IN THIS SECTION WE ARE UNKNOWN ABOUT ALL THE patche[i]'s i and their IDs.
	//=======================================================================================================================
	/*--------------------------------------------------------------*/
	/*  Try to open the routing file in read mode.                    */
	/*--------------------------------------------------------------*/
	if ((routing_file = fopen(InFilePath->FlowTableName, "r")) == NULL) {
		fprintf(stderr, "FATAL ERROR:  Cannot open routing file %s\n",
			InFilePath->FlowTableName);
		exit(0);
	} /*end if*/
	fscanf(routing_file, "%d", &SimInf->patch_num);

	//allocate memory
	patch = new struct patch_object[SimInf->patch_num]{};


	/*--------------------------------------------------------------*/
	/*	Read in  each patch record and find it		.				*/
	/*	if it is a stream add it to the basin level routing list	*/
	/*	otherwise add it to the hillslope level routing list		*/
	/*--------------------------------------------------------------*/
	for (i = 0; i< SimInf->patch_num; i++) {
		fscanf(routing_file, "%d %lf %lf %lf %lf %d %lf %d",
			&patch[i].ID,
			&patch[i].x, &patch[i].y, &patch[i].z,
			&patch[i].acc_area,
			&patch[i].drainage_type,
			&patch[i].gamma,
			&patch[i].num_neighbours);

		patch[i].num_innundation_depths = 1;
		patch[i].critical_depth = NULLVAL;

		//xu.we only check if runoff generation parameter gamma >0
		if (patch[i].gamma < ZERO) {
			patch[i].drainage_type = STREAM;
			printf("\nAN FATAL ERROR IN Patch %d  \n", patch[i].ID);
		}

		/*--------------------------------------------------------------*/
		/*  Allocate neighbour array									*/
		/*--------------------------------------------------------------*/
		patch[i].neighbours = new struct neighbour_object[(patch[i].num_neighbours)]{};

		//currently unknown about neigh's struct address
		//it would be allocated by .pch file that contains the address information
		//construct patch.cpp
		for (int neigh = 0; neigh != patch[i].num_neighbours; neigh++) {
			    fscanf(routing_file, "%d %d %lf",
				&patch[i].neighbours[neigh].patchorder,//elevation order
				&patch[i].neighbours[neigh].patchID,//ID
				&patch[i].neighbours[neigh].gamma);
				patch[i].neighbours[neigh].patch = &patch[patch[i].neighbours[neigh].patchorder];//address
		}

	}

	//printf("\nFinishing construct_routing_topology.cpp\n");
	return (patch);
}

