#include "Managers.h"
#include "Imprimeries.h"
#include "ExceptionsModule.h"
#include <QFile>
#include <QTextStream>

using namespace managers;
using namespace Imprimerie;


//Class NotesManager
NotesManager* NotesManager::instance=0;
TagsManager* TagsManager::instance=0;

NotesManager *NotesManager::createInstance(const QString &workspace,
                                           const QString &file){
    if(!instance)
        instance = new NotesManager(workspace,file);
    return instance;
}

NotesManager *NotesManager::getInstance(){
    if (!instance) throw NotesManagerException("le gestionnaire de notes n'est pas initialisé");
    return instance;
}

void NotesManager::freeInstance(){

    if (instance)
        instance->save();
    delete instance;
    instance=0;
    qDebug() << "sortie de freeInstance"  << '\n';
}

NotesManager::NotesManager(const QString &workspace, const QString &file):
    path_workspace(workspace),path_of_file(file),notes_ (QVector<NotesModule::Note*>()){}

void NotesManager::saveNote(NotesModule::Note &n){
    n.save();
}

void NotesManager::saveNote(const QString &id){
    (*this)[id]->save();
}


NotesModule::Note* NotesManager::getNewNote(const QString& type, const QString& id){
    NotesModule::Note* note = factory->getFactories().find(type).value()->getNewNote(id);
    note->set_path_workspace(path_workspace);
    notes_.push_back(note);
    qDebug() << "nouvelle note, size du notes manager: " << notes_.size() <<'\n';
    return note;
}

NotesModule::Note* NotesManager::getNote(const QString& id){
    //la note existe déjà
    for(unsigned int i=0;i<notes_.size();++i)
        if(notes_[i]->getId()==id) return notes_[i];

    // il faut d'abord charger la note
    QFile file(path_workspace + "\\" + id);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) throw NotesException("Error, can't open the file !");
    QTextStream flow(&file);
    QString type=flow.readLine();
    file.close();
    NotesModule::Note* note = factory->getFactories().find(type).value()->getNote(id);
    note->set_path_workspace(path_workspace);
    note->load();
    notes_.push_back(note);
    qDebug() << "nouvelle note, size du notes manager: " << notes_.size() <<'\n';
    return note;
}

void NotesManager::rewrite_notes_manager(){
    QFile f(path_of_file);
    f.remove();
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    qDebug() << "rewrite notes manager: " << path_of_file << '\n';

    QTextStream out(&f);
    for(ConstNoteIterator it = notes_.constBegin();
        it != notes_.constEnd(); ++it){
        qDebug() << "rewrite notes manager: " << (**it).getId() << '\n';
        out << (**it).getId() << '\n';
    }

    f.close();
}

void NotesManager::destroy_files_and_notes(){
    for(NoteIterator it = getIterator(); it != getEndIterator(); ++it){
        (**it).destroy_file();
        delete (*it);
    }
    notes_.clear();
}

void NotesManager::load(){

    QFile file(path_of_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line;
    NotesModule::Note* n;
    //Lecture du fichier de tags
    while (!in.atEnd()){
        //Lecture d'une ligne
        line = in.readLine();
        n = this->getNote(line);
        n->load();
        this->notes_.push_back(n);
    }
}

void NotesManager::save(){
    rewrite_notes_manager();
    for(NoteIterator it = notes_.begin(); it != notes_.end(); ++it){
        qDebug() << "sauvegarde de la note: " << (**it).getId() << '\n';
        (**it).save();
    }
}

//TAGS MANAGER
bool TagsManager::tag_exists(const QString& t) const{
    return (this->tags_.constFind(t) != this->tags_.constEnd());
}

bool TagsManager::note_already_bound(const QString &t, NotesModule::Note& n) const{
    return note_already_bound(t,n.getId());
}

bool TagsManager::note_already_bound(const QString &t, const QString& id) const{
    if(!tag_exists(t)) return false;
    list_notes* notes = this->tags_.find(t).value();
    return (notes->find(id) != notes->end());
}

void TagsManager::operator+=(const QString& t){
    if(this->tags_.find(t) != this->tags_.end())  TagsManagerException("ce tag existe déjà");
    this->tags_.insert(t,new list_notes);
}

void TagsManager::operator-=(const QString& t){
    this->tags_.remove(t);
}

void TagsManager::bind_tag(const QString &t, NotesModule::Note& n){
    bind_tag(t,n.getId());
}

void TagsManager::unbind_tag(const QString &t, NotesModule::Note& n){
    unbind_tag(t,n.getId());
}


void TagsManager::bind_tag(const QString &t, const QString& id){
    if(!tag_exists(t)) (*this)+=t;
    if(note_already_bound(t,id)) throw TagsManagerException("cette note est déjà marquée");
    list_notes* set = tags_.find(t).value();
    set->insert(id);
}

void TagsManager::unbind_tag(const QString &t, const QString& id){
    if(!tag_exists(t)) throw TagsManagerException("ce tag n'existe pas");
    if(!note_already_bound(t,id)) throw TagsManagerException("cette note n'est pas marquée par ce tag");
    list_notes* notes = this->tags_.find(t).value();

    notes->remove(id);
}


QSet<QString> *TagsManager::get_collection_of_tags(QSet<QString>* collection){
    if(collection == 0) throw TagsManagerException("erreur lors de la récupération des tags");
    for(iterator it = this->tags_.begin(); it != this->tags_.end(); ++it)
        collection->insert(it.key());
    return collection;
}

TagsManager::list_notes* TagsManager::filter(const QString& tag){
    return this->tags_.find(tag).value();
}

void TagsManager::load(){
    QFile file(path_of_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line,tag;
    list_notes* lst = new list_notes();
    //Lecture du fichier de tags
    tag = in.readLine();
    while (!in.atEnd()){
        //Lecture d'une ligne
        line = in.readLine();
        //Si la ligne lue commence par le caractère qui permet de répérer
        //une note
        if(line.at(0) == NOTE){
            //On insère dans la liste de notes la note correspondante
            //en la recherchant dans le NotesManager
            line=line.mid(1);
            lst->insert(line);
        }
        else{
            //Sinon c'est un tag, on insère la paire
            this->tags_.insert(tag,lst);
            //on vide la liste
            lst->clear();
            //On affecte à tag la valeur lue;
            tag = line;
        }
    }
}

void TagsManager::save() const{
    QFile file(path_of_file);
    qDebug() << "before removing tags manager's file' " <<'\n';
    file.remove();
    qDebug() << "before removing tags manager's file' " <<'\n';
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) throw TagsManagerException("erreur lors de la réécriture du fichier de tags");

    qDebug() << "rewriting tags manager's file' " <<'\n';
    QTextStream out(&file);
    iterator_notes it_n;
    for(const_iterator it = tags_.constBegin(); it != tags_.constEnd(); ++it){

        out << it.key() << '\n';
        qDebug() << "clé enregistréee: " << it.key() <<'\n';
        //On enregistre chaque Note liée
        for(it_n = it.value()->begin(); it_n != it.value()->end();
            ++it_n){
            qDebug() << "note enregistréee: " << *it_n <<'\n';
            out << QChar(NOTE) << *it_n << '\n';
            // qDebug() << "note enregistréee: " << (**it_n).getId() <<'\n';
        }

    }
}

TagsManager *TagsManager::createInstance(const QString &workspace,
                                         const QString &file){
    if(!instance)
        instance = new TagsManager(workspace,file);
    return instance;
}

TagsManager *TagsManager::getInstance(){
    if (!instance) throw TagsManagerException("le gestionnaire de tags n'est pas initialisé");
    return instance;
}
void TagsManager::freeInstance(){
    if (instance){
        qDebug() << "delete du tags manager" << '\n';
        instance->save();
        qDebug() << "saving tags manager's file success " <<'\n';
        delete instance;
    }
    instance=0;
    qDebug() << "sortie du free instance du tafs manager" << '\n';
}
