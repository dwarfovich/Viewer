#include "preview_plot_scene.hpp"
#include "plot_drawer.hpp"
#include "preview_plot_frame_item.hpp"
#include "measurement.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>
#define DEB qDebug()

PreviewPlotScene::PreviewPlotScene(PlotDrawer &drawer, QObject* parent)
    : QGraphicsScene{parent}
    , drawer_{drawer}
    , preview_item_{new QGraphicsPixmapItem {}}
    , frame_item_{new PreviewPlotFrameItem {}}
{
    preview_item_->setZValue(1);
    frame_item_->setZValue(2);
    addItem(preview_item_);
    addItem(frame_item_);
    connect(frame_item_, &PreviewPlotFrameItem::posChanged, this, &PreviewPlotScene::frameItemPosChanged);
}

PreviewPlotFrameItem *PreviewPlotScene::frameItem() const
{
    return frame_item_;
}

void PreviewPlotScene::updatePreview(int width, int height)
{
    drawer_.drawPreview(width, height);
    preview_item_->setPixmap(drawer_.plotPreview());
}

void PreviewPlotScene::resetFrameItem(int height, int expected_scene_width)
{
    frame_item_->setHeight(height);

    min_frame_width_ = minFrameWidth(expected_scene_width);
    max_frame_width_ = maxFrameWidth(expected_scene_width);
    normal_width_ = (max_frame_width_ + min_frame_width_) / 2.;
    frame_item_->setWidth(normal_width_);
    frame_item_->setX(0);
}

void PreviewPlotScene::onPlotScaleRequest(qreal angle_delta)
{
    qreal new_width = 0.;
    if (angle_delta > 0.) {
        new_width = std::min(frame_item_->width() + width_delta_, max_frame_width_);
    } else {
        new_width = std::max(frame_item_->width() - width_delta_, min_frame_width_);
    }
    frame_item_->setWidth(new_width);
    qreal offset = width() - (frame_item_->x() + new_width);
    if (offset < 0) {
        frame_item_->setX(frame_item_->x() + offset);
    }
    update();
    emit frameItemChanged(frame_item_->boundingRect());
}

qreal PreviewPlotScene::minFrameWidth(int expected_scene_width) const
{
    Q_UNUSED(expected_scene_width);

    const auto& data = drawer_.measurement()->data;
    qreal min_width = std::max(min_frame_points_ * 100. / data.size(), 1.);
    return min_width;
}

qreal PreviewPlotScene::maxFrameWidth(int expected_scene_width) const
{
    const auto& data = drawer_.measurement()->data;
    qreal max_width = std::min<qreal>(max_frame_points_ * 100. / data.size(), expected_scene_width);
    return max_width;
}

qreal PreviewPlotScene::xPercentAtPoint(size_t point) const
{
    const auto& measurement = *drawer_.measurement();
    const auto& data = measurement.data;
    qreal point_x = data[point].x();
    qreal x_range = measurement.stats.max_x - measurement.stats.min_x;
    qreal point_percent = (point_x - measurement.stats.min_x) * 100. / x_range;

    return point_percent;
}


void PreviewPlotScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (!frame_item_->mapToScene(frame_item_->boundingRect()).containsPoint(event->scenePos(), Qt::OddEvenFill)) {
        frame_item_->setPos(event->scenePos());
    } else {
        QGraphicsScene::mousePressEvent(event);
    }
}
