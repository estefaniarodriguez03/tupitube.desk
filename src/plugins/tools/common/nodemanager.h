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

#ifndef NODEMANAGER_H
#define NODEMANAGER_H

#include "tglobal.h"
#include "node.h"

#include <QHash>
#include <QGraphicsItem>
#include <QGraphicsScene>

class TUPITUBE_PLUGIN NodeManager : public QObject
{
    Q_OBJECT

    public:
        NodeManager(Node::Context context, QGraphicsItem *parent, QGraphicsScene *scene, int zValue);
        ~NodeManager();

        void syncNodes(const QRectF &rect);
        void syncNodesFromParent();
        QGraphicsItem *parentItem() const;
        bool isModified() const;
        
        void beginToEdit();
        void restoreItem();
        
        void show();
        
        void scale(qreal sx, qreal sy);
        void rotate(double angle);
        void horizontalFlip();
        void verticalFlip();
        void crossedFlip();
        
        void setPressedStatus(bool isPressed);
        bool isPressed();
        
        void toggleAction();
        
        void setActionNode(Node::NodeAction action);
        
        void setVisible(bool visible);
        
        double rotation();

        void setProportion(bool flag);
        bool proportionalScale();
        void clear();

        void resizeNodes(qreal factor);

    signals:
        void positionUpdated(const QPointF &point);
        void rotationUpdated(int angle);
        void scaleUpdated(double x, double y);
        void transformationUpdated();

    private:
        QHash<Node::NodeType, Node *> nodes;
        QGraphicsItem *parent;
        QGraphicsScene *scene;

        QTransform origTransform;
        QPointF origPos;

        bool pressed;
        bool proportional;
        qreal rotationValue;
        double scaleX;
        double scaleY;
};

#endif
