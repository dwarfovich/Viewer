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

void PlotDrawer::draw()
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
    plot_preview_ = plot_.scaled(width, height);
}

const QPixmap &PlotDrawer::plot() const
{
    return plot_;
}

const QPixmap& PlotDrawer::plot_preview() const
{
    return plot_preview_;
}
