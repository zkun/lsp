import QtQuick 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import zkun 1.0

ApplicationWindow {
    width: 640
    height: 480
    visible: server.visible
    onClosing: function(close) {
        close.accepted = false;
        server.visible = false;
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 7
        GroupBox {
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent
                Label {
                    elide: Label.ElideRight
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    Layout.fillWidth: true
                }
                Button {
                    text: qsTr("选择文件")
                    onClicked: dialog.open()
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            DropArea {
                anchors.fill: parent
                id: dragArea
                onEntered: function(drag) {
                    if (server.hasFiles(drag.urls))
                        drag.accepted = true;
                    else
                        drag.accepted = false;
                }
                onDropped: function(drag) {
                    server.uploading(drag.urls)
                }
            }
        }
    }

    Services {
        id: server
    }

    FileDialog {
        id: dialog
        folder: shortcuts.pictures
        onAccepted: {
            server.uploading(fileUrls)
        }
    }
}
