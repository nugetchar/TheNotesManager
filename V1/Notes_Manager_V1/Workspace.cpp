#include <windows.h>
#include <QFile>
#include <QDir>
#include "Workspace.h"
#include <exception>
#include "time.h"
using namespace workspace;


const QString& Workspace::file_of_tags =  "list_of_tags.txt";
const QString& Workspace::file_of_notes =  "list_of_notes.txt";
const QString& Workspace::file_of_trash =  "trash.txt";

Workspace* Workspace::workspace = 0;
//Initialise le NotesManager
void Workspace::create_NotesManager(){
    this->notes_manager = managers::NotesManager::getInstance();
    //Demander au notes_manager de charger les notes de "list_of_notes.txt"
    notes_manager->load();
}

//Initialise le TagsManager
void Workspace::create_TagsManager(){
    this->tags_manager = managers::TagsManager::getInstance();

    //Lire le fichier de description des tags
    //pour chaque ligne, insérer un tag.
    tags_manager->load();
}

//Vérifie que le chemin référence un workspace lisible par l'application
bool Workspace::path_valide(const QString &path) const{

    if(path == "") return false;
    QDir dir(path);
    if(!dir.makeAbsolute())
        return false;

    //Vérifier si le répertoire existe; Si non le créer avec les fichiers
    //de descriptions
    try{
        create_new(path);
    }catch(WorkspaceException e){
        qDebug() << e.getInfo();
        return false;
    }

    return true;

}

bool Workspace::exists_file_of_tags(const QString& path) const{
    QFile f(path + "\\" + file_of_tags);
    return f.exists();
}
bool Workspace::exists_file_of_notes(const QString& path) const{
    QFile f(path + "\\" + file_of_notes);
    return f.exists();
}


bool Workspace::file_exists(const QString& path) const{
    QFile f(path);
    return f.exists();
}
//Créer un nouveau Workspace tout beau tout neuf

void Workspace::create_new(const QString& path) const{

    QDir dir(".");
    dir.setPath(dir.cleanPath(path));
    if(!dir.exists()){
        try{
            dir.mkpath(path);
        }catch(std::exception e){
            qDebug() << e.what();
            throw WorkspaceException("erreur lors de la création du nouveau workspace");
        }
    }
    QFile f(path + "\\" + file_of_tags),f2(path + "\\" + file_of_notes),
            f3(path + "\\" + file_of_trash);

    qDebug() << "nouveau file of notes: " << path + "\\" + file_of_notes<<'\n';
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)){
        throw WorkspaceException("erreur lors de la création du fichier de tags");
    }
    qDebug() << "nouveau file of tags: " << path + "\\" + file_of_tags<<'\n';
    if (!f2.open(QIODevice::WriteOnly | QIODevice::Text)){
        throw WorkspaceException("erreur lors de la création du fichier de notes");
    }
    qDebug() << "nouveau file of trash: " << path + "\\" + file_of_trash<<'\n';
    if (!f3.open(QIODevice::WriteOnly | QIODevice::Text)){
        throw WorkspaceException("erreur lors de la création du fichier de trash");
    }

    f.close();
    f2.close();
    f3.close();
}

NotesModule::Note* Workspace::create_note(const QString& type, const QString& id){

    NotesModule::Note* n = this->notes_manager->getNewNote(type,id);
    QFile f(this->path_ + "\\" + file_of_notes);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        throw WorkspaceException("impossible d'ouvrir le fichier: " + this->path_ + "\\" + file_of_notes);
    QTextStream out(&f);
    out << n->getId() << '\n';
    f.close();
    n->save();
    return n;
}

//Permettre l'édition d'une note
NotesModule::Note *Workspace::edit_note(const QString& id) const{
    //Récupérer la note depuis le NotesManager
    NotesModule::Note* n = (*notes_manager)[id];
    n->setSaved(false);
    return n;
}

NotesModule::Note* Workspace::operator[](const QString& path) const{
    //Vérifier que le fichier existe
    if(!file_exists(path)) throw WorkspaceException("le chemin spécifié ne référence aucune note");
    return edit_note(path.mid(path.lastIndexOf(QChar('/'))));
}

//Enregistrer l'ID de la note et réécrire son fichier dans le
//workspace
void Workspace::save_note(const QString& type, const QString& id){
    //Sauvegarde dans le fichier de notes
    try{
        (*this->notes_manager)[id]->save();
    }catch(NotesManagerException e){
        qDebug() << e.getInfo();
        QFile f(this->path_ + "\\" + file_of_notes);
        if(f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) throw WorkspaceException();
        QTextStream out(&f);
        out << id << '\n';
        f.close();
        this->create_note(type,id);
        (*this->notes_manager)[id]->save();
    }
}


//Supprimer une note
void Workspace::delete_note(const QString& id){
    //Envoyer dans la trash
    try{
        trash_->push((*notes_manager)[id]);
        notes_manager->pop(id);
    }catch(NotesManagerException e){
        qDebug() << e.getInfo();
    }
}

//Restaurer une note
void Workspace::undo_delete_note(const QString& id){
    //Reprendre de la trash
    notes_manager->push(trash_->pop(id));
}

void Workspace::export_note(const QString& id, const QString& type,
                            const QString& path_export) const{
    (*notes_manager)[id]->exporter(path_export,type_export::get_type_from_label(type));
}

//Sauvegarder un tag
void Workspace::save_tag(const QString& tag){
    tags_manager->push(tag);
    QFile f(this->path_ + "\\" + file_of_tags);
    if(f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) throw WorkspaceException();
    QTextStream out(&f);
    out << tag;
    f.close();
}

//Supprimer un tag
void Workspace::delete_tag(const QString& tag){
    tags_manager->pop(tag);

}

//Sauvegarder le workspace
void Workspace::save() {
    qDebug() << "save notes manager" << '\n';
    notes_manager->save();
    qDebug() << "save tags manager" << '\n';
    tags_manager->save();
    qDebug() << "trash" << '\n';
    trash_->save();
}

//Charger le workspace
void Workspace::load(){
    notes_manager->load();
    tags_manager->load();
    trash_->load(this->notes_manager->getFactory());
}

//Vider la corbeille
void Workspace::empty_trash(){
    this->trash_->empty_trash();
}

bool Workspace::already_a_workspace(const QString& new_path) const{
    QDir dir(new_path);
    dir.setPath(dir.cleanPath(new_path));
    if(!dir.exists())
        return false;
    QFile f(path_ + "\\" + file_of_tags),
            f2(path_ + "\\" + file_of_notes),
            f3(path_ + "\\" + file_of_trash);
    return (f.exists() && f2.exists() && f3.exists());
}

//Ecraser le fichier de tags
void Workspace::erase_file_of_tags()const{
    QFile file(path_ + "\\" + file_of_tags);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) throw WorkspaceException("erreur lors de la réécriture du fichier des tags");
    QTextStream out(&file);
    out << "";
    file.close();
}
//Ecraser le fichier de notes
void Workspace::erase_file_of_notes() const{
    QFile file(path_ + "\\" + file_of_notes);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) throw WorkspaceException("erreur lors de la réécriture du fichier des tags");
    QTextStream out(&file);
    out << "";
    file.close();
}
//Ecraser le fichier de la trash
void Workspace::erase_file_of_trash() const{
    QFile file(path_ + "\\" + file_of_trash);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) throw WorkspaceException("erreur lors de la réécriture du fichier des tags");
    QTextStream out(&file);
    out << "";
    file.close();
}

//Lors du reset d'un Workspace,
//cette méthode effectue un backup dans un dossier
//special nommé "backup_sysdate"
void Workspace::do_a_backup() const{


    time_t now = time(NULL);
    tm* localtm = localtime(&now);

    QString old = path_;
    char concat[20];
    unsigned long long i;
    i =  1 + localtm->tm_sec;
    i += ((1 + localtm->tm_min)*10);
    i +=  (localtm->tm_hour*100);
    i += (localtm->tm_mday*1000);
    i += ((localtm->tm_mon + 1)*10000);
    i +=  ((localtm->tm_year + 1900)*100000);



    sprintf(concat,"%llu",i);
    qDebug() << "copy paste do a backup" << '\n';
    copy_paste(path_ + '\\' + "backup_" + QString(concat));
    qDebug() << "copy paste do a backup SUCCESS" << '\n';
    workspace->path_=old;
    workspace->notes_manager->set_path(old);
    workspace->tags_manager->set_path(old);
    workspace->trash_->set_path(old);

}
