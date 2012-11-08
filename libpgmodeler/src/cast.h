/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: Cast
# Description: Implements the operations to manipulate data type cast
#              on the database
# Creation date: 03/06/2008
#
# Copyright 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef CAST_H
#define CAST_H

#include "baseobject.h"
#include "function.h"

class Cast: public BaseObject {
 private:
  /* Data types used on the cast:
    0 -> Source data type
    1 -> Destination data type */
  TipoPgSQL types[2];

  //Conversion type (ASSIGNMENT or IMPLICIT)
  unsigned cast_type;

  //Function used to do the type cast
  Function *cast_function;

  /* Indicates that the type cast is inout, this means that
     the function used to cast the types will be the 'output'
     function of the source data type */
  bool is_in_out;

 public:
  //Constants used to access the data types envolved in the cast
  static const unsigned SRC_TYPE=0,
                        DST_TYPE=1,
  //Constants used to define the cast type
                        ASSIGNMENT=10,
                        IMPLICIT=20;
  Cast(void);

  /* Defines one of the data types envolved on the cast
     (using constants SRC_TYPE | DST_TYPE) */
  void setDataType(unsigned type_idx, TipoPgSQL type);

  //Defines the cast type (using constants ASSINGMENT | IMPLICIT)
  void setCastType(unsigned cast_type);

  //Defines the function used to do the type cast
  void setCastFunction(Function *cast_func);

  /* Defines if the conversion is inout. If so the conversion function
     assigned to the cast is  ignored since the PostgreSQL will use
     the output function of the source type. */
  void setInOut(bool value);

  //Returns one of the cast envolved data types
  TipoPgSQL getDataType(unsigned type_idx);

  //Returns the cast type
  unsigned getCastType(void);

  //Returns the cast function
  Function *getCastFunction(void);

  //Returs whether the cast is inout or not
  bool isInOut(void);

  //Returns the SQL/XML code definition for the cast
  QString getCodeDefinition(unsigned def_type);
};

#endif
