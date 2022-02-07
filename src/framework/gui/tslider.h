/***************************************************************************
 *   Project TUPITUBE DESK                                                 *
 *   Project Contact: info@maefloresta.com                                 *
 *   Project Website: http://www.maefloresta.com                           *
 *                                                                         *
 *   Developers:                                                           *
 *   2012:                                                                 *
 *    Gustavo Gonzalez / @xtingray                                         *
 *    Andres Calderon / @andresfcalderon                                   *
 *    Antonio Vanegas / @hpsaturn                                          *
 *                                                                         *
 *   TupiTube Desk is a fork of the KTooN project                          *
 *   KTooN's versions:                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       *
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2012 Mae Floresta - http://www.maefloresta.com          *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef TSLIDER_H
#define TSLIDER_H

#include "tglobal.h"

#include <QGraphicsView>
#include <QMouseEvent>
#include <QImage>

class TUPITUBE_EXPORT TSlider : public QGraphicsView
{
    Q_OBJECT

    public:
        enum Mode { Color = 0, Size, Opacity, FPS };

        explicit TSlider(Qt::Orientation orientation, Mode mode, const QColor& start, const QColor& end, QWidget *parent = nullptr);
        ~TSlider();

        void setBrushSettings(Qt::BrushStyle style, double opacity);
        void setRange(int min, int max);
        void setColors(const QColor& start, const QColor& end);
        void setValue(int pos);
        void setEnabled(bool flag);
        bool isEnabled();

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void paintEvent(QPaintEvent *event);

        void paintScales();

    signals:
        void valueChanged(int value);
        void colorChanged(const QColor &color);

    private:
       void calculateNewPosition(int pos);
       void calculateColor(int pos);

       int minRange;
       int maxRange;
       QColor startColor;
       QColor endColor;
       QImage *image;
       int imageW;
       int imageH;

       int value;
       bool enabled;
       Qt::Orientation sliderOrientation;
       TSlider::Mode sliderMode;
       Qt::BrushStyle sliderStyle;
       double sliderOpacity;
       int currentBase;
       int rangeR;
       int rangeG;
       int rangeB;
};

#endif
