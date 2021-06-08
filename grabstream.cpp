#include "grabstream.h"

#include <QDebug>

GrabStream::GrabStream(QObject *parent)
    : QThread(parent)
{
    // if opencv needed, make sure multithread error
//    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<GrabStream::Status>("GrabStream::Status");
}

GrabStream::~GrabStream()
{
    m_runStatus = Status::STOP;
    // quit safely
    this->quit(); // send quit signal
    this->wait(); // wait until quit
    this->deleteLater(); // release cpu memory

}

void GrabStream::setRunning()
{
    m_runStatus = Status::RUNNING;
}

void GrabStream::setStop()
{
    if (this->isRunning())
    {
        m_runStatus = Status::STOP;
        m_frameIndex = 0;
//        this->quit(); // send quit signal
//        this->wait(); // wait until quit
//        this->deleteLater(); // release cpu memory
    }
}

void GrabStream::setPause()
{
    m_runStatus = Status::PAUSE;
}

void GrabStream::setResume()
{
    m_runStatus = Status::RESUME;
}

GrabStream::Status GrabStream::getRunStatus()
{
    return m_runStatus;
}

void GrabStream::run()
{
    qDebug() << "enter thread : " << QThread::currentThreadId();


    if (m_fileName.isEmpty())
    {
        qDebug() << "fileName empty";
        return;
    }

    while (true)
    {
        if (m_runStatus == Status::RUNNING)
        {
            m_depthQImage.load(m_fileName + QString::number(m_frameIndex++)+".png");
            // if grab frame is empty, don't send signal
            if (m_depthQImage.isNull())
            {
                Q_ASSERT_X(false, "Error", "Failed to grab frame");
                continue;;
            }
            emit sigSendFrame(m_colorQImage, m_depthQImage);
        }
        else if (m_runStatus == Status::PAUSE)
        {
//            qDebug() << "pause to grab stream";
        }
        else if (m_runStatus == Status::RESUME)
        {
//            qDebug() << "resume stream";

            m_depthQImage.load(m_fileName + QString::number(m_frameIndex++)+".png");
            // if grab frame is empty, don't send signal
            if (m_depthQImage.isNull())
            {
                Q_ASSERT_X(false, "Error", "Failed to grab frame");
                continue;;
            }
            emit sigSendFrame(m_colorQImage, m_depthQImage);

        }
        else if (m_runStatus == Status::STOP)
        {

            qDebug() << "stop to grab stream";
            break;
        }
        else
        {
            //  work when no definiton of macro QT_NO_DEBUG
            Q_ASSERT_X(false, "Error", "ERROR to grab stream");
            qDebug() << "ERROR to grab stream";
        }
    }

    qDebug() << "exit thread : " << QThread::currentThreadId();
}
