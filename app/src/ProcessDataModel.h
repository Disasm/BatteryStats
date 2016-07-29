#ifndef PROCESSDATAMODEL_H
#define PROCESSDATAMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QPair>
#include <QString>
#include <QVector>
#include <QVariant>
#include "LogFile.h"

class ProcessDataModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(LogFile* logFile READ logFile WRITE setLogFile)

public:
    ProcessDataModel();

    QHash<int, QByteArray> roleNames() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    LogFile* logFile() { return m_logFile; }

signals:
    void sizeChanged();

public slots:
    void setLogFile(LogFile* logFile);

private slots:
    void logFileChanged();
    void sort();

private:
    QVector<QPair<QString, float> > m_items;
    LogFile* m_logFile;
};

#endif // PROCESSDATAMODEL_H
