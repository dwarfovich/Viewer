#include "plot_widget.hpp"

#include <QPainter>
#include <QHBoxLayout>

PlotWidget::PlotWidget(const Measurement &m, QWidget *parent)
    : QWidget{parent}
    , drawer_{m}
    , m_{m}
{
    setLayout(new QHBoxLayout {this});
}

QPixmap PlotWidget::plot() const
{
    return plot_;
}

void PlotWidget::setPlot(const QPixmap &plot)
{
    plot_ = plot;
    update();
}

void PlotWidget::paintEvent(QPaintEvent *event)
{
    QPainter p {this};
    p.drawPixmap(QPoint{0, 0}, plot_);
}

QSize PlotWidget::sizeHint() const
{
    return {1920, 1080};
}
