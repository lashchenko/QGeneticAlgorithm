#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(400,200);

    modify = false;
}

Widget::~Widget()
{

}

void Widget::slotUpdate()
{
    modify = true;
    calculateCoefficients();
    update();
}

void Widget::setCurve(QVector<float> c)
{
    curve = c;
    slotUpdate();
}

void Widget::setPopulation(QVector<QPointF> p)
{
    population = p;
    slotUpdate();
}

void Widget::drawCurve()
{
    image = QImage(size(), QImage::Format_ARGB32);
    image.fill(Qt::gray);

    painter.begin(&image);

    QPolygonF poly;
    for (int i=0; i<curve.size(); ++i) {
        float x = i*kX;
        float y = height() - curve.at(i)*kY;

        poly.append(QPointF(x,y));
//        painter.fillRect(x-5,y-5,10,10, QBrush(Qt::red));
        painter.setBrush(QBrush(Qt::red));
        painter.drawEllipse(x,y,5,5);
    }

    painter.setPen(QPen(Qt::red,1));
    painter.drawPolyline(poly);;
    painter.end();
}

void Widget::drawPopulation()
{
    painter.begin(&image);
    float avr = 0;
    for (int i=0; i<population.size(); ++i) {
        float x = population[i].x()*kX;
        float y = height() - population[i].y()*kY;

//        painter.fillRect(x-5,y-5,10,10, QBrush(Qt::green));
        painter.setBrush(QBrush(Qt::green));
        painter.drawEllipse(x,y,10,10);

        avr += population[i].x();
    }
    avr /= population.size();

    painter.fillRect(avr*kX,0, 1, height(), QBrush(Qt::black));

    painter.end();

    modify = false;
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (modify) {
        drawCurve();
        drawPopulation();
    }

    painter.begin(this);
    painter.drawImage(0,0,image);
    painter.end();
}

void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (!curve.isEmpty() && !population.isEmpty())
        slotUpdate();
}

void Widget::calculateCoefficients()
{
    N = curve.size();

    float maxValue;
    float minValue;

    for (int x=0; x<N; ++x) {
        float y = curve.at(x);

        if (!x) {
            maxValue = y;
            minValue = y;
        }
        else if( maxValue < y ) {
            maxValue = y;
        }
        else if( minValue > y ) {
            minValue = y;
        }
    }

    kX = width() / N;
    kY = height() / (qAbs(maxValue)+qAbs(minValue)) ;
}

