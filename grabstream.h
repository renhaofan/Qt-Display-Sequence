#ifndef GRABSTREAM_H
#define GRABSTREAM_H


#include <QThread>
#include <QObject>
#include <QImage>
// to solve cpu working problem when pause
#include <atomic>
#include <QMutex>
#include <QWaitCondition>

// reference: https://blog.csdn.net/wzz953200463/article/details/108752377
// reference: https://blog.csdn.net/zyhse/article/details/106307616
class GrabStream : public QThread
{
    Q_OBJECT

signals:
    void sigSendFrame(const QImage &image, const QImage &depth);

public:
    GrabStream(QObject *parent = nullptr);
    ~GrabStream();

    enum class Status
    {
        ERROR,
        STOP,
        RUNNING,
        PAUSE,
        RESUME
    };

    void setStop();
    void setRunning();
    void setPause();
    void setResume();

    Status getRunStatus();




protected:
    void run();

private:
    Status m_runStatus{Status::STOP};


    int m_frameIndex{0};

    QImage m_colorQImage;
    QImage m_depthQImage;
    QString m_fileName{"C:/Users/Kris/Pictures/out/depth/"};


    QMutex mutex;
    QWaitCondition condition;
};




#endif // GRABSTREAM_H
