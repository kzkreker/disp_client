#include "mypaintlayer.h"


///////////////////////////////////////////////
///Конструктор слоя
///////////////////////////////////////////////
MyPaintLayer::MyPaintLayer(MarbleWidget* widget) : m_widget(widget), m_index(0)
{
    // nothing to do
    marker_active.load("marker_active.png");
    marker_no_active.load("marker_no_active.png");
}

QStringList MyPaintLayer::renderPosition() const
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
bool MyPaintLayer::eventFilter(QObject *obj, QEvent *event)
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
GeoDataCoordinates MyPaintLayer::approximate(const GeoDataCoordinates &base, qreal angle, qreal dist) const
{
    // This is just a rough estimation that ignores projections.
    // It only works for short distances. Don't use in real code.
    GeoDataCoordinates::Unit deg = GeoDataCoordinates::Degree;
    return GeoDataCoordinates ( base.longitude(deg) + 1.5 * dist * sin(angle),
                base.latitude(deg) + dist * cos(angle), 0.0, deg);
}



void MyPaintLayer::newActiveGPSResiv(gpsdata data)
{
gpssender = data;
}

////////////////////////////////////////////
///Отрисовка маркера и текущего положения
////////////////////////////////////////////
bool MyPaintLayer::render( GeoPainter *painter, ViewportParams *viewport,
    const QString& renderPos, GeoSceneLayer * layer )
{
    // Have window title reflect the current paint layer
    m_widget->setWindowTitle(renderPosition().first());
    painter->setRenderHint(QPainter::Antialiasing, true);

    QImage marker;

    foreach(gpssender.gpsitem, gpssender.gpslist)
     {
        //создаем положение маркера
        GeoDataCoordinates  home(gpssender.gpsitem.lat,
                                 gpssender.gpsitem.lon, 0.0, GeoDataCoordinates::Degree);

        //выбираем иконку взависимости от статуса машины
        if(gpssender.gpsitem.now_online==true){
             marker = Rotate(marker_active, gpssender.gpsitem.course);
        } else {
             marker = Rotate(marker_no_active, gpssender.gpsitem.course);
        }
        //рисуем иконку
        painter->drawImage(home, marker);
     }

    return true;
}

QImage MyPaintLayer::Rotate(QImage img, double grad)
{
    myTransform.reset();
    QImage resoult;
    resoult=img;
    myTransform.rotate(grad);
    resoult=resoult.transformed(myTransform);
    return resoult;
}
