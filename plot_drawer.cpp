#include "plot_drawer.hpp"
#include "measurement.hpp"

#include <QPainter>
#include <QPen>

#include <QDebug>
#define DEB qDebug()

PlotDrawer::PlotDrawer(const Measurement &measurement, const QRectF &size)
    : measurement_{measurement}
    , size_({size})
{}

void PlotDrawer::draw(size_t first, size_t last, int width, int height)
{
    const auto& data = measurement_.data;
    const auto& stats = measurement_.stats;

    plot_ = QPixmap{int(size_.width()), int(size_.height())};
    plot_.fill(Qt::white);
    QPainter painter {&plot_};
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen {Qt::black};
    pen.setWidth(5);
    painter.setPen(pen);

    qreal ax = size_.width();
    qreal ay = size_.height();
    qreal delta_x = stats.max_x - stats.min_x;
    qreal delta_y = stats.max_y - stats.min_y;
    for (size_t i = 1; i < data.size(); ++i) {
        QPointF p1;
        p1.setX(ax * (data[i - 1].x() - stats.min_x) / delta_x);
        p1.setY(ay * (data[i - 1].y() - stats.min_y) / delta_y);
        QPointF p2;
        // TODO: Инвертировать значения по y
        p2.setX(ax * (data[i].x() - stats.min_x) / delta_x);
        p2.setY(ay * (data[i].y() - stats.min_y) / delta_y);
        painter.drawLine(p1, p2);
//        DEB << p1 << p2;
    }

    painter.drawRect(0, 0, ax, ay);
}

void PlotDrawer::generatePreview(int width, int height)
{
    const auto& data = measurement_.data;
    const auto& stats = measurement_.stats;

    plot_preview_ = QPixmap{width, height};
    QPainter painter {&plot_preview_};
    plot_preview_.fill(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen {Qt::green};
    pen.setWidth(1);
    painter.setPen(pen);

    qreal delta_x = stats.max_x - stats.min_x;
    qreal delta_y = stats.max_y - stats.min_y;
    size_t estimated_rarefaction = std::max(size_t(1), data.size() / width);
    size_t rarefaction = std::min(default_rarefraction_, estimated_rarefaction);
    rarefaction = 1;
    for (size_t i = rarefaction; i < data.size(); i += rarefaction) {
        QPointF point1;
        point1.setX(width  * (data[i - rarefaction].x() - stats.min_x) / delta_x);
        point1.setY(height * (data[i - rarefaction].y() - stats.min_y) / delta_y);
        QPointF point2;
//         TODO: Инвертировать значения по y
        point2.setX(width  * (data[i].x() - stats.min_x) / delta_x);
        point2.setY(height * (data[i].y() - stats.min_y) / delta_y);
        painter.drawLine(point1, point2);
//        DEB << p1 << p2;
    }

    painter.drawRect(0, 0, width, height);
}

void PlotDrawer::generatePlotArea(int first, int last, int width, int height)
{
    const auto& data = measurement_.data;
    const auto& stats = measurement_.stats;

    int first_point = (data.size() * first) / 100;
    if (first_point == 0) {
        ++first_point;
    }
    int last_point = (data.size() * last) / 100;
    if (last_point >= data.size()) {
        last_point = data.size() - 1;
    }
    plot_ = QPixmap{width, height};
    QPainter painter {&plot_};
    plot_.fill(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen {Qt::green};
    painter.setPen(pen);

    auto min_max_y = std::minmax_element(data.begin() + first_point, data.begin() + last_point,
                                         [](const QPointF p1, const QPointF& p2){ return p1.y() < p2.y();});
    qreal delta_x = stats.max_x - stats.min_x;
    qreal delta_y = stats.max_y - stats.min_y;
    delta_x = data[last_point].x() - data[first_point].x();
    delta_y = min_max_y.second->y() - min_max_y.first->y();
    for (size_t i = first_point; i <= last_point; i += 1) {
        QPointF point1;
        point1.setX(width  * (data[i - 1].x() - data[first_point].x()) / delta_x);
        point1.setY(height * (data[i - 1].y() - min_max_y.first->y()) / delta_y);
        QPointF point2;
//         TODO: Инвертировать значения по y
        point2.setX(width  * (data[i].x() - data[first_point].x()) / delta_x);
        point2.setY(height * (data[i].y() - min_max_y.first->y()) / delta_y);
        painter.drawLine(point1, point2);
    }
}

const QPixmap &PlotDrawer::plot() const
{
    return plot_;
}

const QPixmap& PlotDrawer::plot_preview() const
{
    return plot_preview_;
}
