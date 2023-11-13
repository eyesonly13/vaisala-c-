#ifndef VOXEL_VAISALA_H_
	#define VOXEL_VAISALA_H_

#include <vector>
#include <iostream>
#include <qgspoint.h>
#include <geodesic.h>

class VoxelVaisala
{
public:
	VoxelVaisala(std::vector<QgsPoint> vect_voxel, QgsPoint point):vect_voxel(vect_voxel),point(point) {};
	void calculate_coord_rect(std::vector<QgsRectangle>* vect_coord_rect, std::vector<double>* altitude, float x_voxel_size, float y_voxel_size);


private:
	void calculate_voxel_center();
	std::vector<QgsPoint> vect_voxel_center;
	std::vector<QgsPoint> vect_voxel;
	QgsPoint point;
};
#endif