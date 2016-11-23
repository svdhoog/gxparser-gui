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

#ifndef GXPMAINWINDOW_H
#define GXPMAINWINDOW_H

#include <qdebug.h>
#include <QMainWindow>
#include <ui_GXPMainWindow.h>
#include <qprocess.h>
#include <globals.hpp>

class GXPMainWindow : public QMainWindow, Ui::GXPMainWindow {

Q_OBJECT

public:

  GXPMainWindow(QString modelFile);
  void refreshQDirView();

private:

private slots:

  void runXParser();
  void runGNUMake();
  void chooseModelFile();
  void saveCompilationOutput();
  void quitProgram();
  void showSettings();
  void about();
  void itemClicked(const QModelIndex &item = QModelIndex());
};
#endif
