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
    width: 320
    height: 320
    color: "transparent"

    Canvas {
        id: canvas
        width: parent.width
        anchors.top: toDate.bottom
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

        function drawPrice(ctx, from, to, color, price, points, highest)
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
                var x = i * w;
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
            if (!stockModel.ready)
                return;

            first = stockModel.indexOf(chart.startDate);
            last = stockModel.indexOf(chart.endDate);

            // Uncomment the line below to see the range of model
            // item that gets painted based on the current settings.
            //console.log("Painting " + stockModel.stockId + " from " + chart.startDate + " to " + chart.endDate + ", indices [" + first + ".." + last + "], " + (first-last+1) + "/" + stockModel.count + " points");

            if (first >= 0 && last >= 0) {
                // Use the model's high-values to scale the price and volume charts.
                // Note that this may result in noticeably flat graphs if the loaded
                // timespan is much longer than the timespan shown. E.g. a high price
                // from a year back may effectively flatten the past week view.
                var highestPrice = stockModel.highestPrice;
                var highestVolume = stockModel.highestVolume;
                var points = [];

                // In the model the quotes are stored from newest to oldest
                // but we want our points array to contain quotes in chronological
                // order. Hence we walk the model backwards.
                for (var i = first; i >= last; i-=pixelSkip) {
                    var price = stockModel.get(i);
                    points.push({
                        open: price.open,
                        close: price.close,
                        high: price.high,
                        low: price.low,
                        volume:price.volume
                    });
                }

                // As a final sanity check, make sure the above resulted in more than one point.
                if (points.length > 1) {
                    if (settings.drawHighPrice)
                        drawPrice(ctx, first, last, settings.highColor, "high", points, highestPrice);
                    if (settings.drawLowPrice)
                        drawPrice(ctx, first, last, settings.lowColor, "low", points, highestPrice);
                    if (settings.drawOpenPrice)
                        drawPrice(ctx, first, last,settings.openColor, "open", points, highestPrice);
                    if (settings.drawClosePrice)
                        drawPrice(ctx, first, last, settings.closeColor, "close", points, highestPrice);
                    if (settings.drawVolume)
                        drawVolume(ctx, first, last, settings.volumeColor, "volume", points, highestVolume);
                    if (settings.drawKLine)
                        drawKLine(ctx, first, last, points, highestPrice);
                }
            }
        }
    }
}
