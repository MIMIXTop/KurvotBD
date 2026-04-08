#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "Models/ButtonFilterModel.hpp"
#include "Models/EmployeesTableModel.hpp"
#include "Models/EmployeesWorkloadModel.hpp"
#include "Models/ProjectsTableModel.hpp"
#include "Models/ClientsTableModel.hpp"
#include "Models/BugsTableModel.hpp"
#include "Models/ReleasesTableModel.hpp"
#include "Models/TestingEfficiencyTableModel.hpp"

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ButtonFilterModel filterModel;
    engine.rootContext()->setContextProperty("filterModel", &filterModel);

    EmployeesTableModel employeesTableModel;
    engine.rootContext()->setContextProperty("employeesTableModel", &employeesTableModel);

    EmployeesWorkloadModel workloadModel;
    engine.rootContext()->setContextProperty("workloadModel", &workloadModel);

    ProjectsTableModel projectsModel;
    engine.rootContext()->setContextProperty("projectsModel", &projectsModel);

    ClientsTableModel clientsModel;
    engine.rootContext()->setContextProperty("clientsModel", &clientsModel);

    BugsTableModel bugsModel;
    engine.rootContext()->setContextProperty("bugsModel", &bugsModel);

    ReleasesTableModel releasesModel;
    engine.rootContext()->setContextProperty("releasesModel", &releasesModel);

    TestingEfficiencyTableModel testingEfficiencyModel;
    engine.rootContext()->setContextProperty("testingEfficiencyModel", &testingEfficiencyModel);

    qmlRegisterType<EmployeesTableModel>("Kurvot.Models", 1, 0, "EmployeesTableModel");
    qmlRegisterType<EmployeesWorkloadModel>("Kurvot.Models", 1, 0, "EmployeesWorkloadModel");
    qmlRegisterType<ProjectsTableModel>("Kurvot.Models", 1, 0, "ProjectsTableModel");
    qmlRegisterType<ClientsTableModel>("Kurvot.Models", 1, 0, "ClientsTableModel");
    qmlRegisterType<BugsTableModel>("Kurvot.Models", 1, 0, "BugsTableModel");
    qmlRegisterType<ReleasesTableModel>("Kurvot.Models", 1, 0, "ReleasesTableModel");
    qmlRegisterType<TestingEfficiencyTableModel>("Kurvot.Models", 1, 0, "TestingEfficiencyTableModel");

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
