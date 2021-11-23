/***************************************************************************
 *   Project TUPITUBE DESK                                                *
 *   Project Contact: info@maefloresta.com                                 *
 *   Project Website: http://www.maefloresta.com                           *
 *   Project Leader: Gustav Gonzalez <info@maefloresta.com>                *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 *
 *    Gustavo Gonzalez / xtingray                                          *
 *                                                                         *
 *   KTooN's versions:                                                     * 
 *                                                                         *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       *
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Gustav Gonzalez - http://www.maefloresta.com       *
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

#ifndef POLYLINETOOL_H
#define POLYLINETOOL_H

#include "tglobal.h"
#include "tuptoolplugin.h"
#include "tuppathitem.h"
#include "polylinesettings.h"
#include "tcontrolnode.h"
#include "tnodegroup.h"

#include <QGraphicsView>
#include <QGraphicsPathItem>
#include <QGraphicsLineItem>
#include <QPainterPath>
#include <QDir>

class TUPITUBE_PLUGIN PolyLineTool : public TupToolPlugin
{    
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.maefloresta.tupi.TupToolInterface" FILE "polylinetool.json")
    
    public:
        PolyLineTool();
        virtual ~PolyLineTool();
        
        virtual void init(TupGraphicsScene *scene);
        virtual QList<TAction::ActionId> keys() const;
        virtual QMap<TAction::ActionId, TAction *>actions() const;
        TAction * getAction(TAction::ActionId toolId);
        int toolType() const;

        virtual void press(const TupInputDeviceInformation *input, TupBrushManager *brushManager, TupGraphicsScene *scene);
        virtual void move(const TupInputDeviceInformation *input, TupBrushManager *brushManager, TupGraphicsScene *scene);
        virtual void release(const TupInputDeviceInformation *input, TupBrushManager *brushManager, TupGraphicsScene *scene);
        
        virtual void keyPressEvent(QKeyEvent *event);
        virtual void keyReleaseEvent(QKeyEvent *event);

        virtual QWidget *configurator();
        void aboutToChangeScene(TupGraphicsScene *scene);
        virtual void aboutToChangeTool();
        virtual void saveConfig();
        virtual QCursor toolCursor(); //  const;

        void resizeNode(qreal scaleFactor);
        void updateZoomFactor(qreal scaleFactor);
        void clearSelection();
        void updatePos(QPointF pos);

    protected:
        virtual void itemResponse(const TupItemResponse *response);

    signals:
        void closeHugeCanvas();
        void callForPlugin(int menu, int index);

    private slots:
        void nodeChanged();
        // void initEnv(bool postInit = true);
        void initEnv();
        
    private:
        void setupActions();

        bool begin;
        QPointF center;
        QPointF right;
        QPointF mirror;
        QPointF lastPoint;

        TNodeGroup *nodeGroup;
        QPainterPath path;

        QMap<TAction::ActionId, TAction *> polyActions;

        TupPathItem *pathItem;
        TupGraphicsScene *scene;

        QGraphicsLineItem *line1;
        QGraphicsLineItem *line2;
        PolylineSettings *configPanel;
        QCursor cursor;
        qreal realFactor;
        bool cutterOn;
        bool movingOn;
};

#endif
