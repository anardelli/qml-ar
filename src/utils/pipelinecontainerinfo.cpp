/**
 * @file pipelinecontainerinfo.cpp
 * @brief This class contains information which travels with
 * a pipeline container
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "pipelinecontainerinfo.h"
#include <QDateTime>
#include <QTextStream>

PipelineContainerInfo::PipelineContainerInfo(int id)
{
    this->id_ = id;
}

void PipelineContainerInfo::checkpoint(QString name)
{
    stages.append(name);
    timestamps.append(QDateTime::currentMSecsSinceEpoch());
}

PipelineContainerInfo PipelineContainerInfo::checkpointed(QString name)
{
    PipelineContainerInfo info = *this;
    info.checkpoint(name);
    return info;
}

qint64 PipelineContainerInfo::delta()
{
    if(timestamps.size() == 0) return 0;
    return timestamps.last() - timestamps.first();
}

qint64 PipelineContainerInfo::start()
{
    if(timestamps.size() == 0) return 0;
    return timestamps.first();
}

QString PipelineContainerInfo::toString()
{
    QString res;
    QTextStream ts(&res);
    ts << "ID " << id_ << " ";
    for(int i = 0; i < stages.size(); i++)
    {
        qint64 delta_t = timestamps[i];
        if(i > 0) delta_t -= timestamps[i - 1];
        ts << stages[i] << " " << delta_t << " ";
    }
    ts << "TOT " << delta();
    return res;
}

int PipelineContainerInfo::id()
{
    return id_;
}
