#ifndef BATTERYINFOMODEL_H
#define BATTERYINFOMODEL_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>
#include "LogFile.h"


class BatteryInfoModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
    Q_PROPERTY(LogFile* logFile READ logFile WRITE setLogFile)

    struct Info
    {
        QString batteryCharge;
        QString batteryWatts;
        QString batteryCapacity;
    };

public:
    BatteryInfoModel();

    int size() const;

    LogFile* logFile() { return m_logFile; }

    Q_INVOKABLE
    QVariant get(int index) const;

signals:
    void sizeChanged();

public slots:
    void setLogFile(LogFile* logFile);
    void forceUpdate();

private slots:
    void logFileChanged();

private:
    QVector<Info> m_info;
    LogFile* m_logFile;
};

#endif // BATTERYINFOMODEL_H
