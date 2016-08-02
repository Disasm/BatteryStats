import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.batterystats 1.0


Page {
    id: page
    SilicaFlickable {
        VerticalScrollDecorator {}

        Column {
            Label {
                text: "Size: " + model.size
            }

            Button {
                text: "Update"
                onClicked: model.forceUpdate()
            }

            SilicaListView {
                model: processModel
                delegate: ListItem {
                    Label {
                        text: model.name + " (" + model.load + ")"
                    }
                }
            }
        }

        LogFile {
            id: log
            fileName: "/home/nemo/battery.log"
        }

        BatteryDataModel {
            id: model
            logFile: log
        }

        ProcessDataModel {
            id: processModel
        }


    }
}
