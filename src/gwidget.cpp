#include "gwidget.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSplitter>

#include <cmath>

GWidget::GWidget(QWidget *parent) :
    QWidget(parent)
{
    QPushButton *startButton = new QPushButton("start");
    connect(startButton, SIGNAL(clicked()), this, SLOT(slotStart()));

    QPushButton *stopButton = new QPushButton("stop");
    connect(stopButton, SIGNAL(clicked()), this, SLOT(slotStop()));

    connect(&timer, SIGNAL(timeout()), this, SLOT(evalute()));

    delay = new QSpinBox;
    delay->setRange(1,1000);

    iterationLabel.setText("Iterations : 0");

    plus = 0;
    minus = 0;
    statisticLabel.setText(tr("Positive: <H2><font color=green>%1</font></H2> Negative: <H2><font color=red>%2</font></H2>").arg(plus).arg(minus));

    QHBoxLayout *hl = new QHBoxLayout();
    hl->addWidget(new QLabel("Delay: "));
    hl->addWidget(delay);
    hl->addWidget(startButton);
    hl->addWidget(stopButton);
    hl->addWidget(&iterationLabel);
    hl->addWidget(&statisticLabel);

    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(&w);

    QGroupBox *plotBox = new QGroupBox("Plot");
    plotBox->setLayout(vl);

    QGroupBox *statisticBox = new QGroupBox("Managment and statistic");
    statisticBox->setLayout(hl);

    QVBoxLayout *l = new QVBoxLayout;

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(plotBox);
    splitter->addWidget(statisticBox);
    l->addWidget(splitter);

    setLayout(l);
}

void GWidget::slotStop()
{
    stop = true;
}

void GWidget::slotStart()
{
    stop = false;

    curve.clear();
    population.clear();

    iterations = 0;

    N = 100;

    for (int i=0; i<N; ++i) {
        curve.append(Fx(i));
    }

    for (int i=0; i<8; ++i) {
        float x = rand()%N;
        float y = Fx(x);

        population.append(QPointF(x,y));
    }

    w.setCurve(curve);
    w.setPopulation(population);

    timer.stop();
    timer.start(delay->value());
}

float GWidget::Fx(int x)
{
    return qAbs(x*x-1000);
}

void GWidget::evalute()
{
    ++iterations;

    // Find the best
    float minX = 0;
    float minY = population[0].y();
    for (int i=0; i<population.size(); ++i) {
        if (minY > population[i].y()) {
            minX = i;
            minY = population[i].y();
        }
    }

    // Only the best have the right to mate with the other and produce offspring
    int oldSize = population.size();
    for (int i=0; i<population.size(); ++i) {
        if (i==minX) {
            continue;
        }
        if (rand()%51 == 13) {
            float x = population[i].x() + minX;
            x /= 2;
            float y = Fx(x);
            population.append(QPointF(x,y));
        }
    }
    int count = population.size() - oldSize;

    // Not all survived ... :(
    while (count--) {
        population.remove(rand()%oldSize);
    }

    // And now mutate ...
    for (int i=0; i<population.size(); ++i) {
        float x = population[i].x();
        if (x<0) { x = 0; }
        if (x>N-1) { x = N-1;}

        if (i%3 == 0) {
            population[i].setX(x+=rand()%5);
        }
        else {
            population[i].setX(x-=rand()%5);
        }

        population[i].setY(Fx(population[i].x()));
    }

    // check our goal ...
    int avrIndex=0;
    for (int i=0; i<population.size(); ++i) {
        avrIndex += population[i].x();
    }
    avrIndex /= population.size();

    if (avrIndex < 1) { avrIndex = 1; }
    if (avrIndex > N-2){ avrIndex = N-2; }

    float left = Fx(avrIndex-1);
    float right= Fx(avrIndex+1);
    float center=Fx(avrIndex);

    if ((center <= left) && (center <= right)) {
        stop = true;
    }

    iterationLabel.setText(tr("Iterations : %1").arg(iterations));

    w.setPopulation(population);
    if (stop == true) {

        if (iterations <= N) { plus++; }
        else { minus++; }

        statisticLabel.setText(tr("Positive: <H2><font color=green>%1</font></H2> Negative: <H2><font color=red>%2</font></H2>").arg(plus).arg(minus));

        timer.stop();
    }
}
