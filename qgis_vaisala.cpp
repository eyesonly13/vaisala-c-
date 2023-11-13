#include "qgis_vaisala.h"

void QgisVaisala::create_layer(std::vector<QgsRectangle>vect, const char* namelayer)
{
        QgsVectorLayer* layer = new QgsVectorLayer("Polygon?crs=EPSG:4326", "points", "memory");
        if (!layer->isValid()) {
            qDebug("La couche de points n'est pas valide.");
        }
        QList<QgsField> fields;
        fields.append(QgsField("altitude", QVariant::Double));
        fields.append(QgsField("valeur", QVariant::Int));
        layer->dataProvider()->addAttributes(fields);
        layer->updateFields();
        layer->startEditing();
        for (int i = 0; i < vect.size(); i++) {
            {
                QgsFeature feature;
                feature.setGeometry(QgsGeometry::fromRect(vect[i]));
                feature.setAttributes(QgsAttributes() << QVariant(alti[i]) << QVariant(converted[i]));
                layer->addFeature(feature);
            }
        }
        layer->commitChanges();
        
        QgsVectorFileWriter::SaveVectorOptions options;
        options.fileEncoding = QStringLiteral("UTF-8");
        options.layerName = namelayer;
        QgsVectorFileWriter::QgsVectorFileWriter::writeAsVectorFormatV3(layer, filename, QgsCoordinateTransformContext(), options);
        

}

void QgisVaisala::create_layer_point(std::vector<QgsPoint>vect,std::vector<double>rotation,const char* namelayer)
{
    QgsVectorLayer* layer = new QgsVectorLayer("Point?crs=EPSG:4326", "points", "memory");
    if (!layer->isValid()) {
        qDebug("La couche de points n'est pas valide.");
    }
    QList<QgsField> fields;
    fields.append(QgsField("altitude", QVariant::Int));
    fields.append(QgsField("valeur", QVariant::Int));
    fields.append(QgsField("rotation", QVariant::Double));
    layer->dataProvider()->addAttributes(fields);
    layer->updateFields();
    layer->startEditing();
    for (int i = 0; i < vect.size(); i++) {
        {
            QgsFeature feature;
            QString wkt = QString("POINT(%1 %2 %3)").arg(vect[i].x()).arg(vect[i].y()).arg(vect[i].z());
            feature.setGeometry(QgsGeometry::fromWkt(wkt));
            feature.setAttributes(QgsAttributes() << QVariant(alti[i]) << QVariant(converted[i])<< QVariant(rotation[i]));
            layer->addFeature(feature);
        }
    }
    layer->commitChanges();

    QgsVectorFileWriter::SaveVectorOptions options;
    options.fileEncoding = QStringLiteral("UTF-8");
    options.layerName = namelayer;
    QgsVectorFileWriter::QgsVectorFileWriter::writeAsVectorFormatV3(layer, filename_point, QgsCoordinateTransformContext(), options);
}
