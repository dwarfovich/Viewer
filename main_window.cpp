#include "main_window.hpp"
#include "ui_main_window.h"
#include "measurement.hpp"
#include "file_reader.hpp"
#include "plot_drawer.hpp"

#include <QGraphicsScene>
#include <QAction>
#include <QFileDialog>
#include <QGraphicsPixmapItem>

#include <QDebug>
#define DEB qDebug()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui_{new Ui::MainWindow}
    , scene_{new QGraphicsScene {this}}
    , preview_scene_{new QGraphicsScene {this}}
{
    ui_->setupUi(this);
    ui_->mainGraphicsView->setScene(scene_);
    ui_->overallGraphicsView->setScene(preview_scene_);
    ui_->overallGraphicsView->fitInView(scene_->sceneRect());
    ui_->splitter->setSizes({10, 200});

    connect(ui_->actionLoad, &QAction::triggered, this, &MainWindow::loadFile);
    connect(ui_->actionQuit, &QAction::triggered, this, &MainWindow::close);

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
    drawer.draw();
    auto* item = new QGraphicsPixmapItem {drawer.plot()};
    scene_->addItem(item);

    drawer.generatePreview(ui_->overallGraphicsView->width(), ui_->overallGraphicsView->height());
    item = new QGraphicsPixmapItem {drawer.plot_preview()};
    preview_scene_->addItem(item);
    ui_->overallGraphicsView->fitInView(preview_scene_->sceneRect());
}

