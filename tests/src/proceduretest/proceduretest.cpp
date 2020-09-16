/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include <QtTest/QtTest>
#include "databasemodel.h"
#include "globalattributes.h"
#include "pgmodelerunittest.h"
#include "defaultlanguages.h"
#include "qtcompat/qtextstreamcompat.h"

class ProcedureTest: public QObject, public PgModelerUnitTest {
	private:
		Q_OBJECT

	public:
		ProcedureTest() : PgModelerUnitTest(SCHEMASDIR) {}

	private slots:
		void throwsErrorOnInvalidParameter();
		void generatesNameAndSignatureCorrectly();
		void generatesSQLCorrectly();
		void generatesXMLCorrectly();
		void modelReturnsProcedureDepsRefs();
		void modelCreatesProcedureFromXML();
};

void ProcedureTest::throwsErrorOnInvalidParameter()
{
	Procedure proc;

	try
	{
		Parameter param;
		param.setName("p1");
		param.setOut(true);
		param.setIn(false);
		proc.addParameter(param);
		QFAIL("No error related to invalid parameter raised!");
	}
	catch(Exception &e)
	{
		QCOMPARE(e.getErrorCode(), ErrorCode::InvProcedureParamOutMode);
	}
}

void ProcedureTest::generatesNameAndSignatureCorrectly()
{
	Procedure proc;
	Schema schema;
	Parameter param;
	QString expected_sign = "public.procedure(smallint,text,integer,VARIADIC \"any\")";

	try
	{
		schema.setName("public");

		proc.setSchema(&schema);
		proc.setName("procedure");

		param.setName("p1");
		param.setType(PgSqlType("smallint"));
		param.setIn(true);
		param.setOut(false);
		proc.addParameter(param);

		param.setName("p2");
		param.setType(PgSqlType("text"));
		param.setIn(true);
		param.setOut(true);
		proc.addParameter(param);

		param.setName("p3");
		param.setType(PgSqlType("integer"));
		param.setIn(true);
		param.setOut(false);
		proc.addParameter(param);

		param.setName("p4");
		param.setType(PgSqlType("\"any\""));
		param.setVariadic(true);
		proc.addParameter(param);

		QCOMPARE(proc.getSignature(), expected_sign);
	}
	catch(Exception &e)
	{
		QFAIL(e.getErrorMessage().toStdString().c_str());
	}
}

void ProcedureTest::generatesSQLCorrectly()
{
	Procedure proc;
	Schema schema;
	Parameter param;
	Language lang;
	QString expected_code, result_code;

	expected_code = QString("-- object: public.procedure | type: PROCEDURE --\n\
-- DROP PROCEDURE IF EXISTS public.procedure(smallint,text,integer,VARIADIC \"any\") CASCADE;\n\
CREATE PROCEDURE public.procedure (IN p1 smallint, IN OUT p2 text, IN p3 integer, VARIADIC p4 \"any\")\n\
	LANGUAGE sql\n\
	SECURITY INVOKER\n\
	AS $$\n\
select 1+1;\n\
$$;\n\
-- ddl-end --\n\
COMMENT ON PROCEDURE public.procedure(smallint,text,integer,VARIADIC \"any\") IS E'This is a comment!';\n\
-- ddl-end --\n");

	try
	{
		schema.setName("public");
		lang.BaseObject::setName(DefaultLanguages::Sql);

		proc.setSchema(&schema);
		proc.setLanguage(&lang);
		proc.setName("procedure");
		proc.setSourceCode("select 1+1;");
		proc.setComment("This is a comment!");

		param.setName("p1");
		param.setType(PgSqlType("smallint"));
		param.setIn(true);
		param.setOut(false);
		proc.addParameter(param);

		param.setName("p2");
		param.setType(PgSqlType("text"));
		param.setIn(true);
		param.setOut(true);
		proc.addParameter(param);

		param.setName("p3");
		param.setType(PgSqlType("integer"));
		param.setIn(true);
		param.setOut(false);
		proc.addParameter(param);

		param.setName("p4");
		param.setType(PgSqlType("\"any\""));
		param.setVariadic(true);
		proc.addParameter(param);

		result_code = proc.getCodeDefinition(SchemaParser::SqlDefinition);

		/*QTextStream out(stdout);
		out << result_code.simplified() << QtCompat::endl;
		out << expected_code.simplified() << QtCompat::endl;*/
		QCOMPARE(result_code.simplified(), expected_code.simplified());
	}
	catch(Exception &e)
	{
		QFAIL(e.getExceptionsText().toStdString().c_str());
	}
}

void ProcedureTest::generatesXMLCorrectly()
{
	Procedure proc;
	Schema schema;
	Parameter param;
	Language lang;
	QString expected_code, result_code;

	expected_code = QString("<procedure name=\"procedure\" security-type=\"SECURITY INVOKER\">\n\
	<schema name=\"public\"/>\n\
	<comment><![CDATA[This is a comment!]]></comment>\n\
	<appended-sql><![CDATA[-- APPENDED SQL --;]]></appended-sql>\n\
	<prepended-sql><![CDATA[-- PREPENDED SQL --;]]></prepended-sql>\n\
	<language name=\"sql\"/>\n\
	<parameter name=\"p1\" in=\"true\">\n\
		<type name=\"smallint\" length=\"0\"/>\n\
	</parameter>\n\
	<parameter name=\"p2\" in=\"true\" out=\"true\">\n\
		<type name=\"text\" length=\"0\"/>\n\
	</parameter>\n\
	<parameter name=\"p3\" in=\"true\">\n\
		<type name=\"integer\" length=\"0\"/>\n\
	</parameter>\n\
	<parameter name=\"p4\" variadic=\"true\">\n\
		<type name=\"&quot;any&quot;\" length=\"0\"/>\n\
	</parameter>\n\
	<definition><![CDATA[select 1+1;]]></definition>\n\
</procedure>\n\
");

	try
	{
		schema.setName("public");
		lang.BaseObject::setName(DefaultLanguages::Sql);

		proc.setAppendedSQL("-- APPENDED SQL --;");
		proc.setPrependedSQL("-- PREPENDED SQL --;");

		proc.setSchema(&schema);
		proc.setLanguage(&lang);
		proc.setName("procedure");
		proc.setSourceCode("select 1+1;");
		proc.setComment("This is a comment!");

		param.setName("p1");
		param.setType(PgSqlType("smallint"));
		param.setIn(true);
		param.setOut(false);
		proc.addParameter(param);

		param.setName("p2");
		param.setType(PgSqlType("text"));
		param.setIn(true);
		param.setOut(true);
		proc.addParameter(param);

		param.setName("p3");
		param.setType(PgSqlType("integer"));
		param.setIn(true);
		param.setOut(false);
		proc.addParameter(param);

		param.setName("p4");
		param.setType(PgSqlType("\"any\""));
		param.setVariadic(true);
		proc.addParameter(param);

		result_code = proc.getCodeDefinition(SchemaParser::XmlDefinition);

		/*QTextStream out(stdout);
		out << result_code << QtCompat::endl;
		out << "---" << QtCompat::endl;
		out << expected_code << QtCompat::endl;*/
		QCOMPARE(result_code.simplified(), expected_code.simplified());
	}
	catch(Exception &e)
	{
		QFAIL(e.getExceptionsText().toStdString().c_str());
	}
}

void ProcedureTest::modelReturnsProcedureDepsRefs()
{
	QFAIL("Not implemented!");
}

void ProcedureTest::modelCreatesProcedureFromXML()
{
	QFAIL("Not implemented!");
}

QTEST_MAIN(ProcedureTest)
#include "proceduretest.moc"
