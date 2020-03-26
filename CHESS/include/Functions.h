#include "constants.h"
//---------------------------------------------------------------------------------------------------------------------------
// PART I: INITIALIZATION	                                                
//---------------------------------------------------------------------------------------------------------------------------

struct CommandLineObject* construct_command_line(int main_argc, char** main_argv, CommandLineObject* ComLin);
void construct_infile_path(struct  SimulationInformation* SimInf, struct InFilePath* InputFilePath);
struct patch_object* construct_routing_topology(struct patch_object* patch,struct  SimulationInformation* SimInf, struct InFilePath* InFilePath);
void	read_geo_images(struct patch_object* patch, struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf,
	struct InFilePath* InFilePath, struct  InputGridData* InputGridData);
void construct_patch(struct patch_object* patch, struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf,
	struct InFilePath* InputFilePath, struct InputGridData* InputGridData);
struct	input_Clim_Files open_Clim_Files(struct  SimulationInformation* SimInf, struct InFilePath* InFilePath, struct daily_clim_object* daily_clim);
void	init_parallel_environment(struct patch_object* patch, struct  SimulationInformation* SimInf);


//---------------------------------------------------------------------------------------------------------------------------
// PART II: SIMULATION                                                
//---------------------------------------------------------------------------------------------------------------------------
//main
void chess_climate_daily(struct patch_object* patch, struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf,
	struct	date	current_date, struct input_Clim_Files inClimFiles, struct  daily_clim_object* daily_clim);
void  chess_patch_daily(struct patch_object* patch, struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf,
	struct	date	current_date, struct  daily_clim_object* daily_clim);
void  chess_channel_daily(struct patch_object* patch, struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf,
	struct	date	current_date, struct  daily_clim_object* daily_clim);

//sub
void		parallel_hillslope_subunit(struct patch_object* patch, struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf,
	struct	date current_date, struct  daily_clim_object* daily_clim, int	thread_inx);
void  parallel_route_subunit(patch_object* patch, struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf, struct	date current_date,
	struct  daily_clim_object* daily_clim, int layer_inx, int thread_inx);


//---------------------------------------------------------------------------------------------------------------------------
// PART III: OUTPUT 	                                                
//---------------------------------------------------------------------------------------------------------------------------
//construct file title
void  construct_basin_output_files(struct CommandLineObject* ComLin, struct InFilePath* InFilePath, struct output_hydro_plant* DM_outfiles);
void  construct_gauge_output_files(struct patch_object* patch, struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf,
	struct InFilePath* InFilePath, struct output_hydro_plant* DM_outfiles);
void  construct_patch_output_files(struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf, struct InFilePath* InFilePath,
	struct OutputDateRange* OutDate, struct output_hydro_plant* DM_outfiles, struct OutArray_object* OutArray, struct date current_date);

//write files
void out_basin_level_daily(	struct patch_object* patch,	struct CommandLineObject* ComLin,	struct  SimulationInformation* SimInf,
	struct date current_date,	struct OutputDateRange* OutDate,	struct output_hydro_plant* DM_outfiles);
void out_patch_level_daily(	struct patch_object* patch,	struct CommandLineObject* ComLin,	struct  SimulationInformation* SimInf,	
	struct date current_date,struct OutputDateRange* OutDate,	struct output_hydro_plant* DM_outfiles,	struct OutArray_object* OutArray);
void out_gauge_level_daily(struct patch_object* patch,struct CommandLineObject* ComLin,struct  SimulationInformation* SimInf,
	struct date current_date,struct OutputDateRange* OutDate,struct output_hydro_plant* DM_outfiles);

//close files
void    close_patch_output_files(struct output_hydro_plant *, struct OutArray_object *OutArray);
void    close_all_output_files(struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf,
	struct output_hydro_plant* DM_outfiles);



//---------------------------------------------------------------------------------------------------------------------------
// PART IV: OTHERS                                                
//---------------------------------------------------------------------------------------------------------------------------
int     end_day_norm_leap(int, int);
void    init_out_monthly(struct patch_object *, struct accumulate_patch_object *, int, int, struct CommandLineObject *);



//---------------------------------------------------------------------------------------------------------------------------
// Patch daily .function	                                                
//---------------------------------------------------------------------------------------------------------------------------
void	zero_patch_object(struct patch_object *patch);
void    patch_climate_initial(struct patch_object *, struct daily_clim_object, struct CommandLineObject *, struct	date);
void    patch_land_initial(struct patch_object *,  struct CommandLineObject *, struct date);


void    patch_radiation_daily(struct patch_object *, struct CommandLineObject *, struct	date);
void	patch_daily_final(struct patch_object *, struct CommandLineObject *, struct date);

//patch lateral flow
void    patch_lateral_flow(patch_object *, struct CommandLineObject *,  struct date);
void	update_drainage_stream(struct patch_object *, int, double, int, int);
void	update_drainage_land(struct patch_object *, int, double, int, int);



void    out_daily(int, struct patch_object *, struct date, struct OutputDateRange, struct output_hydro_plant *, struct CommandLineObject *);
void    output_patch_daily_hydrology(struct patch_object *patch, int pch,double(*HydroMon)[HYDRO_NUM],struct	date current_date,FILE *outfile,
		struct CommandLineObject *ComLin);
void	output_patch_daily_growth(struct patch_object *patch, int pch,double(*PlantMon)[PLANT_NUM],struct	date current_date,FILE *outfile,
		struct CommandLineObject *ComLin);
//xu.
void	output_gauge_daily_hydrology(struct	patch_object *patch, struct	date	current_date, FILE *outfile, struct CommandLineObject *ComLin,int cellsize);
void	output_gauge_daily_growth(struct patch_object *patch, struct	date current_date, FILE *outfile, struct CommandLineObject *ComLin, int cellsize);

void    *alloc(size_t, char *, char *);
void    rewind_daily_clim(struct input_Clim_Files);
void    read_daily_climate(struct input_Clim_Files, struct InputDateRange, double[][12][31], double[][12][31], double[][12][31], double[]);


int read_record(FILE *, char *);
void init_phenology_object(patch_object* patch, struct  SimulationInformation* SimInf);





//LOCAL FUNCTION IN PATCH DAILY I

void   canopy_stratum_daily_I(struct	patch_object *,struct canopy_strata_object *,struct CommandLineObject *,struct date);

double	compute_layer_field_capacity(int,int,double,double,double,double,double,double,double,double);

double	compute_delta_water(int,double,double,double,double,double);

double	compute_z_final(int,double,double,double,double,double);

//---------------------------------------------------------------------------------------------------------------------------
//  HYDRO .function	                                                
//---------------------------------------------------------------------------------------------------------------------------

double	compute_capillary_rise(int, double, double, double, double, double);

double  compute_potential_exfiltration(int,double,double,double,double,double,double,double,double);

double  compute_soil_water_potential(int,int,double,double,double,double,double,double,double,double);

int compute_potential_decomp(double,double,double,double,double,struct  soil_c_object   *,struct  soil_n_object   *,struct  litter_c_object *,
	struct  litter_n_object *,struct  cdayflux_patch_struct *,struct  ndayflux_patch_struct *);

void sort_patch_layers(struct patch_object *);

void update_litter_interception_capacity(double,struct  litter_c_object *,struct  litter_object *);


//LOCAL FUNCTION IN PATCH DAILY F
void zone_daily_final(struct patch_object *, struct CommandLineObject *, struct date);
void compute_Lstar(struct patch_object *);
double compute_delta_water(int, double, double, double, double, double);

double compute_layer_field_capacity(int,int,double,double,double,double,double,double,double,double);

void canopy_stratum_daily_F(struct patch_object *,struct layer_object *,struct canopy_strata_object *,struct CommandLineObject *,
	struct date);

void   surface_daily_F(struct patch_object *,struct CommandLineObject *,struct date);

double	snowpack_daily_F(struct date,int,struct	snowpack_object	*,double,double,double,double,double,double,double,double *,double *,
	double *,double *,double,double,double,double,double);


double	compute_infiltration(int,double,double,double,double,double,double,double,
	double,double,double);

double  compute_surface_heat_flux(int,double,double,double,double,double,double,double,double,double);

double	compute_unsat_zone_drainage(int,int,double,double,double,double,double,double);

double  compute_radiative_fluxes(int,double *,double,double,double);

//double  compute_stability_correction(int ,double,double,double,double,double,double);
int 	compute_year_day(struct date);

void 	check_zero_stores(struct  soil_c_object   *,struct  soil_n_object   *,struct  litter_c_object *,struct  litter_n_object *);

void	update_decomp(struct	date,struct  soil_c_object   *,struct  soil_n_object   *,struct  litter_c_object *,struct  litter_n_object *,
	struct cdayflux_patch_struct *,struct ndayflux_patch_struct *,struct patch_object *);

void	update_dissolved_organic_losses(struct	date,double,struct  soil_c_object   *,struct  soil_n_object   *,struct  litter_c_object *,
	struct  litter_n_object *,struct cdayflux_patch_struct *,struct ndayflux_patch_struct *);

void	update_septic(struct	date,struct  patch_object   *);

void	update_nitrif(struct  soil_c_object   *,struct  soil_n_object   *,struct cdayflux_patch_struct *,struct ndayflux_patch_struct *,
	struct soil_class,double,double,double,double,double,double);

void update_denitrif(struct  soil_c_object   *,struct  soil_n_object   *,struct cdayflux_patch_struct *,struct ndayflux_patch_struct *,
	struct soil_class,double,double);

int	resolve_sminn_competition(struct  soil_n_object   *,double, double,struct ndayflux_patch_struct *);

void canopy_stratum_growth(struct patch_object *,struct canopy_strata_object *,struct CommandLineObject *,struct date);

void update_gw_drainage( struct patch_object *,struct CommandLineObject *,struct date);

long julday(struct date);

void compute_ground_water_loss(struct	patch_object *, struct 	CommandLineObject *, struct 	date);



//---------------------------------------------------------------------------------------------------------------------------
// Update_Drainage_Stream.cpp		                                                
//---------------------------------------------------------------------------------------------------------------------------
double  compute_delta_water(int, double, double, double, double, double);
double  compute_N_leached(int, double, double, double, double, double, double, double,
						double, double, double, double, double);
double  compute_transmissivity_curve(double, double, double, double);
double  compute_varbased_returnflow(double, double, double, struct litter_object *);
double  compute_varbased_flow(double, double, double, double, double, double);
double  recompute_gamma(struct patch_object *, double);
double  compute_infiltration(int, double, double, double, double, double, double, double, 
						double, double, double);

//---------------------------------------------------------------------------------------------------------------------------
// CN .function	                                                
//---------------------------------------------------------------------------------------------------------------------------




void construct_resvoir_correponding_data(char *filename, int patchID, struct patch_object *patch);
long 	yearday(struct date current_date);


//---------------------------------------------------------------------------------------------------------------------------
// math .function	                                                
//---------------------------------------------------------------------------------------------------------------------------
double max(double, double);
double min(double, double);



bool check_water_balance(struct patch_object* patch);
void  parallel_channel_subunit(patch_object* patch, struct CommandLineObject* ComLin, struct  SimulationInformation* SimInf,
	struct	date current_date, struct  daily_clim_object* daily_clim, int layer_inx, int thread_inx);