/***************************************************************************
                          aimlparser.h  -  description
                             -------------------
    begin                : mer sep 21 2005
    copyright            : (C) 2005 by houssem bdioui
    email                : houssem.bdioui@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef AIMLPARSER_H
#define AIMLPARSER_H

#include <qdom.h>
#include <qmap.h>
#if QT_VERSION >= 0x040000
#include <q3ptrlist.h>
#define QPTRLIST_CLASSNAME Q3PtrList
#include <q3valuelist.h>
#define QVALUELIST_CLASSNAME Q3ValueList
#else
#include <qptrlist.h>
#define QPTRLIST_CLASSNAME QPtrList
#include <qvaluelist.h>
#define QVALUELIST_CLASSNAME QValueList
#endif
#include <qstringlist.h>
#include <qregexp.h>

#include <QDataStream> // lvk

#define MAX_LIST_LENGTH 50

/**
  *@author houssem bdioui
  */

struct Node;

struct Leaf
{
  Node *parent;
  QDomNode tmplate;
  QString topic;
  QString that;
  Leaf();
};

struct Node
{
  Node *parent;
  QString word;
  Node();
  QPTRLIST_CLASSNAME<Node> childs;
  QPTRLIST_CLASSNAME<Leaf> leafs;
  void debug(QDataStream &logStream, uint indent = 0);
  bool match(QStringList::const_iterator input, const QStringList &inputWords,
             const QString &currentThat, const QString &currentTopic, QStringList &capturedThatTexts,
             QStringList &capturedTopicTexts, Leaf *&leaf);
};

class AIMLParser
{

public:
  AIMLParser(QIODevice *logDevice);
  virtual ~AIMLParser();
  bool loadAiml(const QString&);

  // lvk start
  bool loadAimlFromString(const QString& xml);
  // lvk end

  bool loadSubstitutions(const QString&);
  bool loadVars(const QString&, const bool&);
  bool saveVars(const QString &);
  QString getResponse(QString, const bool &srai = false);
  void displayTree();

private:
  QString resolveNode(QDomNode*, const QStringList & = QStringList(),
     const QStringList & = QStringList(), const QStringList & = QStringList());
  void parseCategory(QDomNode*);
  void normalizeString(QString &);

  // lvk: removed for security reasons
  //QString executeCommand(const QString&);

  QMap<QString, QString> _parameterValue;
  QMap<QString, QString> _botVarValue;
  QVALUELIST_CLASSNAME<QRegExp> _subOld;
  QStringList _subNew;
  QStringList _inputList;
  QVALUELIST_CLASSNAME<QStringList> _thatList;
  Node _root;
  int _indent;
  QVALUELIST_CLASSNAME<QDomNode*> _visitedNodeList;
  QDataStream _logStream; //lvk
};

#endif




