#include "TYPES_EXPORT.h"


TYPE_EXPORT type_export::get_type_from_label(const QString& label){
    unsigned int i;
    for(i=0;i<NB_EXPORTS;++i)
        if(label == LABEL_EXPORT[i])
            return TYPE_EXPORT(i);
    return NONE;

}
