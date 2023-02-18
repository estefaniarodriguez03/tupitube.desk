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

#include "tupcommandexecutor.h"
#include "tupscene.h"
#include "tupprojectrequest.h"
#include "tuprequestbuilder.h"
#include "tuplayer.h"
#include "tupframe.h"
#include "tupprojectresponse.h"

bool TupCommandExecutor::createFrame(TupFrameResponse *response)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupCommandExecutor::createFrame()]";
    #endif

    int sceneIndex = response->getSceneIndex();
    int layerIndex = response->getLayerIndex();
    int pos = response->getFrameIndex();
    QString name = response->getArg().toString();

    TupScene *scene = project->sceneAt(sceneIndex);
    if (scene) {
        scene->insertStoryboardScene(pos);
        TupLayer *layer = scene->layerAt(layerIndex);
        if (layer) {
            if (response->getMode() == TupProjectResponse::Do) {
                TupFrame *frame = layer->createFrame(name, pos);
                if (frame) {
                    emit responsed(response);
                    return true;
                }
            }

            if (response->getMode() == TupProjectResponse::Redo || response->getMode() == TupProjectResponse::Undo) {
                bool success = layer->restoreFrame(pos);
                if (success) {
                    emit responsed(response);
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool TupCommandExecutor::removeFrame(TupFrameResponse *response)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupCommandExecutor::removeFrame()]";
    #endif

    int sceneIndex = response->getSceneIndex();
    int layerIndex = response->getLayerIndex();
    int pos = response->getFrameIndex();

    TupScene *scene = project->sceneAt(sceneIndex);
    if (scene) {
        scene->removeStoryboardScene(pos);
        TupLayer *layer = scene->layerAt(layerIndex);
        if (layer) {
            if (pos >= layer->framesCount())
                pos = layer->framesCount() - 1;

            TupFrame *frame = layer->frameAt(pos);
            if (frame) {
                QDomDocument doc;
                doc.appendChild(frame->toXml(doc));
                scene->removeTweensFromFrame(layerIndex, pos);

                if (layer->removeFrame(pos)) {
                    response->setState(doc.toString(0));
                    emit responsed(response);
                    return true;
                }
            }
        }
    }

    return false;
}

bool TupCommandExecutor::removeFrameSelection(TupFrameResponse *response)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupCommandExecutor::removeFrameSelection()]";
        qWarning() << "*** Selection -> " << response->getArg().toString();
    #endif

    int sceneIndex = response->getSceneIndex();
    int layerIndex = response->getLayerIndex();
    int frameIndex = response->getFrameIndex();
    QString selection = response->getArg().toString();

    QStringList blocks = selection.split(":");
    QStringList params = blocks.at(0).split(",");
    int layers = params.at(0).toInt();  
    int frames = params.at(1).toInt();

    TupScene *scene = project->sceneAt(sceneIndex);
    if (scene) {
        int layersTotal = layerIndex + layers;
        for (int i=layerIndex; i<layersTotal; i++) {
            TupLayer *layer = scene->layerAt(i);
            if (layer) {
                int framesTotal = frameIndex + frames;
                for (int j=frameIndex; j<framesTotal; j++) {
                    if (response->getMode() == TupProjectResponse::Do || response->getMode() == TupProjectResponse::Redo) {
                        if (frameIndex < layer->framesCount()) {
                            TupFrame *frame = layer->frameAt(frameIndex);
                            if (frame) {
                                scene->removeStoryboardScene(frameIndex);
                                scene->removeTweensFromFrame(i, frameIndex);

                                if (layer->framesCount() == 1) {
                                    if (!layer->resetFrame(frameIndex))
                                        return false; 
                                } else {
                                    if (!layer->removeFrame(frameIndex)) {
                                        #ifdef TUP_DEBUG
                                            qDebug() << "[TupCommandExecutor::removeFrameSelection()] - "
                                                        "Fatal Error: Can't remove frame at index: "
                                                     << frameIndex;
                                        #endif
                                        return false;
                                    }
                                }
                            }
                        }
                    } else {
                        int resetAction = layer->resettedFramesCount();
                        if (resetAction && layer->framesCount() == 1) {
                            bool success = layer->restoreResettedFrame(frameIndex);
                            if (!success) {
                                #ifdef TUP_DEBUG
                                    qDebug() << "[TupCommandExecutor::restoreFrameSelection()] "
                                                "- Fatal Error: Can't restore resetted frame at index 0";
                                #endif
                                return false;
                            }
                            resetAction = false;
                        } else {
                            bool success = layer->restoreFrame(frameIndex);
                            if (!success) {
                                #ifdef TUP_DEBUG
                                    qDebug() << "[TupCommandExecutor::restoreFrameSelection()] "
                                                "- Fatal Error: Can't restore frame at index: "
                                             << frameIndex;
                                #endif
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    emit responsed(response);
    return true; 
}

bool TupCommandExecutor::resetFrame(TupFrameResponse *response)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupCommandExecutor::resetFrame()]";
    #endif

    int sceneIndex = response->getSceneIndex();
    int layerIndex = response->getLayerIndex();
    int pos = response->getFrameIndex();
   
    TupScene *scene = project->sceneAt(sceneIndex);
    if (scene) {
        scene->resetStoryboardScene(pos);
        TupLayer *layer = scene->layerAt(layerIndex);
        if (layer) {
            if (response->getMode() == TupProjectResponse::Do || response->getMode() == TupProjectResponse::Redo) {
                TupFrame *frame = layer->frameAt(pos);
                if (frame) {
                    if (layer->resetFrame(pos)) {
                        emit responsed(response);
                        return true;
                    }
                }
            } else {
                if (layer->restoreResettedFrame(pos)) {
                    emit responsed(response);
                    return true;
                }
            }
        }
    }
   
    return false;
}

bool TupCommandExecutor::moveFrame(TupFrameResponse *response)
{
    int sceneIndex = response->getSceneIndex();
    int layerIndex = response->getLayerIndex();
    int pos = response->getFrameIndex();
    int newPosition = response->getArg().toInt();

    TupScene *scene = project->sceneAt(sceneIndex);
    if (scene) {
        scene->moveStoryboardScene(pos, newPosition); 
        TupLayer *layer = scene->layerAt(layerIndex);
        if (layer) {
            if (layer->moveFrame(pos, newPosition)) {
                emit responsed(response);
                return true;
            } else {
                #ifdef TUP_DEBUG
                    qDebug() << "[TupCommandExecutor::moveFrame()] - Fatal Error: Can't move frame!";
                #endif  
                return false;
            }
        }
    }
    
    return false;
}

bool TupCommandExecutor::exchangeFrame(TupFrameResponse *response)
{
    int sceneIndex = response->getSceneIndex();
    int layerIndex = response->getLayerIndex();
    int pos = response->getFrameIndex();
    int newPosition = response->getArg().toInt();

    TupScene *scene = project->sceneAt(sceneIndex);
    if (scene) {
        scene->moveStoryboardScene(pos, newPosition);
        TupLayer *layer = scene->layerAt(layerIndex);
        if (layer) {
            if (layer->exchangeFrame(pos, newPosition)) {
                emit responsed(response);
                return true;
            } else {
                #ifdef TUP_DEBUG
                    qDebug() << "[TupCommandExecutor::exchangeFrame()] - Error while exchanging frames";
                #endif  
                return false;
            }
        }
    }
   
    return false;
}

/*
bool TupCommandExecutor::lockFrame(TupFrameResponse *response)
{
    int sceneIndex = response->sceneIndex();
    int layerIndex = response->layerIndex();
    int pos = response->frameIndex();
    bool lock = response->arg().toBool();

    TupScene *scene = m_project->sceneAt(sceneIndex);
    if (scene) {
        TupLayer *layer = scene->layerAt(layerIndex);
        if (layer) {
            TupFrame *frame = layer->frameAt(pos);
            if (frame) {
                frame->setLocked(lock);
                emit responsed(response);
                return true;
            }
        }
    }
    
    return false;
}
*/

bool TupCommandExecutor::renameFrame(TupFrameResponse *response)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupCommandExecutor::renameFrame()]";
    #endif	

    int sceneIndex = response->getSceneIndex();
    int layerIndex = response->getLayerIndex();
    int pos = response->getFrameIndex();
    QString newName = response->getArg().toString();

    TupScene *scene = project->sceneAt(sceneIndex);
    if (scene) {
        TupLayer *layer = scene->layerAt(layerIndex);
        if (layer) {
            TupFrame *frame = layer->frameAt(pos);
            if (frame) { 
                QString oldName = frame->getFrameName();
                if (oldName.compare(newName) != 0)
                    frame->setFrameName(newName);
        
                emit responsed(response);
                return true;
            }
        }
    }
    
    return false;
}


bool TupCommandExecutor::selectFrame(TupFrameResponse *response)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupCommandExecutor::selectFrame()]";
    #endif	
	
    int sceneIndex = response->getSceneIndex();
    int layerIndex = response->getLayerIndex();
    int pos = response->getFrameIndex();

    if (sceneIndex < 0 || pos < 0)
        return false;

    TupScene *scene = project->sceneAt(sceneIndex);
    if (scene) {
        TupLayer *layer = scene->layerAt(layerIndex);
        if (layer) {
            if (pos < layer->framesCount()) {
                TupFrame *frame = layer->frameAt(pos);
                if (frame) {
                    emit responsed(response);
                    return true;
                } else {
                    #ifdef TUP_DEBUG
                        qDebug() << "[TupCommandExecutor::selectFrame()]"
                                    " - Invalid frame index -> "
                                 << pos;
                    #endif
                    return false;
                }
            }
        }
    } 
    
    return false;
}

bool TupCommandExecutor::setFrameVisibility(TupFrameResponse *response)
{
    int sceneIndex = response->getSceneIndex();
    int layerIndex = response->getLayerIndex();
    int pos = response->getFrameIndex();
    bool view = response->getArg().toBool();
    TupScene *scene = project->sceneAt(sceneIndex);

    if (scene) {
        TupLayer *layer = scene->layerAt(layerIndex);
        if (layer) {
            TupFrame *frame = layer->frameAt(pos);
            if (frame) { 
                frame->setVisible(view);
                emit responsed(response);
                return true;
            }
        }
    }
    
    return false;
}

bool TupCommandExecutor::extendFrame(TupFrameResponse *response)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupCommandExecutor::extendFrame()]";
    #endif

    int sceneIndex = response->getSceneIndex();
    int layerIndex = response->getLayerIndex();
    int frameIndex = response->getFrameIndex();
    int pos = response->getFrameIndex();
    int times = response->getArg().toInt();

    TupScene *scene = project->sceneAt(sceneIndex);
    if (scene) {
        TupLayer *layer = scene->layerAt(layerIndex);
        if (layer) {
            if (response->getMode() == TupProjectResponse::Do || response->getMode() == TupProjectResponse::Redo) {
                if (layer->extendFrame(pos, times)) {
                    for (int i=0; i<times; i++)
                        scene->insertStoryboardScene(pos + i);
                    emit responsed(response);
                    return true;
                }
            } else {
                for (int i=0; i<times;i++) {
                    if (!layer->removeFrame(frameIndex))
                        return false;
                }
                emit responsed(response);
                return true;
            }
        }
    }

    return false;
}

bool TupCommandExecutor::copyFrameSelection(TupFrameResponse *response)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupCommandExecutor::copyFrameSelection()]";
    #endif

    selectionFramesCopy.clear();
    int sceneIndex = response->getSceneIndex();
    QString selection = response->getArg().toString();
    QStringList params = selection.split(",");

    if (params.count() == 4) {
        copyParams = params;
        int initLayer = params.at(0).toInt();
        int endLayer = params.at(1).toInt();
        int initFrame = params.at(2).toInt();
        int endFrame = params.at(3).toInt();

        TupScene *scene = project->sceneAt(sceneIndex);
        if (scene) {
            for (int i=initLayer; i<=endLayer; i++) {
                 TupLayer *layer = scene->layerAt(i);
                 if (layer) {
                     for (int j=initFrame; j<=endFrame; j++) {
                         TupFrame *frame = layer->frameAt(j);
                         if (frame) {
                             QDomDocument doc;
                             doc.appendChild(frame->toXml(doc));
                             selectionFramesCopy << doc.toString();
                         } else {
                             return false;
                         }
                     }
                 } else {
                     return false;
                 }
            }

            emit responsed(response);
            return true;
        }
    }

    return false;
}

bool TupCommandExecutor::pasteFrameSelection(TupFrameResponse *response)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupCommandExecutor::pasteFrameSelection()]";
    #endif

    int sceneIndex = response->getSceneIndex();
    int layerIndex = response->getLayerIndex();
    int pos = response->getFrameIndex();

    if (copyParams.count() == 4) {
        int layerLimit = layerIndex + (copyParams.at(1).toInt() - copyParams.at(0).toInt()) + 1;
        int frameLimit = pos + (copyParams.at(3).toInt() - copyParams.at(2).toInt()) + 1;

        TupScene *scene = project->sceneAt(sceneIndex);
        if (scene) {
            if (response->getMode() == TupProjectResponse::Do || response->getMode() == TupProjectResponse::Redo) {
                int index = 0;
                for (int i=layerIndex; i<layerLimit; i++) {
                    if (i < scene->layersCount()) {
                        TupLayer *layer = scene->layerAt(i);
                        if (layer) {
                            for (int j=pos; j<frameLimit; j++) {
                                TupFrame *frame = new TupFrame(layer);
                                frame->fromXml(selectionFramesCopy.at(index));
                                layer->setFrame(j, frame);
                                index++;
                            }
                        } else {
                            return false;
                        }
                    }
                }
            } else {
                for (int i=layerIndex; i<layerLimit; i++) {
                    TupLayer *layer = scene->layerAt(i);
                    if (layer) {
                        for (int j=pos; j<frameLimit; j++) {
                            if (pos < layer->framesCount()) {
                                TupFrame *frame = layer->frameAt(pos);
                                if (frame) {
                                    // SQA: Check if these instructions are required in this context 
                                    scene->removeStoryboardScene(pos);
                                    scene->removeTweensFromFrame(i, pos);

                                    if (layer->framesCount() == 1) {
                                        if (!layer->resetFrame(pos))
                                            return false;
                                    } else {
                                        if (!layer->removeFrame(pos)) {
                                            #ifdef TUP_DEBUG
                                                qDebug() << "[TupCommandExecutor::pasteFrameSelection()] - "
                                                            "Fatal Error: Can't remove frame at index -> "
                                                         << pos;
                                            #endif
                                            return false;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            response->setArg(copyParams.at(0) + "," + copyParams.at(1) + "," 
                             + copyParams.at(2) + "," + copyParams.at(3));
            emit responsed(response);
            return true;
        }
    }

    return false;
}

bool TupCommandExecutor::reverseFrameSelection(TupFrameResponse *response)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupCommandExecutor::reverseFrameSelection()]";
    #endif

    selectionFramesCopy.clear();
    int sceneIndex = response->getSceneIndex();
    QString selection = response->getArg().toString();
    QStringList params = selection.split(",");

    if (params.count() == 4) {
        TupScene *scene = project->sceneAt(sceneIndex);
        if (scene) {
            int initLayer = params.at(0).toInt();
            int endLayer = params.at(1).toInt();
            int initFrame = params.at(2).toInt();
            int endFrame = params.at(3).toInt();
            int segment = endFrame - initFrame;
            int iterations = 1;
            if (segment > 1)
                iterations = (segment + 1) / 2;

            /*
            tError() << "TupCommandExecutor::reverseFrameSelection() - initFrame: " << initFrame;
            tError() << "TupCommandExecutor::reverseFrameSelection() - endFrame: " << endFrame;
            tError() << "TupCommandExecutor::reverseFrameSelection() - segment: " << segment;
            tError() << "TupCommandExecutor::reverseFrameSelection() - iterations: " << iterations;
            */

            for (int i=initLayer; i<=endLayer; i++) {
                 TupLayer *layer = scene->layerAt(i);
                 if (layer) {
                     int initIndex = initFrame;
                     int endIndex = endFrame;
                     for (int j=0; j<iterations; j++) {
                         if (!layer->exchangeFrame(initIndex, endIndex)) {
                             #ifdef TUP_DEBUG
                                 qDebug() << "[TupCommandExecutor::reverseFrameSelection()] - Fatal Error: Can't exchange frames!";
                             #endif

                             return false;
                         }
                         initIndex++;
                         endIndex--;
                     }
                 } else {
                     return false;
                 }
            }
            emit responsed(response);

            return true;
        }
    }

    return false;
}
