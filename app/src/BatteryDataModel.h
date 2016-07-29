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

signals:
    void elapsedChanged();
    void sizeChanged();

public slots:
    void setLogFile(LogFile* logFile);
    void forceUpdate();

private slots:
    void logFileChanged();

private:
    int m_elapsed;
    QVector<Item> m_items;
    LogFile* m_logFile;
};

#endif // BATTERYDATAMODEL_H
