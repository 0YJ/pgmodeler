#include "objectselectorwidget.h"

QObject *ObjectSelectorWidget::current_selector=NULL;

ObjectSelectorWidget::ObjectSelectorWidget(ObjectType sel_obj_type, bool install_highlighter, QWidget *parent): QWidget(parent)
{
	try
	{
		Ui_ObjectSelectorWidget::setupUi(this);

		obj_view_wgt=new VisaoObjetosWidget(true);

		model=NULL;
		selected_obj=NULL;
		obj_name_hl=NULL;

		this->sel_obj_type=sel_obj_type;

		if(install_highlighter)
		{
			obj_name_hl=new SyntaxHighlighter(obj_name_txt, false);
			obj_name_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																			GlobalAttributes::DIR_SEPARATOR +
																			GlobalAttributes::SQL_HIGHLIGHT_CONF +
																			GlobalAttributes::CONFIGURATION_EXT);
		}

		connect(sel_object_tb, SIGNAL(clicked(bool)), this, SLOT(showObjectView(void)));
		connect(rem_object_tb, SIGNAL(clicked(bool)), this, SLOT(clearSelector(void)));
		connect(obj_view_wgt, SIGNAL(s_visibilityChanged(BaseObject*,bool)), this, SLOT(showSelectedObject(BaseObject*, bool)));
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

ObjectSelectorWidget::~ObjectSelectorWidget(void)
{
	delete(obj_view_wgt);
}

BaseObject *ObjectSelectorWidget::getSelectedObject(void)
{
	return(selected_obj);
}

void ObjectSelectorWidget::setSelectedObject(BaseObject *object)
{
	ObjectType obj_type;
	QString obj_name;

	if(object)
	{
		obj_type=object->getObjectType();

		if(obj_type==OBJ_FUNCTION)
			obj_name=dynamic_cast<Function *>(object)->getSignature();
		else if(obj_type==OBJ_OPERATOR)
			obj_name=dynamic_cast<Operator *>(object)->getSignature();
		else if(obj_type==OBJ_COLUMN || obj_type==OBJ_CONSTRAINT || obj_type==OBJ_RULE ||
						obj_type==OBJ_TRIGGER ||obj_type==OBJ_INDEX)
		{
			BaseObject *tab_pai=dynamic_cast<TableObject *>(object)->getParentTable();
			if(tab_pai)
				obj_name+=tab_pai->getName(true) + ".";

			obj_name+=object->getName();
		}
		else
			obj_name=object->getName(true);
	}

	if(object && obj_type==sel_obj_type)
	{
		obj_name_txt->setPlainText(QString::fromUtf8(obj_name));
		rem_object_tb->setEnabled(object);
		this->selected_obj=object;
		emit s_objectSelected();
	}
	else
		clearSelector();
}

void ObjectSelectorWidget::setModel(DatabaseModel *modelo)
{
	this->model=modelo;
}

void ObjectSelectorWidget::showSelectedObject(BaseObject *obj_sel, bool)
{
	if(ObjectSelectorWidget::current_selector==this && obj_sel)
		setSelectedObject(obj_sel);
}

void ObjectSelectorWidget::clearSelector(void)
{
	this->selected_obj=NULL;
	obj_name_txt->clear();
	rem_object_tb->setEnabled(false);
	emit s_selectorCleared();
}

void ObjectSelectorWidget::showObjectView(void)
{
	ObjectSelectorWidget::current_selector=this;
	obj_view_wgt->definirObjetoVisivel(sel_obj_type, true);
	obj_view_wgt->definirModelo(this->model);
	obj_view_wgt->show();
}

