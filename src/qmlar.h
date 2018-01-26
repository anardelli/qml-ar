#ifndef CELLULOAR_H
#define CELLULOAR_H

#include <QtCore>
#include <QVector2D>
#include <QtQml>
#include <QString>
#include <QVariantList>
#include <QMatrix4x4>
#include <QQuickImageProvider>

// forward declarations
class ImageProviderAsync;
class UchiyaMarkerDetector;
class PerspectiveCamera;
class MarkerBackEnd;
class MarkerMVPProvider;
class CalibratedCamera;
class IMUMVPDecorator;
class PosePredictor;
class TrackingDecorator;
class BlobDetector;
class QtCamera2QML;
class ImageScaler;
class MarkerStorage;
class HSVIntervalDetector;
class HueThreshold;
class IMU;

/*
 * This class is the C++/QML interface to the
 * QML AR library
 */

class QMLAR : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int camera_id WRITE setCameraId READ getCameraId)
    Q_PROPERTY(QString image_filename WRITE setImageFilename READ getImageFilename)
    Q_PROPERTY(QObject* camera READ getCamera NOTIFY never)
    Q_PROPERTY(QMatrix4x4 mvp_matrix READ getMVPMatrix NOTIFY newMVPMatrix)
    Q_PROPERTY(int image_width READ getImageWidth WRITE setImageWidth)
    Q_PROPERTY(QVariantList blobs READ getBlobs NOTIFY newBlobs)
    Q_PROPERTY(QVariantList markers READ getMarkers NOTIFY newMarkers)

public:
    // enum for initialization type
    enum InitType
    {
        INIT_CAMERA,
        INIT_IMAGE
    };
    Q_ENUMS(InitType)

    // enum for output type
    enum OutputImage
    {
        OUTPUT_CAMERA,
        OUTPUT_MARKERS
    };
    Q_ENUMS(OutputImage)

    // initialization type
    int init_type;

    // empty constructor
    // object must be initialized with
    // one of the methods below
    QMLAR();

    // returns camera id to be used
    // as raw input
    int getCameraId();

    // return filename of the image to be used as input
    QString getImageFilename();

    // returns resulting MVP matrix
    QMatrix4x4 getMVPMatrix();

    // returns image width
    int getImageWidth();

    // get processed image provider
    QQuickImageProvider* getImageProvider();

    // return camera object
    QObject *getCamera();

    // return list of blobs
    QVariantList getBlobs();

    // return list of marker corners
    QVariantList getMarkers();
public slots:
    // initialize from camera id (default value -1)
    void setCameraId(int camera_id = -1);

    // initialize from image
    void setImageFilename(QString filename);

    // set width of the image to process (on startup)
    void setImageWidth(int new_width);

    // start camera if required
    void startCamera();

    // set MVP for QML
    void setMVP(QMatrix4x4 mvp);

    // set blobs from detector
    void setBlobs(QVector<QVector2D> blobs);

    // set markers
    void setMarkers(MarkerStorage& storage);

    // sets mean and std hue of dots
    void hueAvailable(double mean, double std);

    // set image and dots from detector
    void setDots(QPair<QImage, QVector<QVector2D>> image_dots);
private:
    // vector with blobs
    QVector<QVector2D> last_blobs;

    // storage for markers
    MarkerStorage* marker_storage;

    // mvp buffer for QML
    QMatrix4x4 mvp_buffer;

    // width of the input camera image
    int image_width;

    // name of the image to be used
    // as raw input
    QString image_filename;

    // camera id to be used
    // as raw input
    int camera_id;

    // true iff can obtain MVP matrix
    bool is_initialized;

    // instantiate objects
    void initialize();

    // image provider (camera)
    ImageProviderAsync* raw_provider;

    // marker detector
    UchiyaMarkerDetector* detector;

    // camera matrix
    CalibratedCamera* camera_matrix;

    // perspective camera model
    PerspectiveCamera* perspective_camera;

    // marker backend for visualizing
    // detected markers
    MarkerBackEnd* marker_backend;

    // ModelViewProjection matrix
    // provider
    MarkerMVPProvider* mvp_provider;

    // Inertial Measurement object
    IMU* imu;

    // MVP decorated with last pose from IMU
    IMUMVPDecorator* mvp_imu_decorated;

    // Pose Estimator
    PosePredictor* predictor;

    // Tracking object
    TrackingDecorator* tracking;

    // blob detector
    BlobDetector* blob_detector;

    // drawn blobs
    QImage detected_blobs;

    // maximal number of dots to detect
    const int max_dots = 50;

    // camera wrapper
    QtCamera2QML* camera_wrapper;

    // scaler for input images
    ImageScaler* scaler;

    // detector of dots color interval
    HSVIntervalDetector* hsv_interval;

    // Hue HSV thresholder
    HueThreshold* hue_threshold;

    // connect underlying objects
    void connectAll();
signals:
    // notify QML part when new matrix is available
    void newMVPMatrix();

    // notify about new image
    void imageUpdated();

    // never called
    void never();

    // on new blobs from detector
    void newBlobs();

    // on new markers
    void newMarkers();
};

#endif // CELLULOAR_H
