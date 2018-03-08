#ifndef LATENCYCALCULATOR_H
#define LATENCYCALCULATOR_H

#include <QObject>
#include "pipelinecontainerinfo.h"
#include <QLinkedList>

/*
 * This class calculates latency based on PipelineContainerInfos
 */

class LatencyCalculator : public QObject
{ Q_OBJECT
private:
    // averaging window
    int frames_window;

    // latency array
    QLinkedList<qint64> latency_ms;

    // mean/std local copy
    double latency_mean, latency_std;

    // call on new container
    void update();

public:
    LatencyCalculator();

    // result
    double mean();
    double std();

public slots:
    // called on each processed container
    void onNewContainerInfo(PipelineContainerInfo i);

signals:
    // tells current mean value
    void newMean(double);
};

#endif // LATENCYCALCULATOR_H
