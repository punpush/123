#ifndef DATAMODELLIST_H
#define DATAMODELLIST_H

#include <QAbstractListModel>
#include <vector>
#include "datamodellistinfo.h"

class DataModelList : public QAbstractListModel
{
    Q_OBJECT

public:

    explicit DataModelList(QObject *parent = nullptr);

    enum CounterRoles {
        Id,
        Letters,
        Words,
        Sentences
    };
    Q_ENUM(CounterRoles);

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    void addNewInfo(const DataModelListInfo &newInfo, const QString &newId);
    void recalcInfo(const DataModelListInfo &newInfo, const QString &Id);
    void clearData();
    std::vector<QString> &getId();
    std::vector<DataModelListInfo> &getData();

private:
     std::vector<DataModelListInfo> m_data;
     std::vector<QString> m_id;
     int m_curIndex;
};

#endif // DATAMODELLIST_H
