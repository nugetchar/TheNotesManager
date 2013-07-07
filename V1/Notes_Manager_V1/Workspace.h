#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "Managers.h"
#include "TrashModule.h"
#include <qdebug.h>

/**
 * @brief Package contenant le singleton Workspace
 */
namespace workspace{

/**
 * @brief The Workspace class: singleton permettant de gérer
 * de manière centralisée les managers et la poubelle.
 * C'est le contrôleur de l'application;
 */
class Workspace{

    /**
     * @brief notes_manager: le manager de notes
     */
    managers::NotesManager* notes_manager;

    /**
     * @brief tags_manager: le manager de tags
     */
    managers::TagsManager* tags_manager;

    /**
     * @brief trash_: la poubelle
     */
    Trash* trash_;


    /**
     * @brief path_: le chemin du workspace
     */
    QString path_;

    /**
     * @brief workspace: l'unique instance du workspace
     */
    static Workspace* workspace;

    /**
     * @brief file_of_tags
     */
    const static QString& file_of_tags;
    /**
     * @brief file_of_notes
     */
    const static QString& file_of_notes;
    /**
     * @brief file_of_trash
     */
    const static QString& file_of_trash;

    //Initialise le NotesManager
    /**
     * @brief create_NotesManager: initialise le notes manager
     */
    void create_NotesManager();
    //Initialise le TagsManager
    /**
     * @brief create_TagsManager: initialise le tags manager
     */
    void create_TagsManager();

    //Vérifie que le chemin référence un workspace lisible par l'application

    /**
     * @brief path_valide: vérifie que le path est valide.
     * Si il ne référence pas un workspace, on en crée un à l'emplacement référé.
     * @param path
     * @return TRUE si le path est valide, FALSE sinon
     */
    bool path_valide(const QString& path) const;

    /**
     * @brief exists_file_of_tags
     * @param path
     * @return TRUE si le fichier des tags existe, FALSE sinon
     */
    bool exists_file_of_tags(const QString& path) const;

    /**
     * @brief exists_file_of_notes
     * @param path
     * @return TRUE si le fichier des notes existe, FALSE sinon
     */
    bool exists_file_of_notes(const QString& path) const;

    /**
     * @brief file_exists
     * @param path
     * @return TRUE si le fichier existe à l'emplacement path, FALSE sinon
     */
    bool file_exists(const QString& path) const;

    //Créer un nouveau Workspace tout beau tout neuf

    /**
     * @brief create_new: créer un workspace vide.
     * @param path
     */
    void create_new(const QString& path) const;


    /**
     * @brief Workspace
     * @param path
     */
    Workspace(const QString& path):path_(path){
        if(!path_valide(path)) throw WorkspaceException("chemin spécifié non valide");

        notes_manager = managers::NotesManager::createInstance(path_,path_ + "\\" + file_of_notes);
        notes_manager->load();
        tags_manager = managers::TagsManager::createInstance(path_,path_ + "\\" + file_of_tags);
        tags_manager->load();
        trash_ = new Trash(path_,path_ + "\\" + file_of_trash);
        trash_->load(notes_manager->getFactory());
    }

    ~Workspace(){
        managers::NotesManager::freeInstance();
        managers::TagsManager::freeInstance();
        delete trash_;
    }

    Workspace(const Workspace& w);
    Workspace& operator=(const Workspace& w);

public:

    //Permettre de créer une note
    /**
     * @brief create_note: créer une note, stockée directement dans le NotesManager
     * @param type: audio, video, image, article ou document
     * @param id
     * @return un pointeur sur la note créée
     */
    NotesModule::Note* create_note(const QString& type, const QString &id="");
    //Permettre l'édition d'une note
    /**
     * @brief edit_note: récupérer une note pour l'éditer
     * @param id
     * @return un pointeur sur la note
     */
    NotesModule::Note* edit_note(const QString& id) const;
    /**
     * @brief operator []
     * @param path
     * @return un pointeur sur la note trouvée
     */
    NotesModule::Note* operator[](const QString& path) const;

    /**
     * @brief get_path
     * @return le chemin du workspace
     */
    QString get_path()const{
        return path_;
    }
    //Sauvegarder une note
    /**
     * @brief save_note
     */
    void save_note(const QString&, const QString&);

    //Supprimer une note
    /**
     * @brief delete_note
     */
    void delete_note(const QString&);
    //Restaurer une note
    /**
     * @brief undo_delete_note
     */
    void undo_delete_note(const QString&);


    /**
     * @brief export_note
     * @param path_export
     */
    void export_note(const QString&, const QString&,
                     const QString& path_export) const;
    //Sauvegarder un tag
    /**
     * @brief save_tag
     */
    void save_tag(const QString&);
    //Supprimer un tag
    /**
     * @brief delete_tag
     */
    void delete_tag(const QString&);


    //Lier ou délier une note à un tag
    /**
     * @brief bind_note: liér une note à un tag
     * @param id
     * @param tag
     */
    void bind_note(const QString& id, const QString& tag){
        qDebug() << "debut bind note" << '\n';
        NotesModule::Note* n;
        try{
            n = (*this->notes_manager)[id];
            this->tags_manager->bind_tag(tag,*n);
        }catch(NotesManagerException e){
            qDebug() << e.getInfo() << '\n';
            return;
        }catch(TagsManagerException e1){
            qDebug() << e1.getInfo() << '\n';
            return;
        }
        qDebug() << "fin bind_note" << '\n';
    }

    /**
     * @brief unbind_note: enlever le lien entre une note et un tag
     * @param id
     * @param tag
     */
    void unbind_note(const QString& id, const QString& tag){
        NotesModule::Note* n;
        try{
            n = (*this->notes_manager)[id];
            this->tags_manager->unbind_tag(tag,*n);
        }catch(NotesManagerException e){
            qDebug() << e.getInfo() << '\n';
            return;
        }catch(TagsManagerException e1){
            qDebug() << e1.getInfo() << '\n';
            return;
        }
    }

    //Sauvegarder le workspace
    /**
     * @brief save: sauvegarder le Workspace
     */
    void save();
    //Charger le workspace
    /**
     * @brief load: charger le Workspace
     */
    void load();

    /**
     * @brief empty_trash: vider la corbeille
     */
    void empty_trash();

    /**
     * @brief already_a_workspace: vérifie si le dossier référencé par le path
     * est déjà un workspace
     * @param new_path
     * @return
     */
    bool already_a_workspace(const QString& new_path) const;

    /**
     * @brief donner_20_au_projet: donne 20 au projet
     * @return TRUE. Toujours.
     */
    bool donner_20_au_projet(){
        return TRUE;
    }

    //Ecraser le fichier de tags
    /**
     * @brief erase_file_of_tags: supprimer le fichier de tags
     */
    void erase_file_of_tags()const;
    //Ecraser le fichier de notes
    /**
     * @brief erase_file_of_notes: supprimer le fichier de notes
     */
    void erase_file_of_notes() const;
    /**
     * @brief erase_file_of_trash: supprimer le fichier de la corbeille
     */
    void erase_file_of_trash() const;

    //Lors du reset d'un Workspace,
    //cette méthode effectue un backup dans un dossier
    //special nommé "backup_sysdate"
    /**
     * @brief do_a_backup: fait un backup du Workspace
     */
    void do_a_backup() const;

    /**
     * @brief releaseTheWorkspace: initialiser une première fois le Workspace
     * @param path
     * @return un pointeur sur l'unique instance
     */
    static Workspace* releaseTheWorkspace(const QString& path){
        if(!workspace){
            try{
                workspace = new Workspace(slash_to_back_slash(path));
            }catch(WorkspaceException e){
                qDebug() << e.getInfo();
                throw e;
            }
        }
        return workspace;
    }

    /**
     * @brief killTheWorkspace: désallouer de la mémoire l'unique instance
     */
    static void killTheWorkspace(){
        qDebug() << "kill the workspace" << '\n';
        if(workspace){
            workspace->save();
            delete workspace;
        }

        workspace = 0;
        qDebug() << "fin kill the workspace" << '\n';
    }

    /**
     * @brief getInstance
     * @return un pointeur sur l'unique intance
     */
    static Workspace* getInstance(){
        if(workspace == 0)
            throw WorkspaceException("workspace non-initialisé!");
        return workspace;
    }

    //Remettre le Workspace à 0
    //efface toutes les notes et tous les tags des fichiers
    //de description
    /**
     * @brief reset: effectuer une remise à 0 du Workspace
     */
    static void reset(){
        QString s = workspace->path_;

        workspace->do_a_backup();
        try{
            workspace->erase_file_of_tags();
            workspace->erase_file_of_notes();
            workspace->notes_manager->destroy_files_and_notes();
        }catch(WorkspaceException e){
            qDebug()
                    << e.getInfo() << '\n';
        }

        qDebug() << "BBBBBBBBBBB " << '\n';
        killTheWorkspace();
        qDebug() << "CCCCCCCCCCC " << '\n';
        releaseTheWorkspace(s);
        qDebug() << "DDDDDDDDDDD " << '\n';
    }

    //Charger le Workspace dans un nouvel environnement
    /**
     * @brief copy_paste: copier/coller le Workspace dans un nouvel emplacement
     * @param new_path: l'emplacement où l'on veut coller le Workspace
     */
    static void copy_paste(const QString& new_path){
        if(workspace->path_ == new_path) throw WorkspaceException();
        if(workspace->already_a_workspace(new_path)) throw WorkspaceException("déjà un workspace");

        if(!workspace->path_valide(new_path)) throw WorkspaceException("path invalide");
        try{
            //Enregistrement du workspace actuel
            qDebug() << "enregistrement du workspace actuel" << '\n';
            workspace->save();

            //Affectation du nouveau path
            workspace->path_ = new_path;
            qDebug() << "set path pour le notes manager" << '\n';
            workspace->notes_manager->set_path(new_path);
            qDebug() << "set path pour le tags manager" << '\n';
            workspace->tags_manager->set_path(new_path);
            qDebug() << "set path pour la trash" << '\n';
            workspace->trash_->set_path(new_path);

            //Copie par sauvegarde au nouvel emplacement
            qDebug() << "create  new" << '\n';
            //workspace->create_new(new_path);
            qDebug() << "enregistrement du nouveau workspace" << '\n';
            //Rendre toutes les notes au statut "non-sauvegardées"
            workspace->save();
            qDebug() << "FIN D'ENREGISTREMENT DU NOUVEAU WORKSPACE" << '\n'
                     << "////////////////////////////////////////////////////";
        }catch(NotesManagerException e){
            qDebug() << e.getInfo() << '\n';
            throw e;
        }catch(WorkspaceException e1){
            qDebug() << e1.getInfo() << '\n';
            throw e1;
        }
    }

    //Changer de workspace
    /**
     * @brief change_workspace: faire référencer le Workspace autre part sur le disque
     * @param new_path: le nouveau chemin du Workspace
     */
    static void change_workspace(const QString& new_path){
        if(workspace->path_ == new_path) throw WorkspaceException();
        if(!workspace->path_valide(new_path)) throw WorkspaceException("path invalide");
        workspace->save();
        workspace->path_ = new_path;
        workspace->load();
    }


    /**
     * @brief slash_to_back_slash
     * @param path
     * @return une chaine modifiée
     */
    static QString slash_to_back_slash(const QString& path){
        QString tmp = path;
        for(unsigned int i=0; i<path.size(); ++i){
            if(path[i] == '/'){
                tmp.replace(i,1,'\\');
            }
        }
        return tmp;
    }

    /**
     * @brief back_slash_to_slash
     * @param path
     * @return une chaine modifiée
     */
    static QString back_slash_to_slash(const QString& path){
        QString tmp = path;
        for(unsigned int i=0; i<path.size(); ++i){
            if(path[i] == '\\'){
                tmp.replace(i,1,'/');
            }
        }
        return tmp;
    }

    /**
     * @brief get_notes_by_type
     * @param type
     * @return la liste des notes correspondantes au type spécifié
     */
    QSet<NotesModule::Note*> get_notes_by_type(const QString& type){
        QSet<NotesModule::Note*> res;
        for(managers::NotesManager::ConstNoteIterator it = notes_manager->getConstIterator();
            it = notes_manager->getConstEndIterator(); ++it){
            if((**it).getType() == type)
                res.insert(*it);
        }
        return res;
    }
};

}

#endif // WORKSPACE_H
