#include "netcdf_vaisala.h"


std::pair<double, double> min_max_vect(std::vector<double> vect)
{
    std::vector<double> converted;
    for (size_t i = 0; i < vect.size(); i++)
    {
        if (!isnan(vect[i]))
        {
            converted.push_back(vect[i]);
        }
    }
     auto it = std::minmax_element(converted.begin(), converted.end());
     double min = *it.first;
     double max = *it.second;
     return std::make_pair(min, max);
}

int NetCdfVaisala:: generate_tab_voxel(std::vector<QgsPoint>* const vect_voxel)
{
    int varid_x_voxel = 0;
    int varid_y_voxel = 0;
    int varid_z_voxel = 0;
    int dimid_x_voxel = 0;
    int dimid_y_voxel = 0;
    int dimid_z_voxel = 0;
    size_t x_len = 0;
    size_t y_len = 0;
    size_t z_len = 0;
    double coord_x[100] = { 0 };
    double coord_y[100] = { 0 };
    double coord_z[100] = { 0 };
    int status = 0;
    do
    {
        status = nc_inq_varid(this->ncid, "x", &varid_x_voxel);
        if (status != 0) break;
        status = nc_inq_varid(this->ncid, "y", &varid_y_voxel);
        if (status != 0) break;
        status = nc_inq_varid(this->ncid, "z", &varid_z_voxel);
        if (status != 0) break;
        status = nc_inq_vardimid(this->ncid, varid_x_voxel, &dimid_x_voxel);
        if (status != 0) break;
        status = nc_inq_vardimid(this->ncid, varid_y_voxel, &dimid_y_voxel);
        if (status != 0) break;
        status = nc_inq_vardimid(this->ncid, varid_z_voxel, &dimid_z_voxel);
        if (status != 0) break;
        status = nc_inq_dimlen(this->ncid, dimid_x_voxel, &x_len);
        if (status != 0) break;
        status = nc_inq_dimlen(this->ncid, dimid_y_voxel, &y_len);
        if (status != 0) break;
        status = nc_inq_dimlen(this->ncid, dimid_z_voxel, &z_len);
        if (status != 0) break;
        status = nc_get_var_double(this->ncid, varid_x_voxel, coord_x);
        if (status != 0) break;
        status = nc_get_var_double(this->ncid, varid_y_voxel, coord_y);
        if (status != 0) break;
        status = nc_get_var_double(this->ncid, varid_z_voxel, coord_z);
    } while (0);
    if (status == 0)
    {
        for (int i = 0; i < x_len; i++)
        {
            for (int y = 0; y < y_len; y++)
            {
                for (int z = 0; z < z_len; z++)
                {
                    vect_voxel->push_back(QgsPoint(coord_x[i], coord_y[y], coord_z[z]));
                }
            }
        }
    }
    return status;
}

QgsPoint NetCdfVaisala::instrument_position()
{
    int status = 0;
    int varid_x_instrument = 0;
    int varid_y_instrument = 0;
    int varid_z_instrument = 0;
    int dimid_x_instrument = 0;
    int dimid_y_instrument = 0;
    int dimid_z_instrument = 0;
    size_t x_len = 0;
    size_t y_len = 0; 
    size_t z_len = 0;
    double coord_x[100] = { 0 };
    double coord_y[100] = { 0 };
    double coord_z[100] = { 0 };
    do
    {
        status = nc_inq_varid(this->ncid, "instrument_longitude", &varid_x_instrument);
        if (status != 0) break;
        status = nc_inq_varid(this->ncid, "instrument_latitude", &varid_y_instrument);
        if (status != 0) break;
        status = nc_inq_varid(this->ncid, "instrument_altitude", &varid_z_instrument);
        if (status != 0) break;
        status = nc_inq_vardimid(this->ncid, varid_x_instrument, &dimid_x_instrument);
        if (status != 0) break;
        status = nc_inq_vardimid(this->ncid, varid_y_instrument, &dimid_y_instrument);
        if (status != 0) break;
        status = nc_inq_vardimid(this->ncid, varid_z_instrument, &dimid_z_instrument);
        if (status != 0) break;
        status = nc_inq_dimlen(this->ncid, dimid_x_instrument, &x_len);
        if (status != 0) break;
        status = nc_inq_dimlen(this->ncid, dimid_y_instrument, &y_len);
        if (status != 0) break;
        status = nc_inq_dimlen(this->ncid, dimid_z_instrument, &z_len);
        if (status != 0) break;
        status = nc_get_var_double(this->ncid, varid_x_instrument, coord_x);
        if (status != 0) break;
        status = nc_get_var_double(this->ncid, varid_y_instrument, coord_y);
        if (status != 0) break;
        status = nc_get_var_double(this->ncid, varid_z_instrument, coord_z);
    } while (0);
    return QgsPoint(coord_x[0], coord_y[0], coord_z[0]);
}



QgsPoint NetCdfVaisala::voxel_size()
{
    int status = 0;
    int varid_x_size = 0;
    int varid_y_size = 0;
    int dimid_x_size = 0;
    int dimid_y_size = 0;
    size_t x_len = 0;
    size_t y_len = 0;
    double size_x[100] = { 0 };
    double size_y[100] = { 0 };
    do
    {
        status = nc_inq_varid(this->ncid, "voxel_x_size", &varid_x_size);
        if (status != 0) break;
        status = nc_inq_varid(this->ncid, "voxel_y_size", &varid_y_size);
        if (status != 0) break;
        
        status = nc_inq_vardimid(this->ncid, varid_x_size, &dimid_x_size);
        if (status != 0) break;
        status = nc_inq_vardimid(this->ncid, varid_y_size, &dimid_y_size);
        
        if (status != 0) break;
        status = nc_inq_dimlen(this->ncid, dimid_x_size, &x_len);
        if (status != 0) break;
        status = nc_inq_dimlen(this->ncid, dimid_y_size, &y_len);
        if (status != 0) break;
        
        status = nc_get_var_double(this->ncid, varid_x_size, size_x);
        if (status != 0) break;
        status = nc_get_var_double(this->ncid, varid_y_size, size_y);
    } while (0);
    return QgsPoint(size_x[0], size_y[0]);
}

int NetCdfVaisala::generate_tab_coord(std::vector<QgsPoint>* coord)
{
    int status_first = 0;
    int status_se= 0;
    int varid_z_altitude = 0;
    int dimid_z_altitude = 0;
    double altitude_tab[100] = { 0 };
    int dimids[3] = { 0 };
    size_t dimlen[3] = { 0 };
    size_t start[3] = { 0 };
    size_t count[3] = { 0 };
    int varid_longitude = 0;
    int varid_windspeed = 0;
    int varid_rotation = 0;
    int varid_latitude = 0;
    do
    {
        status_first = nc_inq_varid(this->ncid, "z", &varid_z_altitude);
        if (status_first != 0) break;
        status_first = nc_inq_vardimid(this->ncid, varid_z_altitude, &dimid_z_altitude);
        if (status_first != 0) break;
        status_first = nc_get_var_double(this->ncid, varid_z_altitude, altitude_tab);
        if (status_first != 0) break;
        status_first = nc_inq_varid(this->ncid, "longitude", &varid_longitude);;
        if (status_first != 0) break;
        status_first = nc_inq_varid(this->ncid, "latitude", &varid_latitude);
        if (status_first != 0) break;
        status_first = nc_inq_vardimid(this->ncid, varid_longitude, dimids);
        if (status_first != 0) break;
        for (int i = 0; i < 3; i++)
        {
            status_first = nc_inq_dimlen(this->ncid, dimids[i], &dimlen[i]);
        }
    } while (0);
    double* data_longitude = new double[dimlen[0] * dimlen[1] * dimlen[2]];
    double* data_latitude = new double[dimlen[0] * dimlen[1] * dimlen[2]];
    memset(data_longitude, 0, dimlen[0] * dimlen[1] * dimlen[2] * sizeof(double));
    memset(data_latitude, 0, dimlen[0] * dimlen[1] * dimlen[2] * sizeof(double));
    for (int i = 0; i < 3; i++)
    {
        count[i] = dimlen[i];
    }
    do
    {
        status_se = nc_get_vara_double(this->ncid, varid_longitude, start, count, data_longitude);
        if (status_se != 0) break;
        status_se = nc_get_vara_double(this->ncid, varid_latitude, start, count, data_latitude);
    } while (0);
        for (size_t i = 0; i < dimlen[0]; i++)
        {
            for (size_t j = 0; j < dimlen[1]; j++)
            {
                for (size_t k = 0; k < dimlen[2]; k++)
                {
                    size_t index = i * dimlen[1] * dimlen[2] + j * dimlen[2] + k;
                    
                    coord->push_back(QgsPoint(data_longitude[index], data_latitude[index], altitude_tab[k]));
                   
                   
                }
            }
        }

    delete[] data_longitude;
    delete[] data_latitude;
    return status_first | status_se;
}

int NetCdfVaisala::generate_tab_wind_speed(std::vector<double>* wind_speed, std::vector<int>* wind_speed_converted, std::vector<double>* rotation)
{
    int status_first = 0;
    int status_se = 0;
    int varid_z_altitude = 0;
    int dimid_z_altitude = 0;
    double altitude_tab[100] = { 0 };
    int dimids[4] = { 0 };
    int dimids_rotation[4] = { 0 };
    size_t dimlen[4] = { 0 };
    size_t start[4] = { 0 };
    size_t count[4] = { 0 };
    int varid_windspeed = 0;
    int varid_rotation = 0;
    do
    {
        status_first = nc_inq_varid(this->ncid, "z", &varid_z_altitude);
        if (status_first != 0) break;
        status_first = nc_inq_vardimid(this->ncid, varid_z_altitude, &dimid_z_altitude);
        if (status_first != 0) break;
        status_first = nc_get_var_double(this->ncid, varid_z_altitude, altitude_tab);
        if (status_first != 0) break;
        status_first = nc_inq_varid(this->ncid, "horizontal_wind_speed", &varid_windspeed);
        if (status_first != 0) break;
        status_first = nc_inq_varid(this->ncid, "wind_direction", &varid_rotation);
        if (status_first != 0) break;
        status_first = nc_inq_vardimid(this->ncid, varid_windspeed, dimids);
        if (status_first != 0) break;
        status_first = nc_inq_vardimid(this->ncid, varid_rotation, dimids_rotation);
        if (status_first != 0) break;
        for (int i = 0; i < 4; i++)
        {
            status_first = nc_inq_dimlen(this->ncid, dimids[i], &dimlen[i]);
        }
    } while (0);
    double* data_wind_speed = new double[dimlen[0] * dimlen[1] * dimlen[2] * dimlen[3]];
    memset(data_wind_speed, 0, dimlen[0] * dimlen[1] * dimlen[2] * dimlen[3] * sizeof(double));
    double* data_rotation = new double[dimlen[0] * dimlen[1] * dimlen[2] * dimlen[3]];
    memset(data_rotation, 0, dimlen[0] * dimlen[1] * dimlen[2] * dimlen[3] * sizeof(double));
    for (int i = 0; i < 4; i++)
    {
        count[i] = dimlen[i];
    }
    
    status_se = nc_get_vara_double(this->ncid, varid_windspeed, start, count, data_wind_speed);
    status_se = nc_get_vara_double(this->ncid, varid_rotation, start, count, data_rotation);
    for (size_t i = 0; i < dimlen[0]; i++)
        {
            for (size_t j = 0; j < dimlen[1]; j++)
            {
                for (size_t k = 0; k < dimlen[2]; k++)
                {
                    size_t index = i * dimlen[1] * dimlen[2] + j * dimlen[2] + k;
                    wind_speed->push_back(data_wind_speed[index]);
                    if (!isnan(data_rotation[index]))
                    {
                        rotation->push_back(data_rotation[index]);
                    }
                    else
                    {
                        rotation->push_back(0);
                    }
                    
                }
            }
        }
    std::pair<double, double> min_max = min_max_vect(*wind_speed);
    for (size_t i = 0; i < wind_speed->size(); i++)
    {
        if (!isnan((*wind_speed)[i]))
        {
            wind_speed_converted->push_back((int)(((*wind_speed)[i] - min_max.first) * 1000 / (min_max.second - min_max.first)) * .7 + 150);
        }
        else
        {
            wind_speed_converted->push_back(0);
        }

    }
    
    delete[] data_wind_speed;
    return status_first | status_se;
}
   
