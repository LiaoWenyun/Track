#include<stdlib.h>
#include<stdio.h>
#include<sqlite3.h> 
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include"tracker.h"

//command : 
//track -status
//track -add class_name
//track -remove class_name
//track -remove -all
//track -start class_name
//track -finish class_name
//we use sqlite to store the tracking information 


/*
 * check if the year is a leap year or not 
 * @param year(int) 
 * @return 0 or 1 
 */
int is_leap_year(int year){
	int leap_year;
	if (year%400 == 0){
		leap_year = 1;
	}else if(year%100 == 0){
		leap_year = 0;
	}else if (year%4 == 0){
		leap_year = 1;
	}else{
		leap_year = 0;
	}
	return leap_year;
}


/*
 * calculate the number of days of a given month
 * @param month(int), year(int)
 * @return days(int)
 */
int days_in_month(int month, int year){
	int days;
	switch (month){
		case 1:
			days = 31;
			break;
		case 2:
			if(is_leap_year(year) == 0){
				days = 28;
			}else{
				days = 29;
			}	
			break;
		case 3:
			days = 31;
			break;
		case 4:
			days = 30;
			break;
		case 5:
			days = 31;
			break;
		case 6:
			days = 30;
			break;
		case 7:
			days = 31;
			break;
		case 8:
			days =31;
			break;
		case 9:
			days = 30;
			break;
		case 10:
			days = 31;
			break;
		case 11:
			days = 30;
			break;
		case 12:
	 		days = 31;
			break;
		default:
			days = 30;
                break; 
	}
	return days;
}


/*
 * get the current day
 * @param None
 * @return result
 */
int get_today(){
	int year, month, day;
	time_t now;
	time(&now);
	struct tm *local = localtime(&now);
	day = local ->tm_mday;
	month = local ->tm_mon +1;
	year = local->tm_year+1900;
	int result = year *10000 + month * 100 + day;
	return result;
}


/*
 * get the current date and time ad integer
 * @param None
 * @return time(int)
 */
int get_current_time_int(){	
	int time_int, total_day, year, month, day, hour, min, sec, i;	
	time_t now;
	time(&now);  //get the current time 
	struct tm *local = localtime(&now);
	hour = local->tm_hour;
	min = local->tm_min;
	sec = local->tm_sec;
	day = local->tm_mday;
	month = local->tm_mon + 1;
	year = local->tm_year + 1900;
	if(is_leap_year(year) == 1){
		total_day = 366;
	}else{
		total_day = 365;
	}
	for(i = 1; i < month; i++){
		total_day += days_in_month(i, year); 
	}
	total_day += (day-1);
	time_int = total_day * 24 * 60 * 60 + hour * 60 * 60 + min * 60 + sec;
	return time_int;
}


/*
 * convert integer time into string  
 * @param time(int)
 * @return time(str)
 */
char* str_time(int int_time){
	char s_time[100];
	int day, hour, min, sec, rest_hour, rest_min, rest_sec;
	char* result;
	rest_hour = int_time % (24*60*60);
	day = (int_time - rest_hour) / (24*60*60);       //get the number of days 
	rest_min = rest_hour % (60*60);
	hour = (rest_hour - rest_min) / (60*60);         //get the number of hour
	rest_sec = rest_min % 60;			
	min = (rest_min - rest_sec) / 60;				 //get the number of min
	sec = rest_sec;								     //get the number of second
	if(day == 0 && hour ==0 && min == 0){
			sprintf(s_time, "%d second ",sec);
	}else if(day == 0 && hour == 0 ){
			sprintf(s_time,"%d minute, %d second ", min, sec);
	}else if(day == 0 ){
			sprintf(s_time,"%d hour, %d minute, %d second ", hour, min, sec);
	}else{
			sprintf(s_time, "%d day, %d hour, %d minute, %d second", day, hour, min, sec); 
	}
	result = s_time;
	return result;

}


/*
 * callback function to print out the data rows 
 * @param 
 * @return
 */
int callback( void *data, int numCol, char **colContent, char **colName){
	fprintf(stderr, "%s: \n", (const char*)data);
	int i;
	for(i=0; i<numCol; i++){
		if( i== 0){
			printf(" Class name:          %s  \n", colContent[i]);
		}else if ( i == 1){
			printf(" Total working time:  %s  \n", str_time(atoi((char*)colContent[i])));
		}else if(i == 2 ){
			printf(" Total working days:  %s  \n", colContent[i]);
		}else if(i == 3){
			char date[10];
			strcpy(date, colContent[i]);
			printf(" Adding date:         %c%c%c%c-%c%c-%c%c  \n", date[0],date[1],date[2],date[3],date[4],date[5],date[6],date[7]);
		}
	}
	return 0;
}


/*
 * function to add a class
 * @param database handle, stmt, class name
 * @return none
 */
void add(sqlite3 *db, sqlite3_stmt *stmt,char *class ){
	//check if the course is in the database
	int rc;
	char* class_name = class;
	int total_time = 0;
	int total_days =1;
	int start_date = get_today();
	int current_start_time = 0;
	int status = 0;
	//call function add
	sqlite3_prepare_v2(db, "INSERT INTO TRACK (CLASS_NAME,TOTAL_TIME,TOTAL_DAYS,START_DATE,CURRENT_START_TIME,STATUS) values (?1, ?2, ?3, ?4, ?5, ?6);", -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, class_name, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, total_time);
	sqlite3_bind_int(stmt, 3, total_days);
	sqlite3_bind_int(stmt, 4, start_date);
	sqlite3_bind_int(stmt, 5, current_start_time);
	sqlite3_bind_int(stmt, 6, status);
	rc = sqlite3_step(stmt);
	if( rc != SQLITE_DONE ){
		if(strncmp(sqlite3_errmsg(db),"UNIQUE constraint failed: TRACK.CLASS_NAME",42)==0){
			printf("Error: Class %s exists, Please look up tracker -status",class_name);
		}else if(strncmp(sqlite3_errmsg(db), "NOT NULL constraint failed: TRACK.CLASS_NAME",44 )==0){
			printf("Error: Class can not be NULL\n");
		}else{
			printf("SQL error: %s\n", sqlite3_errmsg(db));
		}
		sqlite3_close(db);
	}else{
		fprintf(stdout, "records created successfully\n");
	}
	sqlite3_finalize(stmt);		
}


/*
 * remove all the classes
 * @param database handle, stmt, class name
 * @return None
 */
void remove_all(sqlite3 *db, sqlite3_stmt *stmt){
	int rc;	
	sqlite3_prepare_v2(db, "DROP TABLE TRACK;", -1, &stmt,NULL);
	rc = sqlite3_step(stmt);
	if( rc != SQLITE_DONE ){
		printf("SQL error: %s\n", sqlite3_errmsg(db));
	}else {
		printf("All classes are deleted from TRACK \n");
	}
}


/*
 * remove a class from the class list 
 * @param database handle, stmt, class name
 * @return None
 */
void remove_class(sqlite3 *db, sqlite3_stmt *stmt,char *class){
	int rc;
	sqlite3_prepare_v2(db, "SELECT * FROM TRACK WHERE CLASS_NAME = ?1;", -1, &stmt,0);
	sqlite3_bind_text(stmt, 1, class, -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW){
	}else{
		printf("Error: Class %s does not exist.\n", class);
		return ;
	}	
	sqlite3_prepare_v2(db, "DELETE FROM TRACK WHERE CLASS_NAME = ?1 ;", -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, class, -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);

	if(rc != SQLITE_DONE ){
		printf("SQL error: %s\n", sqlite3_errmsg(db));
	}else{
		printf("Class %s is deleted from TRACK \n", class);
	}
}


/*
 * Start tracking on a class 
 * @param database handle, stmt, class name
 * @return 0 or 1
 */
int start(sqlite3 *db, sqlite3_stmt *stmt,char *class){
	int result = 0;
	int rc;    
	char * class_name = class;
	int current_start_time;
	int status = 1;
	sqlite3_prepare_v2(db, "SELECT * FROM TRACK WHERE CLASS_NAME = ?1;", -1, &stmt,0);
	sqlite3_bind_text(stmt, 1, class_name, -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW){
		current_start_time = atoi((char*)sqlite3_column_text(stmt,4));
	}else{
		printf("Error: Class %s does not exist.\n", class);
		return 0;
	}
	if (current_start_time == 0){
		current_start_time = get_current_time_int();
	}else{
		printf("Error: Currently tracking the course.\n");
		return 0;
	}
	sqlite3_prepare_v2(db, "SELECT CLASS_NAME FROM TRACK WHERE STATUS = 1;", -1, &stmt, 0);
	rc = sqlite3_step(stmt);
	if (rc ==SQLITE_ROW){
			char *class_to_stop = (char*)sqlite3_column_text(stmt, 0);
			stop(db, stmt, class_to_stop);
	}
	sqlite3_prepare_v2(db, "UPDATE TRACK SET CURRENT_START_TIME= ?1 , STATUS = ?2 WHERE CLASS_NAME = ?3;", -1, &stmt, NULL);
	sqlite3_bind_int(stmt, 1, current_start_time);
	sqlite3_bind_int(stmt, 2, status);
	sqlite3_bind_text(stmt, 3, class_name, -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);
	if(rc != SQLITE_DONE ){
		printf("SQL error: %s\n", sqlite3_errmsg(db));   //need to handle the error if the course is not in the database
	}else{
		printf("Start tracking class %s\n", class);
		result = 1;
	}
	return result;
}


/*
 * Stop tracking on a class 
 * @param database handle, stmt, class name
 * @return 0 or 1 
 */
int stop(sqlite3 *db, sqlite3_stmt *stmt,char *class){
	int result = 0;
	int rc;
	char *class_name = class;
	int total_time ;
	int total_days ;
	int start_date ;
	int current_start_time;
	int status ;
	sqlite3_prepare_v2(db, "SELECT * FROM TRACK WHERE CLASS_NAME = ?1;", -1, &stmt,0);
	sqlite3_bind_text(stmt, 1, class_name, -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW) {
		total_time = atoi((char*)sqlite3_column_text(stmt,1));
		total_days = atoi((char*)sqlite3_column_text(stmt,2));
		start_date = atoi((char*)sqlite3_column_text(stmt,3));
		current_start_time = atoi((char*)sqlite3_column_text(stmt,4));
		status = atoi((char*)sqlite3_column_text(stmt,5));
	}else{
		printf("Error: Class %s does not exist.\n", class_name);
		return 0;
	}
	if ( status == 0){
		printf("Error: The class is not currently tracked.\n");
		return 0;
	}
	//calculate total time 
	int current_time = get_current_time_int();
	total_time = total_time + (current_time - current_start_time);
	//calculate total days 
	if(start_date != get_today()){
		total_days = total_days + 1;
		start_date = get_today();
	}
	//assign the other fileds
	current_start_time = 0;
	status = 0;
	sqlite3_prepare_v2(db, "UPDATE TRACK SET TOTAL_TIME = ?1, TOTAL_DAYS =?2, START_DATE = ?3, CURRENT_START_TIME = ?4, STATUS =?5 WHERE CLASS_NAME = ?6", -1,&stmt, NULL);
	sqlite3_bind_int(stmt, 1, total_time);
	sqlite3_bind_int(stmt, 2, total_days);
	sqlite3_bind_int(stmt, 3, start_date);
	sqlite3_bind_int(stmt, 4, current_start_time);
	sqlite3_bind_int(stmt, 5, status);
	sqlite3_bind_text(stmt, 6, class_name, -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);
	if(rc != SQLITE_DONE ){
		printf("SQL error: %s\n", sqlite3_errmsg(db));
	}else{
		printf("Stop tracking class %s", class_name);
		result = 1;
	}
	return result;
}


/*
 * print out all the commands
 * @param None
 * @return None
 */
void help(){
	printf("-status [CLASS_NAME]        Display the class status.\n");
	printf("-add [CLASS_NAME]           Add a class into the class list.\n");
	printf("-start [CLASS NAME]         Start tracking on a class.\n");
	printf("-stop [CLASS NAME]          Stop tracking on a class.\n");
	printf("-remove [CLASS_NAME]        Remove a class from the class list.\n");
	printf("-remove all                 REmove all the classes.\n");
	printf("-report                     Get the report of the class list\n");
}


/*
 * Show the status of a specific class
 * @param database handle, stmt, class name
 * @return None
 */
void status(sqlite3 *db, sqlite3_stmt *stmt,char *class ){
	int rc;
	char *class_name = class;
	int status;
	sqlite3_prepare_v2(db, "SELECT * FROM TRACK WHERE CLASS_NAME = ?1;", -1, &stmt,0);
	sqlite3_bind_text(stmt, 1, class_name, -1, SQLITE_STATIC);
	rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW){
		status = atoi((char*)sqlite3_column_text(stmt,5));
		if(status == 0){
			printf("Class %s : Not tracking\n",sqlite3_column_text(stmt,0) );
		}else{
			printf("Class %s : Tracking\n",sqlite3_column_text(stmt,0) );
		}
	}else{
		printf("SQL error: %s\n", sqlite3_errmsg(db));
	}
}


/*
 * Print out the report of alll the classes
 * @param database handle
 * @return None
 */
void report(sqlite3 *db){
	int rc;
	char *zErrMsg;
	char *sql;
	sql = "SELECT * from TRACK";
	const char* data = "Class record:";
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

}



