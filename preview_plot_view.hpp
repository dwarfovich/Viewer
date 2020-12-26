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

protected:
    void mousePressEvent   (QMouseEvent *event) override;
    void mouseMoveEvent    (QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QPointF click_location_ = {};
    bool is_scrolling_ = false;
};

#endif // PREVIEWPLOTVIEW_HPP
