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

#include <QProcess>
#include <qglobal.h>
#include <qdebug.h>
#include <qfile.h>
#include <qdir.h>
#include <qmessagebox.h>
#include <qfileinfo.h>
#include <qprocess.h>
#include <qfiledialog.h>
#include <qsettings.h>
#include <qdirmodel.h>
#include <GXPMainWindow.hpp>
#include <GXPSettingsDialog.hpp>
#include <globals.hpp>
#include <ConsoleOutputViewer.hpp>

GXPMainWindow::GXPMainWindow(QString modelFile){

  setupUi(this);
  connect(this->xparserButton, SIGNAL(clicked()), this, SLOT(runXParser()));
  connect(this->gnuMakeButton, SIGNAL(clicked()), this, SLOT(runGNUMake()));
  connect(this->chooseFilePathButton, SIGNAL(clicked()), this, SLOT(chooseModelFile()));
  connect(actionChoose_Model_File, SIGNAL(triggered()), this, SLOT(chooseModelFile()));
  connect(actionSave_Compilation_Output, SIGNAL(triggered()), this, SLOT(saveCompilationOutput()));
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(quitProgram()));
  connect(actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  connect(this->filesTreeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(itemClicked(const QModelIndex &)));

  if(modelFile != QString()){
	filePathLineEdit->setText(modelFile);
  }
}

void GXPMainWindow::runXParser(){
  QFile file(this->filePathLineEdit->text());

  if(!file.open(QIODevice::ReadOnly) || !QFileInfo(file).isFile()){
	QMessageBox::critical(0, SOFTWARE_NAME,
						  "Could not open the file",
						  QMessageBox::Close);
	return;
  }
  else {
	file.close();
  }

  QString program = "";
  QSettings settings(ORGANIZATION, SOFTWARE_NAME);
  if(settings.value("xparserExecutablePath") == QVariant()){
	qDebug() << "xparserExecutablePath not set in settings file";
  }

  program = settings.value("xparserExecutablePath").toString();
  while(!QFile(program).exists()){
	program = QFileDialog::getOpenFileName(this,
										   "Select the XParser Executable");
	if(program == ""){
	  qDebug() << "User cancelled XParser executable selection.";
	  return;
	}
  }
  settings.setValue("xparserExecutablePath", program);
  
  QFileInfo fileInf(file);
  QString xparserWD = fileInf.absoluteDir().absolutePath();
  QString argAsRelativePath = fileInf.fileName();

  QStringList arguments;
  arguments << argAsRelativePath;

  QProcess xparserProcess(this);
  xparserProcess.setWorkingDirectory(xparserWD);
  xparserProcess.start(program, arguments);

  qDebug() << "xparser runs in" << xparserProcess.workingDirectory();
  qDebug() << "argument to xparser is" << arguments;
  
  if(!xparserProcess.waitForStarted()){
	QMessageBox::critical(0, SOFTWARE_NAME,
						  "Could not start the xparser process",
						  QMessageBox::Close);
	return;
  }

  if(!xparserProcess.waitForFinished()){
	QMessageBox::critical(0, SOFTWARE_NAME,
						  "xparser process could not finish",
						  QMessageBox::Close);
	return;
  }
  else{
	QStringList out;
	out << "<h1>ERRORS</h1>";
	if(xparserProcess.exitStatus() == QProcess::CrashExit){
	  out << "<h2>XParser Terminated Unexpectedly!</h2>";
	}
	else if(xparserProcess.exitCode() != 0){
	  out << "<pre>" << xparserProcess.readAllStandardError() << "</pre>";
	}
	else{
	  out << "<h2>None</h2>";
	  out << "<pre>" << xparserProcess.readAllStandardError() << "</pre>";
	  this->refreshQDirView();
	}
	
	out << "<h1>OUTPUT</h1>";
	out << "<pre>" << xparserProcess.readAllStandardOutput() << "</pre>";

	xparserOutputTextEdit->setHtml(out.join(""));
  }

  return;
}

void GXPMainWindow::runGNUMake(){
  QFile file(this->filePathLineEdit->text());

  if(!file.open(QIODevice::ReadOnly) || !QFileInfo(file).isFile()){
	QMessageBox::critical(0, SOFTWARE_NAME,
						  "Could not open the model XML file",
						  QMessageBox::Close);
	return;
  }
  else {
	file.close();
  }
  
  QString program = "";
  QSettings settings(ORGANIZATION, SOFTWARE_NAME);
  if(settings.value("gnuMakeExecutablePath") == QVariant()){
	qDebug() << "gnuMakeExecutablePath not set in settings file";
  }
  
  program = settings.value("gnuMakeExecutablePath").toString();
  while(!QFile(program).exists()){
	program = QFileDialog::getOpenFileName(this,
										   "Select the GNU Make Executable");
	if(program == ""){
	  qDebug() << "User cancelled GNU Make executable selection.";
	  return;
	}
  }
  settings.setValue("gnuMakeExecutablePath", program);
  
  QFileInfo fileInf(file);
  QString gnuMakeWD = fileInf.absoluteDir().absolutePath();
  qDebug() << "Running in" << gnuMakeWD;
  
  QProcess *compiler = new QProcess;
  compiler->setWorkingDirectory(gnuMakeWD);
  qDebug() << "Process:" << settings.value("gnuMakeExecutablePath").toString() << "LIBMBOARD_DIR=" + settings.value("libmboardDirPath").toString();
  compiler->start(settings.value("gnuMakeExecutablePath").toString(),
				  QStringList() << "LIBMBOARD_DIR='" + settings.value("libmboardDirPath").toString() + "'");

  ConsoleOutputViewer *viewer = new ConsoleOutputViewer(compiler, this);
  
  connect(compiler, SIGNAL(started()),
  		  viewer, SLOT(startedProcess()));
  connect(compiler, SIGNAL(readyReadStandardOutput()),
  		  viewer, SLOT(stdOutReceived()));
  connect(compiler, SIGNAL(readyReadStandardError()),
  		  viewer, SLOT(stdErrReceived()));
  connect(compiler, SIGNAL(finished(int, QProcess::ExitStatus)),
  		  viewer, SLOT(finishedProcess(int, QProcess::ExitStatus)));
  viewer->exec();
  return;
}

void GXPMainWindow::chooseModelFile() {
  QSettings settings(ORGANIZATION, SOFTWARE_NAME);
  QString fileName = QFileDialog::getOpenFileName(this,
												  "Select an XMML Model File",
												  settings.value("lastSelectedFile").toString());
  if (!fileName.isEmpty()){
	filePathLineEdit->setText(fileName);
	settings.setValue("lastSelectedFile", fileName);
  }
}

void GXPMainWindow::saveCompilationOutput() {
  QMessageBox::critical(0, SOFTWARE_NAME,
						"Not implemented yet",
						QMessageBox::Close);
}

void GXPMainWindow::quitProgram() {
  this->close();
}

void GXPMainWindow::showSettings() {
  GXPSettingsDialog sd;
  if(sd.exec()){
	qDebug() << "Accepted";
  }
  else{
	qDebug() << "Not accepted";
  }
}

void GXPMainWindow::about() {
  QMessageBox::information(0, SOFTWARE_NAME,
						   QString("gxparser is a GUI wrapper for xparser developed by University of Sheffield.\n\n") +
						   "Developed by TUBITAK/UEAKE\n" + 
						   "For feedbacks and feature requests:\n" +
						   "Vehbi Sinan Tunalioglu <vst@vsthost.com>\n\n"
						   "Version of this build is: " +
						   VERSION_MAJOR + "." + VERSION_MINOR + "." + VERSION_BUILD,
						   QMessageBox::Close);
}

void GXPMainWindow::itemClicked(const QModelIndex &item) {
  if(item.isValid()){
	qDebug() << item << ((QDirModel*)this->filesTreeView->model())->filePath(item);
	QProcess explorer;

	QString clickedItemPath(QDir::toNativeSeparators(((QDirModel*)this->filesTreeView->model())->filePath(item)));

	if(clickedItemPath.right(3) == "dot"){
	  QSettings settings(ORGANIZATION, SOFTWARE_NAME);
	  if(settings.value("dotVisualizerExecutablePath") == QVariant()){
		qDebug() << "No program was set in the settings for visualizing DOT files.";
		QMessageBox::critical(0, SOFTWARE_NAME,
							  "No program was set in the settings for visualizing DOT files.",
							  QMessageBox::Close);
		return;
		
	  }
	  QString dotVisualizer(settings.value("dotVisualizerExecutablePath").toString());
	  qDebug() << "Executing Program: "
		   << dotVisualizer
		   << "with parameters: "
		   << clickedItemPath;
	  QFileInfo fileItemFI(clickedItemPath);
	  explorer.setWorkingDirectory(fileItemFI.absoluteDir().absolutePath());
	  explorer.start(dotVisualizer, QStringList() << fileItemFI.fileName());
	}
	else{
#ifdef Q_WS_WIN
	  qDebug() << "Detected Win platform";
	  QString explorerExecutable("explorer");
#else
	  qDebug() << "Detected NonWin platform";
	  QString explorerExecutable("/usr/bin/gnome-open");
#endif
	  explorer.start(explorerExecutable, QStringList() << clickedItemPath);
	}
	
	qDebug() << "App launcher phase: 1";
	if (!explorer.waitForStarted()) {
	  qDebug() << "App launcher phase: 2";
	  return;
	}
	qDebug() << "App launcher phase: 3";
	explorer.closeWriteChannel();
	qDebug() << "App launcher phase: 4";
	if (!explorer.waitForFinished()){
	  qDebug() << "App launcher phase: 5";
	  return;
	}
	qDebug() << "App launcher phase: 6";	
  }
  else{
	qDebug() << "No item selected.";
  }
}

void GXPMainWindow::refreshQDirView(){
  QDirModel *model = new QDirModel;
  this->filesTreeView->setModel(model);
  QFile file(this->filePathLineEdit->text());
  QFileInfo fileInfArg(file);
  QString dirRoot = fileInfArg.absoluteDir().absolutePath();
  this->filesTreeView->setRootIndex(model->index(dirRoot));
}
