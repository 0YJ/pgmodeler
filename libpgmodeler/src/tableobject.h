/*
# PostgreSQL Database Modeler (pgModeler)
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

/**
\ingroup libpgmodeler
\class TableObject
\brief This class serves as a base class for objects that are embedded on tables
like columns, rules, triggers, indexes, constraints.
\note <strong>Creation date:</strong> 24/07/2008
*/

#ifndef TABLE_OBJECT_H
#define TABLE_OBJECT_H

#include "basetable.h"

class TableObject: public BaseObject {
 protected:
   /// @details Stores the table that owns this object
   BaseTable *parent_table;

  /** @details Indicates that the object was included automaticaly by "one to many",
     "one to many", "many to many" relationships */
  bool add_by_linking;

  /** @details Indicates that the object was included automaticaly by "one to many",
     "generalization" relationships */
  bool add_by_generalization;

  /** @details Indicates that the object was included automaticaly by "one to many",
     "copy" relationships */
  bool add_by_copy;

  /// @details Defines that the object is included by relationship (1-1, 1-n, n-n)
  void setAddedByLinking(bool value);

  /// @details Defines that the object is included by generalization relationship
  void setAddedByGeneralization(bool value);

  /// @details Defines that the object is include by copy relationship
  void setAddedByCopy(bool value);

 public:
  TableObject(void);

  /// @details Defines the parent table for the object
  void setParentTable(BaseTable *table);

  /// @details Returns the object parent table
  BaseTable *getParentTable(void);

  /** @details This method is purely virtual to force the derived classes
  overload this method. This also makes class TableObject
  not instantiable */
  virtual QString getCodeDefinition(unsigned def_type)=0;

  /// @details Returns whether the object was added by relationship 1-1, 1-n, n-n
  bool isAddedByLinking(void);

  /// @details Returns whether the object was added by generalization
  bool isAddedByGeneralization(void);

  /// @details Returns whether the object was added by copy relationship
  bool isAddedByCopy(void);

  /** @details Returns whether the object was added by relationship (covering all the
     possible relationship types) */
  bool isAddedByRelationship(void);

  void operator = (TableObject &object);

  friend class Relationship;
};

#endif
