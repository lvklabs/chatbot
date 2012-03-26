#include "defaultsanitizer.h"

#define utf8_a_acute                    "\xc3\xa1"
#define utf8_e_acute                    "\xc3\xa9"
#define utf8_i_acute                    "\xc3\xad"
#define utf8_o_acute                    "\xc3\xb3"
#define utf8_u_acute                    "\xc3\xba"
#define utf8_A_acute                    "\xc3\x81"
#define utf8_E_acute                    "\xc3\x89"
#define utf8_I_acute                    "\xc3\x8d"
#define utf8_O_acute                    "\xc3\x93"
#define utf8_U_acute                    "\xc3\x9a"
#define utf8_u_diaeresis                "\xc3\xbc"
#define utf8_U_diaeresis                "\xc3\x9c"
#define utf8_inverted_exclamation_mark  "\xc2\xa1"
#define utf8_inverted_question_mark     "\xc2\xbf"

QString Lvk::Nlp::DefaultSanitizer::sanitize(const QString &str) const
{
    QString sanitized = str;

    sanitized.replace(QString::fromUtf8(utf8_a_acute),     QString("a"));
    sanitized.replace(QString::fromUtf8(utf8_e_acute),     QString("e"));
    sanitized.replace(QString::fromUtf8(utf8_i_acute),     QString("i"));
    sanitized.replace(QString::fromUtf8(utf8_o_acute),     QString("o"));
    sanitized.replace(QString::fromUtf8(utf8_u_acute),     QString("u"));
    sanitized.replace(QString::fromUtf8(utf8_A_acute),     QString("A"));
    sanitized.replace(QString::fromUtf8(utf8_E_acute),     QString("E"));
    sanitized.replace(QString::fromUtf8(utf8_I_acute),     QString("I"));
    sanitized.replace(QString::fromUtf8(utf8_O_acute),     QString("O"));
    sanitized.replace(QString::fromUtf8(utf8_U_acute),     QString("U"));
    sanitized.replace(QString::fromUtf8(utf8_u_diaeresis), QString("u"));
    sanitized.replace(QString::fromUtf8(utf8_U_diaeresis), QString("U"));

    sanitized.remove(",");
    sanitized.remove(";");
    sanitized.remove(".");
    sanitized.remove("!");
    sanitized.remove("?");
    sanitized.remove(QString::fromUtf8(utf8_inverted_exclamation_mark));
    sanitized.remove(QString::fromUtf8(utf8_inverted_question_mark));

    return sanitized;
}
