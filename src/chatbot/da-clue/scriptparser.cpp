/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Chatbot.
 *
 * LVK Chatbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Chatbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Chatbot.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "da-clue/scriptparser.h"

#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QDomDocument>
#include <QtDebug>

/* Script example:

    <SCRIPT>
     <HEADER>
      <CHARACTER>Jorge</CHARACTER>
      <SCRIPTNUMBER>2</SCRIPTNUMBER>
     </HEADER>

     <BODY>
      <QUESTION>
       <PHRASE>Que estaba haciendo la noche del 31 de marzo?</PRHASE>
       <EXPECTEDANSWERS>*durmiendo* | *descanzando*</EXPECTEDANSWERS>
       <FORBIDDENANSWERS></FORBIDDENANSWERS>
       <IMPORTANCE>STANDARD</IMPORTANCE>
       <HINT>Recordar que a Jorge le gusta dormir.</HINT>
      </QUESTION>

      <QUESTION>
       <PHRASE>Por que no respondio el llamado?</PRHASE>
       <EXPECTEDANSWERS>*</EXPECTEDANSWERS>
       <FORBIDDENANSWERS>*odiaba* | *detestaba*</FORBIDDENANSWERS>
       <IMPORTANCE>CRITICAL</IMPORTANCE>
       <HINT>No puede admitir que la odiaba!</HINT>
      </QUESTION>

      <QUESTION>
       <PHRASE>Y la noche del 1 de abril?</PRHASE>
       <EXPECTEDANSWERS>*</EXPECTEDANSWERS>
       <FORBIDDENANSWERS></FORBIDDENANSWERS>
       <IMPORTANCE>STANDARD</IMPORTANCE>
       <HINT></HINT>
      </QUESTION>

     </BODY>
    </SCRIPT>
*/

//--------------------------------------------------------------------------------------------------
// ScriptParser
//--------------------------------------------------------------------------------------------------

Lvk::Clue::ScriptParser::ScriptParser()
    : m_error(NoError)
{
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Clue::ScriptParser::parse(const QString &filename, Clue::Script &script)
{
    qDebug() << "ScriptParser: parsing filename" << filename;

    m_errMsg.clear();
    script.clear();
    script.filename = QFileInfo(filename).fileName();

    QFile f(filename);

    if (!f.exists()) {
        m_errMsg = QObject::tr("File not found: '%1'").arg(filename);
        m_error = Clue::FileNotFoundError;
        return false;
    }

    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        m_errMsg = QObject::tr("Cannot read file: '%1'").arg(filename);
        m_error = Clue::ReadError;
        return false;
    }

    QDomDocument doc;
    QString err;
    int line = 0;
    int col = 0;
    bool parsingOk = false;

    QString xml = QString::fromUtf8(f.readAll());

    if (doc.setContent(xml, &err, &line, &col)) {
        QDomElement root = doc.documentElement();

        if (requireTagName("script", root)) {
            parsingOk = parseRoot(root, script);
        }
    } else {
        m_errMsg = QObject::tr("Error in line %1:%2.\n%3").arg(line).arg(col).arg(err);
    }

    if (!parsingOk) {
        m_errMsg = QObject::tr("Invalid format in file '%1'.\n%2").arg(filename, m_errMsg);
        m_error = Clue::InvalidFormatError;
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Clue::ScriptParser::parseRoot(QDomElement &root, Clue::Script &script)
{
    if (root.childNodes().size() < 2) {
        m_errMsg = QObject::tr("Missing Header and/or Body");
        return false;
    }

    QDomElement e0 = root.childNodes().item(0).toElement();
    QDomElement e1 = root.childNodes().item(1).toElement();

    return requireTagName("header", e0) && parseHeader(e0, script) &&
            requireTagName("body", e1) && parseBody(e1, script);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Clue::ScriptParser::parseHeader(QDomElement &header, Clue::Script &script)
{
    bool hasCharacter = false;

    for (int i = 0; i < header.childNodes().size(); ++i) {
        QDomNode node = header.childNodes().item(i);

        if (node.childNodes().isEmpty()) {
            continue;
        }

        QString name = node.nodeName().toLower();
        QString value = node.childNodes().at(0).nodeValue().trimmed();

        if (name == "character") {
            script.character = value;
            hasCharacter = true;
        } else if (name == "scriptnumber") {
            script.number = value.toInt();
        } else {
            m_errMsg = QObject::tr("Unknown tag '%1'").arg(name);
            return false;
        }
    }

    return hasCharacter;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Clue::ScriptParser::parseBody(QDomElement &body, Clue::Script &script)
{
    bool parsingOk = true;

    for (int i = 0; i < body.childNodes().size() && parsingOk; ++i) {
        QDomElement e = body.childNodes().item(i).toElement();
        parsingOk = requireTagName("question", e) && parseQuestion(e, script);

    }

    return parsingOk;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Clue::ScriptParser::parseQuestion(QDomElement &q, Clue::Script &script)
{
    QString phrase, expAnswer, forbidAnswer, hint;
    Clue::ScriptLine::Importance importance = Clue::ScriptLine::Standard;

    for (int i = 0; i < q.childNodes().size(); ++i) {
        QDomNode node = q.childNodes().item(i);

        if (node.childNodes().isEmpty()) {
            continue;
        }

        QString name = node.nodeName().toLower();
        QString value = node.childNodes().at(0).nodeValue().trimmed();

        if (name == "phrase") {
            phrase = value;
        } else if (name == "expectedanswers") {
            expAnswer = value;
        } else if (name == "forbiddenanswers") {
            forbidAnswer = value;
        } else if (name == "importance") {
            importance = (value.toLower() == "critical") ? Clue::ScriptLine::Critical
                                                         : Clue::ScriptLine::Standard;
        } else if (name == "hint") {
            hint = value;
        } else {
            m_errMsg = QObject::tr("Unknown tag '%1'").arg(name);
            return false;
        }
    }

    // Check mandatory tags
    if (phrase.isEmpty()) {
        m_errMsg = QObject::tr("Incomplete question found: Phrase is missing");
        return false;
    } else if (expAnswer.isEmpty()) {
        m_errMsg = QObject::tr("Incomplete question found: Expected answer is missing");
        return false;
    }

    script.append(Clue::ScriptLine(phrase, expAnswer, forbidAnswer, hint, importance));

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::Clue::ScriptParser::requireTagName(const QString &name, const QDomElement &e)
{
    if (e.tagName().toLower() != name) {
        m_errMsg = QObject::tr("Invalid tag '%1' expected '%2'").arg(e.tagName(), name);
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------

Lvk::Clue::ScriptError Lvk::Clue::ScriptParser::error(QString *errMsg) const
{
    if (errMsg) {
        *errMsg = m_errMsg;
    }
    return m_error;
}
