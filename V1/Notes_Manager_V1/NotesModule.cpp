
#include "Managers.h"
#include "ExportModule.h"

#include <QFile>
#include <QTextStream>
#include <QIODevice>

using namespace NotesModule;
/**
 * CLASSE NOTE
 */

/**
 * @brief setSaved
 * @param saved
 */
void Note::setSaved(const bool saved){
    this->saved_ = saved;
}

/**
 * @brief isSaved
 * @return TRUE si sauvegardé, FALSE sinon
 */
bool Note::isSaved() const{
    return this->saved_;
}

/**
 * @brief getTitle
 * @return le titre de la Note
 */
const QString &Note::getTitle() const{
    return this->title_;
}

/**
 * @brief setTitle
 */
void Note::setTitle(const QString &t){
    this->title_ = t;
    this->setSaved(false);
}

const QString& Note::getId() const {
    return this->id_;
}


void Note::destroy_file() const{
    if(QFile::remove(path_workspace + '\\' + id_))
        qDebug() << "Suppression du fichier: OK" << '\n';
    else
        qDebug() << "Suppression du fichier: KO" << '\n';
}


/**
 * CLASSE MULTIMEDIA
 */

/**
     * @brief setDescription: affecter une nouvelle description
     * �  la Note
     * @param d: la description de la Note
     */
void Multimedia::setDescription(const QString &d){
    this->description_ = d;
    this->setSaved(false);
}

/**
     * @brief setPath: affecter un nouveau chemin référant
     * @param p: le chemin référençant le fichier
     */
void  Multimedia::setPath(const QString &p){
    this->path_ = p;
    this->setSaved(false);
}

/**
     * @brief getDescription: obtenir la description de la Note
     * @return la description
     */
const QString &Multimedia::getDescription() const{
    return this->description_;
}

/**
     * @brief getPath: obtenir le chemin référant
     * @return le chemin
     */
const QString &Multimedia::getPath() const{
    return this->path_;
}



//CLASSE AUDIO
void Audio::exporter(const QString& path_export, TYPE_EXPORT type_export) const{

    export_notes::ExportWrapper* wrapper = new export_notes::ExportWrapper();
    wrapper->getExporters().find(type_export).value()->getExport()->export_note(*this,path_export,type_export);
    delete wrapper;
}

void Audio::exportAsPart(QTextStream &out,
                         TYPE_EXPORT type_export, unsigned int titleLevel) const{
    export_notes::ExportWrapper* wrapper = new export_notes::ExportWrapper();
    wrapper->getExporters().find(type_export).value()->getExport()->body(*this,out,titleLevel);
    delete wrapper;
}

void Audio::rewrite_in_trash(QTextStream& out) const{
    out << this->id_ << '\n' << getType() <<'\n';
}


void Audio::load(){
    QFile file(path_workspace + "\\" + this->getId());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line;
    //Lecture du titre
    line = in.readLine();
    this->setTitle(line);

    //Lecture du path
    /**
          TODO: AJOUTER UN CONTROLE POUR VERIFIER QUE LE PATH EST CORRECT
          */
    line = in.readLine();
    this->setPath(line);


    //Lecture de la description
    while (!in.atEnd())
        line = line + in.readLine();
    this->setDescription(line);
    file.close();
    this->setSaved(true);
}
void Audio::save(){
    if(saved_) return;
    QFile file(path_workspace + "\\" + this->getId());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    qDebug() << path_workspace << "\\" << this->getId() <<'\n';
    QTextStream out(&file);
    out << this->getType() << '\n'
        << this->title_ << '\n' << this->path_ << '\n'
        << this->description_;
    this->setSaved(true);
    file.close();
}

//CLASSE VIDEO
void Video::exporter(const QString& path_export, TYPE_EXPORT type_export) const{

    export_notes::ExportWrapper* wrapper = new export_notes::ExportWrapper();
    wrapper->getExporters().find(type_export).value()->getExport()->export_note(*this,path_export,type_export);
    delete wrapper;
}

void Video::exportAsPart(QTextStream& out,
                         TYPE_EXPORT type_export, unsigned int titleLevel) const{
    export_notes::ExportWrapper* wrapper = new export_notes::ExportWrapper();
    wrapper->getExporters().find(type_export).value()->getExport()->body(*this,out,titleLevel);
    delete wrapper;
}
void Video::rewrite_in_trash(QTextStream &out) const{
    out << id_ << '\n' << getType() <<'\n';
}

void Video::load(){
    QFile file(path_workspace + "\\" + this->getId());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line;
    //Lecture du titre
    line = in.readLine();
    this->setTitle(line);

    //Lecture du path
    /**
          TODO: AJOUTER UN CONTROLE POUR VERIFIER QUE LE PATH EST CORRECT
          */
    line = in.readLine();
    this->setPath(line);


    //Lecture de la description
    while (!in.atEnd())
        line = line + in.readLine();
    this->setDescription(line);
    file.close();
    this->setSaved(true);
}
void Video::save(){
    if(saved_) return;
    QFile file(path_workspace + "\\" + this->getId());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    qDebug() << path_workspace << "\\" << this->getId() <<'\n';
    QTextStream out(&file);
    out << this->getType() << '\n'
        << this->title_ << '\n' << this->path_ << '\n'
        << this->description_;
    this->setSaved(true);
    file.close();
}

//CLASSE IMAGE
void Image::exporter(const QString& path_export, TYPE_EXPORT type_export) const{

    export_notes::ExportWrapper* wrapper = new export_notes::ExportWrapper();
    wrapper->getExporters().find(type_export).value()->getExport()->export_note(*this,path_export,type_export);
    delete wrapper;
}

void Image::exportAsPart(QTextStream& out,
                         TYPE_EXPORT type_export, unsigned int titleLevel) const{
    export_notes::ExportWrapper* wrapper = new export_notes::ExportWrapper();
    wrapper->getExporters().find(type_export).value()->getExport()->body(*this,out,titleLevel);
    delete wrapper;
}
void Image::rewrite_in_trash(QTextStream& out) const{
    out << id_ << '\n' << getType() <<'\n';
}

void Image::load(){
    QFile file(path_workspace + "\\" + this->getId());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line;
    //Lecture du titre
    line = in.readLine();
    this->setTitle(line);

    //Lecture du path
    /**
          TODO: AJOUTER UN CONTROLE POUR VERIFIER QUE LE PATH EST CORRECT
          */
    line = in.readLine();
    this->setPath(line);


    //Lecture de la description
    while (!in.atEnd())
        line = line + in.readLine();
    this->setDescription(line);
    file.close();
    this->setSaved(true);
}
void Image::save(){
    if(saved_) return;
    QFile file(path_workspace + "\\" + this->getId());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    qDebug() << path_workspace << "\\" << this->getId() <<'\n';
    QTextStream out(&file);
    out << this->getType() << '\n'
        << this->title_ << '\n' << this->path_ << '\n'
        << this->description_;
    this->setSaved(true);
    file.close();
}

//CLASSE ARTICLE
void Article::setText(const QString& t) {
    text_= t;
    saved_=false;
}
void Article::load(){
    QFile file(path_workspace + "\\" + this->getId());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line;
    //Lecture du titre
    line = in.readLine();
    this->setTitle(line);

    //Lecture du texte
    while (!in.atEnd())
        line = line + in.readLine();
    this->setText(line);
    file.close();
    this->setSaved(true);
}
void Article::save(){
    if(saved_) return;
    QFile file(path_workspace + "\\" + this->getId());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    qDebug() << path_workspace << "\\" << this->getId() <<'\n';
    QTextStream out(&file);
    out << this->getType() << '\n'
        << this->title_ << '\n' << this->text_ << '\n';
    this->setSaved(true);
    file.close();
}

QString Article::getType() const{
    return "article";
}
void Article::exporter(const QString& path_export, TYPE_EXPORT type_export) const{

    export_notes::ExportWrapper* wrapper = new export_notes::ExportWrapper();
    wrapper->getExporters().find(type_export).value()->getExport()->export_note(*this,path_export,type_export);
    delete wrapper;
}

void Article::exportAsPart(QTextStream& out,
                           TYPE_EXPORT type_export, unsigned int titleLevel) const{
    export_notes::ExportWrapper* wrapper = new export_notes::ExportWrapper();
    wrapper->getExporters().find(type_export).value()->getExport()->body(*this,out,titleLevel);
    delete wrapper;
}

void Article::rewrite_in_trash(QTextStream &out) const{
    out << id_ << '\n' << getType() <<'\n';
}

//CLASSE DOCUMENT
Document&  Document::operator<<(Note& a){
    if(notes_.find(&a)!=notes_.end())  throw NotesException("Error, the note already exists in this document !");
    if(a.getId() == this->getId()) throw NotesException("Impossible d'inclure un document dans lui-même");
    notes_ +=&a;
    return *this;
}
void Document::removeNote(Note& a){
    if(!notes_.remove(&a)) throw NotesException("Error, can't remove the article from this document !");
}


void Document::load(){
    QFile file(path_workspace + "\\" + this->getId());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line;
    //Lecture du titre
    line = in.readLine();
    line = in.readLine();
    this->setTitle(line);

    //Lecture des notes

    //Pour chaque ligne lue, on regarde si la Note est déj�  chargée en mémoire
    //Si oui, on demande au manager le pointeur vers cette note
    //Si non, on la crée
    managers::NotesManager* manager =  managers::NotesManager::getInstance();
    Note* n;
    while (!in.atEnd()){
        line = in.readLine();
        try{
            n = (*manager)[line];
        }catch(NotesManagerException e){
            (*this)<<(*(*manager).getNewNote(in.readLine(),line));
        }
    }
    file.close();
    this->setSaved(true);
}
void Document::save(){
    if(saved_) return;
    QFile file(path_workspace + "\\" + this->getId());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    qDebug() << path_workspace << "\\" << this->getId() <<'\n';
    QTextStream out(&file);
    out << this->title_ << '\n'
        << this->getType() << '\n';
    Iterator it = this->getIterator();
    for(;!it.isDone(); it.next()){
        out << it.currentItem().getId() << '\n'
            << it.currentItem().getType() << '\n';
        if(!it.currentItem().isSaved())
            it.currentItem().save();
    }

    this->setSaved(true);
    file.close();
}

QString Document::getType() const{
    return "document";
}

void Document::exporter(const QString& path_export, TYPE_EXPORT type_export) const{

    export_notes::ExportWrapper* wrapper = new export_notes::ExportWrapper();
    wrapper->getExporters().find(type_export).value()->getExport()->export_note(*this,path_export,type_export);
    delete wrapper;
}

void Document::exportAsPart(QTextStream& out,
                            TYPE_EXPORT type_export, unsigned int titleLevel) const{
    export_notes::ExportWrapper* wrapper = new export_notes::ExportWrapper();
    wrapper->getExporters().find(type_export).value()->getExport()->body(*this,out,titleLevel);
    delete wrapper;
}


void Document::rewrite_in_trash(QTextStream& out) const{
    out << id_ << '\n' << getType() <<'\n';
}
