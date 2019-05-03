/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\class GenericSQLWidget
\brief Implements the operations to create/edit generic sql objects via form.
*/

#ifndef GENERIC_SQL_WIDGET_H
#define GENERIC_SQL_WIDGET_H

#include "ui_genericsqlwidget.h"
#include "baseobjectwidget.h"
#include "codecompletionwidget.h"
#include "objectstablewidget.h"
#include "objectselectorwidget.h"
#include "hinttextwidget.h"

class GenericSQLWidget: public BaseObjectWidget, public Ui::GenericSQLWidget {
	private:
		Q_OBJECT

		HintTextWidget *ref_name_ht, *use_signature_ht, *format_name_ht;

		NumberedTextEditor *definition_txt, *preview_txt;

		SyntaxHighlighter *definition_hl, *preview_hl;

		CodeCompletionWidget *definition_cp;

		ObjectsTableWidget *objects_refs_tab;

		ObjectSelectorWidget *object_sel;

		GenericSQL dummy_gsql;

		void clearObjectReferenceForm(void);
		void updateCodePreview(void);

	public:
		GenericSQLWidget(QWidget * parent = nullptr);
		void setAttributes(DatabaseModel *model, OperationList *op_list, GenericSQL *genericsql=nullptr);

	public slots:
		void applyConfiguration(void);

	private slots:
		void addObjectReference(int row);
};

#endif
