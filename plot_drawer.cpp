#include "plot_drawer.hpp"
#include "measurement.hpp"

#include <QPainter>

#include <QDebug>
#define DEB qDebug()

PlotDrawer::PlotDrawer(const Measurement *measurement)
    : measurement_{measurement}
{}

void PlotDrawer::drawPlot(const PlotParameters& parameters)
{
    QPixmap& pixmap = parameters.pixmap;
    pixmap = QPixmap{parameters.width, parameters.height};
    QPainter painter {&pixmap};
    pixmap.fill(parameters.background_color);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(parameters.pen);

    const auto& data = measurement_->data;
    const auto& rarefaction = parameters.rarefaction;
    const auto& first = parameters.first_point;
    const auto& last = parameters.last_point;
    const auto& delta_x = parameters.max_values.x() - parameters.min_values.x();
    const auto& delta_y = parameters.max_values.y() - parameters.min_values.y();
    const auto& x_coefficient = parameters.width / delta_x;
    const auto& y_coefficient = parameters.height / delta_y;
    int count = 0;
    for (size_t i = first + rarefaction; i <= last; i += rarefaction) {
        QPointF point1;
        point1.setX(x_coefficient * (data[i - rarefaction].x() - parameters.min_values.x()));
        point1.setY(y_coefficient * (data[i - rarefaction].y() - parameters.min_values.y()));
        QPointF point2;
//        //         TODO: Инвертировать значения по y
        point2.setX(x_coefficient * (data[i].x() - parameters.min_values.x()));
        point2.setY(y_coefficient * (data[i].y() - parameters.min_values.y()));
        painter.drawLine(point1, point2);
        ++count;
    }
//    if (&parameters.pixmap == &plot_preview_) {
//        DEB << "Plot preview:";
//        DEB << "Rarefaction:" << parameters.rarefaction;
//        DEB << "Points drawn:" << count;
//    } else {
//        DEB << "Plot main:";
//        DEB << "Rarefaction:" << parameters.rarefaction;
//        DEB << "Points drawn:" << count;
//    }
}

void PlotDrawer::drawPreview(int width, int height)
{
    PlotParameters parameters {plot_preview_};
    parameters.first_point = 0;
    parameters.last_point = measurement_->data.size() - 1;
    parameters.width = width;
    parameters.height = height;

    parameters.rarefaction = calculatePreviewRarefaction();
    const auto& stats = measurement_->stats;
    parameters.min_values = {stats.min_x, stats.min_y};
    parameters.max_values = {stats.max_x, stats.max_y};

    drawPlot(parameters);
}

void PlotDrawer::drawMainPlot(size_t first_point, size_t last_point, int width, int height)
{
    PlotParameters parameters {plot_};
    const auto& data = measurement_->data;
    parameters.first_point = first_point;
    parameters.last_point = last_point;
    parameters.width = width;
    parameters.height = height;
    parameters.rarefaction = calculateMainPlotRarefaction(last_point - first_point);
    auto min_max_y = std::minmax_element(data.begin() + parameters.first_point, data.begin() + parameters.last_point,
                                         [](const QPointF p1, const QPointF& p2){ return p1.y() < p2.y();});
    parameters.min_values = {data[first_point].x(), min_max_y.first->y()};
    parameters.max_values = {data[last_point].x(), min_max_y.second->y()};

    drawPlot(parameters);
}

const QPixmap &PlotDrawer::plot() const
{
    return plot_;
}

const QPixmap& PlotDrawer::plotPreview() const
{
    return plot_preview_;
}

void PlotDrawer::setMeasurement(const Measurement *measurement)
{
    measurement_ = measurement;
}

const Measurement* PlotDrawer::measurement() const
{
    return measurement_;
}

size_t PlotDrawer::calculatePreviewRarefaction() const
{
    const auto& data = measurement_->data;
    if (data.size() < size_without_rarefaction) {
        return 1;
    } else {
        return (data.size() / size_without_rarefaction);
    }
}

size_t PlotDrawer::calculateMainPlotRarefaction(size_t points) const
{
    if (points < size_without_rarefaction) {
        return 1;
    } else {
        return (points / size_without_rarefaction);
    }
}
