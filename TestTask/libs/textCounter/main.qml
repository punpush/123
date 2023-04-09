import QtQuick 2.15
import QtQuick.Window 2.11
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import counter 1.0

Window {
    id: root
    width: 1024
    height: 1024
    visible: true
    title: qsTr("counter")

    Item {
        id: variables


    }

    CounterBackend {
        id:backend
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.alignment: Qt.AlignTop
            Layout.margins: 25

            TextField {
                id: pathToFile
                Layout.fillWidth: true
                placeholderText: qsTr("Путь до файла")
                readOnly: true
            }

            Button {
                width: 100
                text: qsTr("Выберите файл")
                onClicked: fileDialog.open()
            }
        }

        TableView {
            id: table
            model: backend.tableModel
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 25

            TableViewColumn {
                title: "Url"
                role: "id"
                width: table.width / 4
            }

            TableViewColumn {
                title: "Колическтво букв"
                role: "lett"
                width: table.width / 4
            }

            TableViewColumn {
                title: "Колическтво слов"
                role: "words"
                width: table.width / 4
            }

            TableViewColumn {
                title: "Количество предложений"
                role: "sent"
                width: table.width / 4
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignTop
            Layout.margins: 25


            Button {
                width: 100
                text: qsTr("Посчитать")
                onClicked: backend.calc(pathToFile.placeholderText)
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                width: 100
                text: qsTr("Сохранить таблицу")
                onClicked: fileDialogForPath.open()
            }

            Button {
                width: 100
                text: qsTr("Открыть таблицу")
                onClicked: fileDialogForOpenPath.open()
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Выберите файл")
        selectMultiple: false
        onAccepted: {
            pathToFile.placeholderText = fileDialog.fileUrl
        }
    }

    FileDialog {
        id: fileDialogForPath
        title: qsTr("Сохранить как")
        selectMultiple: false
        selectExisting: false
        nameFilters: [ qsTr("bin-файл (*.bin)") ]
        onAccepted: backend.saveFile(fileDialogForPath.fileUrl)
    }

    FileDialog {
        id: fileDialogForOpenPath
        title: qsTr("Открыть")
        selectMultiple: false
        selectExisting: false
        nameFilters: [ qsTr("bin-файл (*.bin)") ]
        onAccepted: backend.openFile(fileDialogForOpenPath.fileUrl)
    }
}
