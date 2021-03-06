#include "preview_plot_frame_item.hpp"

#include <QPainter>
#include <QBrush>
#include <QGraphicsScene>

PreviewPlotFrameItem::PreviewPlotFrameItem(QGraphicsItem *parent)
    : QGraphicsObject{parent}
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    brush_.setStyle(Qt::SolidPattern);
    brush_.setColor(brush_color_);
}

int PreviewPlotFrameItem::type() const
{
    return Type;
}

QRectF PreviewPlotFrameItem::boundingRect() const
{
    return {0., 0., qreal(width_), qreal(height_)};
}

void PreviewPlotFrameItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->fillRect(0, 0, width_, height_, brush_);
}

qreal PreviewPlotFrameItem::firstSceneX() const
{
    qreal preview_width = scene()->width();
    qreal first_scene_x = (x() * 100.) / preview_width;

    return first_scene_x;
}

qreal PreviewPlotFrameItem::lastSceneX() const
{
    qreal preview_width = scene()->width();
    qreal last_scene_x = ((x() + width()) * 100.) / preview_width;

    return last_scene_x;
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

QVariant PreviewPlotFrameItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange) {
        const auto& new_pos = value.toPointF();
        const auto& scene_rect = scene()->sceneRect();
        if (new_pos.x() < 0.) {
            return QPointF{0., pos().y()};
        } else {
            return QPointF{std::min(new_pos.x(), scene_rect.right() - width()), pos().y()};
        }
    } else if (change == ItemPositionHasChanged) {
        emit posChanged(pos());
    }
    return QGraphicsItem::itemChange(change, value);
}
