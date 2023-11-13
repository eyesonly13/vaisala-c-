#ifndef NETCDF_VAISALA_H_
	#define NETCDF_VAISALA_H_

#include <vector>
#include <iostream>
#include <qgspoint.h>
#include <netcdf.h>


class NetCdfVaisala
{
public:
		NetCdfVaisala() = delete;
		NetCdfVaisala(const char* filename) :filename(filename), ncid(0)
		{ 
			if (nc_open(filename, NC_NOWRITE, &ncid) != 0) 
			{
				throw std::runtime_error("Ma condition n'est pas respectée !");
			}
		}
		int generate_tab_voxel(std::vector<QgsPoint>* const vect_voxel);
		int generate_tab_coord(std::vector<QgsPoint>* coord);
		int generate_tab_wind_speed(std::vector<double>* wind_speed, std::vector<int>* wind_speed_converted, std::vector<double>* rotation);
		QgsPoint voxel_size();
		QgsPoint instrument_position();

	
private:
		int ncid;
		const char* filename;
};
#endif