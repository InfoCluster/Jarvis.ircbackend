#include "JarvisServer.h"
#include "ClientConnection.h"
#include <iostream>

JarvisServer::JarvisServer() : settings("InfoAG", "Jarvis.Server")
{
    defaultSetting("Port", 4200);
    defaultSetting("ModulePath", QCoreApplication::applicationDirPath() + "/../modules/");
    settings.setValue("ModulePath", QCoreApplication::applicationDirPath() + "/../modules/");
    emit output(QCoreApplication::applicationDirPath() + "/modules");
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/modules");
    //settings.setValue("Version", 1);
    listen(QHostAddress::Any, settings.value("Port").toUInt());
    parser = std::unique_ptr<ExpressionParser>(new ExpressionParser(QDir(settings.value("ModulePath").toString())));
    QObject::connect(parser.get(), SIGNAL(output(const QString &)), SIGNAL(output(const QString &)));
}

const std::shared_ptr<Scope> &JarvisServer::enterScope(ClientConnection *client, QString scope)
{
    if (! scopes.contains(scope)) {
        scopes.insert(scope, std::make_shared<Scope>(scope, parser.get()));
        connect(scopes[scope].get(), SIGNAL(output(const QString &)), SIGNAL(output(const QString &)));
        for (const auto &it_client: clients) {
            if (it_client.get() != client) it_client->newScope(scope);
        }
    }
    scopes[scope]->addClient(client);
    emit output("ClientEnterScope(" + client->nick() + ", " + scope + ")");
    return scopes[scope];
}

void JarvisServer::leaveScope(ClientConnection *sender, QString scope)
{
    scopes[scope]->removeClient(sender);
    emit output("ClientLeaveScope(" + sender->nick() + ", " + scope + ")");
}

void JarvisServer::msgToScope(ClientConnection *sender, QString scope, QString msg)
{
    if (scopes.contains(scope) && scopes[scope]->hasClient(sender)) {
        scopes[scope]->sendMsg(sender->nick(), msg);
        emit output("MsgToScope(" + sender->nick() + ", " + scope + ", " + msg + ")");
    }
}

void JarvisServer::disconnected(ClientConnection *client)
{
    emit output("ClientDisconnect(" + client->nick() + ")");
    for (auto &scope : scopes) scope->removeClient(client);
    clients.erase(std::find_if(clients.begin(), clients.end(), [&](const std::shared_ptr<ClientConnection> &it_client) { return it_client.get() == client; }));
}

void JarvisServer::deleteScope(const QString &name)
{
    for (const auto &client : clients) client->deleteScope(name);
    scopes.remove(name);
    emit output("DeleteScope(" + name + ")");
}

void JarvisServer::unload(const QString &pkgName)
{
    //ugly, compare parser->unload()
    auto modulePkgs = parser->getModulePkgs();
    if (std::find_if(modulePkgs.begin(), modulePkgs.end(), [&](const ModulePackage &it_pkg) {
            return it_pkg.name() == pkgName;
        }) != modulePkgs.end()) {
        parser->unload(pkgName);
        for (const auto &client : clients) client->unloadPkg(pkgName);
        emit output("UnloadPkg(" + pkgName + ")");
    }
}

void JarvisServer::load(const QString &pkgName)
{
    ModulePackage result(*(parser->load(settings.value("ModulePath").toString() + pkgName + ".jpkg")));
    for (const auto &client : clients) client->loadPkg(result);
    emit output("LoadPkg(" + pkgName + ")");
}

void JarvisServer::incomingConnection(int socketfd)
{
    auto client = std::shared_ptr<ClientConnection>(new ClientConnection(this, socketfd), std::mem_fun(&QObject::deleteLater));
    clients.append(client);
    emit output("NewConnection(" + client->getAddress().toString() + ")");
}
