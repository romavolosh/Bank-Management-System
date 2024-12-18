#include <iostream>
#include <pqxx/pqxx>
#include <string>

// Главное меню
void main_menu() {
    std::cout << "=====================" << std::endl;
    std::cout << "BANK MANAGEMENT SYSTEM" << std::endl;
    std::cout << "=====================" << std::endl;
    std::cout << "1. Register" << std::endl;
    std::cout << "2. Log in" << std::endl;
    std::cout << "3. Deposit" << std::endl;
    std::cout << "4. Transfer" << std::endl;
    std::cout << "5. Exit" << std::endl;
}

int choice() {
    std::cout << "Choose your option: ";
    int val;
    std::cin >> val;
    return val;
}

// Регистрация пользователя
bool registerUser(pqxx::connection& C, const std::string& username, const std::string& password) {
    try {
        pqxx::work W(C);
        pqxx::result R = W.exec("SELECT id FROM user_data WHERE username = " + W.quote(username));

        if (!R.empty()) {
            std::cout << "User already exists!" << std::endl;
            return false;
        }

        W.exec("INSERT INTO user_data (username, password, balance) VALUES (" 
                + W.quote(username) + ", " + W.quote(password) + ", 0)");
        W.commit();
        std::cout << "User registered successfully!" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

// Получение баланса по username
int getBalanceByUsername(pqxx::connection& C, const std::string& username) {
    try {
        pqxx::nontransaction N(C);
        pqxx::result R = N.exec("SELECT balance FROM user_data WHERE username = " + N.quote(username));

        if (R.empty()) {
            std::cerr << "User not found!" << std::endl;
            return -1;
        }

        return R[0][0].is_null() ? 0 : R[0][0].as<int>();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}

// Пополнение баланса
void deposit_func(pqxx::connection& C, const std::string& username, int amount) {
    try {
        pqxx::work W(C);
        W.exec("UPDATE user_data SET balance = balance + " + W.quote(amount) +
               " WHERE username = " + W.quote(username));
        W.commit();

        std::cout << "Deposit successful! " << amount << " added to '" << username << "'." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Авторизация пользователя
bool loginUser(pqxx::connection& C, const std::string& username, const std::string& password) {
    try {
        pqxx::work W(C);
        pqxx::result R = W.exec("SELECT id FROM user_data WHERE username = " + W.quote(username) +
                                " AND password = " + W.quote(password));

        if (!R.empty()) {
            std::cout << "Login successful!" << std::endl;
            return true;
        } else {
            std::cout << "Invalid username or password!" << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

// Перевод средств
void transferMoney(pqxx::connection& C, const std::string& sender, const std::string& receiver, int amount) {
    try {
        int sender_balance = getBalanceByUsername(C, sender);
        int receiver_balance = getBalanceByUsername(C, receiver);

        if (sender_balance == -1 || receiver_balance == -1) {
            std::cerr << "Error: Sender or receiver not found." << std::endl;
            return;
        }

        if (sender_balance < amount) {
            std::cerr << "Insufficient funds!" << std::endl;
            return;
        }

        pqxx::work W(C);
        W.exec("UPDATE user_data SET balance = balance - " + W.quote(amount) +
               " WHERE username = " + W.quote(sender));
        W.exec("UPDATE user_data SET balance = balance + " + W.quote(amount) +
               " WHERE username = " + W.quote(receiver));
        W.commit();

        std::cout << "Transfer successful! " << amount << " transferred from '" << sender
                  << "' to '" << receiver << "'." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    pqxx::connection C("dbname=bms user=postgres password=secret host=localhost");

    bool logined = false;
    std::string username, password;

    while (true) {
        main_menu();
        int user_choice = choice();

        if (user_choice == 1) { // Register
            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;
            registerUser(C, username, password);

        } else if (user_choice == 2) { // Log in
            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;
            logined = loginUser(C, username, password);

        } else if (user_choice == 3) { // Deposit
            if (logined) {
                int deposit_amount;
                std::cout << "Enter deposit amount: ";
                std::cin >> deposit_amount;
                deposit_func(C, username, deposit_amount);
            } else {
                std::cout << "Please log in first!" << std::endl;
            }

        } else if (user_choice == 4) { // Transfer
            if (logined) {
                int amount;
                std::string receiver;

                std::cout << "Receiver: ";
                std::cin >> receiver;
                std::cout << "Amount: ";
                std::cin >> amount;

                transferMoney(C, username, receiver, amount);
            } else {
                std::cout << "Please log in first!" << std::endl;
            }

        } else if (user_choice == 5) { // Exit
            std::cout << "Exiting the system. Goodbye!" << std::endl;
            break;
        } else {
            std::cout << "Invalid choice. Try again!" << std::endl;
        }
    }

    return 0;
}
