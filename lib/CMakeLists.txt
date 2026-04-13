project(Kurvot_lib)

file(GLOB_RECURSE SRCS src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp)

# 1. Находим модуль PkgConfig
find_package(PkgConfig REQUIRED)

find_package(Qt6 COMPONENTS Core)

# 2. Ищем libpqxx через pkg-config
pkg_check_modules(PQXX REQUIRED libpqxx)

add_library(Kurvot_lib STATIC ${SRCS} ${HEADERS})

# 3. Указываем пути к заголовкам
target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/src
        PRIVATE
            ${PQXX_INCLUDE_DIRS})

# 4. Линкуем библиотеки
target_link_libraries(${PROJECT_NAME} PRIVATE ${PQXX_LIBRARIES})
# Альтернативный вариант (если pfr не работает):
target_include_directories(${PROJECT_NAME} PUBLIC ${boost_pfr_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core)

# Добавляем флаги компиляции (на всякий случай)
target_compile_options(${PROJECT_NAME} PRIVATE ${PQXX_CFLAGS_OTHER})