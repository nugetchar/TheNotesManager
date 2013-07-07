#ifndef EXCEPTIONSMODULE_H
#define EXCEPTIONSMODULE_H
#include <QString>
#include <iostream>



class UserException{
    QString msg_;
public:
    UserException():msg_(""){}
    UserException(const QString& msg):msg_(msg){}
    QString getInfo() const {
        return msg_;
    }
};

class NotesException:public UserException{
public:
    NotesException():UserException(){}
    NotesException(const QString& msg):UserException(msg){}
};


class NotesManagerException:public UserException{
public:
    NotesManagerException():UserException(){}
    NotesManagerException(const QString& msg):UserException(msg){}
};


class TagsManagerException:public UserException{
public:
    TagsManagerException():UserException(){}
    TagsManagerException(const QString& msg):UserException(msg){}
};

class WorkspaceException:public UserException{
public:
    WorkspaceException():UserException(){}
    WorkspaceException(const QString& msg):UserException(msg){}
};

class ExportException:public UserException{
public:
    ExportException():UserException(){}
    ExportException(const QString& msg):UserException(msg){}
};

#endif // EXCEPTIONSMODULE_H
