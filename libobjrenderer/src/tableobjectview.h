/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Graphical objects renderer (libobjrenderer)
# Class: TableObjectView
# Description: implements the graphical representation for table objects (columns, constraints, triggers,
#              rules, indexes) as well the view references
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
#ifndef TABLE_OBJECT_VIEW_H
#define TABLE_OBJECT_VIEW_H

#include "view.h"
#include "table.h"
#include "baseobjectview.h"

class TableObjectView: public BaseObjectView
{
 private:
  Q_OBJECT

  //Table object descriptor
  QGraphicsItem *descriptor;

  //Labels used to show objects informatoni (name, type, constraints/aliases)
  QGraphicsSimpleTextItem *lables[3];

  static const QString CONSTR_DELIM_END,
                       CONSTR_DELIM_START,
                       TYPE_SEPARATOR,
                       CONSTR_SEPARATOR,
                       TXT_FOREIGN_KEY,
                       TXT_NOT_NULL,
                       TXT_PRIMARY_KEY,
                       TXT_UNIQUE;

  /* Configures the descriptor object according to the source object.
     The constraint type parameter is only used when the source object is a
     column  and is used to format the descriptor indication that the column
     has a constraint */
  void configureDescriptor(ConstraintType constr_type=BaseType::null);

  QVariant itemChange(GraphicsItemChange, const QVariant &value)
  {
   return(value);
  }

 public:
  TableObjectView(TableObject *object=NULL);
  ~TableObjectView(void);

  //Configures the object as a view reference
  void configureObject(Reference reference);

  //Configures the object as a table object
  void configureObject(void);

  //Sets the horizontal position of the specified child object (index)
  void setChildObjectXPos(unsigned obj_idx, float px);

  //Returns the child object at the specified index
  QGraphicsItem *getChildObject(unsigned obj_idx);

  /* Returns a formatted string containing the keywords indicating the constraints
     that is applyed to the passed column */
  static QString getConstraintString(Column *column);
};

#endif
