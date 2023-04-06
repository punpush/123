#include "counterbackend.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QBuffer>
#include <QDebug>
#include <QDataStream>

CounterBackend::CounterBackend(QObject *parent)
    : QObject{parent}
{
    qmlRegisterType<CounterBackend>("counter", 1, 0, "CounterBackend");
    m_tableModel = new DataModelList(this);
    m_marks = {'<', '>', ',', '.', '/', '?', ' ', '"', ';', ':', '[', ']', '{', '}', '}', 92, 39, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',
               '-', '+', '=', '\n', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\t'};
}

int CounterBackend::start(int argc, char *argv[])
{
    QGuiApplication::setOrganizationName("stc");
    QGuiApplication::setOrganizationDomain("stc");
    QGuiApplication app(argc, argv);

        QQmlApplicationEngine engine;
        const QUrl url(QStringLiteral("qrc:/main.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                         &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
        engine.load(url);



        return app.exec();
}

void CounterBackend::calc(QUrl path)
{

    QString id = path.toLocalFile();
    QFile file(id);

    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "файл не открылся";
        return;
    }

    QByteArray text = file.readAll();
    auto textForWork = QString::fromUtf8(text);

    if (textForWork.length() == 0) {
        return;
    }

    bool recalc = false;
    int flag = 0;
    int letCounter = 0;
    int wordsCounter = 0;
    int sentCounter = 0;
    DataModelListInfo mi;

    for (int i = 0; i < textForWork.length(); i++) {
        flag = 0;
        for (int j = 0; j < static_cast<int>(m_marks.size()); j++) {
            if (textForWork.at(i) != m_marks[j]) {
                flag ++;
            }
        }

        if (flag == static_cast<int>(m_marks.size())) {
            letCounter ++;
        }

        if ((textForWork.at(i) == ' ' && textForWork.at(i + 1) != ' ') || (textForWork.at(i) == '\n' && i != textForWork.length() - 1)
                || (textForWork.at(i) == '\t' && textForWork.at(i + 1) != ' ')) {
            wordsCounter++;
        }

        if ((textForWork.at(i) == '.'|| textForWork.at(i) == '!' || textForWork.at(i) == '?') && (textForWork.at(i+1) != '.')) {
            sentCounter++;
        }
    }

    if (textForWork.at(0) != ' ')
        wordsCounter++;

    mi.setLetters(letCounter);
    mi.setWords(wordsCounter);
    mi.setSentences(sentCounter);

    for (int i = 0; i < static_cast<int>(m_tableModel->getId().size()); i++) {
        if (id == m_tableModel->getId().at(i)) {
            recalc = true;
        }
    }

    if (recalc) {
        m_tableModel->recalcInfo(mi,id);
    } else {
        m_tableModel->addNewInfo(mi,id);
    }
}

void CounterBackend::saveFile(QUrl path)
{
    QFile fileOut(path.toLocalFile());
    QByteArray data;

    if (!fileOut.open(QFile::WriteOnly)) {
        qDebug() << "файл не открылся";
        return;
    }

    if (m_tableModel->getId().size() == 0) {
        qDebug() << "таблица пустая";
        return;
    }


    for (int i = 0; i < static_cast<int>(m_tableModel->getId().size()); i++) {
        data.append("$");
        data.append(m_tableModel->getId().at(i).toUtf8());
        data.append("#");
        data.append(QString::fromStdString(std::to_string(m_tableModel->getData().at(i).letters())).toUtf8());
        data.append("#");
        data.append(QString::fromStdString(std::to_string(m_tableModel->getData().at(i).words())).toUtf8());
        data.append("#");
        data.append(QString::fromStdString(std::to_string(m_tableModel->getData().at(i).sentences())).toUtf8());
        data.append("#");
    }

    QDataStream stream(&fileOut);
    stream << data;

    if(stream.status() != QDataStream::Ok)
    {
        qDebug() << "Ошибка записи";
    }

    fileOut.close();

}

void CounterBackend::openFile(QUrl path)
{
    QFile fileIn(path.toLocalFile());
    QByteArray data;

    if (!fileIn.open(QFile::ReadOnly)) {
        qDebug() << "файл не открылся";
        return;
    }

    QDataStream stream(&fileIn);
    stream >> data;

    if(stream.status() != QDataStream::Ok)
    {
        qDebug() << "Ошибка чтения файла";
    }

    fileIn.close();

    if (data.length() == 0) {
        qDebug() << "Файл пустой";
        return;
    }

    auto info = QString::fromUtf8(data);
    m_tableModel->clearData();

    int target = 0;
    int countCycles = 0;

    for (int counter = 0; counter < info.length(); counter ++) {
        if (info.at(counter) == "$")
            countCycles++;
    }

    for (int j = 0; j < countCycles; j++) {

        DataModelListInfo mi;
        QString id = 0;
        QString letters = 0;
        QString words = 0;
        QString sent = 0;

        for (int i = target; i < info.length(); i++) {

            if (info.at(i) == "$")
                continue;

            if (info.at(i) != '#') {
                id.append(info.at(i));
            } else {
                target = i + 1;
                break;
            }
        }

        for (int i = target; i < info.length(); i++) {
            if (info.at(i) != '#') {
                letters.append(info.at(i));
            } else {
                target = i + 1;
                break;
            }
        }

        for (int i = target; i < info.length(); i++) {
            if (info.at(i) != '#') {
                words.append(info.at(i));
            } else {
                target = i + 1;
                break;
            }
        }

        for (int i = target; i < info.length(); i++) {
            if (info.at(i) != '#') {
                sent.append(info.at(i));
            } else {
                target = i + 1;
                break;
            }
        }

        mi.setLetters(letters.toInt());
        mi.setWords(words.toInt());
        mi.setSentences(sent.toInt());
        m_tableModel->addNewInfo(mi,id);

    }
}

DataModelList *CounterBackend::tableModel() const
{
    return m_tableModel;
}


