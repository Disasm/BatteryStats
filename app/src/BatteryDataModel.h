#ifndef BATTERYDATAMODEL_H
#define BATTERYDATAMODEL_H

#include <QObject>
#include <QVariant>
#include <QVector>
#include <QPair>


class BatteryDataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)

public:
    BatteryDataModel();

    int size() const;

    Q_INVOKABLE
    QVariant get(int index) const;

signals:
    void sizeChanged();

public slots:

private:
    QVector<QPair<float, bool> > m_items;
};

#endif // BATTERYDATAMODEL_H
