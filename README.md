
# Bank Management System

A simple **C++ Bank Management System** built using the `pqxx` library for PostgreSQL integration. The system allows users to register, log in, deposit money, and transfer funds between accounts.

---

## Features

1. **User Registration**: New users can register with a username and password.
2. **User Login**: Existing users can log in securely.
3. **Deposit Funds**: Users can deposit money into their accounts.
4. **Transfer Funds**: Users can transfer funds to other accounts.
5. **PostgreSQL Integration**: Data is stored and managed in a PostgreSQL database.

---

## Prerequisites

1. **PostgreSQL**:
   - Install PostgreSQL on your system:
     ```bash
     sudo apt-get install postgresql postgresql-contrib
     ```
   - Create a database called `bms`:
     ```sql
     CREATE DATABASE bms;
     ```

2. **libpqxx**:
   - Install the `libpqxx` library:
     ```bash
     sudo apt-get install libpqxx-dev
     ```

---

## Setting Up the Database

1. Connect to the `bms` database:
   ```bash
   psql -U postgres -d bms
   ```

2. Create the `user_data` table:
   ```sql
   CREATE TABLE user_data (
       id SERIAL PRIMARY KEY,
       username VARCHAR(100) UNIQUE NOT NULL,
       password VARCHAR(255) NOT NULL,
       balance INTEGER DEFAULT 0
   );
   ```

---

## Building and Running the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/romavolosh/Bank-Management-System.git
   cd Bank-Management-System
   ```

2. Build the project using **CMake**:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. Run the executable:
   ```bash
   ./bank_management
   ```

---

## Usage

1. When the program starts, you will see a main menu:
   ```
   =====================
   BANK MANAGEMENT SYSTEM
   =====================
   1. Register
   2. Log in
   3. Deposit
   4. Transfer
   5. Exit
   ```

2. **Register**:
   - Choose option `1` to register a new user.
   - Enter a username and password.

3. **Log in**:
   - Choose option `2` to log in with your credentials.

4. **Deposit**:
   - After logging in, choose option `3` to deposit money.
   - Enter the deposit amount.

5. **Transfer**:
   - After logging in, choose option `4` to transfer funds.
   - Enter the receiver's username and the amount.

6. **Exit**:
   - Choose option `5` to exit the system.

---

## Technologies Used

- **C++**
- **PostgreSQL**
- **libpqxx**
---

## Author

Created by **Roma Voloshyn**.
