#include <iostream>
#include <memory>
#include <print>

#include <DataBase.hpp>
int main() {
    try {
        std::unique_ptr<DataBase> db = std::make_unique<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass");
        std::println("{} ", db->test());


    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}