#include "data_parse_worker.hpp"

void DataParseWorker::parse(const QString &text)
{
    stats_ = {};
    if (begin_ == end_) {
        emit finished();
        return;
    }

    data_.reserve(reserved_data_size_);
    int progress_step = 0;
    for (int first = begin_; first < end_;) {
        QPointF point;
        // first value
        bool ok1 = true;
        int delimiter_pos = text.indexOf(' ', first);
        QStringRef first_value (&text, first, delimiter_pos - first);
        point.setX(first_value.toDouble(&ok1));
        if (!ok1) {
            data_errors_.append(tr("Cannot parse value ") + first_value);
        }
        // second value
        bool ok2 = true;
        int end_of_line = text.indexOf('\n', delimiter_pos + 1);
        QStringRef second_value (&text, delimiter_pos + 1, end_of_line - delimiter_pos - 1);
        point.setY(second_value.toDouble(&ok2));
        if (!ok2) {
            data_errors_.append(tr("Cannot parse value ") + second_value);
        }

        if (ok1 && ok2){
            data_.push_back(point);
            updateDataStats(point);
        }

        ++progress_step;
        if (progress_step == steps_per_progress_) {
            progress_step = 0;
            emit progressChanged(first * 100. / (text.size() - 1));
        }
        first = end_of_line + 1;
    }

    emit finished();
}

void DataParseWorker::setReadParameters(int begin, int end)
{
    begin_ = begin;
    end_ = end;
}

std::vector<QPointF> DataParseWorker::takeData()
{
    return std::move(data_);
}

DataStats DataParseWorker::takeStats()
{
    return std::move(stats_);
}

void DataParseWorker::updateDataStats(const QPointF &new_value)
{
    stats_.min_x = std::min(stats_.min_x, new_value.x());
    stats_.max_x = std::max(stats_.max_x, new_value.x());
    stats_.min_y = std::min(stats_.min_y, new_value.y());
    stats_.max_y = std::max(stats_.max_y, new_value.y());
}

QStringList DataParseWorker::takeDataErrors() const
{
    return std::move(data_errors_);
}
