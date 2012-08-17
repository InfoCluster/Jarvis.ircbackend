#ifndef SCOPE_H
#define SCOPE_H

#include <QString>
#include <QList>
#include <memory>
#include "Arithmetic/EvalInfo.h"
#include "ExpressionParser.h"
#include "Arithmetic/Assignment.h"

class ClientConnection;

class Scope
{
private:
    QString name;
    ExpressionParser *parser;
    CAS::EvalInfo scope_info;
    QList<ClientConnection *> clients;

public:
    Scope() {};
    Scope(const QString &name, ExpressionParser *parser) : name(name), parser(parser) {};

    void getInitInfo(QDataStream &stream) const;
    void removeClient(ClientConnection *client);
    void sendMsg(const QString &sender, const QString &msg);
    void addClient(ClientConnection *client);

};

#endif //SCOPE_H
