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

#ifndef TACTION_H
#define TACTION_H

#include "tglobal.h"
#include "tactionmanager.h"

#include <QAction>
#include <QKeySequence>

class TActionManager;

class T_GUI_EXPORT TAction : public QAction
{
    Q_OBJECT

    public:
        enum ActionId { NoAction = -1, Pencil, Ink, Polyline, Rectangle, Ellipse, Line, FillTool,
                        Text, Motion, Rotation, Scale, Shear, Color, Opacity, ObjectSelection,
                        NodesEditor, LipSyncTool, Eraser, ExportImage };

        TAction(QObject * parent, const QString &id = QString());

        TAction(const QString &text, QObject *parent, const QString &id = QString());
        TAction(const QIcon &icon, const QString &text, QObject *parent, const QString &id = QString());
        TAction(const QIcon &icon, const QString &text, const QString &key, QObject *parent, const QString &id = QString());
        TAction(const QIcon &icon, QObject *parent, const QString &id = QString());
        TAction(const QIcon &icon,  const QKeySequence &key, QObject * parent, const QString &id = QString());
        TAction(const QIcon &icon, const QString &text, const QKeySequence &key, QObject *reciever, const char *slot,
                QObject *parent, const QString &id = QString());
        TAction(const QString &text, const QKeySequence &key, QObject *reciever, const char *slot, QObject *parent,
                const QString &id = QString());
        ~TAction();

        void setCursor(const QCursor &cursor);
        QCursor cursor() const;

        void setActionId(TAction::ActionId code);
        TAction::ActionId actionId();

    private:
        void initWithManager(TActionManager *parent, const QString &id);
        void init();
		
    private:
        QCursor m_cursor;
        ActionId id;
};

#endif
