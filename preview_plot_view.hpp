#ifndef PREVIEWPLOTVIEW_HPP
#define PREVIEWPLOTVIEW_HPP

#include <QGraphicsView>

class PreviewPlotView : public QGraphicsView
{
    Q_OBJECT

public:
    PreviewPlotView(QWidget* parent = nullptr);

signals:
    void frameItemPosChanged(const QPointF delta_pos);

protected:
    void mousePressEvent   (QMouseEvent *event);
    void mouseMoveEvent    (QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent *event);

private:
    QPointF click_location_ = {};
    bool is_scrolling_ = false;
};

#endif // PREVIEWPLOTVIEW_HPP
