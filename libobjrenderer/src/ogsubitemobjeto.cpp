#include "ogsubitemobjeto.h"

const QString OGSubItemObjeto::SEPARADOR_TIPO(" ");
const QString OGSubItemObjeto::SEPARADOR_REST(" ");
const QString OGSubItemObjeto::TEXTO_UNIQUE("uq");
const QString OGSubItemObjeto::TEXTO_PRIMARY_KEY("pk");
const QString OGSubItemObjeto::TEXTO_FOREIGN_KEY("fk");
const QString OGSubItemObjeto::TEXTO_NOT_NULL("nn");
const QString OGSubItemObjeto::DELIMITADOR_REST_INI("«");
const QString OGSubItemObjeto::DELIMITADOR_REST_FIM("»");

OGSubItemObjeto::OGSubItemObjeto(TableObject *objeto) : BaseObjectView(objeto)
{
 //O descritor é alocado no método de configuração conforme o tipo do objeto de origem
 descritor=NULL;

 //Aloca os rótulos e os adiciona ao grupo
 for(unsigned i=0; i < 3; i++)
 {
  rotulos[i]=new QGraphicsSimpleTextItem;
  this->addToGroup(rotulos[i]);
 }
}

OGSubItemObjeto::~OGSubItemObjeto(void)
{
 //Desaloca e remove do grupo todos os objetos filhos
 this->removeFromGroup(descritor);
 delete(descritor);

 for(unsigned i=0; i < 3; i++)
 {
  this->removeFromGroup(rotulos[i]);
  delete(rotulos[i]);
 }
}

void OGSubItemObjeto::configurarDescritor(ConstraintType tipo_rest)
{
 ObjectType tipo_obj=BASE_OBJECT;
 Column *coluna=dynamic_cast<Column *>(this->getSourceObject());
 float fator=font_config[ParsersAttributes::GLOBAL].font().pointSizeF()/DEFAULT_FONT_SIZE;
 bool desc_elipse=false;

 //Obtém o tipo do objeto de origem, é com base nele que o descritor será alocado
 if(this->getSourceObject())
  tipo_obj=this->getSourceObject()->getObjectType();

 /* Descritores em forma de elipse são alocados para colunas (com ou sem not-null)
    todos os demais tipos têm descritores poligonais */
 desc_elipse=((coluna && tipo_rest==BaseType::null) ||
              (tipo_obj!=OBJ_INDEX && tipo_obj!=OBJ_RULE &&
               tipo_obj!=OBJ_TRIGGER && tipo_obj!=OBJ_COLUMN));

 //Destrói o descritor quando o tipo atual é diferente do novo
 if(descritor && ((desc_elipse && !dynamic_cast<QGraphicsEllipseItem *>(descritor)) ||
                  (!desc_elipse && dynamic_cast<QGraphicsEllipseItem *>(descritor))))
 {
  this->removeFromGroup(descritor);
  delete(descritor);
  descritor=NULL;
 }

 //Aloca o descritor e o adiciona ao grupo
 if(!descritor)
 {
  //Caso seja em elipse, cria um QGraphicsEllipseItem
  if(desc_elipse)
   descritor=new QGraphicsEllipseItem;
  //Caso contrário que um QGraphicsPolygonItem
  else
   descritor=new QGraphicsPolygonItem;

  this->addToGroup(descritor);
 }

 //Caso o objeto de origem seja coluna
 if(coluna)
 {
  QString atrib;
  QPolygonF pol;

  /* Caso o tipo da restrição não esteja definido (a coluna não tem nenhuma restrição ligada a ela)
     cria um descritor elíptico */
  if(tipo_rest==BaseType::null)
  {
   QGraphicsEllipseItem *desc=dynamic_cast<QGraphicsEllipseItem *>(descritor);

   //Cria um descritor elíptico de 10x10 (por padrão) porém aplica o fator entre a fonte padrão e fonte configurada
   desc->setRect(QRectF(QPointF(0,0),
                        QSizeF(9.0f * fator, 9.0f * fator)));

   if(coluna->isNotNull())
    atrib=ParsersAttributes::NN_COLUMN;
   else
    atrib=ParsersAttributes::COLUMN;

   //Configura o preenchimento de acordo com o atributo selecionado acima
   desc->setBrush(this->getFillStyle(atrib));
   desc->setPen(this->getBorderStyle(atrib));
  }
  //Configura o descritor poligonal
  else
  {
   QGraphicsPolygonItem *desc=dynamic_cast<QGraphicsPolygonItem *>(descritor);

   //Cria um polígono conforme o tipo de restrição da coluna
   if(tipo_rest==ConstraintType::primary_key)
   {
    atrib=ParsersAttributes::PK_COLUMN;
    pol.append(QPointF(2,0)); pol.append(QPointF(0,2)); pol.append(QPointF(0,7));
    pol.append(QPointF(2,9)); pol.append(QPointF(3,8)); pol.append(QPointF(3,6));
    pol.append(QPointF(4,6)); pol.append(QPointF(5,7)); pol.append(QPointF(6,6));
    pol.append(QPointF(7,5)); pol.append(QPointF(9,7)); pol.append(QPointF(9,3));
    pol.append(QPointF(3,3)); pol.append(QPointF(3,1));
   }
   else if(tipo_rest==ConstraintType::foreign_key)
   {
    atrib=ParsersAttributes::FK_COLUMN;
    pol.append(QPointF(0,3)); pol.append(QPointF(0,6)); pol.append(QPointF(4,6));
    pol.append(QPointF(4,9)); pol.append(QPointF(5,9)); pol.append(QPointF(9,5));
    pol.append(QPointF(9,4)); pol.append(QPointF(5,0)); pol.append(QPointF(4,0));
    pol.append(QPointF(4,3));
   }
   else if(tipo_rest==ConstraintType::unique)
   {
    atrib=ParsersAttributes::UQ_COLUMN;
    pol.append(QPointF(4,0)); pol.append(QPointF(0,4)); pol.append(QPointF(0,5));
    pol.append(QPointF(4,9)); pol.append(QPointF(5,9)); pol.append(QPointF(9,5));
    pol.append(QPointF(9,4)); pol.append(QPointF(5,0));
   }

   if(fator!=1.0f)
    this->resizePolygon(pol,
                                pol.boundingRect().width() * fator,
                                pol.boundingRect().height()  * fator);

   //Atribui o polígono configurado e configura o estilo de cores do descritor
   desc->setPolygon(pol);
   desc->setBrush(this->getFillStyle(atrib));
   desc->setPen(this->getBorderStyle(atrib));
  }
 }
 //Configura um descritor poligonal para indice, regra ou gatilho
 else if(tipo_obj==OBJ_INDEX ||
         tipo_obj==OBJ_RULE ||
         tipo_obj==OBJ_TRIGGER)
 {
  TableObject *objeto_tab=dynamic_cast<TableObject *>(this->getSourceObject());
  QGraphicsPolygonItem *desc=dynamic_cast<QGraphicsPolygonItem *>(descritor);
  QPolygonF pol;

  pol.append(QPointF(5,0)); pol.append(QPointF(0,5)); pol.append(QPointF(4,9));
  pol.append(QPointF(9,9)); pol.append(QPointF(9,4));

  if(fator!=1.0f)
   this->resizePolygon(pol,
                               pol.boundingRect().width() * fator ,
                               pol.boundingRect().height() * fator);

  desc->setPolygon(pol);
  desc->setBrush(this->getFillStyle(objeto_tab->getSchemaName()));
  desc->setPen(this->getBorderStyle(objeto_tab->getSchemaName()));
 }
 //Configura um descritor elíptico padrão (usado para referências de visões)
 else
 {
  QGraphicsEllipseItem *desc=dynamic_cast<QGraphicsEllipseItem *>(descritor);

  desc->setRect(QRectF(QPointF(0,0),
                       QSizeF(9.0f * fator, 9.0f * fator)));
  desc->setBrush(this->getFillStyle(ParsersAttributes::REFERENCE));
  desc->setPen(this->getBorderStyle(ParsersAttributes::REFERENCE));
 }
}

void OGSubItemObjeto::configureObject(void)
{
 //Caso haja um objeto de tabela atribuído ao subitem
 if(this->getSourceObject())
 {
  QTextCharFormat fmt;
  float px;
  QString str_rest;
  TableObject *objeto_tab=dynamic_cast<TableObject *>(this->getSourceObject());
  Column *coluna=dynamic_cast<Column *>(objeto_tab);
  ConstraintType tipo_rest=ConstraintType::null;

  //Caso seja uma coluna
  if(coluna)
  {
   //Obtém a string de restrições relacionads �   coluna
   str_rest=this->obterStringRestricoes(coluna);

   /* Determina o tipo primário de restrição conforme as checagens a seguir.
      É com base no tipo da restrição que o descritor será criado. Adicionalmente
      obtém a formatação de fonte para o tipo da restrição */
   if(str_rest.find(TEXTO_PRIMARY_KEY)>=0)
   {
    fmt=font_config[ParsersAttributes::PK_COLUMN];
    tipo_rest=ConstraintType::primary_key;
   }
   else if(str_rest.find(TEXTO_FOREIGN_KEY)>=0)
   {
    fmt=font_config[ParsersAttributes::FK_COLUMN];
    tipo_rest=ConstraintType::foreign_key;
   }
   else if(str_rest.find(TEXTO_UNIQUE)>=0)
   {
    fmt=font_config[ParsersAttributes::UQ_COLUMN];
    tipo_rest=ConstraintType::unique;
   }
   else if(str_rest.find(TEXTO_NOT_NULL)>=0)
    fmt=font_config[ParsersAttributes::NN_COLUMN];
   else
    fmt=font_config[ParsersAttributes::COLUMN];

   if(coluna->isAddedByRelationship())
    fmt=font_config[ParsersAttributes::INH_COLUMN];
   else if(coluna->isProtected())
    fmt=font_config[ParsersAttributes::PROT_COLUMN];
  }
  //Caso não seja uma coluna, obtém a formatação para o tipo do objeto de tabela
  else
   fmt=font_config[objeto_tab->getSchemaName()];

  //Configura o descritor com o tipo da restrição
  configurarDescritor(tipo_rest);

  //Posiciona o descritor como o primeiro item
  descritor->setPos(HORIZ_SPACING, 1);
  px=descritor->pos().x() + descritor->boundingRect().width() + (2 * HORIZ_SPACING);

  /* Configurando os rótulos do subitem.
     Os rótulos do subitem têm o seguinte esquema: [nome do objeto] [tipo] [restrições] */
  rotulos[0]->setText(QString::fromUtf8(objeto_tab->getName()));
  rotulos[0]->setFont(fmt.font());
  rotulos[0]->setBrush(fmt.foreground());
  rotulos[0]->setPos(px, 0);
  px+=rotulos[0]->boundingRect().width();

  //Configura o rótulo de tipo
  fmt=font_config[ParsersAttributes::OBJECT_TYPE];
  if(coluna)
   rotulos[1]->setText(QString::fromUtf8(SEPARADOR_TIPO + (*coluna->getType())));
  else
   rotulos[1]->setText(QString::fromUtf8(SEPARADOR_TIPO + objeto_tab->getSchemaName()));

  rotulos[1]->setFont(fmt.font());
  rotulos[1]->setBrush(fmt.foreground());
  rotulos[1]->setPos(px, 0);
  px+=rotulos[1]->boundingRect().width() + (3 * HORIZ_SPACING);

  /* Configura o rótulo de restrições. Para objetos índice, regras e gatilho
     o rótulo de restrições armazena informações sobre modo de disparo,
     eventos, entre outros */
  fmt=font_config[ParsersAttributes::CONSTRAINTS];
  if(coluna)
   rotulos[2]->setText(QString::fromUtf8(str_rest));
  else
  {
   Rule *regra=dynamic_cast<Rule *>(objeto_tab);
   Trigger *gatilho=dynamic_cast<Trigger *>(objeto_tab);
   Index *indice=dynamic_cast<Index *>(objeto_tab);

   //Configurando a string de restrições para regra
   if(regra)
   {
    str_rest+=(~regra->getEventType()).mid(3,1);
    str_rest+=SEPARADOR_REST;
    str_rest+=(~regra->getExecutionType()).mid(0,1);
    str_rest=str_rest.lower();
   }
   //Configurando a string de restrições para gatilho
   else if(gatilho)
   {
    str_rest+=(~gatilho->getFiringType()).mid(0,1);
    str_rest+=SEPARADOR_REST;

    for(unsigned i=EventType::on_insert; i < EventType::on_truncate; i++)
    {
     if(gatilho->isExecuteOnEvent(EventType(i)))
      str_rest+=(~EventType(i)).mid(3,1);
    }
    str_rest=str_rest.lower();
   }
   //Configurando a string de restrições para índice
   else if(indice)
   {
    if(indice->getIndexAttribute(Index::UNIQUE))
     str_rest+="u";

    if(indice->getIndexAttribute(Index::CONCURRENT))
     str_rest+="c";

    if(indice->getIndexAttribute(Index::FAST_UPDATE))
     str_rest+="f";
   }

   if(!str_rest.isEmpty())
    rotulos[2]->setText(QString::fromUtf8(DELIMITADOR_REST_INI + " " +
                                          str_rest + " " +
                                          DELIMITADOR_REST_FIM));

  }

  rotulos[2]->setFont(fmt.font());
  rotulos[2]->setBrush(fmt.foreground());
  rotulos[2]->setPos(px, 0);

  //Calcula o retângulo de dimensão do subitem, que é composto pela junção de todas as dimensões dos objetos (descritor e rótulos)
  descritor->setPos(HORIZ_SPACING, rotulos[0]->boundingRect().center().y() - descritor->boundingRect().center().y());
  bounding_rect.setTopLeft(QPointF(descritor->boundingRect().left(), rotulos[0]->boundingRect().top()));

  //Caso particular: Caso o rótulo de restrições esteja vazio usa a dimensão do rótulo de tipo
  if(rotulos[2]->boundingRect().width()==0)
   bounding_rect.setBottomRight(QPointF(rotulos[1]->boundingRect().right(), rotulos[0]->boundingRect().bottom()));
  else
   bounding_rect.setBottomRight(QPointF(rotulos[2]->boundingRect().right(), rotulos[0]->boundingRect().bottom()));
 }
}

void OGSubItemObjeto::configureObject(Reference referencia)
{
 QTextCharFormat fmt;
 float px;
 QString str_aux;

 //Configura e posiciona o descritor da referência
 configurarDescritor();
 descritor->setPos(HORIZ_SPACING, 1);
 px=descritor->pos().x() + descritor->boundingRect().width() + (2 * HORIZ_SPACING);

 //Caso o tipo da referência seja a uma coluna
 if(referencia.getReferenceType()==Reference::REFER_COLUMN)
 {
  //Configura o rótulo de nome no formato: [tabela].[coluna]
  fmt=font_config[ParsersAttributes::REF_TABLE];
  rotulos[0]->setText(referencia.getTable()->getName() + ".");
  rotulos[0]->setFont(fmt.font());
  rotulos[0]->setBrush(fmt.foreground());
  rotulos[0]->setPos(px, 0);
  px+=rotulos[0]->boundingRect().width();

  fmt=font_config[ParsersAttributes::REF_COLUMN];
  if(referencia.getColumn())
   rotulos[1]->setText(referencia.getColumn()->getName());
  else
   rotulos[1]->setText("*");

  rotulos[1]->setFont(fmt.font());
  rotulos[1]->setBrush(fmt.foreground());
  rotulos[1]->setPos(px, 0);
  px+=rotulos[1]->boundingRect().width();
 }
 //Caso a referência seja a uma expressão
 else
 {
  fmt=font_config[ParsersAttributes::REF_TABLE];

  //Trunca a expressão em 20 caracters caso a mesma ultrapasse este comprimento
  str_aux=referencia.getExpression().mid(0,20);
  if(referencia.getExpression().size() > 20) str_aux+="...";

  rotulos[0]->setText(str_aux);
  rotulos[0]->setFont(fmt.font());
  rotulos[0]->setBrush(fmt.foreground());
  rotulos[1]->setText("");
  rotulos[0]->setPos(px, 0);
  px+=rotulos[0]->boundingRect().width();
 }

 //Caso a referência possua um alias configura o rótulo para exibi-lo
 if((referencia.getColumn() && referencia.getColumnAlias()!="") ||
    (referencia.getAlias()!="" && referencia.getReferenceType()==Reference::REFER_EXPRESSION))
 {
  if(referencia.getReferenceType()==Reference::REFER_EXPRESSION)
   str_aux=referencia.getAlias();
  else
   str_aux=referencia.getColumnAlias();

  str_aux=" (" + str_aux + ") ";
  fmt=font_config[ParsersAttributes::ALIAS];
  rotulos[2]->setText(str_aux);
  rotulos[2]->setFont(fmt.font());
  rotulos[2]->setBrush(fmt.foreground());
  rotulos[2]->setPos(px, 0);
 }

 //Configura o retângulo de dimensão do subitem
 descritor->setPos(HORIZ_SPACING, rotulos[0]->boundingRect().center().y() - descritor->boundingRect().center().y());
 bounding_rect.setTopLeft(QPointF(descritor->pos().x(), rotulos[0]->pos().y()));

 if(rotulos[2]->text().isEmpty())
  bounding_rect.setBottomRight(QPointF(rotulos[1]->boundingRect().right(), rotulos[0]->boundingRect().bottom()));
 else
  bounding_rect.setBottomRight(QPointF(rotulos[2]->boundingRect().right(), rotulos[0]->boundingRect().bottom()));
}

void OGSubItemObjeto::definirPosXObjetoFilho(unsigned idx_obj, float px)
{
 if(idx_obj >= 4)
  throw Exception(ERR_REF_OBJ_INV_INDEX, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 if(idx_obj==0)
  descritor->setPos(px, descritor->pos().y());
 else
  rotulos[idx_obj-1]->setPos(px, rotulos[idx_obj-1]->pos().y());
}

QGraphicsItem *OGSubItemObjeto::obterObjetoFilho(unsigned idx_obj)
{
 if(idx_obj >= 4)
  throw Exception(ERR_REF_OBJ_INV_INDEX, __PRETTY_FUNCTION__, __FILE__, __LINE__);

 if(idx_obj==0)
  return(descritor);
 else
  return(rotulos[idx_obj-1]);
}

QString OGSubItemObjeto::obterStringRestricoes(Column *coluna)
{
 if(coluna && coluna->getParentTable())
 {
  Table *tabela=dynamic_cast<Table *>(coluna->getParentTable());
  QString str_rest;
  Constraint *restricao=NULL;
  vector<Constraint *>::iterator itr,itr_end;
  vector<Constraint *> restricoes;
  ConstraintType tipo_rest;
  unsigned i, qtd;

  qtd=tabela->getConstraintCount();
  for(i=0; i < qtd; i++)
   restricoes.push_back(tabela->getConstraint(i));

  //Obtém as referências ao primeiro e último elemento da lita de constraints
  itr=restricoes.begin();
  itr_end=restricoes.end();

  while(itr!=itr_end)
  {
   restricao=(*itr); //Obtém uma constraint
   itr++; //Passa para a próxima da lista

   //Verfica se a coluna está sendo referenciada dentro da constraint
   if(restricao->getColumn(coluna->getName(), Constraint::SOURCE_COLS))
   {
    //Obtém o tipo da constraint
    tipo_rest=restricao->getConstraintType();

    //Para cada tipo de constraint concatena-se o texto referente
    //   a mesma e um separadar de textos de constraints
    if(tipo_rest==ConstraintType::primary_key)
     str_rest=TEXTO_PRIMARY_KEY + SEPARADOR_REST;
    else if(tipo_rest==ConstraintType::foreign_key)
     str_rest+=TEXTO_FOREIGN_KEY + SEPARADOR_REST;
    else if(tipo_rest==ConstraintType::unique)
     str_rest+=TEXTO_UNIQUE + SEPARADOR_REST;
   }
  }

  //Caso a coluna seja não-nula adiciona a QString formatada o texto referente�   constraint NOT NULL
  if(coluna->isNotNull()) str_rest+=TEXTO_NOT_NULL + SEPARADOR_REST;

  //Caso a QString formatada tenha sido criada
  if(str_rest!="")
   //Termina sua formatação concatenando a QString formatada aos caracteres delimitadores de restricoes
   str_rest= DELIMITADOR_REST_INI +
              SEPARADOR_REST + str_rest +
              DELIMITADOR_REST_FIM;

  return(str_rest);
 }
 else return("");
}

