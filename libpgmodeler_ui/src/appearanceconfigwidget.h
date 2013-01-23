/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: pgModeler UI library (libpgmodeler_ui)
# Class: AppearanceConfigWidget
# Description: Implements the operations to manage graphical objects appearance configuration.
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/
#ifndef APPEARANCE_CONFIG_WIDGET_H
#define APPEARANCE_CONFIG_WIDGET_H

#include "ui_appearanceconfigwidget.h"
#include "baseconfigwidget.h"
#include "objectsscene.h"
#include "databasemodel.h"
#include <algorithm>

class AppearanceConfigWidget: public QWidget, public Ui::AppearanceConfigWidget, public BaseConfigWidget {
 private:
  Q_OBJECT

  //Auxiliary class that stores the formating data of each element
  class AppearanceConfigItem {
   public:
    QString conf_id;
    QTextCharFormat font_fmt;
    QColor colors[3];
    bool obj_conf;
  };

  //Color picker dialog
  QColorDialog color_dlg;

  //Viewport used to show the example model
  QGraphicsView *viewp;

  //Object scene used to store the graphical objects
  ObjectsScene *scene;

  //Database model used to store the example base objects
  DatabaseModel *model;

  //Stores the element configuration items
  vector<AppearanceConfigItem> conf_items;

  //Loads the example model from file (conf/exampledb.dbm)
  void loadExampleModel(void);

 public:
  AppearanceConfigWidget(QWidget * parent=0);
  ~AppearanceConfigWidget(void);

  //Saves the configuration to file
  void saveConfiguration(void);

  //Loads the configuration from file
  void loadConfiguration(void);

 private slots:
  void enableConfigElement(void);
  void applyFontStyle(void);
  void applyElementColor(void);

  /* Disabled method */
  void applyConfiguration(void){}

 public slots:
  void restoreDefaults(void);
};

#endif
