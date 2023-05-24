/***************************************************************************
 *   Project TUPITUBE DESK                                                *
 *   Project Contact: info@maefloresta.com                                 *
 *   Project Website: http://www.maefloresta.com                           *
 *   Project Leader: Gustav Gonzalez <info@maefloresta.com>                *
 *                                                                         *
 *   Developers:                                                           *
 *   2019:                                                                 *
 *    Alejandro Carrasco Rodríguez                                         *
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

#ifndef TCONTROLNODE_H
#define TCONTROLNODE_H

#include "tglobal.h"

#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QStyleOptionButton>
#include <QApplication>
#include <QCursor>
#include <QGraphicsPathItem>
#include <QGraphicsItem>
#include <QObject>
#include <QPointF>

class TNodeGroup;

class T_GUI_EXPORT TControlNode : public QGraphicsItem
{
    public:
        enum State { Pressed = 1, Released };
        TControlNode(int index, TNodeGroup *nodeGroup, const QPointF & pos = QPoint(0, 0),
                     QGraphicsItem * parent = nullptr, QGraphicsScene * scene = nullptr, int level = 0);
        
        ~TControlNode();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
        QRectF boundingRect() const;
        enum { Type = UserType + 100 };
        
        QPointF position();
        int type() const { return Type; }
        
        void setLeft(TControlNode *left);
        void setRight(TControlNode *right);
        void setCentralNode(TControlNode *node);
        int index() const;
        
        void setGraphicParent(QGraphicsItem * newParent);
        QGraphicsItem *graphicParent();
        
        TControlNode *left();
        TControlNode *right();
        TControlNode *centralNode();
        bool isCentralNode();
        
        void hasChanged(bool notChange);
        void resize(qreal factor);

    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
     
    private:
        void paintLinesToChildNodes(QPainter * painter);
        
    public slots:
        void showChildNodes(bool visible);
        void setSeletedChilds(bool select);
        
    signals:
        void showBrothers(bool show);
        
    private:
        QPointF nodePos;
        int nodeIndex;
        bool unchanged;
        QGraphicsItem *itemParent;
        QGraphicsScene *globalScene;
        TControlNode *cNode;
        TControlNode *leftNode;
        TControlNode *rightNode;
        TNodeGroup *nodeGroup;
};

#endif
