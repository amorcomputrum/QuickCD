#include "sqlitelib.h"

#include <iostream>
#include <exception>

sqlitelib::Sqlite* db;

/**
 * Check if Alias Alread Exists
 * 
 * @param name The Alias to search for
 * 
 * @return The result of the search
**/
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

/**
 * Add new Alias
 * 
 * @param dir The directory to store under Alias
 * @param name The Alias to create
**/
void add(std::string dir, std::string name){
	//check if name exists in database
	if(!bmExists(name)){

	}else{
		std::cerr << "name: " << name << "\nAlready exists\n";
		exit(1);
	}
}

/**
 * List Database
**/
void list(){
	std::cout << "NAME --> DIR\n";
	//list in format above
}

/**
 * Remove Alias from Database
 * 
 * @param name The name of the Alias
**/
void remove(std::string name){

}

/**
 * Display Help Information
**/
void help(){
	std::cout << "sudo qcd init      --> Allows QuickCD to initialize itself, MUST BE RUN IN SUDO\n"
				 "qcd add $DIR $NAME --> Adds $DIR to QuickCD under alias of $NAME\n"
				 "qcd list           --> Lists all alias and thier dir\n"
				 "qcd remove $NAME   --> Remove alias($NAME) from QuickCD\n";
	exit(1);
}

/**
 * Initialize Database File
**/
void init(){
	system("touch /etc/QuickCD.db");
	std::cout << "QuickCD initialized\n";
}

int main(int argc, char const *argv[]){
	//get what operation to call
	std::string operation{argv[1]};

	//Operations that don't require DB
	if(operation == "init")
		init();
	else if(operation == "help")
		help();
	
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
