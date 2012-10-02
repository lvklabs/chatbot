#include "restmock.h"

#include <QFile>

//--------------------------------------------------------------------------------------------------
// RestMock
//--------------------------------------------------------------------------------------------------

RestMock::RestMock(QObject *parent) :
    Rest(parent)
{
}

//--------------------------------------------------------------------------------------------------

void RestMock::abort()
{
}

//--------------------------------------------------------------------------------------------------

bool RestMock::request(const QString &/*url*/)
{
    emit response(m_response);
}

//--------------------------------------------------------------------------------------------------

bool RestMock::setResponse(const QString &filename)
{
    QFile file(filename);

    if (file.open(QFile::ReadOnly)) {
        m_response = QString::fromUtf8(file.readAll());

        return true;
    } else {
        return false;
    }
}
