#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <QGraphicsView>

class MainView : public QGraphicsView
{
public:
    MainView(QWidget *parent = nullptr);

protected:
    void mousePressEvent   (QMouseEvent *event);
    void mouseMoveEvent    (QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent *event);

private:
    QPointF click_location_;
    QPointF scene_center_;
    bool is_scrolling_;
};

#endif // MAINVIEW_HPP
