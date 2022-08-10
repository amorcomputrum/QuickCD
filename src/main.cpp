#include "sqlitelib.h"

#include <iostream>
#include <exception>

sqlitelib::Sqlite* db;

bool bmExists(std::string name){
	std::string sql = "SELECT COUNT(name) FROM bm WHERE name='" + name + "';";

	int count;

	try{
		//get number of times this name has been used
		count = db->execute_value<int>(sql.c_str());
	}catch(std::exception& e){
		std::cerr << "Something went wrong\n";
		exit(1);
	}

	//if name has not been used then return false otherwise return true
	if(count != 0) return true;
	else return false;
}

void add(std::string dir, std::string name){
	//check if name exists in database
	if(!bmExists(name)){

	}else{
		std::cerr << "name: " << name << "\nAlready exists\n";
		exit(1);
	}
}

int main(int argc, char const *argv[]){
	//get what operation to call
	std::string operation{argv[1]};

	//initialize QuickCD Database
	if(operation == "init"){
		system("touch /etc/QuickCD.db");
		std::cout << "QuickCD initialized\n";
	}
	
	//init table
	try{
		db = new sqlitelib::Sqlite("/etc/QuickCD.db");
		db->execute(R"(
		  CREATE TABLE IF NOT EXISTS bm (
		    name TEXT,
		    loc TEXT
		  )
		)");
	}
	catch(std::exception& e){
		std::cerr << "First run: sudo qcd init\n";
		return 0;
	}

	//if operation is add, call add function
	if(operation == "add"){
		std::string dir{argv[2]};
		std::string name{argv[3]};

		add(dir,name);
	}

	return 0;
}
