#define NOMINMAX
#include "netcdf_vaisala.h"
#include "voxel_vaisala.h"
#include "qgis_vaisala.h"
#include <string>
#include <windows.h>
#include <codecvt>
#include <qgsmapcanvas.h>
#include <qgsproject.h>
#include <qgsmapsettings.h>
#include <qgslayertreegroup.h>
#include <qgslayertree.h>
#include <qgsrasterlayer.h>



std::vector<std::string> print_files(std::wstring folder_path)
{
    auto wstring_to_string = [](const std::wstring& wstr) -> std::string {
        int num_chars = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string str(num_chars, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], num_chars, nullptr, nullptr);
        return str;
    };

    std::vector<std::string> filenames;

    WIN32_FIND_DATAW find_data;
    HANDLE find_handle;
    std::wstring search_pattern = folder_path + L"\\*";
    find_handle = FindFirstFileW(search_pattern.c_str(), &find_data);

    if (find_handle != INVALID_HANDLE_VALUE) {
        do {
            if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::wstring full_path = folder_path + L"\\" + find_data.cFileName;
                filenames.push_back(wstring_to_string(full_path));
            }
        } while (FindNextFileW(find_handle, &find_data) != 0);

        FindClose(find_handle);
    }

    return filenames;
}

std::string wstring_to_string(const std::wstring& wideStr) {
    int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (requiredSize <= 0) {
        throw std::runtime_error("Erreur de conversion");
    }

    std::string str(requiredSize, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &str[0], requiredSize, nullptr, nullptr);

    return str;
}

int main(int argc, char** argv) 
{
        
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(NULL, path, MAX_PATH);
        std::wstring folder_pat = path;
        folder_pat = folder_pat.substr(0, folder_pat.find_last_of(L"\\/"));
        std::wstring create_dir = folder_pat + L"\\qgis";
        CreateDirectory(create_dir.c_str(), nullptr);
        std::wstring full_path = folder_pat + L"\\" + L"nc";
        std::vector<std::string> filenames = print_files(full_path);

        QgsApplication::setPrefixPath("C:\\Program Files\\QGIS 3.30.0", true);
        QgsApplication app(argc, argv, true);
        QString pluginPath = "C:\\Program Files\\QGIS 3.30.0\\apps\\qt5\\plugins";
        QCoreApplication::addLibraryPath(pluginPath);
        app.initQgis();

        int compteur = 0;
        std::vector<std::string>liste_file_voxel;
        std::vector<std::string>liste_file_point;
        std::vector<std::string>liste_suffixe;
        std::vector<std::string>liste_suffixe_2;
        std::string suffixe;
        std::string suffixe_2;
        for (const std::string& file_path_original : filenames)
        {
            std::string file_path = file_path_original;
            ++compteur;
            suffixe = "voxel" + std::to_string(compteur);
            suffixe_2 = "point" + std::to_string(compteur);
            std::string cacahuete = file_path.substr(91, 16);
            file_path += suffixe;
            std::string filename_2 = wstring_to_string(folder_pat + L"\\" + L"qgis\\" + L"\\voxel - " + std::wstring(cacahuete.begin(), cacahuete.end())+  L".gpkg");
            std::string filename_4 = wstring_to_string(folder_pat + L"\\" + L"qgis\\" + L"\\point - " + std::wstring(cacahuete.begin(), cacahuete.end()) + L".gpkg");
            const char* file_path_cstr = file_path.c_str();
            
            

            std::vector<QgsPoint> vect;
            std::vector<double> wind_speed;
            std::vector<double> rotation;
            std::vector<int> wind_speed_converted;
            std::vector<QgsPoint> vect_zain;
            std::vector<QgsRectangle> vect_voxel_2;
            std::vector<double> altitude;
            
            NetCdfVaisala kenzi(file_path_cstr);

            kenzi.generate_tab_voxel(&vect);
            kenzi.generate_tab_coord(&vect_zain);
            kenzi.generate_tab_wind_speed(&wind_speed, &wind_speed_converted,&rotation);
            QgsPoint point = kenzi.instrument_position();
            QgsPoint size_voxel = kenzi.voxel_size();
            VoxelVaisala zain(vect, point);
            zain.calculate_coord_rect(&vect_voxel_2, &altitude,size_voxel.x(),size_voxel.y());
            QgisVaisala qgsprocess(filename_2.c_str(), filename_4.c_str(), altitude, wind_speed_converted);
            for (size_t i = 0; i < vect_voxel_2.size(); i++)
            {
            }
            std::cout << "fichier " << suffixe << std::endl;
            std::cout << "fichier " << suffixe_2 << std::endl;
            std::cout << "------------------" << std::endl;
            qgsprocess.create_layer(vect_voxel_2, suffixe.c_str());
            qgsprocess.create_layer_point(vect_zain,rotation,suffixe_2.c_str());
            liste_file_voxel.push_back(filename_2);
            liste_file_point.push_back(filename_4);
            liste_suffixe.push_back(suffixe);
            liste_suffixe_2.push_back(suffixe_2);
    }
        QgsProject* project = QgsProject::instance();
        std::string kenzikenzi = wstring_to_string(folder_pat + L"\\" + L"qgis\\mon_projet.qgs");
        project->setFileName(kenzikenzi.c_str());
        QgsCoordinateReferenceSystem crs;
        crs.createFromString("EPSG:3857");
        project->setCrs(crs);
        QgsMapCanvas* canvas = new QgsMapCanvas();
        QgsLayerTree* root = project->layerTreeRoot();
        /*QgsRasterLayer* layer_streetmap = new QgsRasterLayer("type=xyz&url=http://tile.openstreetmap.org/{z}/{x}/{y}.png", "OpenStreetMap", "wms");*/
        for (size_t i = 0; i < liste_file_voxel.size(); i++)
        {
            QString groupName = QString("T%1").arg(std::to_string(i+1).c_str());
            QgsLayerTreeGroup* group = root->addGroup(groupName);
            QgsVectorLayer* layer_voxel = new QgsVectorLayer((liste_file_voxel[i]).c_str(), liste_suffixe[i].c_str(), "ogr");
            QgsVectorLayer* layer_point = new QgsVectorLayer((liste_file_point[i]).c_str(), liste_suffixe_2[i].c_str(), "ogr");
            QgsMapLayer* added_voxel_layer = project->addMapLayer(layer_point, false);
            QgsMapLayer* added_point_layer = project->addMapLayer(layer_voxel, false);
            group->addLayer(layer_point);
            group->addLayer(layer_voxel);
   
        }
        
        /*project->addMapLayer(layer_streetmap, false);
        QgsLayerTreeLayer* layerTreeLayer = root->insertLayer(-1, layer_streetmap);*/
        project->write();
        std::string file_1_delete = wstring_to_string(folder_pat + L"\\" + L"qgis\\mon_projet.qgs~");
        std::string file_2_delete = wstring_to_string(folder_pat + L"\\" + L"qgis\\mon_projet_attachments.zip");
        std::remove(file_1_delete.c_str());
        std::remove(file_2_delete.c_str());
        Sleep(2000);
        return 0;
    }
    


