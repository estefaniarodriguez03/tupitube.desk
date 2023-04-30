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

#ifndef TUPPATHITEM_H
#define TUPPATHITEM_H

#include "tupabstractserializable.h"

#include <QGraphicsPathItem>
#include <QMimeData>
#include <QBrush>
#include <QGraphicsSceneDragDropEvent>
#include <QPainter>
#include <QPainterPath>

class TUPITUBE_EXPORT TupPathItem : public TupAbstractSerializable, public QGraphicsPathItem
{
    public:
        TupPathItem(QGraphicsItem *parent = nullptr);
        ~TupPathItem();
        
        virtual void fromXml(const QString &xml);
        virtual QDomElement toXml(QDomDocument &doc) const;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
        bool contains(const QPointF &point) const;
        bool isNotEdited();
        void saveOriginalPath();
        QString pathToString() const;
        void setPathFromString(const QString &route);
        void undoPath();
        void redoPath();

        QString refactoringPath(NodePosition policy, int nodesTotal);
        QString pathRestored(int nodesTotal) const;
        void resetPathHistory();
 
    protected:
        virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
        virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
        virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
        
    private:
        bool dragOver;
        QList<QString> undoList;
        QList<QString> doList;
        QHash<int, QString> pathCollection;
};

#endif
