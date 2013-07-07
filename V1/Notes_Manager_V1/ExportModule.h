#ifndef EXPORTMODULE_H
#define EXPORTMODULE_H
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <exception>
#include <QDir>
#include "NotesModule.h"
#include "TYPES_EXPORT.h"

/**
 * @brief package pour les exports des notes
 */
namespace export_notes{


/**
 * @brief The FatherOfExporters class: la classe abstraite de strategy d'export
 */
class FatherOfExporters{
    const static char *types_export_str[3];
    //VIRTUAL
    void virtual start_header(const NotesModule::Note& n, QTextStream& out) const = 0;
    void virtual end_header(QTextStream& out) const = 0;
    void virtual header(const NotesModule::Note& note, QTextStream& out,
                        unsigned int titleLevel)const = 0;
    void virtual footer(QTextStream& out) const = 0;
    void virtual start_body(QTextStream& out) const = 0;
    void virtual end_body(QTextStream& out) const = 0;


public:



    void virtual body(const NotesModule::Article &n, QTextStream& out,
              unsigned int titleLevel) const = 0;
    void virtual body(const NotesModule::Document &n, QTextStream& out,
              unsigned int titleLevel) const = 0;
    void virtual body(const NotesModule::Audio &n, QTextStream &out,
              unsigned int titleLevel) const = 0;
    void virtual body(const NotesModule::Video &n, QTextStream &out,
              unsigned int titleLevel) const = 0;
    void virtual body(const NotesModule::Image &n, QTextStream &out,
              unsigned int titleLevel) const = 0;


    void export_note(const NotesModule::Article& n, const QString& path_export,
                     TYPE_EXPORT type_export)const{
        QDir dir(path_export);
        if(!dir.exists())
            dir.mkpath(path_export);

        QFile file(path_export + '\\' + n.getTitle()
                   + types_export_str[type_export] );
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
            return;

        QTextStream out(&file);
        start_header(n,out);
        header(n,out,1);
        end_header(out);
        start_body(out);
        body(n,out,1);
        end_body(out);
        footer(out);
    }
    void export_note(const NotesModule::Document& n, const QString& path_export,
                     TYPE_EXPORT type_export)const{
        QFile file(path_export + '\\' + n.getTitle()
                   + types_export_str[type_export] );
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
            return;

        QTextStream out(&file);
        start_header(n,out);
        header(n,out,1);
        end_header(out);
        start_body(out);
        body(n,out,1);
        end_body(out);
        footer(out);
    }
    void export_note(const NotesModule::Audio& n, const QString& path_export,
                     TYPE_EXPORT type_export)const{
        QFile file(path_export + '\\' + n.getTitle()
                   + types_export_str[type_export] );
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
            return;

        QTextStream out(&file);
        start_header(n,out);
        header(n,out,1);
        end_header(out);
        start_body(out);
        body(n,out,1);
        end_body(out);
        footer(out);
    }
    void export_note(const NotesModule::Video& n, const QString& path_export,
                     TYPE_EXPORT type_export)const{
        QFile file(path_export + '\\' + n.getTitle()
                   + types_export_str[type_export] );
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
            return;

        QTextStream out(&file);
        start_header(n,out);
        header(n,out,1);
        end_header(out);
        start_body(out);
        body(n,out,1);
        end_body(out);
        footer(out);
    }
    void export_note(const NotesModule::Image& n, const QString& path_export,
                     TYPE_EXPORT type_export)const{
        QFile file(path_export + '\\' + n.getTitle()
                   + types_export_str[type_export] );
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
            return;

        QTextStream out(&file);
        start_header(n,out);
        header(n,out,1);
        end_header(out);
        start_body(out);
        body(n,out,1);
        end_body(out);
        footer(out);
    }

};


/**
 * @brief The ExporterHTML class
 */
class ExporterHTML: public FatherOfExporters{
    //VIRTUAL
    void start_header(const NotesModule::Note& n, QTextStream& out) const;
    void end_header(QTextStream& out) const;
    void header(const NotesModule::Note& note, QTextStream& out,
                unsigned int titleLevel) const;
    void footer(QTextStream& out) const;
    void start_body(QTextStream& out) const;
    void end_body(QTextStream& out) const;

public:

    void body(const NotesModule::Article &n, QTextStream& out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Document &n, QTextStream& out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Audio &n, QTextStream &out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Video &n, QTextStream &out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Image &n, QTextStream &out,
              unsigned int titleLevel) const;
    ExporterHTML(){}
    ~ExporterHTML(){}
};

/**
 * @brief The ExporterTex class
 */
class ExporterTex: public FatherOfExporters{
    void start_header(const NotesModule::Note& n, QTextStream& out) const{

        out << "\\documentclass[a4paper,11pt]{report} %" << n.getType() << '\n'
            << "% Imports de bibliothèques" << '\n'
            << "usepackage{graphicx} %utilisé pour inclure des images" << '\n'
            << "%gestion de la police" << '\n'
            << "\\usepackage[french]{babel}" << '\n'
            << "\\usepackage[latin1]{inputenc}" << '\n'
            << "\\usepackage[T1]{fontenc}" << '\n'
            << "\\begin{document}" << '\n' ;
    }
    void end_header(QTextStream& out) const{}
    void header(const NotesModule::Note& note, QTextStream& out,
                unsigned int titleLevel) const;
    void footer(QTextStream& out) const;
    void start_body(QTextStream& out) const{}
    void end_body(QTextStream& out) const{}

public:

    void body(const NotesModule::Article &n, QTextStream& out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Document &n, QTextStream& out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Audio &n, QTextStream &out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Video &n, QTextStream &out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Image &n, QTextStream &out,
              unsigned int titleLevel) const;
    ExporterTex(){}
    ~ExporterTex(){}
};

/**
 * @brief The ExporterText class
 */
class ExporterText: public FatherOfExporters{
    void start_header(const NotesModule::Note& n, QTextStream& out) const{}
    void end_header(QTextStream& out) const{}
    void header(const NotesModule::Note& note, QTextStream& out,
                unsigned int titleLevel) const;
    void footer(QTextStream& out) const;
    void start_body(QTextStream& out) const{}
    void end_body(QTextStream& out) const{}

public:
    void body(const NotesModule::Article &n, QTextStream& out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Document &n, QTextStream& out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Audio &n, QTextStream &out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Video &n, QTextStream &out,
              unsigned int titleLevel) const;
    void body(const NotesModule::Image &n, QTextStream &out,
              unsigned int titleLevel) const;
    ExporterText(){}
    ~ExporterText(){}
};


class ExportWrapper;

//Getteurs d'exporters
/**
 * @brief The AbstractGetterExport class
 * getter pour obtenir un exporter
 */
class AbstractGetterExport{
public:
    virtual FatherOfExporters* getExport() const = 0;
};


template <typename EXPORT>
/**
 * @brief The GetterExport class: classe concrète qui renvoit un exporter
 */
class GetterExport : public AbstractGetterExport{
public:
    FatherOfExporters* getExport() const{
        return new EXPORT();
    }
};


//WRAPPER
/**
 * @brief The ExportWrapper class: contient tous les types de getter d'export
 * et permet ainsi de renvoyer n'importe quel type d'export
 */
class ExportWrapper{

    ExportWrapper(const ExportWrapper& );
    ExportWrapper& operator=(const ExportWrapper&);

    //La map lie le type de Note avec l'exporter approprié
    static QMap<TYPE_EXPORT, AbstractGetterExport*>* exporters;
    static int nbInstances;

public:
    ExportWrapper(){

        nbInstances ++;
    }
    const QMap<TYPE_EXPORT, AbstractGetterExport*> getExporters(){

        //Si la liste doit être initialisée
        if(exporters == 0){
            exporters = new QMap<TYPE_EXPORT, AbstractGetterExport*>();
            exporters->insert(HTML,new GetterExport<ExporterHTML>());
            exporters->insert(LATEX,new GetterExport<ExporterTex>());
            exporters->insert(TEXT,new GetterExport<ExporterText>());
        }

        return *(exporters);
    }

    ~ExportWrapper(){
        nbInstances--;
        if(exporters && !nbInstances){
            QMap<TYPE_EXPORT,AbstractGetterExport*>::iterator it = exporters->begin();
            while(it != exporters->end()){
                delete it.value();
                it++;
            }
            exporters->clear();
            delete exporters;
            exporters = 0;
        }
    }

};



}

#endif // EXPORTMODULE_H
