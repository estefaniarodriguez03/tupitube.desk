/***************************************************************************
 *   Project TUPI: Magia 2D                                                *
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
 *   the Free Software Foundation; either version 3 of the License, or     *
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

#ifndef KAUDIOPLAYER_H
#define KAUDIOPLAYER_H

#include <qobject.h>

#include "kaudioengineiface.h"
#include "tglobal.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_CORE_EXPORT KAudioPlayer : public QObject
{
    Q_OBJECT;
    public:
        KAudioPlayer();
        ~KAudioPlayer();
        
        static KAudioPlayer *instance();
        void loadEngine(const QString &engine);
        int load(const QUrl &url, int id = -1);
        void play(int offset = 0);
        void pause();
        
        void setCurrentPlayer(int id);
        
        void stop();
        void seek(uint ms);
        void setVolume(int percent);
        
    private:
        static KAudioPlayer *s_instance;
        KAudioEngineIface *m_engine;
};

namespace KPlayer
{
    inline int play(const QUrl &url, int seek = 0) {
        int id = KAudioPlayer::instance()->load(url);
        KAudioPlayer::instance()->play(seek);
        
        return id;
    }
};

#endif

