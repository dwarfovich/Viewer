#ifndef PLOTWIDGET_HPP
#define PLOTWIDGET_HPP

#include "plot_drawer.hpp"

#include <QWidget>

#include <QVBoxLayout>
#include <QScrollArea>

#include "measurement.hpp"

class PlotWidget : public QWidget
{
    Q_OBJECT

public:
    PlotWidget(PlotDrawer &drawer, QWidget* parent = nullptr);

    void drawArea(int first_point, int last_point);

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
    PlotDrawer& drawer_;
    QPixmap plot_;
};

#endif // PLOTWIDGET_HPP
