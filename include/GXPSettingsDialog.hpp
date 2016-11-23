////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2008 Vehbi Sinan Tunalioglu <vst@vsthost.com>           //
//                                                                        //
//  This file is part of gxparser.                                        //
//                                                                        //
//  gxparser is free software: you can redistribute it and/or             //
//  modify it under the terms of the GNU General Public License           //
//  as published by the Free Software Foundation, either version 3        //
//  of the License, or (at your option) any later version.                //
//                                                                        //
//  gxparser is distributed in the hope that it will be useful, but       //
//  WITHOUT ANY WARRANTY; without even the implied warranty of            //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     //
//  General Public License for more details.                              //
//                                                                        //
//  You should have received a copy of the GNU General Public License     //
//  along with gxparser.  If not, see <http://www.gnu.org/licenses/>.     //
////////////////////////////////////////////////////////////////////////////

#ifndef GXPSETTINGSDIALOG_H
#define GXPSETTINGSDIALOG_H

#include <qdebug.h>
#include <QDialog>
#include <ui_GXPSettingsDialog.h>

class GXPSettingsDialog : public QDialog, Ui::GXPSettingsDialog {

Q_OBJECT

public:

  GXPSettingsDialog();

private:

  QString xparserExecutablePath;
  QString gnuMakeExecutablePath;
  QString dotVisualizerExecutablePath;
  QString libmboardDirPath;

private slots:

  void chooseXparserLocation();
  void chooseGNUMakeLocation();
  void chooseDotVisualizerLocation();
  void settingsInfoChanged();
  void chooseLibMboardLocation();

};
#endif
