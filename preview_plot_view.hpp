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
    void sizeChanged(const QSize& new_size);
    void scaleChangeRequest(qreal angle_delta);

protected:
    void mousePressEvent   (QMouseEvent *event) override;
    void mouseMoveEvent    (QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QPointF click_location_ = {};
    bool is_scrolling_ = false;
//    qreal x_scale = 1.0;
//    const qreal scale_delta = 0.1;
//    const qreal min_scale = 0.1;
//    const qreal max_scale = 1.5;
};

#endif // PREVIEWPLOTVIEW_HPP
