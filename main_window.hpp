#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "measurement.hpp"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QGraphicsScene;
QT_END_NAMESPACE

class PlotWidget;
class PreviewPlotFrameItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loadFile();
    void onPreviewFrameItemPosChanged(const QPointF& new_pos);

private:
    void populateScene();

private:
    Ui::MainWindow *ui_;
    QGraphicsScene* scene_;
    QGraphicsScene* preview_scene_;
    PlotWidget* plot_widget_;
    PreviewPlotFrameItem* frame_item_;
    Measurement measurement_;
};
#endif // MAINWINDOW_HPP
