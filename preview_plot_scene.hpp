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

private:
    PlotDrawer& drawer_;
    QGraphicsPixmapItem* preview_item_;
    PreviewPlotFrameItem* frame_item_;
};

#endif // PREVIEWPLOTSCENE_HPP
