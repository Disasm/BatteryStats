#ifndef BATTERYDATAMODEL_H
#define BATTERYDATAMODEL_H

#include <QObject>
#include <QVariant>
#include <QVector>
#include <QPair>
#include "LogFile.h"


class BatteryDataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
    Q_PROPERTY(int elapsed READ elapsed NOTIFY elapsedChanged)
    Q_PROPERTY(LogFile* logFile READ logFile WRITE setLogFile)
    Q_PROPERTY(int prediction READ prediction NOTIFY predictionChanged)
    Q_PROPERTY(bool charging READ charging NOTIFY chargingChanged)

    struct Item
    {
        int time;
        float capacity;
        bool isCharging;
    };

public:
    BatteryDataModel();

    int elapsed() const;
    int size() const;

    LogFile* logFile() { return m_logFile; }

    Q_INVOKABLE
    QVariant get(int index) const;

    Q_INVOKABLE
    int prediction();

    bool charging();

signals:
    void elapsedChanged();
    void sizeChanged();
    void predictionChanged();
    void chargingChanged();

public slots:
    void setLogFile(LogFile* logFile);
    void forceUpdate();

private slots:
    void logFileChanged();

private:
    int m_elapsed;
    QVector<Item> m_items;
    LogFile* m_logFile;
    bool    m_predictionValid;
    int     m_prediction;
};

#endif // BATTERYDATAMODEL_H
