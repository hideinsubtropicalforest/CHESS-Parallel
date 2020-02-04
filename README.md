# Coupled Hydrology Ecology Simulation System - Parallelized (CHESS)
## Brief introduction
The "CHESS" is developed based on the Regional Hydroecological Simulation Systems (RHESSys, Tague and Band, 2004)
but excluded basin, hillslope and zone related hierarchical framework/structure in its predecessor. It also excluded
the TOP-MODEL embedded in its predecessor. It is a strictly cell-by-cell-based model and uses spatially explicit
routing algorithm to simulate spatial connectivity and movement of water and dissolved nutrients among simulated cells.
Since vegetation growth is dynamically simulated, a spin-up simulation control for vegetation growth and associated
carbon dynamics was added. This enables users to model vegetation and cycling of C and N in terrestrial ecosystems.
In addition, land surface water balance in each cell will be checked before routing water and solutes. The model codes
were checked/debugged and greatly simplified compared to the original RHESSys model. In author's personal view, it is
easier to handle than its predecessor while most subroutines are kept identical as their counterparts in its
predecessor.

## Required data
Running CHESS, a potential alternative to the RHESSys, requires the following data:
* DEM data and DEM-derived slope, aspect, streams, roads raster data in ArcInfor ASCII format;
* Meteorological data: daily minimum and maximum temperature (in Celsius degree), and precipitation (in meter);
* Annual time-series atmospheric CO2 concentration (in ppm);
* Rasterized land-cover, land use or vegetation data to predefine land cover, land use or vegetation types;
* Rasterized soil texture class data to predefine soil types for the study region;
* A flow table file describes the spatial connectivity of water and nutrients movement among simulated patches.
Besides, four default files of patch, soil, vegetation and land-use are provided. These files include parameters
relevant to soil, vegetation and land-use that are required to parameterize before running R-RHESSys.

## Simulation Dynamics
As its predecessor, R-RHESSys is able to simulate::
* Land surface hydrology, including river stream flow, base flow, ET, plant transpiration, soil moisture, and etc.
* Ecosystem dynamics: vegetation growth and productivity, cycling of carbon and nitrogen (e.g., soil autotrophic and heterotrophic respiration) in terrestrial ecosystems, and etc. 
* Biogeochemical dynamics:: soil nitrification and denitrification, river nitrate, dissolved DOC and DON, soil mineralization, and etc.
Note: For simulating land surface hydrology, a least of 20-year spin-up simulations are required for leaf area index
to reach stable state. For simulating soil and vegetation carbon, spin-up simulations more than 150 years are required.

## Other issues
Users are welcome to employ CHESS to regions of their own interests. Examples of model forcing data can be provided for reference or learning. 
