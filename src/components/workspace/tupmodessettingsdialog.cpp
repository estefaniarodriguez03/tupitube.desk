﻿/***************************************************************************
 *   Project TUPITUBE DESK                                                *
 *   Project Contact: info@maefloresta.com                                 *
 *   Project Website: http://www.maefloresta.com                           *
 *   Project Leader: Gustav Gonzalez <info@maefloresta.com>                *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 *
 *    Gustavo Gonzalez                                                     *
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

#include "tupmodessettingsdialog.h"
#include "tseparator.h"

#include <QVBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>

TupModesSettingsDialog::TupModesSettingsDialog(QList<TupBackground::BgType> bgLayers, QList<bool> bgVisibility,
                                               QWidget *parent): QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("Modes Settings"));
    setWindowIcon(QIcon(QPixmap(THEME_DIR + "icons/modes_settings.png")));
    bool fgVisibility = true;

    // Background Tab

    QWidget *bgWidget = new QWidget;
    QVBoxLayout *bgLayout = new QVBoxLayout(bgWidget);
    modesList = new TupModesList(this);

    for (int i=0; i<bgLayers.count(); i++) {
        QString label = "";
        switch(bgLayers.at(i)) {
            case TupBackground::VectorDynamic:
            {
                label = tr("Vector Dynamic Background");
            }
            break;
            case TupBackground::RasterDynamic:
            {
                label = tr("Raster Dynamic Background");
            }
            break;
            case TupBackground::VectorStatic:
            {
                label = tr("Vector Static Background");
            }
            break;
            case TupBackground::RasterStatic:
            {
                label = tr("Raster Static Background");
            }
            break;
            case TupBackground::VectorForeground:
            {
                fgVisibility = bgVisibility.at(i);
                continue;
            }
            break;
            default:
            {
            }
        }

        TupListItem* bgItem = new TupListItem;
        modesList->addItem(bgItem);
        TupModesItem *bgWidget = new TupModesItem(bgLayers.at(i), label, bgVisibility.at(i));
        modesList->setItemWidget(bgItem, bgWidget);
    }

    modesList->setDragDropMode(QAbstractItemView::InternalMove);
    modesList->setFixedHeight(170);
    connect(modesList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(updateListUI()));
    connect(modesList, SIGNAL(listEdited()), this, SLOT(updateListUI()));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    QPixmap upIcon = QPixmap(THEME_DIR + "icons/mode_up.png");
    upButton = new QPushButton;
    upButton->setToolTip(tr("Move Mode Up"));
    upButton->setIcon(QIcon(upIcon));
    connect(upButton, SIGNAL(clicked()), this, SLOT(moveModeUp()));

    QPixmap downIcon = QPixmap(THEME_DIR + "icons/mode_down.png");
    downButton = new QPushButton;
    downButton->setToolTip(tr("Move Mode Down"));
    downButton->setIcon(QIcon(downIcon));
    connect(downButton, SIGNAL(clicked()), this, SLOT(moveModeDown()));

    buttonsLayout->addSpacing(20);
    buttonsLayout->addWidget(upButton);
    buttonsLayout->addWidget(downButton);
    buttonsLayout->addSpacing(20);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);

    QPushButton *applyButton = new QPushButton;
    applyButton->setMinimumWidth(60);
    applyButton->setIcon(QIcon(THEME_DIR + "icons/apply.png"));
    applyButton->setToolTip(tr("&Apply"));
    applyButton->setDefault(true);

    QPushButton *cancelButton = new QPushButton;
    cancelButton->setIcon(QIcon(THEME_DIR + "icons/close.png"));
    cancelButton->setToolTip(tr("&Cancel"));
    cancelButton->setMinimumWidth(60);

    buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(applyButton, QDialogButtonBox::ActionRole);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));

    bgLayout->addWidget(modesList);
    bgLayout->addLayout(buttonsLayout, Qt::AlignRight);

    // Foreground Tab

    QWidget *fgTabWidget = new QWidget;
    QVBoxLayout *fgLayout = new QVBoxLayout(fgTabWidget);

    fgList = new TupModesList(this);
    TupListItem *fgItem = new TupListItem;
    fgList->addItem(fgItem);
    TupModesItem *fgWidget = new TupModesItem(TupBackground::VectorForeground, tr("Foreground Layer"), fgVisibility);
    fgList->setItemWidget(fgItem, fgWidget);
    fgLayout->addWidget(fgList);

    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->addTab(bgWidget, tr("Backgrounds"));
    tabWidget->addTab(fgTabWidget, tr("Foreground"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);

    modesList->setCurrentRow(0);
}

TupModesSettingsDialog::~TupModesSettingsDialog()
{
}

void TupModesSettingsDialog::apply()
{
    // QList<QPair<TupBackground::BgType, bool>> valuesList;
    QPair<TupBackground::BgType, bool> values;

    for (int i=0; i < modesList->count(); i++) {
         TupModesItem *widget = static_cast<TupModesItem *>(modesList->itemWidget(modesList->item(i)));
         values = widget->getValues();
         idList << values.first;
         visibilityList << values.second;
    }

    for (int i=0; i < fgList->count(); i++) {
         TupModesItem *widget = static_cast<TupModesItem *>(fgList->itemWidget(fgList->item(i)));
         values = widget->getValues();
         idList << values.first;
         visibilityList << values.second;
    }

    emit valuesUpdated(idList, visibilityList);
    close();
}

void TupModesSettingsDialog::moveModeUp()
{
    int currentIndex = modesList->currentRow();
    if (currentIndex != -1) {
        TupModesItem *bgWidget = static_cast<TupModesItem *>(modesList->itemWidget(modesList->item(currentIndex)));
        modesList->takeItem(currentIndex);
        currentIndex--;
        if (currentIndex < 0)
            currentIndex = 0;

        if (currentIndex == 0 && upButton->isEnabled())
            upButton->setEnabled(false);
        if (currentIndex < 3 && !downButton->isEnabled())
            downButton->setEnabled(true);

        TupListItem* bgItem = new TupListItem;
        modesList->insertItem(currentIndex, bgItem);
        TupModesItem *widget = new TupModesItem(bgWidget->bgType(), bgWidget->itemLabel(), bgWidget->visibility());
        modesList->setItemWidget(bgItem, widget);
        modesList->setCurrentRow(currentIndex);
    }
}

void TupModesSettingsDialog::moveModeDown()
{
    int currentIndex = modesList->currentRow();
    if (currentIndex != -1) {
        TupModesItem *bgWidget = static_cast<TupModesItem *>(modesList->itemWidget(modesList->item(currentIndex)));
        modesList->takeItem(currentIndex);
        currentIndex++;
        if (currentIndex > 3)
            currentIndex = 3;

        if (currentIndex > 0 && !upButton->isEnabled())
            upButton->setEnabled(true);
        if (currentIndex == 3 && downButton->isEnabled())
            downButton->setEnabled(false);

        TupListItem* bgItem = new TupListItem;
        modesList->insertItem(currentIndex, bgItem);
        TupModesItem *widget = new TupModesItem(bgWidget->bgType(), bgWidget->itemLabel(), bgWidget->visibility());
        modesList->setItemWidget(bgItem, widget);
        modesList->setCurrentRow(currentIndex);
    }
}

void TupModesSettingsDialog::updateListUI()
{
    int currentIndex = modesList->currentRow();
    if (currentIndex != -1) {
        if (currentIndex == 0 && upButton->isEnabled())
            upButton->setEnabled(false);
        else if (currentIndex > 0 && !upButton->isEnabled())
            upButton->setEnabled(true);

        if (currentIndex == 3 && downButton->isEnabled())
            downButton->setEnabled(false);
        else if (currentIndex < 3 && !downButton->isEnabled())
            downButton->setEnabled(true);
    }
}
