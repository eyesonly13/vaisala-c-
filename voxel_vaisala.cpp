#include "voxel_vaisala.h"


void VoxelVaisala::calculate_voxel_center()
{
    struct geod_geodesic g;
    double lat = 0;
    double lon = 0;
    double new_lat = 0;
    double new_lon = 0;
    double azimuth = 0;
    double alt = 0;
    geod_init(&g, 6378137, 1 / 298.257223563);
    for (size_t i = 0; i < vect_voxel.size(); i++)
    {
        if (vect_voxel[i].x() > 0)
        {
            geod_direct(&g, point.y(), point.x(), 90, vect_voxel[i].x(), &lat, &lon, nullptr);
            
        }
        else
        {
            geod_direct(&g, point.y(), point.x(), 270, abs(vect_voxel[i].x()), &lat, &lon, nullptr);
        }
        if (vect_voxel[i].y() > 0)
        {
            geod_direct(&g, lat, lon, 0, abs(vect_voxel[i].y()), &new_lat, &new_lon, nullptr);
        }
        else
        {
            geod_direct(&g, lat, lon, 180, abs(vect_voxel[i].y()), &new_lat, &new_lon, nullptr);
        }
        alt = vect_voxel[i].z() + point.z();
        vect_voxel_center.push_back(QgsPoint(new_lon, new_lat, alt));
    }
}



void VoxelVaisala::calculate_coord_rect(std::vector<QgsRectangle>* vect_coord_rect, std::vector<double>* altitude,float x_voxel_size, float y_voxel_size)
{
    calculate_voxel_center();

    struct geod_geodesic g;
    double lat_up = 0;
    double lon_up = 0;
    double lat_bottom = 0;
    double lon_bottom = 0;
    double new_lat_up = 0;
    double new_lon_up = 0;
    double new_lat_bottom = 0;
    double new_lon_bottom = 0;
    double azimuth = 0;
    double alt = 0;
    geod_init(&g, 6378137, 1 / 298.257223563);
    for (size_t i = 0; i < vect_voxel_center.size(); i++)
    {
        geod_direct(&g, vect_voxel_center[i].y(), vect_voxel_center[i].x(), 90, x_voxel_size/2, &lat_up, &lon_up, nullptr);
        geod_direct(&g, vect_voxel_center[i].y(), vect_voxel_center[i].x(), 270, x_voxel_size / 2, &lat_bottom, &lon_bottom, nullptr);
        geod_direct(&g, lat_up, lon_up, 0, y_voxel_size / 2, &new_lat_up, &new_lon_up, nullptr);
        geod_direct(&g, lat_bottom, lon_bottom, 180, y_voxel_size / 2, &new_lat_bottom, &new_lon_bottom, nullptr);
        alt = vect_voxel_center[i].z();
        vect_coord_rect->push_back(QgsRectangle(new_lon_bottom, new_lat_bottom,new_lon_up,new_lat_up));
        altitude->push_back(alt);
    }
}