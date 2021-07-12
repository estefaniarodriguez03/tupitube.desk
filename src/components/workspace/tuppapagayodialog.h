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

#ifndef TUPPAPAGAYODIALOG_H
#define TUPPAPAGAYODIALOG_H

#include "tglobal.h"
#include "tapplicationproperties.h"

#include <QDialog>
#include <QLineEdit>
#include <QStackedWidget>

class TUPITUBE_EXPORT TupPapagayoDialog : public QDialog
{
    Q_OBJECT

    public:
        TupPapagayoDialog();
        ~TupPapagayoDialog();

        QString getPGOFile() const;
        QString getImagesDir() const;
        QString getSoundFile() const;

    private slots:
        void openFileDialog();
        void openImagesDialog();
        void openSoundDialog();
        void checkRecords();
        void updateMouthView(int index);

    private:
        QWidget * sampleWidget(int index);
        void setDefaultPath(const QString &path);
        void saveDefaultPath(const QString &dir);

        QLineEdit *filePath;
        QLineEdit *imagesPath;
        QLineEdit *soundPath;
        QStackedWidget *stackedWidget;
        QStringList folder;
};

#endif
