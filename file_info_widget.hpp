#ifndef FILE_INFO_WIDGET_HPP
#define FILE_INFO_WIDGET_HPP

#include <vector>

#include <QWidget>

struct Measurement;

QT_BEGIN_NAMESPACE
namespace Ui {
class FileInfoWidget;
}
class QVBoxLayout;
class QLineEdit;
QT_END_NAMESPACE

class FileInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileInfoWidget(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~FileInfoWidget();

    void setMeasurement(const Measurement* measurement);

private:
    void updateInfo();

private:
    Ui::FileInfoWidget* ui_;
    QVBoxLayout* groupBoxLayout_;
    const Measurement* measurement_;
    std::vector<QLineEdit*> parameters_;
};

#endif // FILE_INFO_WIDGET_HPP
