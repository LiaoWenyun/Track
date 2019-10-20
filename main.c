#include<stdio.h>
#include<stdlib.h>
#include<sqlite3.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include "tracker.h" 


int main(int argc, char *argv[]){
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc ;
		char *sql;
		sqlite3_stmt *stmt = NULL;

		//connect to the databse
		char* path_to_file = getenv("HOME");
		char* f_name =  "/.config/track/TRACK.db";
		strcat( path_to_file , f_name );
		rc = sqlite3_open(path_to_file,&db);
		if (rc) {
				//error eccourred
				fprintf(stderr, "Cannot open database %s\n ", sqlite3_errmsg(db));
				return (0);
		}
		//create a table in the tracker database
		sql = "CREATE TABLE TRACK("  \
			   "CLASS_NAME TEXT PRIMARY KEY NOT NULL," \
			   "TOTAL_TIME INT NOT NULL," \
			   "TOTAL_DAYS INT NOT NULL," \
			   "START_DATE INT NOT NULL,"\
			   "CURRENT_START_TIME INT NOT NULL," \
			   "STATUS INT NOT NULL)";
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
		if( rc != SQLITE_OK ){
				if(strncmp(zErrMsg,"table TRACK already exists",26)==0){
						//do nothing here if the table is already exists
				}else{
						fprintf(stderr, "SQL error: %s\n", zErrMsg);
				}
				sqlite3_free(zErrMsg);
		} else {
				fprintf(stdout, "Table created successfully\n");
		}
		if(strncmp(argv[1],"-add", 4) == 0){
				add(db,stmt,argv[2]);

		}else if(strncmp(argv[1],"-remove",7) == 0){
				if(strncmp(argv[2],"-all",4) ==0){
						remove_all(db, stmt);
				}else{
						remove_class(db, stmt, argv[2]);
				}
		}else if(strncmp(argv[1],"-start", 6) == 0){
				start( db, stmt, argv[2]);
		}else if(strncmp(argv[1],"-stop", 7) == 0){
				stop( db, stmt,argv[2]);
		}else if(strncmp(argv[1],"-help", 5) == 0){
				help();
		}else if(strncmp(argv[1],"-status", 7) == 0){
				status(db, stmt, argv[2]);
		}else if(strncmp(argv[1],"-report",7) == 0){
				report(db);
		}else{
				printf("Type track -help to get help\n");
		}
		sqlite3_close(db);
		return 0;

}

