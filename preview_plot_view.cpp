#include "preview_plot_view.hpp"

#include <QMouseEvent>

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
        scene_center_ = mapToScene(viewport()->rect().center());
//        DEB << "Click location" << click_location_;
    }
    QGraphicsView::mousePressEvent(event);
}

void PreviewPlotView::mouseMoveEvent(QMouseEvent *event)
{
    if (is_scrolling_) {
//        centerOn(scene_center_ - event->pos() + click_location_);
        auto new_pos = scene_center_ - event->pos() + click_location_;
        new_pos = event->pos() - click_location_;
//        click_location_ = new_pos;
        emit frameItemPosChanged(new_pos);
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
