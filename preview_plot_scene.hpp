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

public slots:
    void onPlotScaleRequest(qreal angle_data);

signals:
    void frameItemChanged(const QRectF& rect);

private:
    PlotDrawer& drawer_;
    QGraphicsPixmapItem* preview_item_;
    PreviewPlotFrameItem* frame_item_;
    qreal x_scale = 1.0;
    const qreal scale_delta = 0.1;
    const qreal min_scale = 0.1;
    const qreal max_scale = 1.5;
};

#endif // PREVIEWPLOTSCENE_HPP
