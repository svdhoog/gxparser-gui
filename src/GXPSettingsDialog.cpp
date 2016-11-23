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

#include <qsettings.h>
#include <qfiledialog.h>
#include <GXPSettingsDialog.hpp>
#include <globals.hpp>

GXPSettingsDialog::GXPSettingsDialog(){
  setupUi(this);
  //connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  QSettings settings(ORGANIZATION, SOFTWARE_NAME);
  
  // XParser Executable Related
  if(settings.value("xparserExecutablePath") == QVariant()){
	settings.setValue("xparserExecutablePath", QString());
  }
  this->xparserExecutablePath = settings.value("xparserExecutablePath").toString();
  this->xparserLocationLineEdit->setText(this->xparserExecutablePath);
  connect(this->xparserLocationButton, SIGNAL(clicked()), this, SLOT(chooseXparserLocation()));

  // libmboard directory Related
  if(settings.value("libmboardDirPath") == QVariant()){
	settings.setValue("libmboardDirPath", QString());
  }
  this->libmboardDirPath = settings.value("libmboardDirPath").toString();
  this->libmboardLocationLineEdit->setText(this->libmboardDirPath);
  connect(this->libmboardLocationButton, SIGNAL(clicked()), this, SLOT(chooseLibMboardLocation()));

  // GNU Make Executable Related
  if(settings.value("gnuMakeExecutablePath") == QVariant()){
	settings.setValue("gnuMakeExecutablePath", QString());
  }
  this->gnuMakeExecutablePath = settings.value("gnuMakeExecutablePath").toString();
  this->gnuMakeLocationLineEdit->setText(this->gnuMakeExecutablePath);
  connect(this->gnuMakeLocationButton, SIGNAL(clicked()), this, SLOT(chooseGNUMakeLocation()));

  // Dot Visualizer Executable Related
  if(settings.value("dotVisualizerExecutablePath") == QVariant()){
	settings.setValue("dotVisualizerExecutablePath", QString());
  }
  this->dotVisualizerExecutablePath = settings.value("dotVisualizerExecutablePath").toString();
  this->dotVisualizerLocationLineEdit->setText(this->dotVisualizerExecutablePath);
  connect(this->dotVisualizerLocationButton, SIGNAL(clicked()), this, SLOT(chooseDotVisualizerLocation()));

  // If this is accepted:
  connect(this, SIGNAL(accepted()), this, SLOT(settingsInfoChanged()));
}

void GXPSettingsDialog::chooseXparserLocation(){
  QSettings settings(ORGANIZATION, SOFTWARE_NAME);
  QString fileName = QFileDialog::getOpenFileName(this,
												  "Select xparser executable",
												  this->xparserExecutablePath);
  if(fileName == ""){
	qDebug() << "User cancelled XParser executable selection";
	return;
  }
  while(!QFile(fileName).exists()){
	fileName = QFileDialog::getOpenFileName(this,
											"Select xparser executable",
											this->xparserExecutablePath);
	if(fileName == ""){
	  qDebug() << "User cancelled XParser executable selection";
	  return;
	}
  }
  this->xparserExecutablePath = fileName;
  this->xparserLocationLineEdit->setText(fileName);  
}

void GXPSettingsDialog::chooseLibMboardLocation(){
  QSettings settings(ORGANIZATION, SOFTWARE_NAME);
  QString fileName = QFileDialog::getExistingDirectory(this,
													   "Select libmboard path",
													   this->libmboardDirPath,
													   QFileDialog::DontUseNativeDialog);
  if(fileName == ""){
	qDebug() << "User cancelled libmboard location selection";
	return;
  }
  while(!QFile(fileName).exists()){
	fileName = QFileDialog::getExistingDirectory(this,
												 "Select libmboard path",
												 this->libmboardDirPath,
												 QFileDialog::DontUseNativeDialog);
	if(fileName == ""){
	  qDebug() << "User cancelled libmboard location selection";
	  return;
	}
  }
  this->libmboardDirPath = fileName;
  this->libmboardLocationLineEdit->setText(fileName);  
}

void GXPSettingsDialog::chooseGNUMakeLocation(){
  QSettings settings(ORGANIZATION, SOFTWARE_NAME);
  QString fileName = QFileDialog::getOpenFileName(this,
												  "Select GNU Make executable",
												  this->gnuMakeExecutablePath);
  if(fileName == ""){
	qDebug() << "User cancelled GNU Make executable selection";
	return;
  }
  while(!QFile(fileName).exists()){
	fileName = QFileDialog::getOpenFileName(this,
											"Select GNU Make executable",
											this->gnuMakeExecutablePath);
	if(fileName == ""){
	  qDebug() << "User cancelled GNU Make executable selection";
	  return;
	}
  }
  this->gnuMakeExecutablePath = fileName;
  this->gnuMakeLocationLineEdit->setText(fileName);  
}

void GXPSettingsDialog::chooseDotVisualizerLocation(){
  QSettings settings(ORGANIZATION, SOFTWARE_NAME);
  QString fileName = QFileDialog::getOpenFileName(this,
												  "Select Dot Visualizer executable",
												  this->dotVisualizerExecutablePath);
  if(fileName == ""){
	qDebug() << "User cancelled Dot Visualizer executable selection";
	return;
  }
  while(!QFile(fileName).exists()){
	fileName = QFileDialog::getOpenFileName(this,
											"Select Dot Visualizer executable",
											this->dotVisualizerExecutablePath);
	if(fileName == ""){
	  qDebug() << "User cancelled Dot Visualizer executable selection";
	  return;
	}
  }
  this->dotVisualizerExecutablePath = fileName;
  this->dotVisualizerLocationLineEdit->setText(fileName);
}


void GXPSettingsDialog::settingsInfoChanged(){
  QSettings settings(ORGANIZATION, SOFTWARE_NAME);
  settings.setValue("xparserExecutablePath", this->xparserExecutablePath);
  settings.setValue("gnuMakeExecutablePath", this->gnuMakeExecutablePath);
  settings.setValue("dotVisualizerExecutablePath", this->dotVisualizerExecutablePath);
  settings.setValue("libmboardDirPath", this->libmboardDirPath);
}
