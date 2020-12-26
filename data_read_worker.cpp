#include "data_read_worker.hpp"

#include <QTime>

#include <QDebug>
#define DEB qDebug()

DataReadWorker::DataReadWorker()
{}

void DataReadWorker::read(const QString &text)
{
    QTime timer;
    timer.start();
    data_.reserve(reserved_data_size_);
    int progress_step = 0;
    for (int first = 0; first < text.size() - 1;) {
        // first value
        int delimiter_pos = text.indexOf(' ', first);
        QStringRef first_value (&text, first, delimiter_pos - first);
        // second value
        int end_of_line = text.indexOf('\n', delimiter_pos + 1);
        QStringRef second_value (&text, delimiter_pos + 1, end_of_line - delimiter_pos - 1);

        QPointF point;
        point.setX(first_value.toDouble());
        point.setY(second_value.toDouble());
        data_.push_back(point);

        updateDataStats(point);

        ++progress_step;
        if (progress_step == steps_per_progress_) {
            progress_step = 0;
            emit progressChanged(first * 100. / (text.size() - 1));
        }
        first = end_of_line + 1;
    }

    qreal secs = timer.elapsed() / qreal(1000);
    DEB << "Parsing time: " << secs;
    DEB << "Data size:" << data_.size();

    emit finished();
}

std::vector<QPointF> DataReadWorker::takeData()
{
    return std::move(data_);
}

DataStats DataReadWorker::takeStats()
{
    return std::move(stats_);
}

void DataReadWorker::updateDataStats(const QPointF &new_value)
{
    stats_.min_x = std::min(stats_.min_x, new_value.x());
    stats_.max_x = std::max(stats_.max_x, new_value.x());
    stats_.min_y = std::min(stats_.min_y, new_value.y());
    stats_.max_y = std::max(stats_.max_y, new_value.y());
}
