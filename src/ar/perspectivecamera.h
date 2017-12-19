#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "calibratedcamera.h"
#include <QObject>
#include <QMatrix4x4>

/*
 * This class extends CalibratedCamera
 * by allowing to get Perspective Projection matrix
 */

class PerspectiveCamera : public CalibratedCamera
{
public:
    // initialize with a calibrated camera
    PerspectiveCamera(CalibratedCamera* camera);

    // returns perspective projection matrix obtained from camera matrix
    // near in far in world units
    QMatrix4x4 getPerspectiveMatrix(double n, double f);
};

#endif // PERSPECTIVECAMERA_H
