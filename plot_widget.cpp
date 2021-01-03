#include "plot_widget.hpp"
#include "measurement.hpp"
#include "plot_drawer.hpp"
#include "preview_plot_frame_item.hpp"

#include <QPainter>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QMouseEvent>

#include <QDebug>
#define DEB qDebug()

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

void PlotWidget::drawArea(const PreviewPlotFrameItem& frame_item)
{
    qreal first_scene_x = frame_item.firstSceneX();
    qreal last_scene_x = frame_item.lastSceneX();
    const auto& measurement = *drawer_.measurement();
    qreal x_range = measurement.stats.max_x - measurement.stats.min_x;
    qreal first_x = (x_range * first_scene_x) / 100. + measurement.stats.min_x;
    qreal last_x = (x_range * last_scene_x) / 100. + measurement.stats.min_x;
    auto [first_index, last_index] = indicesOfPoints(first_x, last_x);
    drawer_.drawMainPlot(first_index, last_index, width(), height(), frame_item);
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

std::pair<size_t, size_t> PlotWidget::indicesOfPoints(qreal first_x, qreal last_x) const
{
    const auto& measurement = *drawer_.measurement();

    struct QPointFComparator
    {
        bool operator() (const QPointF& left, const QPointF& right) {
            return left.x() < right.x();
        }
        bool operator() (const QPointF& left, qreal right) {
            return left.x() < right;
        }
        bool operator() (qreal left, const QPointF& right) {
            return left < right.x();
        }
    };

    const auto& data = measurement.data;
    auto first_iter = std::lower_bound(data.cbegin(), data.cend(), first_x, QPointFComparator());
    if (first_iter == data.cend()) {
        first_iter = data.cend() - 1;
    }
    auto last_iter = std::lower_bound(data.cbegin(), data.cend(), last_x, QPointFComparator());
    if (last_iter == data.cend()) {
        last_iter = data.cend() - 1;
    }
    size_t first_point = std::distance(data.cbegin(), first_iter);
    size_t last_point = std::distance(data.cbegin(), last_iter);

    if (last_point == 0) {
        last_point = measurement.data.size() - 1;
    }
    if (first_point == last_point && first_point != 0) {
        --first_point;
        if (last_point < measurement.data.size() - 1) {
            ++last_point;
        }
    }

    return {first_point, last_point};
}
