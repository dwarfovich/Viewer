#include "preview_plot_view.hpp"

#include <QMouseEvent>
#include <QResizeEvent>

#include <QDebug>
#define DEB qDebug()

PreviewPlotView::PreviewPlotView(QWidget *parent)
    : QGraphicsView{parent}
{}

void PreviewPlotView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        is_scrolling_ = true;
        click_location_ = event->pos();
    }
    QGraphicsView::mousePressEvent(event);
}

void PreviewPlotView::mouseMoveEvent(QMouseEvent *event)
{
    if (is_scrolling_) {
        emit frameItemPosChanged(event->pos() - click_location_);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void PreviewPlotView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton){
        is_scrolling_ = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void PreviewPlotView::resizeEvent(QResizeEvent *event)
{
    emit sizeChanged(event->size());
}
