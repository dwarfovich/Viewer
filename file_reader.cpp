#include "file_reader.hpp"
#include "measurement.hpp"
#include "header.hpp"

#include <QTextStream>
#include <QFile>
#include <QMessageBox>

#include <algorithm>

#include <QDebug>
#define DEB qDebug()

FileReader::FileReader(Measurement &data)
    : data_{data}
{}

void FileReader::readFile(const QString &filename)
{
    QFile file {filename};

    if (file.open(QFile::ReadOnly)) {
        clearData();
        QTextStream input {&file};
        auto result = readHeader(input);
        readData(input);
        emit finished();
    } else {
        static const QString caption = QObject::tr("Critical error");
        static const QString text = QObject::tr("Cannot open file");
        QMessageBox::critical(nullptr, caption, text);
    }
}

bool FileReader::hasErrors() const
{
    return false;
}

bool FileReader::readHeader(QTextStream &input)
{
    auto pos = input.pos();
    QChar starter;
    input >> starter;
    if (starter == header_line_starter_) {
        QString line = input.readLine();
        bool result = readOrganiationAndApp(line);
        if (!result) {
            return false;
        }
        line = input.readLine();
        result = readMeasurementType(line);
        if (!result) {
            return false;
        }
        line = input.readLine();
        result = readStartTime(line);
        if (!result) {
            return false;
        }
        result = readParameters(input);
    } else {
        input.seek(pos);
    }

//    printHeader();

//    return istream;
    return true;
}

bool FileReader::checkHeaderLineStarter(const QString &line) const
{
    return line.front() == header_line_starter_;
}

bool FileReader::readOrganiationAndApp(QString line)
{
    const auto& parts = line.split(",");
    const int expected_lines = 2;
    if (parts.size() == expected_lines) {
        data_.header.organization_ = parts[OrganizationAndAppParts::OrganizationPart].simplified();
        data_.header.application_ = parts[OrganizationAndAppParts::ApplicationPart].simplified();
        return true;
    } else {
        static const QString caption = QObject::tr("Warning");
        static const QString text = QObject::tr("Cannot parse organization and application. Proceed anyway?");
        auto reply = QMessageBox::warning(nullptr, caption, text, QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            data_.header.organization_ = line.simplified();
            return true;
        } else {
            return false;
        }
    }
}

bool FileReader::readMeasurementType(QString line)
{
    bool result = checkHeaderLineStarter(line);
    line.remove(0, 1);
    data_.header.measurement_type_ = line.simplified();

    return true;
}

bool FileReader::readStartTime(QString line)
{
    line.remove(0, 2);
    static const QString time_format = "ddd MMM dd hh:mm:ss yyyy";
    static const QLocale locale {"en_US"};
    data_.header.start_time_ = locale.toDateTime(line, time_format);

    return true;
}

bool FileReader::readParameters(QTextStream &input)
{
    bool is_parameters_line = true;
    while (is_parameters_line) {
        auto pos = input.pos();
        QString line = input.readLine();
        if (checkHeaderLineStarter(line)) {
            line.remove(0, 1);
            data_.header.parameters_.push_back(line.simplified());
        } else {
            input.seek(pos);
            is_parameters_line = false;
        }
    }

    return true;
}

void FileReader::readData(QTextStream &input)
{
    QTime timer;
    auto& data = data_.data;
    timer.start();
    data.reserve(default_data_reserved_size());
    QString d = input.readAll();
    int first = 0;
    while (first < d.size() - 1) {
        // first value
        int delimiter_pos = d.indexOf(' ', first);
        QStringRef first_value (&d, first, delimiter_pos - first);
        // second value
        int end_of_line = d.indexOf('\n', delimiter_pos + 1);
        QStringRef second_value (&d, delimiter_pos + 1, end_of_line - delimiter_pos - 1);

        QPointF point;
        point.setX(first_value.toDouble());
        point.setY(second_value.toDouble());
        data.push_back(point);

        data_.stats.min_x = std::min(data_.stats.min_x, point.x());
        data_.stats.max_x = std::max(data_.stats.max_x, point.x());
        data_.stats.min_y = std::min(data_.stats.min_y, point.y());
        data_.stats.max_y = std::max(data_.stats.max_y, point.y());

        first = end_of_line + 1;
    }

    qreal secs = timer.elapsed() / qreal(1000);
    DEB << "Time: " << secs;
    DEB << data.size();
}

qint64 FileReader::estimateMeasurementsCount(QTextStream &input) const
{
    auto start = input.pos();
    return {};
}

void FileReader::clearData()
{
    data_.data.clear();
    data_.header.parameters_.clear();
}

void FileReader::printHeader() const
{
    const auto& header = data_.header;
    DEB << "Organization:" << header.organization_;
    DEB << "Application:" << header.application_;
    DEB << "Measurement type:" << header.measurement_type_;
    DEB << "Start time:" << header.start_time_;
    for (const auto& str : header.parameters_) {
        DEB << str;
    }
}

size_t FileReader::default_data_reserved_size() const
{
    return default_data_reserved_size_;
}

void FileReader::setDefault_data_reserved_size(const size_t &default_data_reserved_size)
{
    default_data_reserved_size_ = default_data_reserved_size;
}
