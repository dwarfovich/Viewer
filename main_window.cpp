#include "main_window.hpp"
#include "ui_main_window.h"
#include "measurement.hpp"
#include "file_reader.hpp"
#include "plot_drawer.hpp"
#include "plot_widget.hpp"

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
    , scene_{new QGraphicsScene {this}}
    , preview_scene_{new QGraphicsScene {this}}
    , plot_widget_{new PlotWidget {measurement_, this}}
{
    ui_->setupUi(this);
//    ui_->mainGraphicsView->setScene(scene_);
    ui_->overallGraphicsView->setScene(preview_scene_);
    ui_->overallGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->overallGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui_->overallGraphicsView->fitInView(scene_->sceneRect());
    ui_->splitter->setSizes({10, 200});
    ui_->scrollArea->setWidget(plot_widget_);

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
    populateScene();
}

void MainWindow::populateScene()
{
    scene_->clear();
    PlotDrawer drawer {measurement_, {0., 0., 1920., 1080.}};
    QTime timer;
    timer.start();
//    drawer.draw();
    DEB << ui_->overallGraphicsView->geometry().width() << ui_->overallGraphicsView->geometry().height();
    drawer.generatePreview(ui_->overallGraphicsView->width(), ui_->overallGraphicsView->height());
    qreal secs = timer.elapsed() / qreal(1000);
    DEB << "Preview painting time: " << secs;
//    auto* item = new QGraphicsPixmapItem {drawer.plot()};
//    scene_->addItem(item);

//    drawer.generatePreview(ui_->overallGraphicsView->width(), ui_->overallGraphicsView->height());
    auto* item = new QGraphicsPixmapItem {drawer.plot_preview()};
    preview_scene_->addItem(item);
    DEB << preview_scene_->sceneRect();
//    ui_->overallGraphicsView->fitInView(preview_scene_->sceneRect());
//    ui_->plotWidget->setPlot(drawer.plot());

//    ui_->overallGraphicsView->fitInView(preview_scene_->sceneRect());
//    ui_->scrollArea->verticalScrollBar()->setMinimum()
//    plot_widget_->setPlot(drawer.plot());
    plot_widget_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    ui_->scrollArea->horizontalScrollBar()->setMaximum(10000);
//    ui_->scrollArea->setWidgetResizable(true);
//    plot_widget_->update();
}

