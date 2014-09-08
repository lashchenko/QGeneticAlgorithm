#ifndef GWIDGET_H
#define GWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QTimer>

#include "widget.h"

class GWidget : public QWidget
{
Q_OBJECT
public:
    explicit GWidget(QWidget *parent = 0);

signals:

public slots:
    void slotStart();
    void evalute();

    void slotStop();

protected:
    float Fx(int x);


private:
    Widget w;

    QTimer timer;

    QVector<float> curve;
    QVector<QPointF> population;

    QSpinBox *delay;

    long iterations;
    QLabel iterationLabel;
    QLabel statisticLabel;

    long plus;
    long minus;

    bool stop;

    int N;
};

#endif // GWIDGET_H
