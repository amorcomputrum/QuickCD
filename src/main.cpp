#include "sqlitelib.h"

#include <iostream>
#include <exception>
#include <string>
#include <unistd.h>
#include <filesystem>

sqlitelib::Sqlite* db;
std::string usr;

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
 * Initialize Database File For User
**/
void init(){
	std::string command = "touch /home/" + usr + "/.QuickCD.db";
	system(command.c_str());
}

/**
 * Connects to DataBase
**/
void connectDB(){
	//init table
	try{
		std::string dbFile = "/home/" + usr + "/.QuickCD.db";
		db = new sqlitelib::Sqlite(dbFile.c_str());

		std::string sql = "CREATE TABLE IF NOT EXISTS bm ( name TEXT, loc TEXT );";
		
		db->execute(sql.c_str());
	}
	catch(std::exception& e){
		//Failed So create File For User
		init();

		std::string dbFile = "/home/" + usr + "/.QuickCD.db";
		db = new sqlitelib::Sqlite(dbFile.c_str());

		std::string sql = "CREATE TABLE IF NOT EXISTS bm ( name TEXT, loc TEXT );";
		
		db->execute(sql.c_str());
	}
}

/**
 * Add new Alias
 * 
 * @param dir The directory to store under Alias
 * @param name The Alias to create
**/
void add(std::string dir, std::string name){

	connectDB();

	//check if name exists in database
	if(!bmExists(name)){
		chdir(dir.c_str());

		std::string cwd = std::filesystem::current_path();

		auto command = db->prepare("INSERT INTO bm (name, loc) VALUES (?, ?)");
		command.execute(name.c_str(), cwd.c_str());

	}else{
		std::cerr << "name: " << name << "\nAlready exists\n";
		exit(1);
	}
}

/**
 * List Database
**/
void list(){

	connectDB();

	std::cout << "NAME --> DIR\n";
	std::cout << "============\n";

	//list in format above
	auto rows = db->execute<std::string, std::string>("SELECT name, loc FROM bm");

	for(int i = 0; i < rows.size(); i++){
		auto [name, loc] = rows[i];

		std::cout << name << " --> " << loc << "\n";
	}
}

/**
 * Remove Alias from Database
 * 
 * @param name The name of the Alias
**/
void remove(std::string name){

	connectDB();

	std::string sql = "DELETE FROM bm WHERE name='" + name + "'";
	db->execute(sql.c_str());
}

/**
 * Display Help Information
**/
void help(){
	std::cout << "qcd --add $DIR $NAME --> Adds $DIR to QuickCD under alias of $NAME\n"
				 "qcd --list           --> Lists all alias and thier dir\n"
				 "qcd --remove $NAME   --> Remove alias($NAME) from QuickCD\n"
				 "qcd $NAME            --> CDs into dir that uses alias($NAME)\n";
	exit(1);
}
/**
 * Enter Dir from alias/name
 * 
 * @param name The name of the Alias
**/
void cd(std::string name){
	connectDB();

	if(bmExists(name)){
		std::string sql = "SELECT loc FROM bm WHERE name='" + name + "';";
		std::string dir = db->execute_value<std::string>(sql.c_str());

		std::string command = "xdotool type 'cd " + dir + "' > /dev/null 2>&1";
		system(command.c_str());
		system("xdotool key ENTER");

	}
}

int main(int argc, char const *argv[]){
	usr = getlogin();

	//get what operation to call
	if(argv[1] != NULL){
		std::string operation{argv[1]};

		//Operations that don't require DB
		if(operation == "--help"|| operation == "-h")
			help();
		else if(operation == "--add" || operation == "-a"){
			if(argv[2] != NULL && argv[3] != NULL){
				std::string dir{argv[2]};
				std::string name{argv[3]};
				add(dir,name);
			}else
				std::cout << "to few arguments\n";
		}
		else if(operation == "--remove" || operation == "-r"){
			if(argv[2] != NULL){
				std::string name{argv[2]};
				remove(name);
			}
		}
		else if(operation == "--list" || operation == "-l")
			list();
		else {
			if(argv[1] != NULL){
				std::string name{argv[1]};
				cd(name);
			}
		}
	}

	return 0;
}
