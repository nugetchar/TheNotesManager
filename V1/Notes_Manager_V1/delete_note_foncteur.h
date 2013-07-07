#ifndef DELETE_NOTE_FONCTEUR_H
#define DELETE_NOTE_FONCTEUR_H
#include "Workspace.h"

/**
 * @brief The DeleterForEditors class: foncteur dédié aux Editors lorsque l'utilisateur
 * veut supprimer une note
 */
class DeleterForEditors{
public:
    DeleterForEditors(){}
    /**
     * @brief operator ()
     * @param n
     */
    void operator()(NotesModule::Note* n)const{
        n->destroy_file();
        workspace::Workspace::getInstance()->delete_note(n->getId());

    }
};

#endif // DELETE_NOTE_FONCTEUR_H
