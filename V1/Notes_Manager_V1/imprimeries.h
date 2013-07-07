#ifndef IMPRIMERIES_H
#define IMPRIMERIES_H

#include <QMap>
#include <QString>
#include "NotesModule.h"


/**
 * @brief Namespace Imprimerie: les factories qui génèrent les Notes
 */
namespace Imprimerie {



/**
     * @brief The TheAbstractFactoryOfNotes class
     */
class TheAbstractFactoryOfNotes{
public:
    virtual NotesModule::Note* getNote(const QString& id) const = 0;
    virtual NotesModule::Note* getNewNote(const QString& id = "") const = 0;
};



/**
     * @brief The TheMultimediasFactory class
     */
template <typename VieuxMachin>
class TheConcreteFactory: public TheAbstractFactoryOfNotes {
public:

    /**
         * @brief getNote
         * @param id
         * @return un pointeur sur un Multimedia
         */
    NotesModule::Note* getNote(const QString &id) const{
        return new VieuxMachin(id);
    }

    /**
         * @brief getNewNote
         * @return un pointeur sur un nouveau multimédia
         */
    NotesModule::Note* getNewNote(const QString &id = "") const{
        return new VieuxMachin(id);
    }

    ~TheConcreteFactory(){}
};

/**
     * @brief The MotherOfFactories class
     */
class MotherOfFactories{

    /**
         * @brief factories la liste des Factories disponibles
         */
    static QMap<QString, TheAbstractFactoryOfNotes*>* factories;
    static int nbInstances;
    MotherOfFactories(const MotherOfFactories& );
    MotherOfFactories& operator=(const MotherOfFactories& t);

public:

    MotherOfFactories(){
        MotherOfFactories::nbInstances++;
    }

    /**
         * @brief getFactories: retourne et initialise si besoin
         * la liste des Factories disponibles
         * @return la liste des Factories disponibles
         */
    const QMap<QString, TheAbstractFactoryOfNotes*> getFactories(){

        //Si la liste doit être initialisée
        if(factories == 0){
            factories = new QMap<QString, TheAbstractFactoryOfNotes*>();
            factories->insert("article",new TheConcreteFactory<NotesModule::Article>());
            factories->insert("document",new TheConcreteFactory<NotesModule::Document>());
            factories->insert("image",new TheConcreteFactory<NotesModule::Image>());
            factories->insert("audio",new TheConcreteFactory<NotesModule::Audio>());
            factories->insert("video",new TheConcreteFactory<NotesModule::Video>());
        }

        return *(factories);
    }

    /**
         * @brief destructor
         */
    ~MotherOfFactories(){
        MotherOfFactories::nbInstances--;
        qDebug() << "nbInstances: " << nbInstances;
        if(factories && !nbInstances){
            qDebug() << "nbInstances: " << nbInstances;
            QMap<QString,TheAbstractFactoryOfNotes*>::Iterator it = factories->begin();
            while(it != factories->end()){
                qDebug() << "Suppression de la factory: " << it.key() << '\n';
                delete it.value();
                ++it;
            }
            qDebug() << "clear de la factory";
            factories->clear();
            qDebug() << "delete de la factory";
            delete factories;
            factories = 0;
        }
    }
};

}

#endif // IMPRIMERIES_H
