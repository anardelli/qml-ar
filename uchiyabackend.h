#ifndef UCHIYABACKEND_H
#define UCHIYABACKEND_H

#include "qtbackend.h"
#include "uchiyamarkerdetection.h"

// backend for QML for UchiyaMarkers detection
// uses QtCameraBackEnd class as base
// this class is a decorator on top of the camera

class UchiyaBackEnd : public QObject, public QQuickImageProvider
{
public:
    // decorate camera
    UchiyaBackEnd(QQuickImageProvider* image_provider);

    // obtain processed image
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    ~UchiyaBackEnd();
private:
    // return processed by UchiyaMarkers image
    // includes QImage<->Mat<->IplImage conversions
    QImage processUchiya(QImage image);

    // marker detection pipeline
    UchiyaMarkerDetection* md;

    // image provider
    QQuickImageProvider* provider;
};

#endif // UCHIYABACKEND_H
