import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    property var ind: null
    property var data: null
    Rectangle {
        anchors.left: parent.left
        width: parent.width - 0.5 * parent.width
        height: parent.height
        color: "green"
        Label {
            x: Theme.paddingSmall
            text: qsTr("Item") + " " + ind
            anchors.verticalCenter: parent.verticalCenter
            color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
        }
    }
}

