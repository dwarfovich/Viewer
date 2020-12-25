#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <QGraphicsView>

class MainView : public QGraphicsView
{
public:
    MainView(QWidget *parent = nullptr);

protected:
    void wheelEvent (QWheelEvent* event);
    void mousePressEvent   (QMouseEvent *event);
    void mouseMoveEvent    (QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent *event);

private:
    const qreal positive_factor_;
    const qreal negative_factor_;
    QPointF click_location_;
    QPointF scene_center_;
    bool is_scrolling_;
};

#endif // MAINVIEW_HPP
