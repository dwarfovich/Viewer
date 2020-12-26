#include "plot_widget.hpp"

#include <QPainter>
#include <QHBoxLayout>

#include <QDebug>
#define DEB qDebug()

PlotWidget::PlotWidget(PlotDrawer &drawer, QWidget *parent)
    : QWidget{parent}
    , drawer_{drawer}
//    , m_{drawer}
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
    p.drawPixmap(QPoint{0, 0}, plot_);
}

QSize PlotWidget::sizeHint() const
{
    return {200, 200};
}

void PlotWidget::drawArea(int first, int last, int height)
{
    drawer_.generatePlotArea(first, last, this->width(), this->height());
    plot_ = drawer_.plot();

    update();
}
