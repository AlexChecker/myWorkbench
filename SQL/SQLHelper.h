//
// Created by alex Cheker on 11.02.2023.
//

#ifndef MYWORKBENCH_SQLHELPER_H
#define MYWORKBENCH_SQLHELPER_H
#include <iostream>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/build_config.h>
#include <jdbc/cppconn/config.h>
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/datatype.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/metadata.h>
#include <jdbc/cppconn/parameter_metadata.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset_metadata.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/sqlstring.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/variant.h>
#include <jdbc/cppconn/version_info.h>
#include <jdbc/cppconn/warning.h>

using ::std::string;
using ::std::cout;
using ::std::endl;
using namespace sql;

class SQLHelper {

private:
    Driver * driver;
    std::unique_ptr<Connection> connection;
    std::unique_ptr<Statement> statement;

public:
    SQLHelper(const string& url, const string& user, const string& password);
    std::unique_ptr<ResultSet> Query(const string& query);
    std::unique_ptr<ResultSet> fetchDatabases();
    void selectDB(const string& db);
};


#endif //MYWORKBENCH_SQLHELPER_H
