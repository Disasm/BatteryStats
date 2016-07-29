import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    property var name: null
    property var load: null
    Rectangle {
        anchors.left: parent.left
        width: parent.width - load * parent.width
        height: parent.height
        color: "green"
        Label {
            x: Theme.paddingSmall
            text: name
            anchors.verticalCenter: parent.verticalCenter
            color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
        }
    }
}

