//
// Created by alex Cheker on 11.02.2023.
//

#include "SQLHelper.h"


SQLHelper::SQLHelper(const string& url, const string& user, const string& password) {
    try {
        cout << "Initializing SQL driver..." << endl;
        driver=mysql::get_driver_instance();
        cout<<"Driver initialized succesfully!"<<endl;
        cout<<"Creating session..."<<endl;
        connection=std::unique_ptr<Connection>(driver->connect(url,user,password));
        cout<<"Conected successfully!"<<endl;
        cout<<"Creating statement..."<<endl;
        statement=std::unique_ptr<Statement>(connection->createStatement());
    }
    catch (SQLException &e)
    {
        cout<<e.what()<<endl;
        cout<<"Terminating..."<<endl;
    }
}

std::unique_ptr<ResultSet> SQLHelper::Query(const string& query) {
    try{
    std::unique_ptr<ResultSet> result(statement->executeQuery(query));
    return result;
    }
    catch (SQLException &e)
    {
        cout<<e.what()<<endl;
        cout<<"Terminating..."<<endl;
    }
    return nullptr;
}

std::unique_ptr<ResultSet> SQLHelper::fetchDatabases() {
    try {
        return Query("SHOW DATABASES");
    }
    catch (SQLException &e)
    {
        cout<<e.what()<<endl;
        cout<<"Terminating..."<<endl;
    }
    return nullptr;
}

void SQLHelper::selectDB(const string& db) {
    connection->setSchema(db);
}


