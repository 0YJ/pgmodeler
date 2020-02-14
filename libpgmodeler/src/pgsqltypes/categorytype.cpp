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

#include "categorytype.h"

QStringList CategoryType::type_names =
{
	"", // Reserved for null value

	//See table 44-43 on PostgreSQL 8.4 documentation
	"U", //User-defined types
	"A", //Array types
	"B", //Boolean types
	"C", //Composite types
	"D", //Date/time types
	"E", //Enum types
	"G", //Geometric types
	"I", //Network address types
	"N", //Numeric types
	"P", //Pseudo-types
	"S", //String types
	"T", //Timespan types
	"V", //Bit-string types
	"X", //Unknown type
};

CategoryType::CategoryType()
{
	type_idx = UserDefined;
}

CategoryType::CategoryType(const QString &type_name)
{
	(*this)=type_name;
}

CategoryType::CategoryType(unsigned type_id)
{
	(*this)=type_id;
}

QStringList CategoryType::getTypes()
{
	return BaseType::getTypes(type_names);
}

unsigned CategoryType::operator = (unsigned type_id)
{
	BaseType::setType(type_id, type_names);
	return type_idx;
}

unsigned CategoryType::operator = (const QString &type_name)
{
	unsigned type_id = BaseType::getType(type_name, type_names);
	BaseType::setType(type_id, type_names);
	return type_id;
}

QString CategoryType::operator ~ ()
{
	return type_names[type_idx];
}
