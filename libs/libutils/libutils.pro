include(../../pgmodeler.pri)

TEMPLATE = lib
TARGET = utils
windows: DESTDIR = $$PWD

HEADERS += src/exportsym.h \
	   src/exception.h \
	   src/enumtype.h \
           src/globalattributes.h \
           src/pgsqlversions.h \
	   src/doublenan.h \
	   src/application.h \
	   src/utilsns.h

SOURCES += src/exception.cpp \
           src/globalattributes.cpp \
	   src/pgsqlversions.cpp \
	   src/application.cpp \
	   src/utilsns.cpp

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
