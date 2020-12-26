#include "file_reader.hpp"
#include "measurement.hpp"
#include "header.hpp"

#include <QTextStream>
#include <QFile>
#include <QMessageBox>

#include <algorithm>

#include <QDebug>
#define DEB qDebug()

FileReader::FileReader()
{}

void FileReader::readFile(const QString &filename)
{
    QFile file {filename};
    if (file.open(QFile::ReadOnly)) {
        clearMeasurement();
        QTextStream input {&file};
        auto header_lines = readHeaderLines(input);
        const auto& header_errors = parseHeaderLines(header_lines);
        header_errors_.append(header_errors);
//        printHeader();
        readData(input);
        emit finished();
    } else {
        data_errors_.push_back(tr("Cannot open file"));
    }
}

bool FileReader::hasErrors() const
{
    return false;
}

QStringList FileReader::readHeaderLines(QTextStream &input) const
{
    QStringList lines;
    while (true) {
        auto pos = input.pos();
        QString line = input.readLine();
        if (isHeaderLine(line)) {
            lines.push_back(line.remove(0, 1).simplified()); // Remove first '#' symbol
        } else {
            input.seek(pos);
            break;
        }
    }

    return lines;
}

QStringList FileReader::parseHeaderLines(const QStringList &lines)
{
    if (lines.size() > HeaderLines::OrganizationAndApp) {
        parseOrganiationAndApp(lines[HeaderLines::OrganizationAndApp]);
    }
    if (lines.size() > HeaderLines::MeasurementType) {
        parseMeasurementType(lines[HeaderLines::MeasurementType]);
    }
    if (lines.size() > HeaderLines::StartTime) {
        parseStartTime(lines[HeaderLines::StartTime]);
    }
    if (lines.size() > HeaderLines::Duration) {
        parseDuration(lines[HeaderLines::Duration]);
    }
    if (lines.size() > HeaderLines::Parameters) {
        parseParameters(lines, HeaderLines::Parameters);
    }

    if (lines.size() < HeaderLines::Parameters + 1) {
        return {tr("One or more header lines are missing")};
    } else {
        return {};
    }
}

bool FileReader::isHeaderLine(const QString &line) const
{
    return (line.front() == header_line_starter_);
}

void FileReader::parseOrganiationAndApp(QString line)
{
    int organization_delimiter_pos = line.indexOf(',');
    if (organization_delimiter_pos == -1){
        measurement_.header.organization = line;
    } else {
        measurement_.header.organization = line.left(organization_delimiter_pos);
        measurement_.header.application =
                line.right(line.size() - organization_delimiter_pos - 1).simplified();
    }
}

void FileReader::parseMeasurementType(QString line)
{
    measurement_.header.measurement_type = line.simplified();
}

void FileReader::parseStartTime(QString line)
{
    static const QString time_format = "ddd MMM dd hh:mm:ss yyyy";
    static const QLocale locale {"en_US"};
    auto time = locale.toDateTime(line, time_format);
    if (!time.isValid()) {
        header_errors_.push_back(tr("Cannot parse start time."));
    } else{
        measurement_.header.start_time = locale.toDateTime(line, time_format);
    }
}

void FileReader::parseDuration(QString line)
{
    measurement_.header.duration = line;
}

void FileReader::parseParameters(const QStringList &header_lines, int first_parameters_line)
{
    std::vector<QString> parameters;
    for (size_t i = first_parameters_line; i < header_lines.size(); ++i) {
        parameters.push_back(header_lines[i]);
    }
    measurement_.header.parameters = parameters;
}

void FileReader::readData(QTextStream &input)
{
    QTime timer;
    auto& data = measurement_.data;
    timer.start();
    data.reserve(dataReservedSize());
    QString text = input.readAll();
    qreal secs = timer.elapsed() / qreal(1000);
    DEB << "Reading data:" << secs;
    int first = 0;
    while (first < text.size() - 1) {
        // first value
        int delimiter_pos = text.indexOf(' ', first);
        QStringRef first_value (&text, first, delimiter_pos - first);
        // second value
        int end_of_line = text.indexOf('\n', delimiter_pos + 1);
        QStringRef second_value (&text, delimiter_pos + 1, end_of_line - delimiter_pos - 1);

        QPointF point;
        point.setX(first_value.toDouble());
        point.setY(second_value.toDouble());
        data.push_back(point);

        measurement_.stats.min_x = std::min(measurement_.stats.min_x, point.x());
        measurement_.stats.max_x = std::max(measurement_.stats.max_x, point.x());
        measurement_.stats.min_y = std::min(measurement_.stats.min_y, point.y());
        measurement_.stats.max_y = std::max(measurement_.stats.max_y, point.y());

        first = end_of_line + 1;
    }

    secs = timer.elapsed() / qreal(1000);
    DEB << "Parsing time: " << secs;
    DEB << "Data size:" << data.size();

    measurement_.header.data_size = data.size();
}

void FileReader::clearMeasurement()
{
    measurement_.data.clear();
    measurement_.header.parameters.clear();
    measurement_.stats.min_x = std::numeric_limits<double>::max();
    measurement_.stats.max_x = 0.;
    measurement_.stats.min_y = std::numeric_limits<double>::max();
    measurement_.stats.max_y = 0.;
}

void FileReader::printHeader() const
{
    const auto& header = measurement_.header;
    DEB << "Organization:" << header.organization;
    DEB << "Application:" << header.application;
    DEB << "Measurement type:" << header.measurement_type;
    DEB << "Start time:" << header.start_time;
    for (const auto& str : header.parameters) {
        DEB << str;
    }
}

const QStringList &FileReader::dataErrors() const
{
    return data_errors_;
}

const QStringList &FileReader::headerErrors() const
{
    return header_errors_;
}

size_t FileReader::dataReservedSize() const
{
    return data_reserved_size_;
}

void FileReader::setDataReservedSize(const size_t &size)
{
    data_reserved_size_ = size;
}

Measurement FileReader::takeMeasurement()
{
    return std::move(measurement_);
}
