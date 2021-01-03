#include "plot_drawer.hpp"
#include "measurement.hpp"
#include "preview_plot_frame_item.hpp"

#include <QGraphicsScene>
#include <QPainter>

PlotDrawer::PlotDrawer(const Measurement *measurement)
    : measurement_{measurement}
{}

void PlotDrawer::drawPlotNew(const PlotParameters& parameters)
{
    QPixmap& pixmap = parameters.pixmap;
    pixmap = QPixmap{parameters.width, parameters.height};
    QPainter painter {&pixmap};
    pixmap.fill(parameters.background_color);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(parameters.pen);

    const auto data = measurement_->data;
    const auto rarefaction = parameters.rarefaction;
    const auto first_point = parameters.first_point;
    const auto last_point = parameters.last_point;
    const auto delta_x = parameters.max_values.x() - parameters.min_values.x();
    const auto delta_y = (parameters.max_values.y() - parameters.min_values.y() != 0
            ? parameters.max_values.y() - parameters.min_values.y()
            : data[last_point].y() - data[first_point].y());
    const auto x_coefficient = parameters.width / delta_x;
    const auto y_coefficient = parameters.height / delta_y;

    size_t last_point_painted = 0;
    for (size_t i = first_point + rarefaction; i <= last_point; i += rarefaction) {
        QPointF point1;
        if (i - rarefaction == first_point) {
            point1.setX(0.);
            point1.setY(parameters.height - (y_coefficient * (data[i - rarefaction].y() - parameters.min_values.y())));
        } else {
            point1.setX(x_coefficient * (data[i - rarefaction].x() - parameters.min_values.x()));
            point1.setY(parameters.height - (y_coefficient * (data[i - rarefaction].y() - parameters.min_values.y())));
        }

        QPointF point2;
        if (i == last_point) {
            point2.setX(x_coefficient * delta_x);
            point2.setY(parameters.height - (y_coefficient * (data[i].y() - parameters.min_values.y())));
        } else {
            point2.setX(x_coefficient * (data[i].x() - parameters.min_values.x()));
            point2.setY(parameters.height - (y_coefficient * (data[i].y() - parameters.min_values.y())));
        }

        painter.drawLine(point1, point2);

        last_point_painted = i;
    }

    if (last_point_painted != last_point) {
        QPointF point1;
        point1.setX(x_coefficient * (data[last_point - rarefaction].x() - parameters.min_values.x()));
        point1.setY(parameters.height - (y_coefficient * (data[last_point - rarefaction].y() - parameters.min_values.y())));
        QPointF point2;
        point2.setX(x_coefficient * (data[last_point].x() - parameters.min_values.x()));
        point2.setY(parameters.height - (y_coefficient * (data[last_point].y() - parameters.min_values.y())));
        painter.drawLine(point1, point2);
    }
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

    drawPlotNew(parameters);
}

void PlotDrawer::drawMainPlot(size_t first_point, size_t last_point, int width, int height, const PreviewPlotFrameItem& item)
{
    PlotParameters parameters {plot_};
    const auto& data = measurement_->data;
    parameters.first_point = first_point;
    parameters.last_point = last_point;
    parameters.width = width;
    parameters.height = height;
    parameters.rarefaction = calculateMainPlotRarefaction(last_point - first_point);
    main_plot_points_range = {first_point, last_point};

    qreal preview_width = item.scene()->width();
    qreal first_percent = (item.pos().x() * 100.) / preview_width;
    const auto& stats = measurement_->stats;
    qreal stats_range_x = stats.max_x - stats.min_x;
    qreal first_x = stats_range_x * first_percent / 100. + stats.min_x;
    qreal last_percent = ((item.pos().x() + item.width()) * 100.) / preview_width;
    qreal last_x = stats_range_x * last_percent / 100. + stats.min_x;
    auto min_max_y = std::minmax_element(data.begin() + parameters.first_point, data.begin() + parameters.last_point,
                                         [](const QPointF p1, const QPointF& p2){ return p1.y() < p2.y();});
    parameters.min_values = {first_x, min_max_y.first->y()};
    parameters.max_values = {last_x, min_max_y.second->y()};
    range_x = {first_x, last_x};
    range_y = {min_max_y.first->y(), min_max_y.second->y()};
    drawPlotNew(parameters);
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

const std::pair<size_t, size_t>& PlotDrawer::mainPlotPointsRange() const
{
    return main_plot_points_range;
}

const std::pair<qreal, qreal>& PlotDrawer::rangeY() const
{
    return range_y;
}

const std::pair<qreal, qreal>& PlotDrawer::rangeX() const
{
    return range_x;
}
