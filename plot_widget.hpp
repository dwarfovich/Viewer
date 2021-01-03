#ifndef PLOTWIDGET_HPP
#define PLOTWIDGET_HPP

#include <QWidget>

class PlotDrawer;
class PreviewPlotFrameItem;

class PlotWidget : public QWidget
{
    Q_OBJECT

public:
    PlotWidget(PlotDrawer &drawer, QWidget* parent = nullptr);

    void drawArea(const PreviewPlotFrameItem& frame_item);

signals:
    void scaleChangeRequest(qreal angle_delta);
    void horizontalRangeChanged(const std::pair<qreal, qreal>& range);
    void verticalRangeChanged(const std::pair<qreal, qreal>& range);
    void selectedPointChanged(size_t point, const QPointF& value);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    std::pair<size_t, size_t> indicesOfPoints(qreal first_x, qreal last_x) const;

private:
    PlotDrawer& drawer_;
    QPixmap plot_;
};

#endif // PLOTWIDGET_HPP
