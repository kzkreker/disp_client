#include "mypaintlayertrack.h"

#include <marble/MarbleWidget.h>
#include <marble/GeoPainter.h>
#include <marble/GeoDataLineString.h>

///////////////////////////////////////////////
///Конструктор слоя
///////////////////////////////////////////////
myPaintLayerTrack::myPaintLayerTrack(MarbleWidget* widget) : m_widget(widget), m_index(0)
{
    // nothing to do
    marker_active.load("marker_active.png");
    marker_no_active.load("marker_no_active.png");

    marker_time.load("pin_map.png");
    marker_end.load("pin_end.png");
    marker_start.load("pin_start.png");

}

QStringList myPaintLayerTrack::renderPosition() const
{
    // We will paint in exactly one of the following layers.
    // The current one can be changed by pressing the '+' key
    QStringList layers = QStringList() << "SURFACE" << "HOVERS_ABOVE_SURFACE";
    layers << "ORBIT" << "USER_TOOLS" << "STARS";

    int index = m_index % layers.size();
    return QStringList() << layers.at(index);
}

///////////////////////////////////
///Обработка событий при работе с картой
///////////////////////////////////
bool myPaintLayerTrack::eventFilter(QObject *obj, QEvent *event)
{
    // Adjust the current layer when '+' is pressed
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Plus) {
            ++m_index;
            return true;
        }
    }

    return false;
}
///////////////////////////////////////////
///
///////////////////////////////////////////
GeoDataCoordinates myPaintLayerTrack::approximate(const GeoDataCoordinates &base, qreal angle, qreal dist) const
{
    // This is just a rough estimation that ignores projections.
    // It only works for short distances. Don't use in real code.
    GeoDataCoordinates::Unit deg = GeoDataCoordinates::Degree;
    return GeoDataCoordinates ( base.longitude(deg) + 1.5 * dist * sin(angle),
                base.latitude(deg) + dist * cos(angle), 0.0, deg);
}

///////////////////////////////////////////
///получение новых кординат слот получения/сигнал обновления
///////////////////////////////////////////
void myPaintLayerTrack::GPSTrackResiv(gpsdata data)
{
    gpssender = data;
    emit TrackResiv();
}

////////////////////////////////////////////
///Отрисовка маркера и текущего положения
////////////////////////////////////////////
bool myPaintLayerTrack::render( GeoPainter *painter, ViewportParams *viewport,
    const QString& renderPos, GeoSceneLayer * layer )
{
    //настройка виджета и кистей
    m_widget->setWindowTitle(renderPosition().first());
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen( QColor( 0, 0, 0 ) );

    if(gpssender.lightvisual==true &&  gpssender.gpstracklist.size()!=0){
     foreach(gpssender.gpstrackitem, gpssender.gpstracklist)
      {
         //создаем положение маркера
          GeoDataCoordinates home(gpssender.gpstrackitem.lat,
                                 gpssender.gpstrackitem.lon, 0.0, GeoDataCoordinates::Degree);

         //выбираем иконку взависимости от статуса машины
         painter->drawImage(home, marker_time);
         //пишем дату
         painter->drawText( home, gpssender.gpstrackitem.datatime.toString("yyyy-MM-dd hh:mm:ss") );

       }
    } else if (gpssender.lightvisual==false && gpssender.gpstracklist.size()!=0){

         GeoDataLineString shapeNoTessellation( RespectLatitudeCircle | Tessellate );

         foreach(gpssender.gpstrackitem, gpssender.gpstracklist)
          {
             //создаем положение маркера
             GeoDataCoordinates  line(gpssender.gpstrackitem.lat,
                                      gpssender.gpstrackitem.lon, 0.0, GeoDataCoordinates::Degree);

             //список кординат
              shapeNoTessellation <<line;
           }
        //рисуем линию
        painter->setPen( QPen( QBrush( Qt::darkCyan), 15.0, Qt::SolidLine, Qt::RoundCap ) );
        painter->drawPolyline(shapeNoTessellation );
        //ставим маркеты
        painter->setPen( QPen( QBrush( Qt::black), 15.0, Qt::SolidLine, Qt::RoundCap ) );
        painter->drawImage(shapeNoTessellation[0],marker_start);
        painter->drawText( shapeNoTessellation[0], "Начало" );
        painter->drawImage(shapeNoTessellation[ gpssender.gpstracklist.length()-1],marker_end);
        painter->drawText( shapeNoTessellation[ gpssender.gpstracklist.length()-1], "Конец" );
     }
    return true;
}

