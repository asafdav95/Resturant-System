#ifndef _HW_3_Header_h
#define _HW_3_Header_h
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
//----KITCHEN STRUCT BUILD----
typedef struct Dish
{
	char* name;
	float price;
	int qty;
	int order_amount;
	struct Dish* next;
}Dish;

/*Struct Build: Manager for the kitchen dishes*/
typedef struct
{
	Dish* manot;
	Dish* tail;
}Kitchen;


//----TABLE STRUCT BUILD----
typedef struct TableDish
{
	char* name;
	float price;
	int qty;
	struct TableDish* next;
	struct TableDish* prev;
}TableDish;

/*Struct Build: Manager for the tables*/
typedef struct
{
	float bill;
	TableDish* orders;
}Table, *PTable;

int CreateProducts(Kitchen* Kitch, FILE* manot, FILE* output);
int AddItems(Kitchen* Kitch, char* ProductName, int qty, FILE* output);
int OrderItem(Kitchen* Kitch, PTable* TableArray, int TableAmount, int TableNumber, char* ProductName, int qty, FILE* output);
int RemoveItem(Kitchen* Kitch, PTable* TableArray, int TableNumber, char* ProductName, int qty, FILE* output);
int RemoveTable(Kitchen* Kitch, PTable* TableArray, int TableAmount, int TableNumber,  FILE* output);

//Help Functions
int IfProduct(Kitchen* Kitch, char str[]);
#endif