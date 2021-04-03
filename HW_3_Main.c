/*This program manages a kitchen stock, orders, table orders, and so on.*/
#include "HW_3_Header.h"

int main()
{
	int i, tables_amount, option, temp_qty, table_num, Allocation_Flag = 1;
	char temp_str[51];
	PTable* TableArray;												/*TableArray: pointer to array of pointers to manage the tables*/
	TableDish* tempTable;											/*pointer for freeing memory*/
	Kitchen Kitch;													/*Kitchen list*/
	Dish* tempKitch;
	FILE* manot, *instruction, *output;

	Kitch.manot = NULL;
	Kitch.tail = Kitch.manot;

	manot = fopen("Manot.txt", "r");								/*Open file Manot for reading*/
	if (!manot)
	{
		printf("The file did not open properly!");
		exit(1);
	}

	instruction = fopen("Instructions.txt", "r");					/*Open file instruction for reading*/
	if (!instruction)
	{
		printf("The file did not open properly!");
		exit(1);
	}

	output = fopen("output.txt", "w");								/*Open file output for writting*/
	if (!output)
	{
		printf("The file did not open properly!");
		exit(1);
	}
	

	fscanf(instruction, "%d", &tables_amount);						/*scan the first instruction*/
	TableArray = (PTable*)malloc(sizeof(PTable) * tables_amount);	/*Allocate memory for the amount of tables in the resturant*/
	if (!TableArray)
	{
		printf("The manager memory allocation was not succesful!");
		exit(1);
	}

	for (i = 0; i < tables_amount; i++)								/*Allocate the pointers in TableArray*/
	{
		(*(TableArray + i)) = (PTable)malloc(sizeof(Table));
		if (!(*(TableArray + i)))
		{
			printf("The waiter memory allocation was not succesful!");
			Allocation_Flag = 0;
		}
		(*(*(TableArray + i))).orders = 0;							/*Start the orders value to 0 for each table*/
		(*(*(TableArray + i))).bill = 0;							/*Start the bills value to 0 for each table*/
	}

	while (!feof(instruction) && Allocation_Flag)					/*While we have not reached the end of the file AND AllocationFlag != 0*/
	{
		fscanf(instruction, "%d", &option);
		switch (option)
		{
		case 1:														/*OPTION 1 - SCAN MANOT*/
			Allocation_Flag = CreateProducts(&Kitch, manot, output);
			if (Allocation_Flag)
			{
				fprintf(output, "The kitchen was created");
			}
			break;
		case 2:														/*OPTION 2 - ADD STOCK TO THE KITCHEN*/
			fscanf(instruction, "%s%d",temp_str, &temp_qty);
			Allocation_Flag = AddItems(&Kitch, temp_str, temp_qty, output);
			break;
		case 3:														/*OPTION 3 - ORDER TO A TABLE*/
			fscanf(instruction, "%d%s%d", &table_num, temp_str, &temp_qty);
			Allocation_Flag = OrderItem(&Kitch, TableArray, tables_amount, table_num, temp_str, temp_qty, output);
			break;
		case 4:														/*OPTION 4 - RETURN AN ORDER FROM A TABLE*/
			fscanf(instruction, "%d%s%d", &table_num, temp_str, &temp_qty);
			Allocation_Flag = RemoveItem(&Kitch, TableArray, table_num, temp_str, temp_qty, output);
			break;
		case 5:														/*OPTION 5 - CLOSE A TABLE*/
			fscanf(instruction, "%d", &table_num);
			Allocation_Flag = RemoveTable(&Kitch, TableArray, tables_amount, table_num,  output);
			break;
		}
	}
																	/*CLOSE ALL THE USED FILES*/
	fclose(manot);
	fclose(instruction);
	fclose(output);

	Kitch.tail = NULL;
	while (Kitch.manot)												/*RELEASE THE MEMORY OF THE KITCHEN*/
	{
		tempKitch = Kitch.manot;
		Kitch.manot = Kitch.manot->next;
		free(tempKitch->name);
		free(tempKitch);
	}
	for (i = 0; i < tables_amount; i++)								/*RELEASE THE MEMORY OF EACH TABLE*/
	{

		while ((*(*(TableArray + i))).orders)
		{
			tempTable = ((*(*(TableArray + i))).orders);
			(*(*(TableArray + i))).orders = (*(*(TableArray + i))).orders->next;
			tempTable->next = NULL;
			tempTable->prev = NULL;
			free(tempTable->name);
			free(tempTable);
		}
	}
	return 0;
}