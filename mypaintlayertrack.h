#ifndef MYPAINTLAYERTRACK_H
#define MYPAINTLAYERTRACK_H

#include <marble/MarbleWidget.h>
#include <marble/MarbleMap.h>
#include <marble/MarbleModel.h>
#include <marble/GeoPainter.h>
#include <marble/LayerInterface.h>

#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QKeyEvent>
#include <QDebug>

#include "client.h"
#include "gpsdata.h"
#include <QImage>

using namespace Marble;

class myPaintLayerTrack : public QObject, public LayerInterface
{
    Q_OBJECT

public:
    // Constructor
    myPaintLayerTrack(MarbleWidget* widget);

    // Implemented from LayerInterface
    virtual QStringList renderPosition() const;

    // Implemented from LayerInterface
    virtual bool render( GeoPainter *painter, ViewportParams *viewport,
    const QString& renderPos = "NONE", GeoSceneLayer * layer = 0 );

    // Overriding QObject
    virtual bool eventFilter(QObject *obj, QEvent *event);

    GeoDataCoordinates approximate(const GeoDataCoordinates &base, qreal angle, qreal dist) const;
    QImage Rotate(QImage img, double grad );

    //
    gpsdata gpssender;

signals:
    void TrackResiv();
public slots:
    void GPSTrackResiv(gpsdata data);

private:
    MarbleWidget* m_widget;
    int m_index;

    //изображения и их трансформатор
    QImage   marker_active;
    QImage   marker_no_active;
    QTransform myTransform;
QImage marker_start;
QImage marker_end;
QImage marker_time;

};



#endif // MYPAINTLAYERTRACK_H
