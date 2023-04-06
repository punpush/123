#ifndef COUNTERBACKEND_H
#define COUNTERBACKEND_H

#include <QObject>
#include <QUrl>
#include "datamodellist.h"

class CounterBackend : public QObject
{
    Q_OBJECT

public:
    explicit CounterBackend(QObject *parent = nullptr);

    int start(int argc, char *argv[]);

    Q_PROPERTY(DataModelList *tableModel
               READ tableModel
               CONSTANT)

    Q_INVOKABLE void calc(QUrl path);
    Q_INVOKABLE void saveFile(QUrl path);
    Q_INVOKABLE void openFile(QUrl path);

    DataModelList *tableModel() const;

private:
    std::vector<char> m_marks;
    DataModelList *m_tableModel = nullptr;
};

#endif // COUNTERBACKEND_H
