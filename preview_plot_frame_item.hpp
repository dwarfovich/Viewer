#ifndef PREVIEWPLOTFRAMEITEM_HPP
#define PREVIEWPLOTFRAMEITEM_HPP

#include <QGraphicsObject>
#include <QBrush>

class PreviewPlotFrameItem : public QGraphicsObject
{
    Q_OBJECT

public:
    enum { Type = UserType + 1 };

    PreviewPlotFrameItem(QGraphicsItem* parent = nullptr);

    int type() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int width() const;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);

signals:
    void posChanged(const QPointF& new_pos);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    int width_ = 100;
    int height_ = 100;
    QBrush brush_;
    const QColor brush_color_ = {128, 128, 128, 128};
};

#endif // PREVIEWPLOTFRAMEITEM_HPP
