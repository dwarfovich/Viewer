#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "data.hpp"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loadFile();

private:
    Ui::MainWindow *ui_;
    Data data_;
};
#endif // MAINWINDOW_HPP
