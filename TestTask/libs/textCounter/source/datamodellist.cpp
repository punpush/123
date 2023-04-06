#include "datamodellist.h"

DataModelList::DataModelList(QObject *parent)
    : QAbstractListModel(parent)
{

}

int DataModelList::rowCount(const QModelIndex &) const
{
    return static_cast<int>(m_data.size());
}

QHash<int, QByteArray> DataModelList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = QByteArrayLiteral("id");
    roles[Letters] = QByteArrayLiteral("lett");
    roles[Words] = QByteArrayLiteral("words");
    roles[Sentences] = QByteArrayLiteral("sent");
    return roles;
}

QVariant DataModelList::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Id:
        return m_id[index.row()];
    case Letters:
        return QString::number(m_data[index.row()].letters());
    case Words:
        return QString::number(m_data[index.row()].words());
    case Sentences:
        return QString::number(m_data[index.row()].sentences());
    }

    return QVariant();
}

bool DataModelList::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Id) {
        const auto res = value.toString();
        m_id[index.row()] = res;
        emit dataChanged(index,index,{role});
        return true;
    }

    if (role == Letters) {
        const auto res = value.toInt();
        m_data[index.row()].setLetters(res);
        emit dataChanged(index,index,{role});
        return true;
    }

    if (role == Words) {
        const auto res = value.toInt();
        m_data[index.row()].setWords(res);
        emit dataChanged(index,index,{role});
        return true;
    }

    if (role == Sentences) {
        const auto res = value.toInt();
        m_data[index.row()].setSentences(res);
        emit dataChanged(index,index,{role});
        return true;
    }

    return QAbstractListModel::setData(index,value,role);
}

void DataModelList::addNewInfo(const DataModelListInfo &newInfo, const QString &newId)
{
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    m_data.push_back(newInfo);
    m_id.push_back(newId);
    endInsertRows();
}

void DataModelList::recalcInfo(const DataModelListInfo &newInfo, const QString &Id)
{
    for (int i = 0; i < static_cast<int>(m_id.size()); i++) {
        if (Id == m_id.at(i)) {
            m_curIndex = i;
            break;
        }
    }

    m_data[m_curIndex] = newInfo;
    auto index_let = createIndex(m_curIndex, 1, this);
    auto index_wrd = createIndex(m_curIndex, 2, this);
    auto index_snt = createIndex(m_curIndex, 3, this);

    setData (index_let, m_data[m_curIndex].letters(), Letters);
    setData (index_wrd, m_data[m_curIndex].words(), Words);
    setData (index_snt, m_data[m_curIndex].sentences(), Sentences);
}

void DataModelList::clearData()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    m_data.clear();
    m_id.clear();
    endRemoveRows();
}

std::vector<QString> &DataModelList::getId()
{
    return m_id;
}

std::vector<DataModelListInfo> &DataModelList::getData()
{
    return m_data;
}
