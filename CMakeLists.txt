cmake_minimum_required(VERSION 3.15)
project(Kurvot)

set(CMAKE_CXX_STANDARD 23)

# 1. Находим модуль PkgConfig
find_package(PkgConfig REQUIRED)

# 2. Ищем libpqxx через pkg-config
pkg_check_modules(PQXX REQUIRED libpqxx)

add_executable(${PROJECT_NAME} src/main.cpp
        src/DataBaseSourse/DataBase.cpp
        src/DataBaseSourse/DataBase.hpp)

# 3. Указываем пути к заголовкам
target_include_directories(${PROJECT_NAME} PRIVATE ${PQXX_INCLUDE_DIRS})

# 4. Линкуем библиотеки
target_link_libraries(${PROJECT_NAME} PRIVATE ${PQXX_LIBRARIES})

# Добавляем флаги компиляции (на всякий случай)
target_compile_options(${PROJECT_NAME} PRIVATE ${PQXX_CFLAGS_OTHER})