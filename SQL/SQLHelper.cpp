//
// Created by alex Cheker on 11.02.2023.
//

#include "SQLHelper.h"


SQLHelper::SQLHelper(const string& url, const string& user, const string& password) {
    try {
        _log.push_back("Initializing SQL driver...");
        driver=mysql::get_driver_instance();
        _log.push_back("Driver initialized succesfully!");
        _log.push_back("Creating session...");
        connection=std::unique_ptr<Connection>(driver->connect(url,user,password));
        _log.push_back("Conected successfully!");
        _log.push_back("Creating statement...");
        statement=std::unique_ptr<Statement>(connection->createStatement());
        _log.push_back("Created statement!");
    }
    catch (SQLException &e)
    {
        _log.push_back(e.what());
        cout<<e.what()<<endl;
    }
}

std::unique_ptr<ResultSet> SQLHelper::Query(const string& query) {
    try{
        _log.push_back("Executing '"+query+"' query string");
    std::unique_ptr<ResultSet> result(statement->executeQuery(query));
    return result;
    }
    catch (SQLException &e)
    {
        _log.push_back(e.what());
        cout<<e.what()<<endl;
    }
    return nullptr;
}

std::unique_ptr<ResultSet> SQLHelper::fetchDatabases() {
    try {
        return Query("SHOW DATABASES");
    }
    catch (SQLException &e)
    {
        _log.push_back(e.what());
        cout<<e.what()<<endl;
        cout<<"Terminating..."<<endl;
    }
    return nullptr;
}

void SQLHelper::selectDB(const string& db) {
    connection->setSchema(db);
}

std::unique_ptr<ResultSet> SQLHelper::fetchTables() {
    try {
        return Query("SHOW TABLES");
    }
    catch(SQLException &e)
    {
        _log.push_back(e.what());
    }
}

std::unique_ptr<ResultSet> SQLHelper::fetchColumns(const string table) {
    try {
        return Query("SHOW COLUMNS FROM "+table);
    }
    catch(SQLException &e)
    {
        _log.push_back(e.what());
    }
}


