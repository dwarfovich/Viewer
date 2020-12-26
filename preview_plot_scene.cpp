#include "preview_plot_scene.hpp"
#include "plot_drawer.hpp"
#include "preview_plot_frame_item.hpp"

#include <QGraphicsPixmapItem>

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
