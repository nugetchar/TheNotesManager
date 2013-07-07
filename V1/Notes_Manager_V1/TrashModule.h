#ifndef TRASHMODULE_H
#define TRASHMODULE_H

#include "NotesModule.h"
#include "imprimeries.h"
#include <QSet>
#include <QFile>

class Trash{
    QSet<NotesModule::Note*> notes_;
    QString path_workspace;
    QString path_of_file;


    void rewrite_trash() const{
        QFile f(path_of_file);
        f.remove();
        if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&f);
        for(QSet<NotesModule::Note*>::const_iterator it = notes_.constBegin();
            it != notes_.constEnd(); ++it)
            (**it).rewrite_in_trash(out);

        f.close();
    }

public:
    Trash(const QString& workspace, const QString& file):
        path_workspace(workspace), path_of_file(file){
        notes_ = QSet<NotesModule::Note*>();
    }

    void push(NotesModule::Note* n){
        notes_.insert(n);
    }

    void save(){
        rewrite_trash();
    }

    void load(Imprimerie::MotherOfFactories& factory){
        QFile f(path_of_file);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream in(&f);
        QString id,type;
        while(!in.atEnd()){
            id = in.readLine();
            type = in.readLine();
            notes_.insert(factory.getFactories()
                          .find(type).value()->getNote(id));
        }
        f.close();
    }

    NotesModule::Note* pop(const QString& id){
        for(QSet<NotesModule::Note*>::iterator it = notes_.begin();
            it != notes_.end(); ++it)
            if(id == (**it).getId())
                return *it;
        return 0;
    }

    void remove(const QString& id){
        for(QSet<NotesModule::Note*>::iterator it = notes_.begin();
            it != notes_.end(); ++it)
            if(id == (**it).getId()){
               QFile file(path_workspace + "/" + id);
               file.remove();
               rewrite_trash();
               return;
            }
    }

    void empty_trash(){
        QFile* f;
        for(QSet<NotesModule::Note*>::iterator it = notes_.begin();
            it != notes_.end(); ++it){
            delete (*it);
            f = new QFile(path_workspace + "/" + (**it).getId());
            f->remove();
            delete f;
            f = 0;
        }
        notes_.clear();
        rewrite_trash();
    }

    void set_path(const QString& new_path){
        QString str = path_of_file.mid(path_of_file
                                       .lastIndexOf(path_workspace)+path_workspace.size()+1);
        qDebug() << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA: " << str << '\n';
        path_workspace = new_path;
        path_of_file = path_workspace + '\\' + str;
    }
    ~Trash(){
        save();
        for(QSet<NotesModule::Note*>::iterator it = notes_.begin();
            it != notes_.end(); ++it)
            delete *it;
        notes_.clear();
    }
};

#endif // TRASHMODULE_H
