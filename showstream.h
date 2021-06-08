#ifndef SHOWSTREAM_H
#define SHOWSTREAM_H

#include "grabstream.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QVBoxLayout>

class ShowStream : public QWidget
{
    Q_OBJECT

public:
    ShowStream(QWidget *parent = nullptr);
    ~ShowStream();

private slots:
    void slotGetFrame(QImage color, QImage depth);
    void slotStartBtnclicked();
    void slotPauseBtnclicked();
    void slotResumeBtnclicked();
    void slotStopBtnclicked();



private:
    QPushButton *m_startBtn;
    QPushButton *m_pauseBtn;
    QPushButton *m_resumeBtn;
    QPushButton *m_stopBtn;



    GrabStream *m_thread;

    QGraphicsScene *scene;
    QGraphicsView *view;

    QImage m_sideColor;
    QImage m_sideDepth;

    QVBoxLayout *vlayout;


    void setShowWidgetDefault();
};




#endif // SHOWSTREAM_H
