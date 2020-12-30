#ifndef PREVIEWPLOTSCENE_HPP
#define PREVIEWPLOTSCENE_HPP

#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsPixmapItem;
QT_END_NAMESPACE

class PlotDrawer;
class PreviewPlotFrameItem;

class PreviewPlotScene : public QGraphicsScene
{
    Q_OBJECT

public:
    PreviewPlotScene(PlotDrawer& drawer, QObject* parent = nullptr);

    PreviewPlotFrameItem *frameItem() const;
    void updatePreview(int width, int height);
    void resetFrameItem(int height, int expected_scene_width);

public slots:
    void onPlotScaleRequest(qreal angle_data);

signals:
    void frameItemChanged(const QRectF& rect);
    void frameItemPosChanged(const QPointF& new_pos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    qreal minFrameWidth(int expected_scene_width) const;
    qreal maxFrameWidth(int expected_scene_width) const;

private:
    PlotDrawer& drawer_;
    QGraphicsPixmapItem* preview_item_;
    PreviewPlotFrameItem* frame_item_;
    qreal normal_width_ = 50.;
    qreal width_delta_ = 50.;
    qreal min_frame_width_ = 0.;
    qreal max_frame_width_ = 0.;
    const size_t min_frame_points_ = 100;
    const size_t max_frame_points_ = 100'000;
};

#endif // PREVIEWPLOTSCENE_HPP
