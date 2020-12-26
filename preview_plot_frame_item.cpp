#include "preview_plot_frame_item.hpp"

#include <QPainter>
#include <QBrush>

PreviewPlotFrameItem::PreviewPlotFrameItem(QGraphicsItem *parent)
    : QGraphicsItem{parent}
{

}


QRectF PreviewPlotFrameItem::boundingRect() const
{
    return {0., 0., qreal(width_), qreal(height_)};
}

void PreviewPlotFrameItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor({128, 128, 128, 128});
    painter->fillRect(0, 0, width_, height_, brush);
}

int PreviewPlotFrameItem::width() const
{
    return width_;
}

void PreviewPlotFrameItem::setWidth(int width)
{
    width_ = width;
}

int PreviewPlotFrameItem::height() const
{
    return height_;
}

void PreviewPlotFrameItem::setHeight(int height)
{
    height_ = height;
}
