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
    PlotWidget(const Measurement& m, QWidget* parent = nullptr);

    QPixmap plot() const;
    void setPlot(const QPixmap &plot);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap plot_;
    PlotDrawer drawer_;
    const Measurement& m_;
    qreal x_scale = 1.0;

    // QWidget interface
public:
    QSize sizeHint() const override;
};

#endif // PLOTWIDGET_HPP
