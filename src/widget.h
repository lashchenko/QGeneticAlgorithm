#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QImage>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void setCurve( QVector<float> c);
    void setPopulation(QVector<QPointF> p);

public:
    void slotUpdate();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void drawCurve();
    void drawPopulation();
    void drawGrid();

    void calculateCoefficients();

private:
    QImage image;
    QPainter painter;

    QVector<float> curve;
    QVector<QPointF> population;

    bool modify;

    int N;

    float kX;
    float kY;
};

#endif // WIDGET_H
