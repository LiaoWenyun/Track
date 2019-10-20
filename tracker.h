#ifndef TRACKER
#define TRACKER

#include<stdio.h>
#include<sqlite3.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>


int is_leap_year(int);
int days_in_month(int, int);
int get_current_time_int();
char* str_datetime(int);
int callback( void *, int , char **, char **);
void add(sqlite3 *, sqlite3_stmt *,char * );
void remove_all(sqlite3 *, sqlite3_stmt *);
void remove_class(sqlite3 *, sqlite3_stmt *,char *);
int start(sqlite3 *, sqlite3_stmt *,char *);
int stop(sqlite3 *, sqlite3_stmt *,char *);
void help();
void status(sqlite3 *, sqlite3_stmt *,char *);
void report();



#endif
