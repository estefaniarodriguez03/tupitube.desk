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

#ifndef TUPTOOLPLUGIN_H
#define TUPTOOLPLUGIN_H

#include "tglobal.h"
#include "tuptoolinterface.h"
#include "taction.h"
#include "tupprojectresponse.h"
#include "tcolorcell.h"
#include "tupbrushmanager.h"
#include "tupinputdeviceinformation.h"

#include <QObject>
#include <QSize>
#include <QCursor>
#include <QMenu>

class TupGraphicsScene;

class TUPITUBE_EXPORT TupToolPlugin : public QObject, public TupToolInterface
{
    Q_OBJECT
    Q_INTERFACES(TupToolInterface)
 
    public:
        enum Mode { Add = 1, Edit, View };
        enum EditMode { Selection = 0, Properties, None };

        explicit TupToolPlugin(QObject *parent = nullptr);
        ~TupToolPlugin();

        void setCurrentToolName(const QString &tool);
        QString currentToolName() const;

        void setToolId(TAction::ActionId code);
        TAction::ActionId toolId();

        virtual void init(TupGraphicsScene *scene);
        virtual void updateScene(TupGraphicsScene *scene);

        virtual void press(const TupInputDeviceInformation *input, TupBrushManager *brushManager, TupGraphicsScene *scene) = 0;
        virtual void doubleClick(const TupInputDeviceInformation *input, TupGraphicsScene *scene);
        virtual void move(const TupInputDeviceInformation *input, TupBrushManager *brushManager, TupGraphicsScene *scene) = 0;
        virtual void release(const TupInputDeviceInformation *input, TupBrushManager *brushManager, TupGraphicsScene *scene) = 0;
        
        virtual QMap<TAction::ActionId, TAction *> actions() const = 0;
        virtual TAction * getAction(TAction::ActionId toolId) = 0;

        virtual QWidget *configurator()  = 0;
        virtual void aboutToChangeTool() = 0;
        virtual void saveConfig() = 0;
        virtual void aboutToChangeScene(TupGraphicsScene *scene);

        virtual void sceneResponse(const TupSceneResponse *event);
        virtual void layerResponse(const TupLayerResponse *event);
        virtual void frameResponse(const TupFrameResponse *event);
        virtual void libraryResponse(const TupLibraryResponse *event);
        virtual void itemResponse(const TupItemResponse *event);

        virtual void keyPressEvent(QKeyEvent *event);
        virtual void keyReleaseEvent(QKeyEvent *event);

        virtual void updatePos(QPointF pos);
        virtual void updatePressure(qreal pressure);

        virtual QMenu *menu() const;
        virtual QCursor toolCursor();
        // QPair<int, int> setKeyAction(int key, Qt::KeyboardModifiers modifiers);

        virtual void resizeNode(qreal factor);
        virtual void updateZoomFactor(qreal factor);

        virtual void setProjectSize(const QSize size);

        virtual TupToolPlugin::Mode currentMode();
        virtual TupToolPlugin::EditMode currentEditMode();

        virtual void setActiveView(const QString &viewID);
        virtual void setCurrentItem(const QString &id);

        virtual void updateWorkSpaceContext();
        virtual void clearSelection();

        virtual void setColorMode(TColorCell::FillType mode);
        virtual void updateTextColor(const QColor &color);

        virtual void updateColorType(TColorCell::FillType fillType);
        virtual void refreshEyeDropperPanel();

    signals:
        void requested(const TupProjectRequest *request);
        
    public:
        // Events
        void begin();
        void end();
        
    private:
        QString currentTool;
        TAction::ActionId currentId;
};

#endif
