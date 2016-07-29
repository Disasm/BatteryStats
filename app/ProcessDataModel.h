#ifndef PROCESSDATAMODEL_H
#define PROCESSDATAMODEL_H

#include <QObject>

class ProcessDataModel : public QObject
{
    Q_OBJECT
public:
    explicit ProcessDataModel(QObject *parent = 0);

signals:

public slots:
};

#endif // PROCESSDATAMODEL_H
