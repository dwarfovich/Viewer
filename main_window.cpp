#include "main_window.hpp"
#include "ui_main_window.h"
#include "measurement.hpp"
#include "file_reader.hpp"
#include "plot_drawer.hpp"
#include "plot_widget.hpp"
#include "preview_plot_frame_item.hpp"

#include <QGraphicsScene>
#include <QAction>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <QTimer>

#include <QDebug>
#define DEB qDebug()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui_{new Ui::MainWindow}
    , preview_scene_{new QGraphicsScene {this}}
    , plot_widget_{new PlotWidget {measurement_, this}}
{
    ui_->setupUi(this);
    ui_->previewPlotView->setScene(preview_scene_);
    ui_->previewPlotView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->previewPlotView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->splitter->setSizes({10, 200});
    ui_->scrollArea->setWidget(plot_widget_);

    connect(ui_->previewPlotView, &PreviewPlotView::frameItemPosChanged,
            this, &MainWindow::onPreviewFrameItemPosChanged);

    resize(1400, 900);
    connect(ui_->actionLoad, &QAction::triggered, this, &MainWindow::loadFile);
    connect(ui_->actionQuit, &QAction::triggered, this, &MainWindow::close);

    show();
    loadFile();
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::loadFile()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Select file"), "C:/Boo/Code/Viewer/SampleFiles");
    FileReader reader {measurement_};
    reader.readFile(filename);
    updatePlot();
}

void MainWindow::onPreviewFrameItemPosChanged(const QPointF &delta_pos)
{
    qreal new_x = 0.;
    if (delta_pos.x() < 0) {
        new_x = frame_item_->x() + delta_pos.x();
        if (new_x < 0) {
            new_x = 0.;
        }
    } else {
        new_x = std::min(delta_pos.x(), qreal(ui_->previewPlotView->width() - frame_item_->width()));

    }
    frame_item_->setX(new_x);

    qreal preview_width = ui_->previewPlotView->width();
    int first = (frame_item_->x() * 100) / preview_width;
    int last = ((frame_item_->x() + frame_item_->width()) * 100) / preview_width;
    int plot_height = plot_widget_->width();
    plot_widget_->drawArea(first, last, plot_height);
}

void MainWindow::updatePlot()
{
    PlotDrawer drawer {&measurement_};
    QTime timer;
    timer.start();
    drawer.generatePreview(ui_->previewPlotView->width(), ui_->previewPlotView->height());
    qreal secs = timer.elapsed() / qreal(1000);
    DEB << "Preview painting time: " << secs;
    auto* item = new QGraphicsPixmapItem {drawer.plotPreview()};
    item->setZValue(1);
    preview_scene_->addItem(item);
    plot_widget_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    frame_item_ = new PreviewPlotFrameItem {};
    frame_item_->setWidth(500);
    frame_item_->setHeight(ui_->previewPlotView->size().height());
    frame_item_->setZValue(2);
    preview_scene_->addItem(frame_item_);
}
