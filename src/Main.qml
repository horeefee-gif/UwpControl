import QtQuick
import QtQuick.Controls
import UwpControl

Window {
    id: root

    width: 600
    height: 650

    minimumWidth: 600
    minimumHeight: 650

    maximumWidth: 600
    maximumHeight: 650

    visible: true
    title: "UwpControl by Horeefee"

    UwpRemove {
        id: uwpRemoveModel
    }

    Rectangle {
        id: background

        anchors.fill: parent
        z: -1

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#0f0f0f" }
            GradientStop { position: 1.0; color: "#242424" }
        }
    }

    Row {
        id: appsR

        anchors.centerIn: parent
        spacing: 20

        Rectangle {
            id: uwpRemove
            color: "#242424"
            radius: 10
            width: 400
            height: 600

            ListView {
                id: appsView
                anchors.fill: parent
                anchors.margins: 15

                model: uwpRemoveModel
                clip: true

                delegate: Row {
                    width: appsView.width
                    height: 40
                    spacing: 20

                    Text {
                        id: uwpText
                        text: appName
                        anchors.verticalCenter: parent.verticalCenter

                        font.family: "Arial"
                        font.pixelSize: 14
                        font.bold: true
                        color: "grey"

                        width: parent.width - customCheckBoxContainer.width - parent.spacing
                        elide: Text.ElideRight
                    }

                    Rectangle {
                        id: customCheckBoxContainer
                        width: 25
                        height: 25
                        radius: 6
                        anchors.verticalCenter: parent.verticalCenter

                        readonly property bool isChecked: appChecked

                        color: isChecked ? "#4CAF50" : "#2a2a2a"
                        border.color: isChecked ? "#4CAF50" : "#555555"

                        Behavior on color { ColorAnimation { duration: 150 } }
                        Behavior on border.color { ColorAnimation { duration: 150 } }

                        TapHandler {
                            onTapped: {
                                appChecked = !appChecked;
                            }
                        }

                        Text {
                            text: "✓"
                            color: "white"
                            font.bold: true
                            font.pixelSize: 14
                            visible: customCheckBoxContainer.isChecked
                            anchors.centerIn: parent
                        }
                    }
                }
            }
        }

        Column {
            spacing: 20

            Rectangle {
                id: removeButton
                width: 80
                height: 50
                radius: 10

                TapHandler {
                    id: tapHandlerRemove
                    onTapped: {
                        uwpRemoveModel.deleteSelectedApps()
                    }
                }

                HoverHandler {
                    id: hoverHandlerRemove
                }

                color: tapHandlerRemove.pressed ? "#151515" :
                       (hoverHandlerRemove.hovered ? "#1f1f1f" : "#242424")

                Text {
                    text: "Remove"
                    font.family: "Arial"
                    font.pixelSize: 14
                    font.bold: true
                    color: "grey"
                    anchors.centerIn: parent
                }
            }
            Rectangle {
                id: selectAll
                width: 120
                height: 50
                radius: 10

                TapHandler {
                    id: tapHandlerSelectAll
                    onTapped: {
                        uwpRemoveModel.selectAll()
                    }
                }

                HoverHandler {
                    id: hoverHandlerSelectAll
                }

                color: tapHandlerSelectAll.pressed ? "#151515" :
                       (hoverHandlerSelectAll.hovered ? "#1f1f1f" : "#242424")

                Text {
                    text: "Select All"
                    font.family: "Arial"
                    font.pixelSize: 14
                    font.bold: true
                    color: "grey"
                    anchors.centerIn: parent
                }
            }
        }
    }
}