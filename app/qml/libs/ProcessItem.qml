import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    id: item
    property var name: null
    property var load: null
    Rectangle {
        anchors.left: parent.left
        width: load * parent.width
        height: parent.height
        color: "green"
        Label {
            x: Theme.paddingSmall
            text: name
            anchors.verticalCenter: parent.verticalCenter
            color: item.highlighted ? Theme.highlightColor : Theme.primaryColor
        }
    }
}

