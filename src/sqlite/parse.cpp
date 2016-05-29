#include <sqlite3.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#define MAXDATASIZE 330 // 154*2 + 28 I saw
using namespace std;



// This returns a vector with all tables that a given sqlite file has
std::vector<std::string> sqlite_get_tables(std::string file) {
//	Define the database object
	sqlite3 *dbh;
	sqlite3_stmt *stmt;
	int stat;
	char *ErrMsg = 0;
	char *sql;
	char fullpath[50];
	char cols[50];
	std::stringstream ss;

//	Input and output
	char *filename = &file[0];
	std::vector<std::string> tables;
	tables.reserve(1);

//	Construct full file path
	cout << "Getting tables \n";
	strcpy(fullpath,"dbs/");
	strcat(fullpath,filename);
	strcat(fullpath,".sqlite");
	cout << fullpath << " is the full file path \n";

// 	Open database file
	stat = sqlite3_open(fullpath,&dbh);
	if (stat) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(dbh));
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}

//	Show tables
	sql = "SELECT name FROM sqlite_master \
		WHERE type='table' \
		ORDER BY name;";

	cout << "Compiling SQL query \n";
	int rc = sqlite3_prepare_v2(dbh,sql,-1,&stmt,NULL);
	if (rc != SQLITE_OK)
		throw string(sqlite3_errmsg(dbh));

	int rq = 0;
	int ct = 1;
	char rs[40];
	cout << "Executing query \n";		
	while (rq==0) {
		rc = sqlite3_step(stmt);
//		Checking for errors
		if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
			string errmsg(sqlite3_errmsg(dbh));
			sqlite3_finalize(stmt);
			throw errmsg;
		}
//	 	Checking if done
		if (rc == SQLITE_DONE) { 
			cout << "End of query \n";
			sqlite3_finalize(stmt);
			rq = 1;
		} else {
			strcpy(rs,reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt,0))));
			if (strcmp(rs,"sqlite_sequence")!=0) { 
				cout << ct << ": " << rs << "\n";
				tables.push_back(rs);
				ct++;
			} else { 
				cout << ct << ": " << rs << " (ignored) \n";
				ct++;
			}
		}
	}

	sqlite3_close(dbh);
	return tables;

}



// Gets a list of columns from a given table, with the type
std::vector<std::string> sqlite_get_columns(std::string file, std::string table) {
//	Define the database object
	sqlite3 *dbh;
	sqlite3_stmt *stmt;
	int stat;
	char *ErrMsg = 0;
	char sql[100];
	char fullpath[50];
	char cols[50];

//	Input and output preparation
	char *filename  = &file[0];	
	char *tablename = &table[0];	
	std::vector<std::string> coldata;
	coldata.reserve(12);
	cout << "Getting columns from a given table \n";

//	Construct full file path
	strcpy(fullpath,"dbs/");
	strcat(fullpath,filename);
	strcat(fullpath,".sqlite");
	cout << fullpath << " is the full file path \n";

// 	Open database file
	stat = sqlite3_open(fullpath,&dbh);
	if (stat) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(dbh));
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}

//	Show tables
	strcpy(sql,"PRAGMA table_info(");
	strcat(sql,tablename);
	strcat(sql,");");
	cout << "SQL: "<< sql << "\n";

	cout << "Compiling SQL query \n";
	int rc = sqlite3_prepare_v2(dbh,sql,-1,&stmt,NULL);
	if (rc != SQLITE_OK)
		throw string(sqlite3_errmsg(dbh));

	int rq = 0;
	int  cintg;
	std::string cnumb;
	char cname[30];
	char ctype[10];
	cout << "Executing query \n";		
	while (rq==0) {
		rc = sqlite3_step(stmt);
//		Checking for errors
		if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
			string errmsg(sqlite3_errmsg(dbh));
			sqlite3_finalize(stmt);
			throw errmsg;
		}
//	 	Checking if done
		if (rc == SQLITE_DONE) { 
			cout << "End of query \n";
			sqlite3_finalize(stmt);
			rq = 1;
		} else {
			cintg = sqlite3_column_int(stmt,0);
			std::ostringstream convert;
			convert << cintg;
			cnumb = convert.str();
			strcpy(cname,reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt,1))));
			strcpy(ctype,reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt,2))));
			coldata.push_back(cnumb);	
			coldata.push_back(cname);
			coldata.push_back(ctype);	
			cout << cnumb << " : " << cname << " : " << ctype << "\n";
		}
	}

	sqlite3_close(dbh);
	return coldata;

}




int main() {
	std::vector<std::string> fn,tn,cn;

//	Reserve space
	fn.reserve(20);
	tn.reserve(2);
	cn.reserve(12);


//	Database files for Ankhbot
	fn.push_back("AutoHostDB");
	fn.push_back("BlackListDB");
	fn.push_back("CommandsDB");
	fn.push_back("CurrencyDB");
	fn.push_back("EditorsDB");
	fn.push_back("ExternalSubDB");
	fn.push_back("ExtraQuotesDB");
	fn.push_back("FollowerDB");
	fn.push_back("JoinDB");
	fn.push_back("QueueDB");
	fn.push_back("QuotesDB");
	fn.push_back("RankDB");
	fn.push_back("RegularsDB");
	fn.push_back("SfxsDB");
	fn.push_back("SongBlackListDB");
	fn.push_back("SpeakDB");
	fn.push_back("TimersDB");
	fn.push_back("WhitelistDB");
	fn.push_back("WordsDB");

//	Let's list tables and columns from those tables
	tn = sqlite_get_tables(fn[0]);
	cn = sqlite_get_columns(fn[0],tn[0]);	
}
