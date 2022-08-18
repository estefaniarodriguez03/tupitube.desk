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

#include "tupcamerastatus.h"
#include "tupscene.h"
#include "tupexportwidget.h"

#include <QHBoxLayout>
#include <QLabel>

TupCameraStatus::TupCameraStatus(QWidget *parent) : QFrame(parent)
{
    #ifdef TUP_DEBUG
        qDebug() << "[TupCameraStatus()]";
    #endif

    mute = false;
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    QBoxLayout *sceneInfoLayout = new QBoxLayout(QBoxLayout::LeftToRight, parent);

    QLabel *sceneNameText = new QLabel("<B>" + tr("Scene") + ":</B> ");
    scenesCombo = new QComboBox();
    scenesCombo->setStyleSheet("combobox-popup: 0;");
    scenesCombo->setMaxVisibleItems(5);
    connect(scenesCombo, SIGNAL(activated(int)), this, SIGNAL(sceneIndexChanged(int)));

    sceneInfoLayout->addWidget(sceneNameText, 1);
    sceneInfoLayout->addWidget(scenesCombo);
    sceneInfoLayout->addSpacing(15);

    QLabel *fpsText = new QLabel("<B>" + tr("FPS") + ":</B> ");

    fpsBox = new QSpinBox();
    fpsBox->setMinimum(1);
    fpsBox->setMaximum(35);
    fpsBox->setValue(24);
    connect(fpsBox, SIGNAL(valueChanged(int)), this, SIGNAL(fpsChanged(int)));

    sceneInfoLayout->addWidget(fpsText, 1);
    sceneInfoLayout->addWidget(fpsBox, 1);

    sceneInfoLayout->addSpacing(15);

    loopBox = new QCheckBox();
    loopBox->setToolTip(tr("Loop"));
    loopBox->setIcon(QPixmap(THEME_DIR + "icons/loop.png"));
    loopBox->setFocusPolicy(Qt::NoFocus);
    loopBox->setShortcut(QKeySequence(tr("Ctrl+L")));
    connect(loopBox, SIGNAL(clicked()), this, SIGNAL(loopChanged()));

    TCONFIG->beginGroup("AnimationParameters");
    loop = TCONFIG->value("Loop").toBool();
    if (loop)
        loopBox->setChecked(true);

    sceneInfoLayout->addWidget(loopBox, 1);

    sceneInfoLayout->addSpacing(15);

    soundButton = new TImageButton(QPixmap(THEME_DIR + "icons/speaker.png"), 22, this, true);
    soundButton->setShortcut(QKeySequence(tr("M")));
    soundButton->setToolTip(tr("Mute"));
    connect(soundButton, SIGNAL(clicked()), this, SLOT(muteAction()));
    sceneInfoLayout->addWidget(soundButton, 1);
    sceneInfoLayout->addSpacing(15);

    exportButton = new QPushButton(tr("Export"));
    exportButton->setIcon(QIcon(THEME_DIR + "icons/export_button.png"));
    exportButton->setToolTip(tr("Export Project as Video File"));
    connect(exportButton, SIGNAL(pressed()), this, SIGNAL(exportClicked()));

    #ifndef TUP_32BIT
        sceneInfoLayout->addWidget(exportButton, 1);
        sceneInfoLayout->addSpacing(5);
    #endif

    postButton = new QPushButton(tr("Post"));
    postButton->setIcon(QIcon(THEME_DIR + "icons/share.png"));
    connect(postButton, SIGNAL(pressed()), this, SIGNAL(postClicked()));
    sceneInfoLayout->addWidget(postButton, 1);

    setLayout(sceneInfoLayout);
}

TupCameraStatus::~TupCameraStatus()
{
    #ifdef TUP_DEBUG
        qDebug() << "[~TupCameraStatus()]";
    #endif
}

void TupCameraStatus::setFPS(int frames)
{
    /*
    #ifdef TUP_DEBUG
        qDebug() << "[TupScreen::setFPS()]";
    #endif
    */

    if (frames > 0 && frames < 101)
        fpsBox->setValue(frames);
    else
        fpsBox->setValue(24);
}

int TupCameraStatus::getFPS()
{
    return fpsBox->value();
}

void TupCameraStatus::setCurrentScene(int index)
{
    if (scenesCombo->currentIndex() != index)
        scenesCombo->setCurrentIndex(index);
}

void TupCameraStatus::setScenes(TupProject *project)
{
    if (scenesCombo->count())
        scenesCombo->clear();

    QStringList scenes;
    int scenesCount = project->getScenes().size();
    for (int i = 0; i < scenesCount; i++) {
         TupScene *scene = project->getScenes().at(i);
         if (scene)
             scenes << scene->getSceneName();
    }

    scenesCombo->addItems(scenes);
    scenesCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

bool TupCameraStatus::isLooping()
{
    loop = loopBox->isChecked();
    TCONFIG->beginGroup("AnimationParameters");
    TCONFIG->setValue("Loop", loop);

    return loop;
}

void TupCameraStatus::muteAction()
{
    QString img("icons/mute.png");

    if (mute) {
        mute = false;
        img = "icons/speaker.png";
        soundButton->setToolTip(tr("Mute"));
    } else {
        mute = true;
        soundButton->setToolTip(tr("Unmute"));
    }

    soundButton->setImage(QPixmap(THEME_DIR + img));

    emit muteEnabled(mute);
}

void TupCameraStatus::enableButtons(bool flag)
{
    exportButton->setEnabled(flag);
    postButton->setEnabled(flag);
}
