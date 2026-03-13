#include <iostream>
#include <pqxx/pqxx>

int main() {
    try {
        // Подключаемся к localhost, так как порт 5432 проброшен из Docker
        pqxx::connection c("host=127.0.0.1 port=5432 dbname=db user=user password=pass");

        if (c.is_open()) {
            std::cout << "Connected to: " << c.dbname() << std::endl;

            pqxx::work txn(c);
            // Создадим таблицу для теста
            txn.exec("CREATE TABLE IF NOT EXISTS test (id serial PRIMARY KEY, data text);");
            txn.exec("INSERT INTO test (data) VALUES ('Hello Arch Linux!');");
            txn.commit();

            std::cout << "Data inserted successfully!" << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}