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

#include "tuplibraryfolder.h"
#include "tuplibraryobject.h"
#include "tupprojectloader.h"
#include "tupproject.h"

TupLibraryFolder::TupLibraryFolder(const QString &key, TupProject *animation, QObject *parent) : QObject(parent)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder()]";
    #endif

    id = key;
    project = animation;
    loadingProject = false;
}

TupLibraryFolder::~TupLibraryFolder()
{
}

TupLibraryObject *TupLibraryFolder::createSymbol(TupLibraryObject::ObjectType type, const QString &name, const QByteArray &data,
                                                 const QString &folder, bool loaded)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::createSymbol()]";
        qDebug() << "*** Creating symbol -> " << name;
        qDebug() << "*** type -> " << type;
        qDebug() << "*** folder -> " << folder;
        qDebug() << "*** size -> " << data.size();
        qDebug() << "---";
    #endif

    if (data.isNull()) {
        #ifdef TUP_DEBUG
            qDebug() << "[TupLibraryFolder::createSymbol()] - Fatal Error: Data is null!";
        #endif
        return nullptr;
    }

    if (data.isEmpty()) {
        #ifdef TUP_DEBUG
            qDebug() << "[TupLibraryFolder::createSymbol()] - Fatal Error: Data is empty!";
        #endif
        return nullptr;
    }

    TupLibraryObject *object = new TupLibraryObject(name, folder, type, this);
    if (!object->loadRawData(data)) {
        #ifdef TUP_DEBUG
            qDebug() << "[TupLibraryFolder::createSymbol()] - Fatal Error: Object have no data raw!";
        #endif

        delete object;
        return nullptr;
    }

    bool ret;
    if (folder.length() == 0)
        ret = addObject(object);
    else
        ret = addObject(folder, object);

    bool success = object->saveData(project->getDataDir());
    if (success) {
        if (type == TupLibraryObject::Sound) {
            SoundResource record;
            record.frame = object->frameToPlay();
            record.path = object->getDataPath();
            record.muted = object->isMuted();
            soundRecords << record;
        }

        if (loaded && ret)
            TupProjectLoader::createSymbol(type, name, id, data, project);

        return object;
    }

    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::createSymbol()] - Fatal Error: Object couldn't be saved!";
    #endif

    return nullptr;
}

bool TupLibraryFolder::addObject(TupLibraryObject *object)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::addObject()] - Adding object -> " << object->getSymbolName();
    #endif

    if (!objects.contains(object->getSymbolName())) {
        objects.insert(object->getSymbolName(), object);
        return true;
    }

    return false;
}

bool TupLibraryFolder::addObject(const QString &folderName, TupLibraryObject *object)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::addObject()] - folderName -> " << folderName;
    #endif

    foreach (TupLibraryFolder *folder, folders) {
        if (folder->getId().compare(folderName) == 0) {
            LibraryObjects bag = folder->getObjects();
            if (!bag.contains(object->getSymbolName())) {
                folder->addObject(object);
                return true;
            }
        }
    }

    return false;
}

bool TupLibraryFolder::reloadObject(const QString &key, LibraryObjects bag)
{
    QStringList keys = bag.keys();
    foreach (QString oid, keys) {
        if (oid.compare(key) == 0) {
            QString path = bag[key]->getDataPath();
            if (QFile::exists(path))
                return bag[key]->loadData(path);
        }
    }

    return false;
}

bool TupLibraryFolder::reloadObject(const QString &key)
{
    bool found = reloadObject(key, objects);
    if (found)
        return true;

    foreach (TupLibraryFolder *folder, folders) {
        LibraryObjects bag = folder->getObjects();
        found = reloadObject(key, bag);
        if (found)
            return true;
    }

    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::reloadObject()] - Fatal Error: Object ID wasn't found -> " << key;
    #endif

    return false;
}

bool TupLibraryFolder::addFolder(TupLibraryFolder *folder)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::addFolder()]";
    #endif

    if (!folders.contains(folder->getId())) {
        folders.insert(folder->getId(), folder);
        return true;
    }

    return false;
}

bool TupLibraryFolder::removeObject(const QString &key, bool absolute)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::removeObject()] - key -> " << key;
    #endif

    QStringList keys = objects.keys();
    foreach (QString oid, keys) {
        if (oid.compare(key) == 0) {
            QString path = objects[key]->getDataPath();
            if (absolute) {
                QFileInfo finfo(path);
                if (finfo.isFile())
                    QFile::remove(path);
            }
            return objects.remove(key);
        }
    }

    foreach (TupLibraryFolder *folder, folders) {
        TupLibraryObject *object = folder->getObject(key);
        if (object)
            return folder->removeObject(key, absolute);
    }

    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::removeObject()] - Fatal Error: Object ID wasn't found -> " << key;
    #endif

    return false;
}

bool TupLibraryFolder::removeFolder(const QString &key)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::removeFolder()]";
    #endif

    if (folders.contains(key)) {
        TupLibraryFolder *folder = getFolder(key);
        if (folder) { 
            LibraryObjects objects = folder->getObjects();
            QStringList keys = objects.keys();
            foreach (QString oid, keys) {
                if (folder->removeObject(oid, true)) {
                    TupLibraryObject::ObjectType extension = static_cast<TupLibraryObject::ObjectType>(objects[oid]->getObjectType());
                    if (extension != TupLibraryObject::Item) {
                        if (!project->removeSymbolFromFrame(oid, extension))
                            return false;
                    }
                }
            }

            bool result = folders.remove(key);
            return result;
        }
    }

    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::removeFolder()] - Fatal Error: Folder wasn't found -> " << key;
    #endif

    return false;
}

bool TupLibraryFolder::moveObject(const QString &key, const QString &dirTarget)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::moveObject()] - key -> " << key << " - folder -> " << dirTarget;
    #endif

    TupLibraryObject *object = getObject(key);
    if (object) {
        if (removeObject(key, false)) {
            foreach (TupLibraryFolder *folder, folders) {
                if (folder->getId().compare(dirTarget) == 0) {
                    #ifdef TUP_DEBUG
                        qDebug() << "[TupLibraryFolder::moveObject()] - folder found!";
                    #endif
                    object->updateFolder(project->getDataDir(), dirTarget);
                    folder->addObject(object);
                    return true;
                }
            }
        }
    }

    return false;
}

bool TupLibraryFolder::moveObjectToRoot(const QString &key)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::moveObjectToRoot()] - key -> " << key;
    #endif

    TupLibraryObject *object = getObject(key);
    if (object) {
        if (removeObject(key, false)) {
            object->updateFolder(project->getDataDir());
            addObject(object);
            return true;
        }
    }

    return false;
}

void TupLibraryFolder::setId(const QString &key)
{
    id = key;
}

QString TupLibraryFolder::getId() const
{
    return id;
}

bool TupLibraryFolder::exists(const QString &key)
{
    QStringList keys = objects.keys();
    foreach (QString oid, keys) {
        if (oid.compare(key) == 0)
            return true;
    }

    foreach (TupLibraryFolder *folder, folders) {
        if (folder->exists(key))
            return true;
    }

    #ifdef TUP_DEBUG
        qWarning() << "[TupLibraryFolder::exists()] - Warning: Object doesn't exist -> " << key;
    #endif

    return false;
}

TupLibraryObject *TupLibraryFolder::getObject(const QString &key) const
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::getObject()] - object key -> " << key;
    #endif

    QStringList keys = objects.keys();
    foreach (QString oid, keys) {
        if (oid.compare(key) == 0) {
            #ifdef TUP_DEBUG
                qDebug() << "[TupLibraryFolder::getObject()] - Found it at folder -> " << id;
            #endif
            return objects[oid];
        }
    }

    foreach (TupLibraryFolder *folder, folders) {
        TupLibraryObject *object = folder->getObject(key);
        if (object) {
            #ifdef TUP_DEBUG
                qDebug() << "[TupLibraryFolder::getObject()] - Found it at folder -> " << folder->getId();
            #endif
            return object;
        }
    }
    
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::getObject()] - Fatal Error: Can't get object with id -> " << key;
    #endif

    return nullptr;
}

QString TupLibraryFolder::getObjectPath(const QString &objectKey) const
{
    TupLibraryObject *object = getObject(objectKey);
    if (object)
        return object->getDataPath();

    return "";
}

TupLibraryFolder *TupLibraryFolder::getFolder(const QString &key) const
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::getFolder()] - folder key -> " << key;
    #endif

    foreach (TupLibraryFolder *folder, folders) {
        if (folder->getId().compare(key) == 0)
            return folder;
    }

    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::getFolder()] - Fatal Error: Can't find folder with id -> " << key;
    #endif
   
    return nullptr;
}

bool TupLibraryFolder::folderExists(const QString &key) const
{
    foreach (TupLibraryFolder *folder, folders) {
        if (folder->getId().compare(key) == 0)
            return true;
    }
  
    #ifdef TUP_DEBUG
        qWarning() << "[TupLibraryFolder::folderExists()] - Fatal Error: Can't find folder with id -> " << key;
    #endif
  
    return false;
}

bool TupLibraryFolder::renameObject(const QString &folder, const QString &oldId, const QString &newId)
{
    TupLibraryObject *object = getObject(oldId);

    if (object) {
        removeObject(oldId, false);
        object->setSymbolName(newId);

        if (folder.length() > 0)
            return addObject(folder, object);
        else
            return addObject(object);   
    } 

    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::renameObject()] - Fatal Error: Object not found -> " << oldId;
    #endif

    return false;
}

bool TupLibraryFolder::renameFolder(const QString &oldId, const QString &newId)
{
    TupLibraryFolder *folder = getFolder(oldId);

    if (folder) {
        folders[oldId]->setId(newId);
        return true;
    } 

    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::renameFolder()] - Fatal Error: Folder not found -> " << oldId;
    #endif

    return false;
}

int TupLibraryFolder::objectsCount() const
{
    return objects.count();
}

int TupLibraryFolder::foldersCount() const
{
    return folders.count();
}

TupProject *TupLibraryFolder::getProject() const
{
    return project;
}

Folders TupLibraryFolder::getFolders() const
{
    return folders;
}

LibraryObjects TupLibraryFolder::getObjects() const
{
    return objects;
}

LibraryObjects TupLibraryFolder::getSoundObjects() const
{
    LibraryObjects items;

    foreach (TupLibraryObject *object, objects) {
        if (object->getObjectType() == TupLibraryObject::Sound)
            items[object->getSymbolName()] = object;
    }

    foreach (TupLibraryFolder *folder, folders) {
        LibraryObjects media = folder->getObjects();
        foreach (TupLibraryObject *object, media) {
            if (object->getObjectType() == TupLibraryObject::Sound)
                items[object->getSymbolName()] = object;
        }
    }

    return items;
}

void TupLibraryFolder::fromXml(const QString &xml)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::fromXml()]";
    #endif

    loadingProject = true;

    QDomDocument document;
    if (!document.setContent(xml))
        return;
    
    QDomElement root = document.documentElement();
    QDomNode domNode = root.firstChild();
    
    while (!domNode.isNull()) {
        QDomElement e = domNode.toElement();
        
        if (!e.isNull()) {
            if (e.tagName() == "object") {
                loadItem(getId(), domNode);
            } else if (e.tagName() == "folder") {
                QDomDocument folderDocument;
                folderDocument.appendChild(folderDocument.importNode(domNode, true));

                TupLibraryFolder *folder = new TupLibraryFolder(e.attribute("id"), project, this);
                addFolder(folder);

                TupProjectLoader::createSymbol(TupLibraryObject::Folder, e.attribute("id"), QString(), 
                                               "FOLDER",  project);

                // Loading the objects inside this folder
                loadObjects(e.attribute("id"), folderDocument.toString(0));
           }
        }

        domNode = domNode.nextSibling();
    }

    loadingProject = false;
}

QDomElement TupLibraryFolder::toXml(QDomDocument &doc) const
{
    QDomElement folder = doc.createElement("folder");
    folder.setAttribute("id", id);

    foreach (TupLibraryFolder *folderObject, folders)
        folder.appendChild(folderObject->toXml(doc));

    QList<TupLibraryObject *> values = objects.values();
    foreach (TupLibraryObject *object, values)
        folder.appendChild(object->toXml(doc));

    return folder;
}

void TupLibraryFolder::loadObjects(const QString &folder, const QString &xml)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::loadObjects()] - Folder -> " << folder;
    #endif

    QDomDocument document;

    if (!document.setContent(xml))
        return;

    QDomElement root = document.documentElement();
    QDomNode domNode = root.firstChild();

    while (!domNode.isNull()) {
        QDomElement e = domNode.toElement();
    
        if (!e.isNull()) {
            if (e.tagName() == "object")
                loadItem(folder, domNode);
        }
        domNode = domNode.nextSibling();
    }
}

void TupLibraryFolder::loadItem(const QString &folder, QDomNode xml)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::loadItem()] - Folder -> " << folder;
    #endif

    QDomDocument objectDocument;
    objectDocument.appendChild(objectDocument.importNode(xml, true));

    TupLibraryObject *object = new TupLibraryObject(this);
    object->fromXml(objectDocument.toString(0));

    switch (object->getObjectType()) {
        case TupLibraryObject::Image:
        case TupLibraryObject::Svg:
        case TupLibraryObject::Item:
        {
            if (!object->loadDataFromPath(project->getDataDir())) {
                #ifdef TUP_DEBUG
                    qDebug() << "[TupLibraryFolder::loadItem()] - Error: Graphic object not found -> " << object->getSymbolName();
                #endif
                return;
            }
        }
        break;
        case TupLibraryObject::Sound:
        {
            if (object->loadDataFromPath(project->getDataDir())) {
                if (object->isSoundResource()) {
                    SoundResource record;
                    record.frame = object->frameToPlay();
                    record.path = object->getDataPath();
                    record.muted = object->isMuted();
                    soundRecords << record;
                }
            } else {
                #ifdef TUP_DEBUG
                    qDebug() << "[TupLibraryFolder::loadItem()] - Error: Sound object not found -> " << object->getSymbolName();
                #endif
                return;
            }
        }
        break;
        default:
        break;
    }

    if (folder.compare("library") == 0)
        addObject(object);
    else
        addObject(folder, object);

    QDomElement objectData = objectDocument.documentElement().firstChild().toElement();
    QString data;

    if (!objectData.isNull()) {
        QTextStream ts(&data);
        ts << objectData;
    }

    TupProjectLoader::createSymbol(TupLibraryObject::ObjectType(object->getObjectType()),
                                   object->getSymbolName(), folder, data.toLocal8Bit(), project);
}

void TupLibraryFolder::reset()
{
    if (!objects.isEmpty())
        objects.clear();

    if (!folders.isEmpty())
        folders.clear();
}

void TupLibraryFolder::updatePaths(const QString &newPath)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::updatePaths()] - newPath -> " << newPath;
    #endif

    QStringList keys = objects.keys();
    foreach (QString oid, keys) {
         QString oldPath = objects[oid]->getDataPath();
         QFileInfo logicalPath(oldPath);
         QString filename = logicalPath.fileName();
         QString path = "";

         if (objects[oid]->getObjectType() == TupLibraryObject::Image)
             path = newPath + "/images/" + filename; 

         if (objects[oid]->getObjectType() == TupLibraryObject::Svg)
             path = newPath + "/svg/" + filename;

         if (objects[oid]->getObjectType() == TupLibraryObject::Sound)
             path = newPath + "/audio/" + filename;

         if (objects[oid]->getObjectType() == TupLibraryObject::Item)
             path = newPath + "/obj/" + filename;

         objects[oid]->setDataPath(path);
    }

    foreach (TupLibraryFolder *folder, folders)
        folder->updatePaths(newPath);    
}

bool TupLibraryFolder::isLoadingProject()
{
    return loadingProject;
}

QList<SoundResource> TupLibraryFolder::soundResourcesList()
{
    return soundRecords;
}

void TupLibraryFolder::updateSoundResourcesItem(TupLibraryObject *item)
{
    int size = soundRecords.count();
    for(int i=0; i<size; i++) {
        SoundResource record = soundRecords.at(i);
        if (item->getDataPath().compare(record.path) == 0) {
            record.frame = item->frameToPlay();
            record.muted = item->isMuted();
            soundRecords.replace(i, record);
            return;
        }
    }
}

void TupLibraryFolder::releaseLipSyncVoices(const QString &soundKey)
{
    if (exists(soundKey)) {
        TupLibraryObject *sound = getObject(soundKey);
        sound->setLipsyncVoiceFlag(false);
    } else {
        #ifdef TUP_DEBUG
            qDebug() << "[TupLibraryFolder::releaseLipSyncVoices()] - Sound file was NOT found! -> " << soundKey;
        #endif
    }
}

TupLibraryObject * TupLibraryFolder::findSoundFile(const QString &folderId)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupLibraryFolder::findSoundFile()] - Folder -> " << folderId;
    #endif

    TupLibraryFolder *folder = this->getFolder(folderId);
    if (folder) {
        LibraryObjects items = folder->getObjects();
        if (!items.isEmpty()) {
            foreach (TupLibraryObject *object, items) {
                if (object->getObjectType() == TupLibraryObject::Sound)
                    return object;
            }
        } else {
            #ifdef TUP_DEBUG
                qDebug() << "[TupLibraryFolder::findSoundFile()] -  Fatal Error: Folder is empty -> " << folderId;
            #endif
        }
    } else {
        #ifdef TUP_DEBUG
            qDebug() << "[TupLibraryFolder::findSoundFile()] -  Fatal Error: Folder is NULL -> " << folderId;
        #endif
    }

    return nullptr;
}

