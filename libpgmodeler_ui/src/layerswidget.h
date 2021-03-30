/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2021 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

/**
\ingroup libpgmodeler_ui
\class LayersWidget
\brief Implements the widget that allows the user to quickly setup object's layers
*/

#ifndef LAYERS_WIDGET_H
#define LAYERS_WIDGET_H

#include <QWidget>
#include "ui_layerswidget.h"
#include "basegraphicobject.h"

class LayersWidget : public QWidget, Ui::LayersWidget {
	private:
		Q_OBJECT

		vector<BaseGraphicObject *> selected_objs;

	public:
		explicit LayersWidget(QWidget *parent = nullptr);

		void setAttributes(const QStringList &layers, const vector<BaseObject *> &selected_objs);

	private slots:
		void updateObjectsLayers();

	signals:
		void s_objectsLayersChanged();
};

#endif
