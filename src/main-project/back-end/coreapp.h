#ifndef COREAPP_H
#define COREAPP_H

#include <QString>

namespace Lvk
{

namespace BE
{

class Rule;


/**
 * \brief This class provides the core functionality of the application.
 */

class CoreApp
{
public:
    CoreApp();

    ~CoreApp();

    bool load(const QString &filename);

    bool saveAs(const QString &filename);

    bool save();

    void close();

    Rule *rootRule();

    QString getResponse(const QString &input, QList<Rule>& matched);

private:
    QString m_filename;
    Rule *m_rootRule;
};

} // namespace BE

} // namespace Lvk

#endif // COREAPP_H
