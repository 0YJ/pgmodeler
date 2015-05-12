/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\class LineNumbersWidget
\brief Implements a simple line number widget that expose the current highlighted line in a text editor
*/

#ifndef LINE_NUMBERS_WIDGET_H
#define LINE_NUMBERS_WIDGET_H

#include <QWidget>

class LineNumbersWidget : public QWidget {
  private:
    Q_OBJECT

    //! brief The first line number that must be drawn
    unsigned first_line,

    //! brief The amount of lines to be drawn
    line_count;

    //! brief The y axis increment to start drawn the line number
    int dy;

  protected:
    void paintEvent(QPaintEvent *event);

  public:
    LineNumbersWidget(QWidget * parent = 0);
    void drawLineNumbers(unsigned first_line, unsigned line_count, int dy);
};

#endif
