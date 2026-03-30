#include <gtest/gtest.h>

#include "DataBaseSourse/SqlRequests/Converters.hpp"
#include "DataBaseSourse/SqlRequests/ToParams.hpp"

using namespace lib::Converters;
using namespace lib::Models;

// ═══════════════════════════════════════════════════════════════════
// toString — enum → русское значение
// ═══════════════════════════════════════════════════════════════════

TEST(ToString, DepartmentType) {
    EXPECT_EQ(toString(DepartmentType::Development), "разработка");
    EXPECT_EQ(toString(DepartmentType::Testing), "тестирование");
    EXPECT_EQ(toString(DepartmentType::Tech_support), "техподдержка");
    EXPECT_EQ(toString(DepartmentType::Project_management), "управление_проектами");
    EXPECT_EQ(toString(DepartmentType::Administrative), "административный");
}

TEST(ToString, PositionCategory) {
    EXPECT_EQ(toString(PositionCategory::Technical_staff), "технический_персонал");
    EXPECT_EQ(toString(PositionCategory::Management), "руководящий_состав");
}

TEST(ToString, ClientType) {
    EXPECT_EQ(toString(ClientType::Corporate), "корпоративный");
    EXPECT_EQ(toString(ClientType::Government), "государственный");
    EXPECT_EQ(toString(ClientType::Individual_entrepreneur), "ИП");
}

TEST(ToString, LicenseType) {
    EXPECT_EQ(toString(LicenseType::Perpetual), "бессрочная");
    EXPECT_EQ(toString(LicenseType::Subscription), "подписка");
}

TEST(ToString, ProjectStatus) {
    EXPECT_EQ(toString(ProjectStatus::In_development), "в_разработке");
    EXPECT_EQ(toString(ProjectStatus::Completed), "завершен");
    EXPECT_EQ(toString(ProjectStatus::Postponed), "отложен");
    EXPECT_EQ(toString(ProjectStatus::Closed_early), "закрыт_досрочно");
}

TEST(ToString, ProjectMethodology) {
    EXPECT_EQ(toString(ProjectMethodology::Waterfall), "Waterfall");
    EXPECT_EQ(toString(ProjectMethodology::Scrum), "Scrum");
    EXPECT_EQ(toString(ProjectMethodology::Kanban), "Kanban");
    EXPECT_EQ(toString(ProjectMethodology::Hybrid), "гибридная");
}

TEST(ToString, ProjectType) {
    EXPECT_EQ(toString(ProjectType::Web_application), "веб-приложение");
    EXPECT_EQ(toString(ProjectType::Mobile_application), "мобильное_приложение");
    EXPECT_EQ(toString(ProjectType::Corporate_system), "корпоративная_система");
    EXPECT_EQ(toString(ProjectType::Microservice), "микросервис");
}

TEST(ToString, BugSeverity) {
    EXPECT_EQ(toString(BugSeverity::Blocking), "блокирующая");
    EXPECT_EQ(toString(BugSeverity::Critical), "критическая");
    EXPECT_EQ(toString(BugSeverity::Medium), "средняя");
    EXPECT_EQ(toString(BugSeverity::Low), "низкая");
}

TEST(ToString, BugStatus) {
    EXPECT_EQ(toString(BugStatus::New), "новая");
    EXPECT_EQ(toString(BugStatus::In_progress), "в_работе");
    EXPECT_EQ(toString(BugStatus::Fixed), "исправлена");
    EXPECT_EQ(toString(BugStatus::Verified), "проверена");
}

TEST(ToString, DocType) {
    EXPECT_EQ(toString(DocType::Architecture), "архитектура");
    EXPECT_EQ(toString(DocType::API), "API");
    EXPECT_EQ(toString(DocType::User_guide), "руководство_пользователя");
    EXPECT_EQ(toString(DocType::Admin_guide), "руководство_администратора");
    EXPECT_EQ(toString(DocType::Specification), "спецификация");
}

TEST(ToString, CloudProvider) {
    EXPECT_EQ(toString(CloudProvider::AWS), "AWS");
    EXPECT_EQ(toString(CloudProvider::Azure), "Azure");
    EXPECT_EQ(toString(CloudProvider::GCP), "GCP");
    EXPECT_EQ(toString(CloudProvider::Yandex_Cloud), "Yandex_Cloud");
    EXPECT_EQ(toString(CloudProvider::Other), "другой");
}

// ═══════════════════════════════════════════════════════════════════
// toPgArray — vector<string> → PostgreSQL array
// ═══════════════════════════════════════════════════════════════════

TEST(ToPgArray, EmptyVector) {
    EXPECT_EQ(toPgArray({}), "{}");
}

TEST(ToPgArray, SingleElement) {
    EXPECT_EQ(toPgArray({"C++"}), "{C++}");
}

TEST(ToPgArray, MultipleElements) {
    EXPECT_EQ(toPgArray({"C++", "Python", "Rust"}), "{C++,Python,Rust}");
}

// ═══════════════════════════════════════════════════════════════════
// toParams — примитивы pass-through
// ═══════════════════════════════════════════════════════════════════

TEST(ToParams, IntPassthrough) {
    EXPECT_EQ(toParams(42), 42);
    EXPECT_EQ(toParams(0), 0);
    EXPECT_EQ(toParams(-1), -1);
}

TEST(ToParams, DoublePassthrough) {
    EXPECT_DOUBLE_EQ(toParams(3.14), 3.14);
    EXPECT_DOUBLE_EQ(toParams(0.0), 0.0);
}

TEST(ToParams, BoolPassthrough) {
    EXPECT_EQ(toParams(true), true);
    EXPECT_EQ(toParams(false), false);
}

TEST(ToParams, StringPassthrough) {
    std::string s = "hello";
    EXPECT_EQ(toParams(s), "hello");
}

// ═══════════════════════════════════════════════════════════════════
// toParams — enum → строка
// ═══════════════════════════════════════════════════════════════════

TEST(ToParams, EnumConversion) {
    EXPECT_EQ(toParams(DepartmentType::Development), "разработка");
    EXPECT_EQ(toParams(BugSeverity::Critical), "критическая");
    EXPECT_EQ(toParams(CloudProvider::AWS), "AWS");
}

// ═══════════════════════════════════════════════════════════════════
// toParams — vector → pg array
// ═══════════════════════════════════════════════════════════════════

TEST(ToParams, VectorToPgArray) {
    EXPECT_EQ(toParams(std::vector<std::string>{"a", "b"}), "{a,b}");
}

// ═══════════════════════════════════════════════════════════════════
// parsePgArray — "{a,b,c}" → vector<string>
// ═══════════════════════════════════════════════════════════════════

TEST(ParsePgArray, Empty) {
    auto v = parsePgArray("{}");
    EXPECT_TRUE(v.empty());
}

TEST(ParsePgArray, SingleElement) {
    auto v = parsePgArray("{C++}");
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], "C++");
}

TEST(ParsePgArray, MultipleElements) {
    auto v = parsePgArray("{C++,Python,Rust}");
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], "C++");
    EXPECT_EQ(v[1], "Python");
    EXPECT_EQ(v[2], "Rust");
}

TEST(ParsePgArray, RoundTrip) {
    std::vector<std::string> original = {"Boost", "Qt", "gtest"};
    auto pg = toPgArray(original);
    auto parsed = parsePgArray(pg);
    EXPECT_EQ(parsed, original);
}

// ═══════════════════════════════════════════════════════════════════
// parseXxx — string → enum (обратные к toString)
// ═══════════════════════════════════════════════════════════════════

TEST(ParseEnum, DepartmentType) {
    EXPECT_EQ(parseDepartmentType("разработка"), DepartmentType::Development);
    EXPECT_EQ(parseDepartmentType("тестирование"), DepartmentType::Testing);
    EXPECT_EQ(parseDepartmentType("техподдержка"), DepartmentType::Tech_support);
    EXPECT_EQ(parseDepartmentType("управление_проектами"), DepartmentType::Project_management);
    EXPECT_EQ(parseDepartmentType("административный"), DepartmentType::Administrative);
}

TEST(ParseEnum, PositionCategory) {
    EXPECT_EQ(parsePositionCategory("технический_персонал"), PositionCategory::Technical_staff);
    EXPECT_EQ(parsePositionCategory("руководящий_состав"), PositionCategory::Management);
}

TEST(ParseEnum, ClientType) {
    EXPECT_EQ(parseClientType("корпоративный"), ClientType::Corporate);
    EXPECT_EQ(parseClientType("государственный"), ClientType::Government);
    EXPECT_EQ(parseClientType("ИП"), ClientType::Individual_entrepreneur);
}

TEST(ParseEnum, LicenseType) {
    EXPECT_EQ(parseLicenseType("бессрочная"), LicenseType::Perpetual);
    EXPECT_EQ(parseLicenseType("подписка"), LicenseType::Subscription);
}

TEST(ParseEnum, ProjectStatus) {
    EXPECT_EQ(parseProjectStatus("в_разработке"), ProjectStatus::In_development);
    EXPECT_EQ(parseProjectStatus("завершен"), ProjectStatus::Completed);
    EXPECT_EQ(parseProjectStatus("отложен"), ProjectStatus::Postponed);
    EXPECT_EQ(parseProjectStatus("закрыт_досрочно"), ProjectStatus::Closed_early);
}

TEST(ParseEnum, ProjectMethodology) {
    EXPECT_EQ(parseProjectMethodology("Waterfall"), ProjectMethodology::Waterfall);
    EXPECT_EQ(parseProjectMethodology("Scrum"), ProjectMethodology::Scrum);
    EXPECT_EQ(parseProjectMethodology("Kanban"), ProjectMethodology::Kanban);
    EXPECT_EQ(parseProjectMethodology("гибридная"), ProjectMethodology::Hybrid);
}

TEST(ParseEnum, ProjectType) {
    EXPECT_EQ(parseProjectType("веб-приложение"), ProjectType::Web_application);
    EXPECT_EQ(parseProjectType("мобильное_приложение"), ProjectType::Mobile_application);
    EXPECT_EQ(parseProjectType("корпоративная_система"), ProjectType::Corporate_system);
    EXPECT_EQ(parseProjectType("микросервис"), ProjectType::Microservice);
}

TEST(ParseEnum, BugSeverity) {
    EXPECT_EQ(parseBugSeverity("блокирующая"), BugSeverity::Blocking);
    EXPECT_EQ(parseBugSeverity("критическая"), BugSeverity::Critical);
    EXPECT_EQ(parseBugSeverity("средняя"), BugSeverity::Medium);
    EXPECT_EQ(parseBugSeverity("низкая"), BugSeverity::Low);
}

TEST(ParseEnum, BugStatus) {
    EXPECT_EQ(parseBugStatus("новая"), BugStatus::New);
    EXPECT_EQ(parseBugStatus("в_работе"), BugStatus::In_progress);
    EXPECT_EQ(parseBugStatus("исправлена"), BugStatus::Fixed);
    EXPECT_EQ(parseBugStatus("проверена"), BugStatus::Verified);
}

TEST(ParseEnum, DocType) {
    EXPECT_EQ(parseDocType("архитектура"), DocType::Architecture);
    EXPECT_EQ(parseDocType("API"), DocType::API);
    EXPECT_EQ(parseDocType("руководство_пользователя"), DocType::User_guide);
    EXPECT_EQ(parseDocType("руководство_администратора"), DocType::Admin_guide);
    EXPECT_EQ(parseDocType("спецификация"), DocType::Specification);
}

TEST(ParseEnum, CloudProvider) {
    EXPECT_EQ(parseCloudProvider("AWS"), CloudProvider::AWS);
    EXPECT_EQ(parseCloudProvider("Azure"), CloudProvider::Azure);
    EXPECT_EQ(parseCloudProvider("GCP"), CloudProvider::GCP);
    EXPECT_EQ(parseCloudProvider("Yandex_Cloud"), CloudProvider::Yandex_Cloud);
    EXPECT_EQ(parseCloudProvider("другой"), CloudProvider::Other);
}

// ═══════════════════════════════════════════════════════════════════
// Round-trip: toString → parseXxx → тот же enum
// ═══════════════════════════════════════════════════════════════════

TEST(ParseEnum, RoundTripDepartment) {
    for (auto e : {DepartmentType::Development, DepartmentType::Testing,
                   DepartmentType::Tech_support, DepartmentType::Project_management,
                   DepartmentType::Administrative}) {
        EXPECT_EQ(parseDepartmentType(toString(e)), e);
    }
}

TEST(ParseEnum, RoundTripBugSeverity) {
    for (auto e : {BugSeverity::Blocking, BugSeverity::Critical,
                   BugSeverity::Medium, BugSeverity::Low}) {
        EXPECT_EQ(parseBugSeverity(toString(e)), e);
    }
}

TEST(ParseEnum, RoundTripCloudProvider) {
    for (auto e : {CloudProvider::AWS, CloudProvider::Azure, CloudProvider::GCP,
                   CloudProvider::Yandex_Cloud, CloudProvider::Other}) {
        EXPECT_EQ(parseCloudProvider(toString(e)), e);
    }
}
