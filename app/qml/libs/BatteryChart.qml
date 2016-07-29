/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0

Rectangle {
    id: chart
    width: parent.width
    height: 400
    color: "transparent"
    property var stockModel: null

    Canvas {
        id: canvas
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        renderTarget: Canvas.Image
        antialiasing: true
        property int frames: first
        property int mouseX: 0
        property int mouseY: 0
        property int mousePressedX: 0
        property int mousePressedY: 0
        property int movedY: 0
        property real scaleX: 1.0
        property real scaleY: 1.0
        property int first: 0
        property int last: 0

        property int pixelSkip: 1

        function drawBackground(ctx) {
            ctx.save();
            ctx.fillStyle = "#272822";
            ctx.fillRect(0, 0, canvas.width, canvas.height);
            ctx.strokeStyle = "#423a2f";
            ctx.beginPath();
            for (var i = 0; i < 10; i++) {
                ctx.moveTo(0, i * (canvas.height/10.0));
                ctx.lineTo(canvas.width, i * (canvas.height/10.0));
            }

            for (i = 0; i < 12; i++) {
                ctx.moveTo(i * (canvas.width/12.0), 0);
                ctx.lineTo(i * (canvas.width/12.0), canvas.height);
            }
            ctx.stroke();

            ctx.strokeStyle = "#5c7a37";
            ctx.beginPath();
            ctx.moveTo(8 * (canvas.width/12.0), 0);
            ctx.lineTo(8 * (canvas.width/12.0), canvas.height);
            ctx.stroke();

            ctx.restore();
        }

        function drawPrice(ctx, color, price, points, lowest, highest, elapsed)
        {
            ctx.save();
            ctx.globalAlpha = 0.7;
            ctx.strokeStyle = color;
            ctx.lineWidth = 1;
            ctx.beginPath();

            var w = canvas.width/points.length;
            var end = points.length;
            for (var i = 0; i < end; i+=pixelSkip) {
                var value = points[i][price];
                var x = i * canvas.width * points[i][elapsed];
                var y = canvas.height * (1.0 - value/highest);

                if (i == 0) {
                    ctx.moveTo(x+w/2, y);
                } else {
                    ctx.lineTo(x+w/2, y);
                }
            }
            ctx.stroke();
            ctx.restore();
        }

        function drawKLine(ctx, from, to, points, highest)
        {
            ctx.save();
            ctx.globalAlpha = 0.4;
            ctx.lineWidth = 2;
            var w1 = canvas.width/points.length;
            var end = points.length;
            for (var i = 0; i < end; i+=pixelSkip) {
                var x = i * w1;
                var open = canvas.height * (1.0 - points[i].open/highest) - canvas.movedY;
                var close = canvas.height * (1.0 - points[i].close/highest) - canvas.movedY;
                var high = canvas.height * (1.0 - points[i].high/highest) - canvas.movedY;
                var low = canvas.height * (1.0 - points[i].low/highest) - canvas.movedY;

                var top, bottom;

                // Note that here we're comparing the coordinates calculated above, not
                // the actual values from point[i]. This means that if close is greater
                // that open, the closing price is actually lower than the opening price.
                if (close >= open) {
                    ctx.fillStyle = Qt.rgba(1, 0, 0, 1);
                    ctx.strokeStyle = Qt.rgba(1, 0, 0, 1);
                    top = close;
                    bottom = open;
                } else {
                    ctx.fillStyle = Qt.rgba(0, 1, 0, 1);
                    ctx.strokeStyle = Qt.rgba(0, 1, 0, 1);
                    top = open;
                    bottom = close;
                }

                var w2 = w1 > 10 ? w1/2 : w1;

                ctx.fillRect(x + (w1 - w2)/2, top, w2, bottom - top);
                ctx.beginPath();
                ctx.moveTo(x+w1/2, high);
                ctx.lineTo(x+w1/2, low);
                ctx.stroke();
            }
            ctx.restore();
        }

        function drawVolume(ctx, from, to, color, price, points, highest)
        {
            ctx.save();
            ctx.fillStyle = color;
            ctx.globalAlpha = 0.6;
            ctx.strokeStyle = Qt.rgba(0.8, 0.8, 0.8, 1);
            ctx.lineWidth = 1;

            var w = canvas.width/points.length;
            var end = points.length;
            for (var i = 0; i < end; i+=pixelSkip) {
                var value = points[i][price];
                var x = i * w;
                var y = canvas.height * (1.0 - value/highest);

                ctx.fillRect(x, y, w, canvas.height - y);
            }
            ctx.restore();
        }

        onPaint: {
            var ctx = canvas.getContext("2d");

            ctx.globalCompositeOperation = "source-over";
            ctx.lineWidth = 1;

            drawBackground(ctx);
            if(stockModel.size > 1) {
                var elapsed = stockModel.get(stockModel.size - 1).time;
                var oldCharging = stockModel.get(0).charging;
                for (var i = 1; i < stockModel.size; i+=1) {
                    var prevmetric = stockModel.get(i - 1);
                    var metric = stockModel.get(i);
                    ctx.save();
                    ctx.globalAlpha = 0.7;
                    ctx.lineWidth = 3;
                    ctx.beginPath();
                    ctx.moveTo(canvas.width * prevmetric.time / elapsed, canvas.height * (1 - prevmetric.capacity));
                    if(metric.charging == true) {
                        ctx.strokeStyle = Qt.rgba(1, 0, 0, 1);
                    } else {
                        ctx.strokeStyle = Qt.rgba(0, 1, 0, 1);
                    }
                    ctx.lineTo(canvas.width * metric.time / elapsed, canvas.height * (1 - metric.capacity));
                    ctx.stroke();
                }
            }
            ctx.restore();
        }
    }
}
