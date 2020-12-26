#include "plot_widget.hpp"

#include <QPainter>
#include <QHBoxLayout>

#include <QDebug>
#define DEB qDebug()

PlotWidget::PlotWidget(const Measurement &m, QWidget *parent)
    : QWidget{parent}
    , drawer_{m}
    , m_{m}
{
    setLayout(new QHBoxLayout {this});
}

QPixmap PlotWidget::plot() const
{
    return plot_;
}

void PlotWidget::setPlot(const QPixmap &plot)
{
    plot_ = plot;
    update();
}

void PlotWidget::paintEvent(QPaintEvent *event)
{
    QPainter p {this};
//    p.drawRect(0, 0, 100,100);
    p.drawPixmap(QPoint{0, 0}, plot_);
}

QSize PlotWidget::sizeHint() const
{
    return {200, 200};
}

void PlotWidget::drawArea(int first, int last, int height)
{
//    DEB << "plot widget width:" << this->width();
    drawer_.generatePlotArea(first, last, this->width(), this->height());
    plot_ = drawer_.plot();

    update();
}
