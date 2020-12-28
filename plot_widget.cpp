#include "plot_widget.hpp"

#include <QPainter>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QWheelEvent>

#include <QDebug>
#define DEB qDebug()

PlotWidget::PlotWidget(PlotDrawer &drawer, QWidget *parent)
    : QWidget{parent}
    , drawer_{drawer}
{
    setLayout(new QHBoxLayout {this});
}

void PlotWidget::setPlot(const QPixmap &plot)
{
    plot_ = plot;
    update();
}

void PlotWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter {this};
    painter.drawPixmap(QPoint{0, 0}, plot_);
}

void PlotWidget::drawArea(int first, int last)
{
//    drawer_.drawPlot(first, last, this->width(), this->height());
    drawer_.drawMainPlot(first, last, width(), height());
    plot_ = drawer_.plot();

    update();
}

void PlotWidget::wheelEvent(QWheelEvent *event)
{
    emit scaleChangeRequest(event->angleDelta().y());

    event->accept();
}
