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
#include "Models/ProfitabilityTableModel.hpp"
#include "Models/InfrastructureTableModel.hpp"
#include "Models/MonthlyFinancialTableModel.hpp"
#include "Models/ProjectTeamTableModel.hpp"
#include "Models/ProjectStatusModel.hpp"
#include "Models/ProjectTechnologiesTableModel.hpp"
#include "Models/DocumentationTableModel.hpp"
#include "Models/SpecificationTableModel.hpp"
#include "Models/SessionManager.hpp"
#include "Models/AdminTable.hpp"
#include "Models/FormHelper.hpp"

static QObject* sessionManagerProvider(QQmlEngine* engine, QJSEngine* scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return SessionManager::instance();
}

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("SessionManager", SessionManager::instance());

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

    ProfitabilityTableModel profitabilityModel;
    profitabilityModel.loadAll();
    engine.rootContext()->setContextProperty("profitabilityModel", &profitabilityModel);

    InfrastructureTableModel infrastructureModel;
    infrastructureModel.loadAll();
    engine.rootContext()->setContextProperty("infrastructureModel", &infrastructureModel);

    MonthlyFinancialTableModel monthlyFinancialModel;
    engine.rootContext()->setContextProperty("monthlyFinancialModel", &monthlyFinancialModel);

    ProjectTeamTableModel projectTeamModel;
    projectTeamModel.loadAll();
    engine.rootContext()->setContextProperty("projectTeamModel", &projectTeamModel);

    ProjectStatusModel projectStatusModel;
    engine.rootContext()->setContextProperty("projectStatusModel", &projectStatusModel);

    ProjectTechnologiesTableModel technologiesModel;
    technologiesModel.loadAll();
    engine.rootContext()->setContextProperty("technologiesModel", &technologiesModel);

    DocumentationTableModel documentationModel;
    documentationModel.loadAll();
    engine.rootContext()->setContextProperty("documentationModel", &documentationModel);

    SpecificationTableModel specificationModel;
    specificationModel.loadAll();
    engine.rootContext()->setContextProperty("specificationModel", &specificationModel);

    auto adminModel = new AdminTable(&engine);
    adminModel->initialize();
    engine.rootContext()->setContextProperty("adminModel", adminModel);

    auto formHelper = new FormHelper(&engine);
    engine.rootContext()->setContextProperty("formHelper", formHelper);

    qmlRegisterType<EmployeesTableModel>("Kurvot.Models", 1, 0, "EmployeesTableModel");
    qmlRegisterType<EmployeesWorkloadModel>("Kurvot.Models", 1, 0, "EmployeesWorkloadModel");
    qmlRegisterType<ProjectsTableModel>("Kurvot.Models", 1, 0, "ProjectsTableModel");
    qmlRegisterType<ClientsTableModel>("Kurvot.Models", 1, 0, "ClientsTableModel");
    qmlRegisterType<BugsTableModel>("Kurvot.Models", 1, 0, "BugsTableModel");
    qmlRegisterType<ReleasesTableModel>("Kurvot.Models", 1, 0, "ReleasesTableModel");
    qmlRegisterType<TestingEfficiencyTableModel>("Kurvot.Models", 1, 0, "TestingEfficiencyTableModel");
    qmlRegisterType<ProfitabilityTableModel>("Kurvot.Models", 1, 0, "ProfitabilityTableModel");
    qmlRegisterType<InfrastructureTableModel>("Kurvot.Models", 1, 0, "InfrastructureTableModel");
    qmlRegisterType<MonthlyFinancialTableModel>("Kurvot.Models", 1, 0, "MonthlyFinancialTableModel");
    qmlRegisterType<ProjectTeamTableModel>("Kurvot.Models", 1, 0, "ProjectTeamTableModel");
    qmlRegisterType<ProjectStatusModel>("Kurvot.Models", 1, 0, "ProjectStatusModel");
    qmlRegisterType<ProjectTechnologiesTableModel>("Kurvot.Models", 1, 0, "ProjectTechnologiesTableModel");
    qmlRegisterType<DocumentationTableModel>("Kurvot.Models", 1, 0, "DocumentationTableModel");
    qmlRegisterType<SpecificationTableModel>("Kurvot.Models", 1, 0, "SpecificationTableModel");
    qmlRegisterType<AdminTable>("Kurvot.Models", 1, 0, "AdminTable");

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
