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

    void setPlot(const QPixmap &plot);
    void drawArea(int first_point, int last_point);

signals:
    void scaleChangeRequest(qreal angle_delta);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    PlotDrawer& drawer_;
    QPixmap plot_;
};

#endif // PLOTWIDGET_HPP
