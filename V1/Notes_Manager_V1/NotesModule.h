#ifndef NOTESMODULE_H
#define NOTESMODULE_H

#include <QString>
#include <QTextStream>
#include <QSet>
#include "ExceptionsModule.h"
#include "TYPES_EXPORT.h"
#include <time.h>
#include <QDebug>
namespace NotesModule {


class Document;
class Article;

/**
     * @brief The Note class
     */
class Note{


    Note& operator=(const Note&);

protected:
    Note(const Note& n);
    //L'ID de la note, référençant le fichier de description
    //dans le WorkSpace
    QString id_;
    //Le titre de la Note
    QString title_;
    //Est-ce que les modifications ont été sauvegardées?
    bool saved_;

    QString path_workspace;
public:
    /**
         * @brief Note: constructeur
         * @param title: le titre de la note
         */
    Note(const QString& id="", const QString& title="")
        :title_(title),saved_(false),id_(id),path_workspace(""){
        if(id==""){
            time_t now = time(NULL);
            tm* localtm = localtime(&now);

            char concat[20];
            unsigned long long i;
            i =  1 + localtm->tm_sec;
            i += ((1 + localtm->tm_min)*10);
            i +=  (localtm->tm_hour*100);
            i += (localtm->tm_mday*1000);
            i += ((localtm->tm_mon + 1)*10000);
            i +=  ((localtm->tm_year + 1900)*100000);



            sprintf(concat,"%llu",i);
            id_ = concat;
            id_ += ".txt";
        }

    }

    Note(const QString &title):title_(title){
        time_t now = time(NULL);
        tm* localtm = localtime(&now);

        char concat[20];
        unsigned long long i;
        i =  1 + localtm->tm_sec;
        i += ((1 + localtm->tm_min)*10);
        i +=  (localtm->tm_hour*100);
        i += (localtm->tm_mday*1000);
        i += ((localtm->tm_mon + 1)*10000);
        i +=  ((localtm->tm_year + 1900)*100000);



        sprintf(concat,"%llu",i);
        id_ = concat;
        id_ += ".txt";

    }
    virtual void load() = 0;
    virtual void save() = 0;

    /**
         * @brief setSaved
         * @param saved
         */
    void setSaved(const bool);


    /**
         * @brief isSaved
         * @return TRUE si sauvegardé, FALSE sinon
         */
    bool isSaved() const;

    /**
         * @brief getTitle
         * @return le titre de la Note
         */
    const QString& getTitle() const;

    /**
         * @brief setTitle
         */
    void setTitle(const QString& );

    const QString& getId() const;

    bool operator==(const Note& n){
        return n.getId() == this->getId();
    }


    virtual QString getType() const = 0;

    virtual void exporter(const QString& path_export,
                          TYPE_EXPORT type_export) const = 0;
    virtual void exportAsPart(QTextStream& out,
                              TYPE_EXPORT type_export, unsigned int titleLevel) const = 0;
    virtual void rewrite_in_trash(QTextStream& out) const = 0;

    void set_path_workspace(const QString& path){
        qDebug() << "set_path Note" << '\n';
        saved_=false;
        path_workspace = path;
    }


    void destroy_file() const;
};



/**
     * @brief The Multimedia class
     */
class Multimedia: public Note{


    Multimedia(const Multimedia&);
    Multimedia& operator=(const Multimedia&);
protected:
    /**
         * @brief description_, la description de la Note
         */
    QString description_;

    /**
         * @brief path_, le chemin référençant le fichier
         */

    QString path_;
public:
    /**
         * @brief Multimedia: constructeur
         * @param title: le titre de la Note
         * @param description: la description de la Note
         * @param path: le chemin référençant le fichier
         */
    Multimedia( const QString& id=0, const QString& title=0,
                const QString& path=0, const QString& description=0)
        :Note(id,title), description_(description), path_(path){}

    Multimedia(const Note& n, const QString& path = 0,
               const QString& description = 0):Note(n),description_(description),
        path_(path){}

    /**
         * @brief setDescription: affecter une nouvelle description
         * �  la Note
         * @param d: la description de la Note
         */
    void setDescription(const QString& d);

    virtual void load() = 0;
    virtual void save() = 0;

    /**
         * @brief setPath: affecter un nouveau chemin référant
         * @param p: le chemin référençant le fichier
         */
    void setPath(const QString& p);

    /**
         * @brief getDescription: obtenir la description de la Note
         * @return la description
         */
    const QString& getDescription() const;

    /**
         * @brief getPath: obtenir le chemin référant
         * @return le chemin
         */
    const QString& getPath() const;

    virtual QString getType() const = 0;

    QString getExtension() const {
        return this->getPath().mid(this->getPath().lastIndexOf(QChar('.')));
    }

    virtual void exporter(const QString& path_export,
                          TYPE_EXPORT type_export) const = 0;
    virtual void exportAsPart(QTextStream& out,
                              TYPE_EXPORT type_export, unsigned int titleLevel) const = 0;
    virtual void rewrite_in_trash(QTextStream& out) const = 0;
};


class Audio : public Multimedia{
    Audio(const Audio&);
    Audio& operator=(const Audio&);
public:
    Audio(const QString& id=0, const QString& title=0,
          const QString& path=0, const QString& description=0):Multimedia(id,title,
                                                                          path,description){}
    QString getType() const{
        return "audio";
    }
    void exporter(const QString& path_export,
                  TYPE_EXPORT type_export) const;
    void exportAsPart(QTextStream& out,
                      TYPE_EXPORT type_export, unsigned int titleLevel)const;
    void rewrite_in_trash(QTextStream &out) const;

    void load();
    void save();
};

class Video : public Multimedia{
    Video(const Video&);
    Video& operator=(const Video&);
public:
    Video(const QString& id=0, const QString& title=0,
          const QString& path=0, const QString& description=0):Multimedia(id,title,
                                                                          path,description){}
    QString getType() const{
        return "video";
    }
    void exporter(const QString& path_export,
                  TYPE_EXPORT type_export) const;
    void exportAsPart(QTextStream& out,
                      TYPE_EXPORT type_export, unsigned int titleLevel) const;
    void rewrite_in_trash(QTextStream& out) const;

    void load();
    void save();
};
class Image : public Multimedia{
    Image(const Image&);
    Image& operator=(const Image&);
public:
    Image(const QString& id=0, const QString& title=0,
          const QString& path=0, const QString& description=0):Multimedia(id,title,
                                                                          path,description){}
    QString getType() const{
        return "image";
    }
    void exporter(const QString& path_export,
                  TYPE_EXPORT type_export) const;
    void exportAsPart(QTextStream& out,
                      TYPE_EXPORT type_export, unsigned int titleLevel) const;
    void rewrite_in_trash(QTextStream &out) const;

    void load();
    void save();
};



/**
     * @brief The Article class
     */
class Article: public Note{
    friend class NotesManager;
    QString text_;
    Article(const Article&);
    Article& operator=(const Article&);
public:
    Article(const QString& id=0,const QString& title = 0): Note(id,title){}
    const QString& getText() const { return text_; }
    void setText(const QString& t);

    void load();
    void save();
    QString getType() const;
    void exporter(const QString& path_export,
                  TYPE_EXPORT type_export) const;
    void exportAsPart(QTextStream& out,
                      TYPE_EXPORT type_export, unsigned int titleLevel) const;
    void rewrite_in_trash(QTextStream& out) const;
};


/**
     * @brief The Document class
     */
class Document: public Note{

    //La liste de notes associées
    QSet<Note*> notes_;

    Document(const Document& m);
    Document& operator=(const Document& m);


public:

    /**
         * @brief Document
         * @param f the filename (ID)
         * @param t the title
         */
    Document(const QString& f=0, const QString& t=0):Note(f,t){
        this->notes_ = QSet<Note*>();
    }

    /**
         * @brief getNbNotes
         * @return the number of Notes
         */
    unsigned int getNbNotes() const { return notes_.size(); }

    Document&  operator<<(Note&);
    void removeNote(Note &);

    class Iterator {
        QSet<Note*>* set_;
        QSet<Note*>::iterator it;
        friend class Document;
        Iterator(QSet<Note*>* s): set_(s),it(s->begin()){}
    public:
        Iterator(): set_(0){}
        bool isDone() const { return it==set_->end(); }
        void next() { if (it==set_->end()) throw NotesException("Error, next on an iterator which is done");  it++; }
        Note& currentItem() const { return **it; }
    };

    Iterator getIterator() { return Iterator(&notes_); }

    class ConstIterator {
        const QSet<Note*>* set_;
        QSet<Note*>::const_iterator it;
        friend class Document;
        ConstIterator(const QSet<Note*>* s): set_(s),it(s->constBegin()){}
    public:
        ConstIterator(): set_(0){}
        bool isDone() const { return it==set_->end(); }
        void next() { if (it==set_->end()) throw NotesException("Error, next on an iterator which is done");  it++; }
        const Note& currentItem() const { return **it; }
    };

    ConstIterator getConstIterator() const { return ConstIterator(&notes_); }

    void load();
    void save();
    QString getType() const;
    void exporter(const QString& path_export, TYPE_EXPORT type_export) const;
    void exportAsPart(QTextStream& out,
                      TYPE_EXPORT type_export, unsigned int titleLevel) const;
    void rewrite_in_trash(QTextStream &out) const;
};



}


#endif // NOTESMODULE_H
