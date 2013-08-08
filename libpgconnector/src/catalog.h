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
\ingroup libpgconnector
\class Catalog
\brief \brief Implements the operations to read data from information_schema as well from pg_catalog schema.
This class is the basis for the reverse engineering feature.
*/

#ifndef CATALOG_H
#define CATALOG_H

/* Possible way to determine the order to import object from database:
 * 1) Order object by type (list below)
 * 2) For each type objects by name

**OK!**		ROLE						(pg_role)
**OK!**		TABLESPACE			(pg_tablespace)
**OK!**		DATABASE				(pg_database)
**OK!**		SCHEMA					(pg_namespace)
**OK!**		EXTENSION				(pg_extension)
**OK!**		FUNCTION				(pg_proc)	[c,sql,internal langs]
**OK!**		USERTYPE				(pg_type)
**OK!**		LANGUAGE				(pg_language) [user-defined]
**OK!**		FUNCTION				(pg_proc) [user-defined]
**OK!**		AGGREGATE				(pg_aggregate)
**OK!**		OPERATOR				(pg_operator)
**OK!**		OPCLASS					(pg_opclass)
**OK!**		OPFAMILY				(pg_opfamily)
**OK!**		COLLATION				(pg_collation)
**OK!**		CONVERSION			(pg_conversion)
**OK!**		TABLE						(pg_tables)
**OK!**		COLUMN					(pg_attribute, pg_attrdef)
**OK!**		INDEX						(pg_index)
**OK!**		RULE						(pg_rule)
**OK!**		TRIGGER					(pg_trigger, pg_constraint (constraint trigger))
**OK!**		CONSTRAINT			(pg_constraints)
**OK!**		CAST						(pg_cast)
**OK!**		VIEW						(pg_view, pg_class [relkind='v'])
**OK!**		SEQUENCE        (pg_class [relkind='S'], information_schema.sequences)
**OK!**		OBJ_DOMAIN      (information_schema.domains)
					TABLE INHERITS	(Retrieved individually for each table)
					PERMISSION			(Retrieved individually for each type of object)

** Details can be found on pg_dump_sort.c line 82 **/


/* Objects that can belongs to a extension
AGGREGATE agg_name (agg_type [, ...] ) |
	CAST (source_type AS target_type) |
	COLLATION object_name |
	CONVERSION object_name |
	DOMAIN object_name |
	FOREIGN DATA WRAPPER object_name |
	FOREIGN TABLE object_name |
	FUNCTION function_name ( [ [ argmode ] [ argname ] argtype [, ...] ] ) |
	OPERATOR operator_name (left_type, right_type) |
	OPERATOR CLASS object_name USING index_method |
	OPERATOR FAMILY object_name USING index_method |
	[ PROCEDURAL ] LANGUAGE object_name |
	SCHEMA object_name |
	SEQUENCE object_name |
	SERVER object_name |
	TABLE object_name |
	TEXT SEARCH CONFIGURATION object_name |
	TEXT SEARCH DICTIONARY object_name |
	TEXT SEARCH PARSER object_name |
	TEXT SEARCH TEMPLATE object_name |
	TYPE object_name |
	VIEW object_name */
#include "connection.h"
#include "baseobject.h"
#include "tableobject.h"
#include <QTextStream>
#include <QApplication>

class Catalog {
	private:
		static const QString QUERY_LIST,	//! \brief Executes a list command on catalog
		QUERY_ATTRIBS, //! \brief Executes a attribute retrieving command on catalog
		CATALOG_SCH_DIR, //! \brief Default catalog schemas directory
		PGSQL_TRUE, //! \brief Replacement for true 't' boolean value
		PGSQL_FALSE, //! \brief Replacement for false 'f' boolean value
		BOOL_FIELD;

		/*! \brief This map stores the oid field name for each object type. The oid field name can be
		composed by the pg_[OBJECT_TYPE] table alias. Refer to catalog query schema files for details */
		static map<ObjectType, QString> oid_fields;

		//! \brief Connection used to query the pg_catalog
		Connection connection;

		//! \brief Stores the last system object identifier. This is used to filter system objects
		QString last_sys_oid;

		//! \brief Indicates if the catalog must filter system objects
		bool filter_sys_objs;

		/*! \brief Executes a query on the catalog for the specified object type. If the parameter 'single_result' is true
		the query will return only one tuple on the result set. Additional attributes can be passed so that SchemaParser will
		use them when parsing the schema file for the object */
		void executeCatalogQuery(const QString &qry_type, ObjectType obj_type, ResultSet &result,
                                 bool single_result=false, attribs_map attribs=attribs_map());

		/*! \brief Recreates the attribute map in such way that attribute names that have
		underscores have this char replaced by dashes. Another special operation made is to replace
		the values of fiels which suffix is _bool to '1' when 't' and to empty when 'f', this is because
		the resultant attribs_map will be passed to XMLParser/SchemaParser which understands bool values as 1 (one) or '' (empty) */
		attribs_map changeAttributeNames(const attribs_map &attribs);

		//! \brief Returns a attribute set for the specified object type and name
		attribs_map getAttributes(const QString &obj_name, ObjectType obj_type, attribs_map extra_attribs=attribs_map());

		/*! \brief Returns the query to retrieve the information if the object (specified by its oid field) is part of
		 a extension. Being part of a extension will cause the object to be created as system object and with
		 SQL code disabled */
		QString getFromExtensionQuery(const QString &oid_field);

		/*! \brief Returns the query that is used to retrieve an objects comment. The 'is_shared_object' is used
		to query the pg_shdescription instead of pg_description */
		QString getCommentQuery(const QString &oid_field, bool is_shared_obj=false);

		//! \brief Creates a comma separated string containing all the oids to be filtered
		QString createOidFilter(const vector<QString> &oids);

	public:
		Catalog(void);

		//! \brief Changes the current connection used by the catalog
		void setConnection(Connection &conn);

		//! \brief Indicates that the catalog will filter system objects excluding them from result sets
		void setFilterSysObjects(bool value);

		//! \brief Returns the current state of system object filtering
		bool isFilterSysObjects(void);

		//! \brief Returns the last system object oid registered on the database
		QString getLastSysObjectOID(void);

		/*! \brief Returns the count for the specified object type. A schema name can be specified
		in order to filter only objects of the specifed schema */
		unsigned getObjectCount(ObjectType obj_type, const QString &sch_name="", const QString &tab_name="");

		/*! \brief Returns a attributes map containing the oids (key) and names (values) of the objects from
		the specified type.	A schema name can be specified in order to filter only objects of the specifed schema */
		attribs_map getObjectsNames(ObjectType obj_type, const QString &sch_name="", const QString &tab_name="");

		//! \brief Returns a set of multiple attributes (several tuples) for the specified object type
		vector<attribs_map> getMultipleAttributes(ObjectType obj_type, attribs_map extra_attribs=attribs_map());

		/*! \brief Retrieve all available objects attributes for the specified type. Internally this method calls the get method for the
		specified type. User can filter items by oids (except for table child objects), by schema (in the object type is suitable to accept schema)
		and by table name (only when retriving child objects for a specific table) */
		vector<attribs_map> getObjectsAttributes(ObjectType obj_type, const QString &schema="", const QString &table="", const vector<QString> &filter_oids={});
};

#endif
