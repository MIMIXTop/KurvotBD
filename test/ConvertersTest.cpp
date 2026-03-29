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
