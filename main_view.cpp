#include "main_view.hpp"

#include <QMouseEvent>

MainView::MainView(QWidget *parent)
    : QGraphicsView{parent}
    , click_location_{}
    , scene_center_{}
    , is_scrolling_{false}
{
    this->setTransformationAnchor (QGraphicsView::AnchorUnderMouse);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

void MainView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        is_scrolling_ = true;
        click_location_ = event->pos();
        scene_center_ = mapToScene(viewport()->rect().center());
    }
    QGraphicsView::mousePressEvent(event);
}

void MainView::mouseMoveEvent(QMouseEvent *event)
{
    if (is_scrolling_) {
        centerOn(scene_center_ - event->pos() + click_location_);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MainView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton){
        is_scrolling_ = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}
