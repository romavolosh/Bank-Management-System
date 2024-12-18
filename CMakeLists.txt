cmake_minimum_required(VERSION 3.10)
project(BankManagementSystem)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)

# Добавляем исполняемый файл
add_executable(bank_management main.cpp)

# Путь к библиотеке libpqxx
find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

# Подключаем библиотеку pqxx
target_link_libraries(bank_management pqxx pq)

