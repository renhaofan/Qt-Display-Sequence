#include "showstream.h"
#include "pcolor.h"
#include <QDebug>


ShowStream::ShowStream(QWidget *parent)
    : QWidget(parent)
{
    m_thread = new GrabStream(this);
    connect(m_thread, &GrabStream::sigSendFrame, this, &ShowStream::slotGetFrame);

    m_startBtn = new QPushButton;
    m_startBtn->setText("Start");
    m_pauseBtn = new QPushButton;
    m_pauseBtn->setText("Pause");
    m_resumeBtn = new QPushButton;
    m_resumeBtn->setText("Resume");
    m_stopBtn = new QPushButton;
    m_stopBtn->setText("Stop");

    connect(m_startBtn, &QPushButton::clicked, this, &ShowStream::slotStartBtnclicked);
    connect(m_pauseBtn, &QPushButton::clicked, this, &ShowStream::slotPauseBtnclicked);
    connect(m_resumeBtn, &QPushButton::clicked, this, &ShowStream::slotResumeBtnclicked);
    connect(m_stopBtn, &QPushButton::clicked, this, &ShowStream::slotStopBtnclicked);


    scene = new QGraphicsScene;
    scene->addPixmap(QPixmap("C:/Users/Kris/Pictures/test/no_image320240.png"));
    view = new QGraphicsView(scene);
    view->setStyleSheet("padding: 0px; border: 0px;");

    vlayout = new QVBoxLayout;
    vlayout->addWidget(m_startBtn);
    vlayout->addWidget(m_pauseBtn);
    vlayout->addWidget(m_resumeBtn);
    vlayout->addWidget(m_stopBtn);
    vlayout->addWidget(view);
    this->setLayout(vlayout);

}

ShowStream::~ShowStream()
{
    m_thread->setStop();
}

void ShowStream::setShowWidgetDefault()
{
    scene->clear();
    scene->addPixmap(QPixmap("C:/Users/Kris/Pictures/test/no_image320240.png"));
    scene->update();
    view->setScene(scene);
}

void ShowStream::slotStartBtnclicked()
{
    qDebug() << "start";

    if (!m_thread->isRunning())
    {
        m_thread->setRunning();
        m_thread->start();
    }
}

void ShowStream::slotPauseBtnclicked()
{
    m_thread->setPause();

}

void ShowStream::slotResumeBtnclicked()
{
    m_thread->setResume();
}

void ShowStream::slotStopBtnclicked()
{
    m_thread->setStop();
}

void ShowStream::slotGetFrame(QImage color, QImage depth)
{
    m_sideDepth = depth.copy();
    m_sideDepth = convertGray16ToGray8(m_sideDepth);
    m_sideDepth = image2turbo(m_sideDepth);

    m_sideDepth = m_sideDepth.scaled(0.5 * m_sideDepth.size(), Qt::KeepAspectRatio);

    scene->clear();
    scene->addPixmap(QPixmap::fromImage(m_sideDepth));
    scene->update();
}
