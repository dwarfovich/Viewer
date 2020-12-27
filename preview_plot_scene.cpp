#include "preview_plot_scene.hpp"
#include "plot_drawer.hpp"
#include "preview_plot_frame_item.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneWheelEvent>

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
    drawer_.generatePreview(width, height);
    preview_item_->setPixmap(drawer_.plotPreview());
    update();
}

void PreviewPlotScene::onPlotScaleRequest(qreal angle_data)
{
    qreal new_scale = 0.;
    if (angle_data > 0.) {
        new_scale = std::min(x_scale + scale_delta, max_scale);
    } else {
        new_scale = std::max(x_scale - scale_delta, min_scale);
    }

    qreal default_width = 500;
    qreal width = frame_item_->width();
    width = default_width * new_scale;
    frame_item_->setWidth(width);
    x_scale = new_scale;
    update();
    emit frameItemChanged(frame_item_->boundingRect());
}
