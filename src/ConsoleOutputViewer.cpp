#include <ConsoleOutputViewer.hpp>
#include <qmessagebox.h>

ConsoleOutputViewer::ConsoleOutputViewer(QProcess *process, GXPMainWindow* parent){
  setupUi(this);
  this->parent = parent;
  this->output->setAcceptRichText(true);
  this->process = process;
  connect(this, SIGNAL(accepted()), this, SLOT(closedOK()));
  connect(this->buttonBox, SIGNAL(clicked(QAbstractButton *)), this, SLOT(closedOK()));
}

void ConsoleOutputViewer::startedProcess() {
  this->output->append("Process started");
}

void ConsoleOutputViewer::stdOutReceived() {
  this->output->append("<font color=blue>" + this->process->readAllStandardOutput() + "</font>");
}

void ConsoleOutputViewer::stdErrReceived() {
  this->output->append("<font color=red>" + this->process->readAllStandardError() + "</font>");
}

void ConsoleOutputViewer::finishedProcess(int exitStat, QProcess::ExitStatus) {
  this->output->append(QString("Process ended with status: %1").arg(exitStat));
  this->status = exitStat;
  this->parent->refreshQDirView();
}

void ConsoleOutputViewer::closedOK(){
  // Check if the process is running
  if(this->process->state() == QProcess::NotRunning){
	this->done(this->status);
  }
  else{
	// The process is running.
	QMessageBox msgBox;
	msgBox.setWindowTitle("Process is still running");
	msgBox.setText("The process is still running.");
	msgBox.setInformativeText("Do you want to terminate the process?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	int ret = msgBox.exec();
	if(ret == QMessageBox::Yes){
	  this->process->kill();
	  this->done(-1);
	}
	else{
	  return;
	}
  }
}
