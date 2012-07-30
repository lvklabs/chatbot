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

#ifndef LVK_BE_CHATBOTRULESFILE_H
#define LVK_BE_CHATBOTRULESFILE_H

#include "back-end/rule.h"

#include <QString>
#include <QHash>
#include <QVariant>
#include <memory>

class QFile;

namespace Lvk
{

/// \addtogroup Lvk
/// @{

namespace BE
{

/// \ingroup Lvk
/// \addtogroup BE
/// @{

/**
 * \brief The ChatbotRulesFile class provides methods to load, save, import and export chatbot
 *         rules files.
 */
class ChatbotRulesFile
{
public:

    /**
     * Constructs an empty ChatbotRulesFile object. After constructing the object, rootRule()
     * returns a valid pointer to star working with.
     */
    ChatbotRulesFile();

    /**
     * Constructs a ChatbotRulesFile object from \a filename. If \a filename cannot be loaded
     * throws std::exception.
     */
    ChatbotRulesFile(const QString &filename);

    /**
     * Loads \a filename. Filename must be a valid chabot file.
     * Returns true on success. Otherwise; false.
     */
    bool load(const QString &filename);

    /**
     * Saves the current file with a new \a filename.
     * Returns true on success. Otherwise; false.
     */
    bool saveAs(const QString &filename);

    /**
     * Saves all the changes made. After calling save() you must have called load() or saveAs();
     * otherwise there is no valid filename set.
     * Returns true on success. Otherwise; false.
     */
    bool save();

    /**
     * Returns true if there are unsaved changes in the current file. Otherwise; false.
     */
    bool hasUnsavedChanges() const;

    /**
     * Closes the file.
     */
    void close();

    /**
     * Returns the root rule.
     */
    Rule *rootRule();

    /**
     * Saves \a value with \a key in the metadata section of the current file.
     */
    void setMetadata(const QString &key, const QVariant &value);

    /**
     * Returns the \a value of \a key in the metadata section of the current file.
     * If the key is not found, returns QVariant().
     */
    const QVariant &metadata(const QString &key) const;

    /**
     * Imports rules from the filename \a inputFile.
     * Returns true on success. Otherwise; false.
     */
    bool importRules(const QString &inputFile);

    /**
     * Imports rules from the filename \a inputFile into \a container.
     * Returns true on success. Otherwise; false.
     */
    static bool importRules(BE::Rule *container, const QString &inputFile);

    /**
     * Exports rules in \a container to file \a outputFile.
     * Returns true on success. Otherwise; false.
     */
    static bool exportRules(const Rule *container, const QString &outputFile);

    /**
     * Merges rules in \a container with the current rules.
     * Returns true on success. Otherwise; false.
     */
    bool mergeRules(Rule *container);

    /**
     * Returns the chatbot ID. The chatbot ID is a unique string for each chatbot file.
     */
    QString chatbotId();

    /**
     * Returns the next rule ID. The first valid rule ID is 1 and 0 is the null ID.
     */
    quint64 nextRuleId();

private:
    ChatbotRulesFile(ChatbotRulesFile&);
    ChatbotRulesFile& operator=(ChatbotRulesFile&);

    typedef QHash<QString, QVariant> FileMetadata;

    QString m_filename;
    FileMetadata m_metadata;
    bool m_metadataUnsaved;
    std::auto_ptr<Rule> m_rootRule;
    quint64 m_nextRuleId;
    QString m_chatbotId;

    void markAsSaved();
    bool read(QFile &file);
    bool write(QFile &file);
    Rule *findEvasivesRule();
    bool loadDefaultRules();
};

/// @}

} // namespace BE

/// @}

} // namespace Lvk


#endif // LVK_BE_CHATBOTRULESFILE_H

