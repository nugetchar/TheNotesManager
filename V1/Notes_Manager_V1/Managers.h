#ifndef MANAGERS_H
#define MANAGERS_H

#include "NotesModule.h"

#include "imprimeries.h"
#include <QMap>
#include <QVector>

/**
 * @brief package permettant d'utiliser les managers de l'application
 */
namespace managers {


/**
 * @brief The NotesManager class
 */
class NotesManager{
    QVector<NotesModule::Note*> notes_; // les notes
    QString path_workspace;
    QString path_of_file;

    NotesManager(const NotesManager&); // empecher la duplication
    NotesManager& operator=(const NotesManager&); //empecher la recopie

    // singleton design pattern
    NotesManager(const QString& workspace, const QString& file);
    ~NotesManager(){
        for(NoteIterator it = getIterator();
            it != getEndIterator();++it){
            qDebug() << "freeInstance notes manager"
                     << ", note courante: " << (**it).getId() << '\n';
            if(!(**it).isSaved())
                (**it).save();
        }
        for(NoteIterator it2 = getIterator();
            it2 != getEndIterator();++it2){
            qDebug() << "freeInstance notes manager"
                     << ", note supprimée: " << (**it2).getId() << '\n';
            delete (*it2);
            (*it2) = 0;
        }
        qDebug() << "delete factory -->" << '\n';
        if(factory){
            delete factory;
            qDebug() << "factory deleted";
        }
        qDebug() << "fin free instance" << '\n';
    }

    static NotesManager* instance; // pointeur sur l'unique instance NotesManager
    Imprimerie::MotherOfFactories* factory;

    /**
     * @brief indexOf
     * @param id
     * @return l'index de la note caractérisée par l'id donné en paramètre
     */
    int indexOf(const QString& id){
        return notes_.indexOf((*this)[id]);
    }

    /**
     * @brief set_path_workspace: associer à une note le chemin du workspace dans lequel
     * elle se trouve
     * @param id
     */
    void set_path_workspace(const QString& id){
        (*this)[id]->set_path_workspace(path_workspace);
    }
public:

    typedef QVector<NotesModule::Note*> list_notes;
    typedef QVector<NotesModule::Note*>::Iterator NoteIterator;
    typedef QVector<NotesModule::Note*>::ConstIterator ConstNoteIterator;

    /**
     * @brief createInstance: initialise une première fois l'instance
     * @return un pointeur sur l'unique instance
     */
    static NotesManager* createInstance(const QString&, const QString&);
    /**
     * @brief getInstance: permet de récupérer l'instance
     * @return un pointeur sur l'unique instance
     */
    static NotesManager* getInstance();

    /**
     * @brief freeInstance: désallouer l'instance de la mémoire
     */
    static void freeInstance();

    /**
     * @brief getFactory
     * @return un pointeur sur le wrapper de factories du NotesManager
     */
    Imprimerie::MotherOfFactories& getFactory() const{
        return *factory;
    }

    //iterateurs
    /**
     * @brief getIterator
     * @return un itérateur sur le begin
     */
    NoteIterator getIterator() { return notes_.begin(); }
    /**
     * @brief getConstIterator
     * @return un itérateur const sur le begin
     */
    ConstNoteIterator getConstIterator() const { return notes_.constBegin();}

    /**
     * @brief getEndIterator
     * @return end()
     */
    NoteIterator getEndIterator() {return notes_.end();}
    /**
     * @brief getConstEndIterator
     * @return constEnd()
     */
    ConstNoteIterator getConstEndIterator() const { return notes_.constEnd();}
    NotesModule::Note* operator[](const QString& id) const{
        for(ConstNoteIterator it = notes_.constBegin(); it!=notes_.constEnd();
            ++it)
            if((**it).getId() == id)
                return *it;
        throw NotesManagerException("cette note n'existe pas");
    }

    //methodes
    /**
     * @brief getNewNote
     * @param type
     * @param id
     * @return un pointeur sur la note nouvellement crée
     */
    NotesModule::Note* getNewNote(const QString& type, const QString &id = "");

    /**
     * @brief getNote
     * @param id
     * @return un pointeur sur la note correspondante à l'ID
     */
    NotesModule::Note* getNote(const QString& id);

    /**
     * @brief saveNote
     */
    void saveNote(NotesModule::Note&);
    /**
     * @brief saveNote
     */
    void saveNote(const QString&);

    /**
     * @brief load le NotesManager
     */
    void load();
    /**
     * @brief save le NotesManager
     */
    void save();

    /**
     * @brief rewrite_notes_manager: réécrire sur le fichier de notes
     * du NotesManager
     */
    void rewrite_notes_manager();

    /**
     * @brief pop: supprimer une note du NotesManager
     * @param id
     */
    void pop(const QString& id){
        notes_.remove(indexOf(id));
    }

    /**
     * @brief push: ajouter une note au NotesManager
     * @param n
     */
    void push(NotesModule::Note* n){
        notes_.push_back(n);
    }


    /**
     * @brief set_path
     * @param new_path
     */
    void set_path(const QString& new_path){
        qDebug() << "set_path NotesManager" << '\n';

        QString str = path_of_file.mid(path_of_file
                                       .lastIndexOf(path_workspace)+path_workspace.size()+1);
        qDebug() << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA: " << str << '\n';
        path_workspace = new_path;
        path_of_file = path_workspace + '\\' + str;
        for(NoteIterator it = notes_.begin(); it != notes_.end(); ++it)
            (**it).set_path_workspace(new_path);
    }

    /**
     * @brief destroy_files_and_notes
     */
    void destroy_files_and_notes();
};


/**
     * @brief The TagsManager class
     */
class TagsManager{

    typedef QSet<QString> list_notes;

    //Liste des tags et notes associées
    QMap<QString,list_notes*> tags_;
    QString path_workspace;
    QString path_of_file;

    enum LOAD {NOTE = '*'};
    static TagsManager* instance;
    TagsManager(const TagsManager& t);
    TagsManager& operator=(const TagsManager& t);
    /**
     * @brief TagsManager
     */
    TagsManager(const QString& workspace, const QString& path):
        path_workspace(workspace), path_of_file(path){
        this->tags_ = QMap<QString,list_notes*>();
    }

    ~TagsManager(){
        iterator it = tags_.begin();
        qDebug() << "destruction tags manager" << '\n';
        while(it!=tags_.end()){
            qDebug() << "destruction de la liste associée au tag: "
                     << it.key() << " et taille de la liste: " << tags_.size()
                     << '\n';
            delete it.value();
            it++;
        }
    }
    bool tag_exists(const QString & ) const;
    bool note_already_bound(const QString &, NotesModule::Note &) const;
    bool note_already_bound(const QString &, const QString &) const;
public:

    typedef list_notes::Iterator iterator_notes;
    typedef list_notes::ConstIterator const_iterator_notes;
    typedef QMap<QString,list_notes*>::Iterator iterator;
    typedef QMap<QString,list_notes*>::ConstIterator const_iterator;

    static TagsManager* createInstance(const QString&, const QString&);
    static TagsManager* getInstance();
    static void freeInstance();


    const_iterator constBegin(){
        return tags_.constBegin();
    }

    iterator begin(){
        return tags_.begin();
    }

    const_iterator constEnd(){
        return tags_.constEnd();
    }

    iterator end(){
        return tags_.end();
    }


    void operator+=(const QString& );
    void operator-=(const QString& );
    void bind_tag(const QString &, NotesModule::Note &);
    void unbind_tag(const QString &, NotesModule::Note &);
    void bind_tag(const QString &, const QString &);
    void unbind_tag(const QString &, const QString &);

    QSet<QString>* get_collection_of_tags(QSet<QString> *);

    list_notes* filter(const QString&);
    void load();
    void save()const;

    void pop(const QString& tag){
        tags_.remove(tag);
    }

    void push(const QString& tag){
        tags_.insert(tag,new list_notes());
    }

    void set_path(const QString& new_path){
        QString str = path_of_file.mid(path_of_file
                                       .lastIndexOf(path_workspace)+path_workspace.size()+1);
        qDebug() << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA: " << str << '\n';
        path_workspace = new_path;
        path_of_file = path_workspace + '\\' + str;
    }
};
}

#endif // MANAGERS_H
