#ifndef HUETHRESHOLD_H
#define HUETHRESHOLD_H

#include "imageproviderasync.h"
#include "opencv2/highgui.hpp"
#include <QtConcurrent>

/*
 * This class outputs binarized image
 * taking colors in [hue-delta, hue+delta]
 */

class HueThreshold : public ImageProviderAsync
{ Q_OBJECT
public:
    HueThreshold();
    virtual ~HueThreshold() {}
    HueThreshold(const HueThreshold &that);

private:
    // for results from thread
    QFutureWatcher<QImage> watcher;

    // minimum and maximum values for thresholding
    QVector<cv::Scalar> min_hsv;
    QVector<cv::Scalar> max_hsv;

    // true if image is pending processing
    int input_buffer_nonempty;

    // buffer for input images
    QImage input_buffer;

    // do color thresholding
    QImage threshold(QImage source);

    // minimal and maximal SV values
    int min_s, max_s, min_v, max_v;

    // set min/max hue values in 0..360
    void addMaxHue(double hue);
    void addMinHue(double hue);
public slots:
    // set color to threshold on
    void setColor(double mean, double std);

    // set V, S distribution
    void setV(double mean, double std);
    void setS(double mean, double std);

    // set input image
    void setInput(QImage input);

    // handle result from thread
    void handleFinished();
};

#endif // HUETHRESHOLD_H