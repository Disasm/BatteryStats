#ifndef PROCESSDATAMODEL_H
#define PROCESSDATAMODEL_H

#include <QObject>
#include <QPair>
#include <QString>
#include <QVector>

class ProcessDataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)

public:
    ProcessDataModel();

    int size() const;

    Q_INVOKABLE
    QVariant get(int index) const;

signals:
    void sizeChanged();

private:
    QVector<QPair<QString, float> > m_items;
};

#endif // PROCESSDATAMODEL_H
