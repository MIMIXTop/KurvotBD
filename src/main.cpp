#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "Models/ButtonFilterModel.hpp"

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ButtonFilterModel filterModel;
    engine.rootContext()->setContextProperty("filterModel", &filterModel);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        [](){QCoreApplication::exit(-1);},
        Qt::QueuedConnection
    );

    engine.loadFromModule("Kurvot_URI", "Main");

    return QCoreApplication::exec();
}
