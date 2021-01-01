#include "plot_widget.hpp"
#include "measurement.hpp"
#include "plot_drawer.hpp"

#include <QPainter>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QMouseEvent>

PlotWidget::PlotWidget(PlotDrawer &drawer, QWidget *parent)
    : QWidget{parent}
    , drawer_{drawer}
{
    setLayout(new QHBoxLayout {this});
    setMouseTracking(true);
}

void PlotWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter {this};
    painter.drawPixmap(QPoint{0, 0}, plot_);
}

void PlotWidget::drawArea(size_t first_point, size_t last_point)
{
    drawer_.drawMainPlot(first_point, last_point, width(), height());
    plot_ = drawer_.plot();
    update();
    emit horizontalRangeChanged(drawer_.rangeX());
    emit verticalRangeChanged(drawer_.rangeY());
}

void PlotWidget::wheelEvent(QWheelEvent *event)
{
    emit scaleChangeRequest(event->angleDelta().y());

    event->accept();
}

void PlotWidget::mouseMoveEvent(QMouseEvent* event)
{
    const auto& points_range = drawer_.mainPlotPointsRange();
    qreal current_x_percent = qreal(event->pos().x()) * 100. / qreal(width());
    size_t current_point = (points_range.second - points_range.first) * current_x_percent / 100. + points_range.first;
    const auto& data = drawer_.measurement()->data;
    if (current_point < data.size()) {
        emit selectedPointChanged(current_point, data[current_point]);
    }
}
