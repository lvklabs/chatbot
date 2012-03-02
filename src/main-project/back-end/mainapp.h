#ifndef MAINAPP_H
#define MAINAPP_H

#include <QString>

namespace Lvk
{

namespace BE
{

class MainApp
{
public:
    MainApp();

    bool load(const QString &filename);
    bool saveAs(const QString &filename);
    bool save();

private:
    QString m_filename;
};

} // namespace BE

} // namespace Lvk

#endif // MAINAPP_H
