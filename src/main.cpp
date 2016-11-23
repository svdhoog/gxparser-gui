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

#include <qapplication.h>
#include <GXPMainWindow.hpp>
#include <QtDebug>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/icons/images/euracelogo.png"));

  QString filePath;

  if(app.arguments().size() == 1){
	filePath = QString();
  }
  else{
	filePath = app.arguments().at(1);
  }
  
  GXPMainWindow w(filePath);
  w.show();
  
  return app.exec();
}
