#ifndef PREVIEWPLOTFRAMEITEM_HPP
#define PREVIEWPLOTFRAMEITEM_HPP

#include <QGraphicsItem>

class PreviewPlotFrameItem : public QGraphicsItem
{
public:
    PreviewPlotFrameItem(QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int width() const;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);

private:
    int width_ = 0;
    int height_ = 0;
};

#endif // PREVIEWPLOTFRAMEITEM_HPP
