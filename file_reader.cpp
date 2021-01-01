#include "file_reader.hpp"
#include "measurement.hpp"
#include "header.hpp"
#include "data_parse_worker.hpp"

#include <QTextStream>
#include <QFile>
#include <QMessageBox>

#include <algorithm>

FileReader::FileReader()
{}

FileReader::~FileReader()
{
    quitThreads();
}

void FileReader::readFile(const QString &filename)
{
    QFile file {filename};
    if (file.open(QFile::ReadOnly)) {
        clear();
        measurement_.header.filename = filename;
        QTextStream input {&file};
        const auto& header_lines = readHeaderLines(input);
        const auto& header_errors = parseHeaderLines(header_lines);
        header_errors_.append(header_errors);
        readData(input);
    } else {
        data_errors_.push_back(tr("Cannot open file ") + filename);
        emit finished();
    }
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

void FileReader::parseOrganiationAndApp(const QString& line)
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

void FileReader::parseMeasurementType(const QString& line)
{
    measurement_.header.measurement_type = line.simplified();
}

void FileReader::parseStartTime(const QString& line)
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
    int thread_count = 0;
    QString text = input.readAll();
    if (text.size() < multithreading_text_size) {
        thread_count = 1;
    } else {
        thread_count = QThread::idealThreadCount();
    }

    prepareThreadsAndWorkers(thread_count);
    prepareWorkersData(text);

    emit parsingStarted(text);
}

void FileReader::clear()
{
    measurement_.data.clear();
    measurement_.header.parameters.clear();
    measurement_.stats = {};
    header_errors_.clear();
    data_errors_.clear();
    workers_data_.clear();
    workers_stats_.clear();
    jobs_done_ = 0;
}

void FileReader::quitThreads() const
{
    for (auto& thread : threads_) {
        thread->quit();
        thread->wait();
    }
}

void FileReader::concatenateWorkersResults()
{
    std::sort(workers_data_.begin(), workers_data_.end(), [](const auto& v1, const auto& v2) {
        if (v1.empty() || v2.empty()) {
            return false;
        } else {
            return v1[0].x() < v2[0].x();
        }
    });
    for (auto& data : workers_data_) {
            measurement_.data.insert(measurement_.data.end(), data.begin(), data.end());
    }
    for (const auto& stats : workers_stats_) {
        measurement_.stats.min_x = std::min(measurement_.stats.min_x, stats.min_x);
        measurement_.stats.min_y = std::min(measurement_.stats.min_y, stats.min_y);
        measurement_.stats.max_x = std::max(measurement_.stats.max_x, stats.max_x);
        measurement_.stats.max_y = std::max(measurement_.stats.max_y, stats.max_y);
    }
}

void FileReader::prepareThreadsAndWorkers(int thread_count)
{
    threads_.clear();
    quitThreads();
    workers_.clear();
    threads_.reserve(thread_count);
    workers_.reserve(thread_count);
    for (int i = 0; i < thread_count; ++i) {
        threads_.push_back(std::make_unique<QThread>());
        workers_.push_back(std::make_unique<DataParseWorker>());
        connect(workers_[i].get(), &DataParseWorker::finished, this, &FileReader::onWorkerFinished);
        connect(this, &FileReader::parsingStarted, workers_[i].get(), &DataParseWorker::parse);
        workers_[i]->moveToThread(threads_[i].get());
    }
    connect(workers_[0].get(), &DataParseWorker::progressChanged, this, &FileReader::progressChanged);

    for (auto& thread : threads_) {
        thread->start();
    }
}

void FileReader::prepareWorkersData(const QString& text)
{
    int size_per_thread = text.size() / threads_.size();
    int begin = 0;
    for (size_t i = 0; i < threads_.size(); ++i) {
        int end_of_line = text.indexOf('\n', size_per_thread * (i + 1));
        if (end_of_line == -1) {
            end_of_line = text.size();
        }
        workers_[i]->setReadParameters(begin, end_of_line);
        begin = end_of_line + 1;
    }
}

void FileReader::onWorkerFinished()
{
    auto* worker = qobject_cast<DataParseWorker*>(sender());
    if (worker) {
        workers_data_.push_back(worker->takeData());
        workers_stats_.push_back(worker->takeStats());
        ++jobs_done_;
        if (jobs_done_ == workers_.size()) {
            quitThreads();
            concatenateWorkersResults();
            emit finished();
        }
    } else {
        Q_ASSERT(false);
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

Measurement FileReader::takeMeasurement()
{
    return std::move(measurement_);
}
