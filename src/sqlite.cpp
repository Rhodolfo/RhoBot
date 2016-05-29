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





static int callback_get_tables(void *data, int argc, char **argv, char **ColName){
	int i;
	printf("[Callback] INIT \n");	
	for(i=0; i<argc; i++) {
		printf("%s = %s\n", ColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("[Callback] FINALIZE \n");
	return 0;
}





static int callback_get_columns(void *data, int argc, char **argv, char **ColName){
	int i;
	printf("[Callback] INIT \n");	
	for(i=0; i<argc; i++) {
		printf("%s = %s\n", ColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("[Callback] FINALIZE \n");
	return 0;
}





int sqlite_parse(char *filename) {
//	Define the database object
	sqlite3 *dbh;
	int stat;
	char *ErrMsg = 0;
	char *sql;
	char fullpath[50];

//	Output
	cout << "[sqlite_parse] Routine has been called \n";

//	Construct full file path
	strcpy(fullpath,"dbs/");
	strcat(fullpath,filename);
	strcat(fullpath,".sqlite");
	cout << "[sqlite_parse] " << fullpath << " is the full file path \n";

// 	Open database file
	stat = sqlite3_open(fullpath,&dbh);
	if(stat){
		fprintf(stderr, "[sqlite_parse] Can't open database: %s\n", sqlite3_errmsg(dbh));
	} else {
		fprintf(stderr, "[sqlite_parse] Opened database successfully\n");
	}

//	Show tables
	sql = "SELECT name FROM sqlite_master \
		WHERE type='table' \
		ORDER BY name;";
	stat = sqlite3_exec(dbh,sql,callback_get_tables,0,&ErrMsg);
	if (stat != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", ErrMsg);
		sqlite3_free(ErrMsg);
	} else {
		fprintf(stdout, "YES\n");
	}

//	 Let's diplay the full table
	sql = "PRAGMA TABLE_INFO(RegCommand)";
	stat = sqlite3_exec(dbh,sql,callback_get_columns,0,&ErrMsg);
	if (stat != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", ErrMsg);
		sqlite3_free(ErrMsg);
	} else {
		fprintf(stdout, "YES\n");
	}

//	Close 
	sqlite3_close(dbh);
}




int sqlite_get_tables(char *filename) {
//	Define the database object
	sqlite3 *dbh;
	sqlite3_stmt *stmt;
	int stat;
	char *ErrMsg = 0;
	char *sql;
	char fullpath[50];
	char cols[50];

//	Output
	cout << "Getting tables \n";

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
	sql = "SELECT name FROM sqlite_master \
		WHERE type='table' \
		ORDER BY name;";

	cout << "Compiling SQL query \n";
	int rc = sqlite3_prepare_v2(dbh,sql,-1,&stmt,NULL);
	if (rc != SQLITE_OK)
		throw string(sqlite3_errmsg(dbh));

	int rq = 0;
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
			cout << "Results: " << sqlite3_column_text(stmt,0) << "\n";
		}
	}

}




int main() {
	char str[20];
	strcpy(str,"CommandsDB");
	sqlite_get_tables(str);
}
