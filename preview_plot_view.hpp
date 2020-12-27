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
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // PREVIEWPLOTVIEW_HPP
