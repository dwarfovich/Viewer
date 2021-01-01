#include "preview_plot_view.hpp"

#include <QResizeEvent>
#include <QGraphicsScene>

PreviewPlotView::PreviewPlotView(QWidget *parent)
    : QGraphicsView{parent}
{}

void PreviewPlotView::resizeEvent(QResizeEvent *event)
{
    emit sizeChanged(event->size());
    fitInView(scene()->sceneRect());
}

void PreviewPlotView::wheelEvent(QWheelEvent *event)
{
    emit scaleChangeRequest(event->angleDelta().y());

    event->accept();
}
