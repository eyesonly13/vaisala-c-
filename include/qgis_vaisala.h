#ifndef QGIS_VAISALA_H_
	#define QGIS_VAISALA_H_

#include <iostream>
#include <qgspoint.h>
#include <geodesic.h>
#include <QCoreApplication>
#include <qgsgeometry.h>
#include <qgsvectorlayer.h>
#include <qgsfields.h>
#include <qgsvectorfilewriter.h>
#include <qgsfeature.h>
#include <qgsproject.h>
#include <qgsapplication.h>


class QgisVaisala
{
public:
	QgisVaisala() = delete;
	QgisVaisala(const char* filename, const char* filename_point, std::vector<double>alti, std::vector<int>converted) : filename(filename),filename_point(filename_point),alti(alti),converted(converted) {}
	void create_layer(std::vector<QgsRectangle>vect, const char* namelayer);
	void create_layer_point(std::vector<QgsPoint>vect, std::vector<double>rotation, const char* namelayer);

private:
	const char* filename;
	const char* filename_point;
	std::vector<double>alti;
	std::vector<int>converted;
};
#endif