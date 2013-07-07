#ifndef TYPES_EXPORT_H
#define TYPES_EXPORT_H
#include <QString>
#include <QSet>

enum TYPE_EXPORT{NONE = -1, HTML = 0, LATEX = 1, TEXT = 2};
const unsigned int NB_EXPORTS = 3;
const QString LABEL_EXPORT[NB_EXPORTS] = {"HTML",  "TeX", "Text"};

namespace type_export{
TYPE_EXPORT get_type_from_label(const QString& label);
}
#endif // TYPES_EXPORT_H
