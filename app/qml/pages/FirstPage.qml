/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import "../libs"
import harbour.batterystats 1.0


Page {
    id: page
    SilicaListView {
        anchors.fill: parent
        spacing: Theme.paddingSmall

        header: Column {
            id: column
            width: parent.width

            PageHeader {
                id: header
                title: qsTr("Battery")
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: Math.floor(batteryDataModel.get(batteryDataModel.size - 1).capacity * 100) + "% - " +
                      (batteryDataModel.charging?"Charging":"Not charging")
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: toDate(batteryDataModel.get(batteryDataModel.size - 1).time) + " on battery"

                function toDate(time) {
                    var t = time;
                    var days = Math.floor(t / 86400);
                    t -= days * 86400;
                    var hours = Math.floor(t / 3600);
                    t -= hours * 3600;
                    var mins = Math.floor(t / 60);
                    t -= mins * 60;
                    var str;
                    if(days) {
                        str = str + days + "d ";
                    }
                    if(hours) {
                        str = str + hours + "h ";
                    }
                    if(mins) {
                        str = str + mins + "min ";
                    }
                    return str + t + "sec";
                }

                function chargingString(time)
                {
                    if (time < 0) return "Unknown"
                    return toDate(time);
                }
            }

            Label {
                id: prediction
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Time left till" + (batteryDataModel.charging?"full":"empty") + ": " + chargingString(batteryDataModel.prediction)
            }

            BatteryChart {
                id: batteryChart
                width: parent.width
                stockModel: batteryDataModel
            }

            Label {
                text: " "
            }
        }



        model: processDataModel
        delegate: ProcessItem {
            name: model.name
            load: model.load
        }

        Component.onCompleted: {
            log.update();
        }

        VerticalScrollDecorator {}
    }

    onStatusChanged: {
        if (status === PageStatus.Active && pageStack.depth === 1) {
            pageStack.pushAttached("SecondPage.qml", {});
        }
    }
}


