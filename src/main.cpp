#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "Models/ButtonFilterModel.hpp"
#include "Models/EmployeesTableModel.hpp"
#include "Models/EmployeesWorkloadModel.hpp"

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ButtonFilterModel filterModel;
    engine.rootContext()->setContextProperty("filterModel", &filterModel);

    EmployeesTableModel employeesTableModel;
    engine.rootContext()->setContextProperty("employeesTableModel", &employeesTableModel);

    EmployeesWorkloadModel workloadModel;
    engine.rootContext()->setContextProperty("workloadModel", &workloadModel);

    qmlRegisterType<EmployeesTableModel>("Kurvot.Models", 1, 0, "EmployeesTableModel");
    qmlRegisterType<EmployeesWorkloadModel>("Kurvot.Models", 1, 0, "EmployeesWorkloadModel");

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
