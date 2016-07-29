import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    property var data: null
    Rectangle {
        anchors.left: parent.left
        width: parent.width - data.load * parent.width
        height: parent.height
        color: "green"
        Label {
            x: Theme.paddingSmall
            text: data.name
            anchors.verticalCenter: parent.verticalCenter
            color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
        }
    }
}

