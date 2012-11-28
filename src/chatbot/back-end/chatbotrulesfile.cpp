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

#include "back-end/chatbotrulesfile.h"

#include <QUuid>
#include <QFile>
#include <QDataStream>
#include <QtDebug>
#include <exception>

#define CRF_MAGIC_NUMBER            (('c'<<0) | ('r'<<8) | ('f'<<16) | ('\0'<<24))
#define CRF_FILE_FORMAT_VERSION     2

#define CEF_MAGIC_NUMBER            (('c'<<0) | ('e'<<8) | ('f'<<16) | ('\0'<<24))
#define CEF_FILE_FORMAT_VERSION     2


//--------------------------------------------------------------------------------------------------
// Non-members Helpers
//--------------------------------------------------------------------------------------------------

namespace
{

inline QString nullChatbotId()
{
    static QString nullId = "00000000-0000-0000-0000-000000000000";

    return nullId;
}

//--------------------------------------------------------------------------------------------------

inline QString newChatbotId()
{
    QString id = QUuid::createUuid().toString().mid(1, 36);

    return id;
}

} // namespace


//--------------------------------------------------------------------------------------------------
// ChatbotRulesFile
//--------------------------------------------------------------------------------------------------

Lvk::BE::ChatbotRulesFile::ChatbotRulesFile()
    : m_dirty(false),
      m_rootRule(new Rule()),
      m_nextRuleId(1),
      m_chatbotId(newChatbotId())
{
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::ChatbotRulesFile::ChatbotRulesFile(const QString &filename)
    : m_dirty(false),
      m_rootRule(new Rule()),
      m_nextRuleId(1),
      m_chatbotId(nullChatbotId())
{
    if (!load(filename)) {
        throw std::exception();
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ChatbotRulesFile::load(const QString &filename)
{
    if (!m_filename.isEmpty()) {
        close();
    }

    m_filename = filename;

    bool success = false;

    QFile file(m_filename);

    if (file.open(QFile::ReadOnly)) {
        success = read(file);
    } else {
        qCritical() << "Cannot open chatbot rules file" << filename;
    }

    if (success) {
        setAsSaved();
    } else {
        close();
    }

    return success;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ChatbotRulesFile::save()
{
    bool success = false;

    QFile file(m_filename);

    if (file.open(QFile::WriteOnly)) {
        success = write(file);
    }

    if (success) {
        setAsSaved();
    }

    return success;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ChatbotRulesFile::saveAs(const QString &filename)
{
    QString filenameBak = m_filename;
    QString chatbotIdBak = m_chatbotId;

    if (m_filename.size() > 0 /*&& m_filename != filename*/) {
        m_chatbotId = newChatbotId();
    }

    m_filename = filename;

    bool success = save();

    if (!success) {
        m_filename = filenameBak;
        m_chatbotId = chatbotIdBak;
    }

    return success;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ChatbotRulesFile::hasUnsavedChanges() const
{
    if (m_dirty) {
        return true;
    }

    for (Rule::iterator it = m_rootRule->begin(); it != m_rootRule->end(); ++it) {
        if ((*it)->status() == Rule::Unsaved) {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::ChatbotRulesFile::setAsSaved()
{
    m_dirty = false;

    for (Rule::iterator it = m_rootRule->begin(); it != m_rootRule->end(); ++it) {
        (*it)->setStatus(Rule::Saved);
    }
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::ChatbotRulesFile::close()
{
    qDebug() << "Closing file" << m_filename;

    m_chatbotId = newChatbotId();
    m_rootRule = std::auto_ptr<Rule>(new Rule());
    m_filename = "";
    m_nextRuleId = 1;
    m_metadata.clear();
    setAsSaved();

    qDebug() << "File closed!";
}

//--------------------------------------------------------------------------------------------------

const QString & Lvk::BE::ChatbotRulesFile::filename()
{
    return m_filename;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ChatbotRulesFile::read(QFile &file)
{
    qDebug() << "Reading rules file" << file.fileName();

    QDataStream istream(&file);

    quint32 magicNumber;
    istream >> magicNumber;

    if (magicNumber != CRF_MAGIC_NUMBER) {
        qCritical("Cannot read rules: Invalid magic number");
        return false;
    }

    quint32 version;
    istream >> version;

    if (version > CRF_FILE_FORMAT_VERSION) {
        qCritical("Cannot read rules: Invalid format version");
        return false;
    }

    m_rootRule = std::auto_ptr<Rule>(new Rule());

    istream >> m_chatbotId;
    istream >> *m_rootRule;
    istream >> m_metadata;
    istream >> m_nextRuleId;

    if (istream.status() != QDataStream::Ok) {
        qCritical("Cannot read rules: Invalid file format");
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ChatbotRulesFile::write(QFile &file)
{
    qDebug() << "Writing rules file" << file.fileName();

    QDataStream ostream(&file);

    ostream.setVersion(QDataStream::Qt_4_7);

    ostream << (quint32)CRF_MAGIC_NUMBER;
    ostream << (quint32)CRF_FILE_FORMAT_VERSION;
    ostream << m_chatbotId;
    ostream << *m_rootRule;
    ostream << m_metadata;
    ostream << m_nextRuleId;

    return true;
}

//--------------------------------------------------------------------------------------------------

void Lvk::BE::ChatbotRulesFile::setMetadata(const QString &key, const QVariant &value)
{
    qDebug() << "Setting file metadata for key" << key;

    m_metadata[key] = value;

    m_dirty = true;
}

//--------------------------------------------------------------------------------------------------

const QVariant & Lvk::BE::ChatbotRulesFile::metadata(const QString &key) const
{
    qDebug() << "Getting file metadata for key" << key;

    FileMetadata::const_iterator it = m_metadata.find(key);

    if (it != m_metadata.end()) {
        return *it;
    } else {
        static const QVariant null;
        return null;
    }
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ChatbotRulesFile::importRules(const QString &inputFile)
{
    BE::Rule container;

    return importRules(&container, inputFile) && mergeRules(&container);
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ChatbotRulesFile::importRules(BE::Rule *container, const QString &inputFile)
{
    qDebug() << "Importing rules from file" << inputFile;

    QFile file(inputFile);

    if (!file.open(QFile::ReadOnly)) {
        qCritical() << "Cannot import rules: Cannot open file" << inputFile;
        return false;
    }

    QDataStream istream(&file);

    quint32 magicNumber;
    istream >> magicNumber;

    quint32 version;
    istream >> version;

    if (magicNumber != CEF_MAGIC_NUMBER) {
        qCritical("Cannot import rules: Invalid magic number");
        return false;
    }

    if (version > CEF_FILE_FORMAT_VERSION) {
        qCritical("Cannot import rules: Invalid file version");
        return false;
    }

    istream >> *container;

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ChatbotRulesFile::exportRules(const BE::Rule *container, const QString &outputFile)
{
    qDebug() << "Exporting rules to file" << outputFile;

    QFile file(outputFile);

    if (!file.open(QFile::WriteOnly)) {
        qCritical() << "Cannot export rules: Cannot open file" << outputFile;
        return false;
    }

    QDataStream ostream(&file);

    ostream.setVersion(QDataStream::Qt_4_7);

    ostream << (quint32)CEF_MAGIC_NUMBER;
    ostream << (quint32)CEF_FILE_FORMAT_VERSION;
    ostream << *container;

    return true;
}

//--------------------------------------------------------------------------------------------------

bool Lvk::BE::ChatbotRulesFile::mergeRules(BE::Rule *container)
{
    Lvk::BE::Rule *evasivesRule = findEvasivesRule();

    foreach (Lvk::BE::Rule *rule, container->children()) {
        rule->setId(0);

        switch (rule->type()) {
        case Rule::ContainerRule:
            m_rootRule->appendChild(new BE::Rule(*rule, true));
            break;
        case Rule::EvasiveRule:
            if (evasivesRule) {
                evasivesRule->output().append(rule->output());
            } else {
                m_rootRule->appendChild(new BE::Rule(*rule, true));
            }
            break;
        case Rule::OrdinaryRule:
            qCritical("Merge of ordinary rules without container is not supported");
            break;
        }
    }

    return true;
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::ChatbotRulesFile::rootRule()
{
    return m_rootRule.get();
}

//--------------------------------------------------------------------------------------------------

Lvk::BE::Rule * Lvk::BE::ChatbotRulesFile::findEvasivesRule()
{
    for (Rule::iterator it = m_rootRule->begin(); it != m_rootRule->end(); ++it) {
        if ((*it)->type() == Rule::EvasiveRule) {
            return *it;
        }
    }

    return 0;
}

//--------------------------------------------------------------------------------------------------

QString Lvk::BE::ChatbotRulesFile::chatbotId() const
{
    return m_chatbotId;
}

//--------------------------------------------------------------------------------------------------

quint64 Lvk::BE::ChatbotRulesFile::nextRuleId()
{
    return m_nextRuleId++;
}


