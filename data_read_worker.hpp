#ifndef DATAREADWORKER_HPP
#define DATAREADWORKER_HPP

#include "data_stats.hpp"

#include <QObject>
#include <QPointF>

class DataReadWorker : public QObject
{
    Q_OBJECT

public:
    DataReadWorker();

    void read(const QString& text);
    void setReadParameters(int begin, int end);
    std::vector<QPointF> takeData();
    DataStats takeStats();

signals:
    void progressChanged(qreal progress);
    void finished();

private:
    void updateDataStats(const QPointF& new_value);

private:
    std::vector<QPointF> data_;
    DataStats stats_;
    int begin_ = 0;
    int end_ = 0;
    size_t reserved_data_size_ = 5'000'000;
    const int steps_per_progress_ = 20'000;
};

#endif // DATAREADWORKER_HPP
