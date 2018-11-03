/**
 * @file threadedqmlar.cpp
 * @brief This class is a wrapper around QMLAR object
 * which is placed at a separate thread and controlled
 * by this object
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "threadedqmlar.h"
#include "qmlar.h"
#include "timelogger.h"

ThreadedQMLAR::ThreadedQMLAR()
{
    TimeLoggerLog("%s", "Starting Threaded QMLAR");

    // creating AR object and a thread
    instance = new QMLAR();
    thread = new QThread();

    qRegisterMetaType<Qt::ApplicationState>("Qt::ApplicationState");

    // moving AR to thread
    instance->moveToThread(thread);

    // calls to this object -> another thread
    connect(this, SIGNAL(setCameraIdSignal(int)), instance, SLOT(setCameraId(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(setImageFilenameSignal(QString)), instance, SLOT(setImageFilename(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(setImageWidthSignal(int)), instance, SLOT(setImageWidth(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(startCameraSignal(void)), instance, SLOT(startCamera()), Qt::QueuedConnection);
    connect(this, SIGNAL(setFilterAlphaSignal(double)), instance, SIGNAL(newFilterAlpha(double)));
    connect(this, SIGNAL(setResetMs_signal(unsigned)), instance, SIGNAL(setResetMs_signal(unsigned)));

    // events from AR -> this object
    connect(instance, SIGNAL(imageUpdated(void)), this, SIGNAL(imageUpdated(void)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newBlobs(QVariantList)), this, SLOT(setBlobs(QVariantList)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newMarkers(QVariantList)), this, SLOT(setMarkers(QVariantList)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newMVPMatrix(QMatrix4x4)), this, SLOT(setMVPMatrix(QMatrix4x4)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newMVMatrix(QMatrix4x4)), this, SLOT(setMVMatrix(QMatrix4x4)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newPMatrix(QMatrix4x4)), this, SLOT(setPMatrix(QMatrix4x4)), Qt::QueuedConnection);

    // running by default
    is_running = true;

    // starting AR
    thread->start();
}

int ThreadedQMLAR::getCameraId()
{
    // works since is set only once from another thread
    return instance->getCameraId();
}

QString ThreadedQMLAR::getImageFilename()
{
    // works since is set only once from another thread
    return instance->getImageFilename();
}

QMatrix4x4 ThreadedQMLAR::getMVPMatrix()
{
    return mvp_matrix;
}

QMatrix4x4 ThreadedQMLAR::getMVMatrix()
{
    return mv_matrix;
}

QMatrix4x4 ThreadedQMLAR::getPMatrix()
{
    return p_matrix;
}

int ThreadedQMLAR::getImageWidth()
{
    // works since is set only once from another thread
    return instance->getImageWidth();
}

QQuickImageProvider *ThreadedQMLAR::getImageProvider()
{
    // works since is set only once from another thread
    return instance->getImageProvider();
}

QObject *ThreadedQMLAR::getCamera()
{
    // works since is set only once from another thread
    return instance->getCamera();
}

QVariantList ThreadedQMLAR::getBlobs()
{
    return last_blobs;
}

QVariantList ThreadedQMLAR::getMarkers()
{
    return last_markers;
}

double ThreadedQMLAR::getFPSMean()
{
    // works since mean is a fixed-location variable
    // which is set from a different thread
    return instance->getFPSMean();
}

double ThreadedQMLAR::getFPSStd()
{
    // works since std is a fixed-location variable
    // which is set from a different thread
    return instance->getFPSStd();
}

double ThreadedQMLAR::getLatencyMean()
{
    // works since mean is a fixed-location variable
    // which is set from a different thread
    return instance->getLatencyMean();
}

double ThreadedQMLAR::getLatencyStd()
{
    // works since std is a fixed-location variable
    // which is set from a different thread
    return instance->getLatencyStd();
}

bool ThreadedQMLAR::markers_visible()
{
    return last_markers.size() > 0;
}

double ThreadedQMLAR::getFilterAlpha()
{
    return filter_alpha;
}

bool ThreadedQMLAR::poseValid()
{
    return !mvp_matrix.isIdentity();
}

double ThreadedQMLAR::getFrameDrop()
{
    return instance->getFrameDrop();
}

int ThreadedQMLAR::getFrameDelay()
{
    return instance->getFrameDelay();
}

bool ThreadedQMLAR::getRunning()
{
    return is_running;
}

double ThreadedQMLAR::getMeanH()
{
    if(!instance) return 0;
    return instance->mean_h;
}

double ThreadedQMLAR::getDeltaH()
{
    if(!instance) return 0;
    return instance->delta_h;
}

double ThreadedQMLAR::getMinS()
{
    if(!instance) return 0;
    return instance->min_s;
}

double ThreadedQMLAR::getMaxS()
{
    if(!instance) return 0;
    return instance->max_s;
}

double ThreadedQMLAR::getMinV()
{
    if(!instance) return 0;
    return instance->min_v;
}

double ThreadedQMLAR::getMaxV()
{
    if(!instance) return 0;
    return instance->max_v;
}

unsigned ThreadedQMLAR::getResetMs()
{
    if(!instance) return 0;
    return instance->getResetMs();
}

void ThreadedQMLAR::setCameraId(int camera_id)
{
    emit setCameraIdSignal(camera_id);
}

void ThreadedQMLAR::setImageFilename(QString filename)
{
    emit setImageFilenameSignal(filename);
}

void ThreadedQMLAR::setImageWidth(int new_width)
{
    emit setImageWidthSignal(new_width);
}

void ThreadedQMLAR::startCamera()
{
    emit startCameraSignal();
}

void ThreadedQMLAR::setFilterAlpha(double alpha)
{
    this->filter_alpha = alpha;
    emit setFilterAlphaSignal(alpha);
}

void ThreadedQMLAR::setBlobs(QVariantList that)
{
    this->last_blobs = that;
    emit newBlobs(last_blobs);
}

void ThreadedQMLAR::setMarkers(QVariantList that)
{
    this->last_markers = that;
    emit newMarkers(last_markers);
}

void ThreadedQMLAR::setMVPMatrix(QMatrix4x4 that)
{
    this->mvp_matrix = that;
    emit newMVPMatrix(mvp_matrix);
}

void ThreadedQMLAR::setMVMatrix(QMatrix4x4 that)
{
    this->mv_matrix = that;
    emit newMVMatrix(mv_matrix);
}

void ThreadedQMLAR::setPMatrix(QMatrix4x4 that)
{
    this->p_matrix = that;
    emit newPMatrix(p_matrix);
}

void ThreadedQMLAR::setRunning(bool value)
{
    is_running = value;
    instance->setRunning(is_running);
}

void ThreadedQMLAR::setShowShader(bool value)
{
    instance->setShowShader(value);
}

void ThreadedQMLAR::setMeanH(double value)
{
    if(!instance) return;
    instance->mean_h = value;
    instance->updateThreshold();
}

void ThreadedQMLAR::setDeltaH(double value)
{
    if(!instance) return;
    instance->delta_h = value;
    instance->updateThreshold();
}

void ThreadedQMLAR::setMinS(double value)
{
    if(!instance) return;
    instance->min_s = value;
    instance->updateThreshold();
}

void ThreadedQMLAR::setMaxS(double value)
{
    if(!instance) return;
    instance->max_s = value;
    instance->updateThreshold();
}

void ThreadedQMLAR::setMinV(double value)
{
    if(!instance) return;
    instance->min_v = value;
    instance->updateThreshold();
}


void ThreadedQMLAR::setMaxV(double value)
{
    if(!instance) return;
    instance->max_v = value;
    instance->updateThreshold();
}

void ThreadedQMLAR::setResetMs(unsigned reset_ms)
{
    if(!instance) return;
    setResetMs_signal(reset_ms);
}
