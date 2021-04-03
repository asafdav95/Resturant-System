#include "HW_3_Header.h"


/*
~FUNCTION 1~
Function Name:CreateProducts
Input: Adr to Kitch from main, pointer to MANOT file and output file
Output: Saves the information from manot to the kitchen stock
Explination: This functions builds the kitchens dishes using the MANOT file. The function adds all the detials for each dish.
*/
int CreateProducts(Kitchen* Kitch, FILE* manot, FILE* output)
{
	int temp_qty;
	float temp_price;
	char product[51];
	Dish* temp;
	while (fscanf(manot, "%s%d%f", product, &temp_qty ,&temp_price) != EOF)
	{
		if (temp_price < 0 && temp_qty < 0)													/*If the price or qty is less than 0*/
		{
			fprintf(output, "\nInvalid input! Price and Quantity must be positive.\n");
			return;
		}
		if (IfProduct(Kitch, product))														/*Checks if the dish already exist in the kitchen*/
		{
			fprintf(output, "\nThe product already exist in the kitchen.");
		}
		else
		{
			temp = (Dish*)malloc(sizeof(Dish));
			if (!temp)
			{
				printf("The allocation was not succesful");
				return 0;
			}
			
			temp->name = (char*)malloc(sizeof(char) * (strlen(product) + 1));				/*allocate memory for the name of the product*/
			if (!temp->name)
			{
				printf("The allocation was not succesful");
				return 0;
			}																				/*Add all the detials to the dish build*/
			strcpy(temp->name, product);	
			temp->price = temp_price;
			temp->qty = temp_qty;
			temp->order_amount = 0;
			temp->next = NULL;
			if (!Kitch->manot)
			{
				Kitch->manot = temp;
			}
			else
			{
				Kitch->tail->next = temp;
			}
			Kitch->tail = temp;

		}
	}
	return 1;
}

/*
~FUNCTION 2~
Function Name:AddItems
Input: Adr to Kitch from main, pointer to ProductName, the value of qty, the pointer to file output
Output: Prints to output any error msgs, prints to output if the stock was added to the kitchen succefully, returns 1/0 if the function corretly finished
Explination: This function adds qty to the kitchen stock. The function prints if it succeeded or not.
*/
int AddItems(Kitchen* Kitch, char* ProductName, int qty, FILE* output)
{
	Dish* temp;
	temp = Kitch->manot;
	if (qty < 0)																		/*if the qty is less than 0*/
	{
		fprintf(output, "\nThe quantity must be positive.");
		return;
	}
	while (temp)																		/*while temp != NULL*/
	{
		if (!strcmp(temp->name, ProductName))											/*If match was found go into if*/
		{
			temp->qty += qty;															/*Adjust the qty of the stock of the item*/
			fprintf(output, "\n%d %s were added to the kitchen",qty, ProductName);
			return 1;
		}
		temp = temp->next;
	}
	fprintf(output, "\nThis product does not exist in the kitchen.");					/*If no match was found*/
	return;
}

/*
~FUNCTION 3~
Function Name: OrderItem
Input: Adr to Kitch from main, Adr to TableArray from main, Values: TableAmount, TableNumber: The string ProductName, the value qty, pointer to output file
Output: prints error msgs, updates the orders of a table
Explination: This function updates the orders of a table and the stock of the kitchen
*/
int OrderItem(Kitchen* Kitch, PTable* TableArray, int TableAmount, int TableNumber, char* ProductName, int qty, FILE* output)
{
	Dish* temp;
	TableDish* tempDish,*newDish;
	temp = Kitch->manot;

	if (TableNumber < 0 && TableNumber > TableAmount)								/*If the tablenumber does not exist or if the tablenumber is less than 0*/
	{
		fprintf(output, "\nThe Table number is not valid.");
		return;
	}

	while (temp)																	/*Check if the dish exist in the kitchen*/
	{
		if (!strcmp(temp->name, ProductName))
		{
			break;
		}
		temp = temp->next;
		if (!temp)
		{
			fprintf(output, "\nWe don't have %s, sorry!",ProductName);
			return 1;
		}
	}
	if (qty < 0 || temp->qty < qty)													/*if the qty is less than zero or if the qty is great than the available stock*/
	{
		fprintf(output, "\nInvalid quantity requested.");
		return;
	}
	if ((*(*(TableArray + TableNumber - 1))).orders == 0)							/*if first order of the table*/
	{
		newDish = (TableDish*)malloc(sizeof(TableDish));							/*Allocate memory for a new dish*/
		if (!newDish)
		{
			printf("The allocation for newDish was not succesful!");
			return 0;
		}
		newDish->name = (char*)malloc(sizeof(char) * (strlen(ProductName) + 1));	/*Allocate memory for the name*/
		if (!newDish->name)
		{
			printf("The allocation for newDish name was not succesful!");
			return 0;
		}
		strcpy(newDish->name, ProductName);											/*Add all the datils of the dish*/
		newDish->price = temp->price;
		newDish->qty = qty;
		newDish->next = NULL;
		newDish->prev = NULL;
		(*(*(TableArray + TableNumber - 1))).bill += temp->price * qty;
		temp->qty -= qty;
		temp->order_amount += qty;

		(*(*(TableArray + TableNumber - 1))).orders = newDish;
	}
	else																			/*if orders already exist at this table*/
	{
		tempDish = (*(*(TableArray + TableNumber - 1))).orders;
		while (tempDish)															/*While tempDish is pointing to an order*/
		{
			if (!strcmp(tempDish->name, ProductName))								/*If the specific dish already exist on the table, go into the if*/
			{
				tempDish->qty += qty;
				(*(*(TableArray + TableNumber - 1))).bill += temp->price * qty;
				temp->qty -= qty;
				temp->order_amount += qty;
				break;
			}
			if (!tempDish->next)													/*If the next dish in the orders list is NULL, go into the if*/
			{
				newDish = (TableDish*)malloc(sizeof(TableDish));					/*Allocate memory for a new dish*/
				if (!newDish)
				{
					printf("The allocation for newDish was not succesful!");
					return 0;
				}
				newDish->name = (char*)malloc(sizeof(char) * (strlen(ProductName) + 1));	/*Allocate memory for the name of the new dish*/
				if (!newDish->name)
				{
					printf("The allocation for newDish name was not succesful!");
					return 0;
				}
				strcpy(newDish->name, ProductName);									/*Update the new dish detials*/
				newDish->price = temp->price;
				newDish->qty = qty;
				newDish->next = NULL;
				newDish->prev = tempDish;
				(*(*(TableArray + TableNumber - 1))).bill += temp->price * qty;
				temp->qty -= qty;
				temp->order_amount += qty;

				tempDish->next = newDish;
				break;
			}
			tempDish = tempDish->next;
		}
	}
	fprintf(output, "\n%d %s were added to the table number %d", qty, ProductName, TableNumber);	
	return 1;
}

/*
~FUNCTION 4~
Function Name: RemoveItem
Input: Adr to Kitch from main, Adr to TableArray from main, value of tablenumber, string ProductName, value of the qty, pointer to output file
Output:prints error msgs, updates the orders of the table and the bill
Explination: this functions handles returns from the table and updates the tables orders and bills
*/
int RemoveItem(Kitchen* Kitch, PTable* TableArray, int TableNumber, char* ProductName, int qty, FILE* output)
{
	Dish* temp;
	TableDish* tempDish;
	tempDish = (*(*(TableArray + TableNumber - 1))).orders;
	temp = Kitch->manot;
	if (!tempDish)																		/*If there are no orders*/
	{
		fprintf(output, "\nTable number %d does not have any orders.", TableNumber);
		return;
	}
	while (temp)																		/*while temp is pointing to a dish in the Kitchen*/
	{
		if (!strcmp(temp->name, ProductName))
		{
			break;
		}
		temp = temp->next;
		if (!temp)																		/*If the dish does not exist in the kitchen*/
		{
			fprintf(output, "\nWe don't have %s, sorry!", ProductName);
			return;
		}
	}
	while (tempDish)																	/*While tempDish is pointing to an order in the tables orders*/
	{
		if (!strcmp(tempDish->name, ProductName))										/*If a match was found and the order exist on the table*/
		{
			if (tempDish->qty < qty || qty < 0)											/*Checks for valid quantity*/
			{
				if (qty < 0)
				{
					fprintf(output, "\nQuantity cannot be negetive.");
				}
				else
				{
					fprintf(output, "\nTable number %d has not orderd %d of %s.",TableNumber,qty,ProductName);
				}
				return;
			}
			tempDish->qty -= qty;
			(*(*(TableArray + TableNumber - 1))).bill -= tempDish->price * qty;
			if (tempDish->qty == 0)
			{
				if (tempDish->prev)														/*If there is a prev dish in the list*/
				{
					tempDish->prev->next = tempDish->next;
				}
				if (tempDish->next)														/*If there is a next dish in the list*/
				{
					tempDish->next->prev = tempDish->prev;
				}
				if (!tempDish->next && !tempDish->prev)
				{
					(*(*(TableArray + TableNumber - 1))).orders = 0;
				}
				free(tempDish->name);
				free(tempDish);
				
			}
			fprintf(output, "\n%d %s was returned to the kitchen from table number %d", qty, ProductName,TableNumber);
			return 1;
		}
		tempDish = tempDish->next;
	}
	fprintf(output, "\nTable %d has not orderd %s", TableNumber, ProductName);
	return;
}

/*
~FUNCTION 5~
Function Name: RemoveTable
Input: Adr to Kitch from main, Adr to TableArray, value of TableAmount, value of TableNumber, pointer to output
Output: prints error msgs, updates the tables and checks if the last table was closed
Explination: This function closes a table, prints the bill, checks if it closed the last table, and prints the most orderd dish if so.
*/
int RemoveTable(Kitchen* Kitch, PTable* TableArray, int TableAmount, int TableNumber, FILE* output)
{
	int i, flagPrint = 1, count = 0;
	PTable* tempTable;
	TableDish* tempDish;
	Dish* MaxDish = Kitch->manot, *checkDish = Kitch->manot;

	tempDish = (*(*(TableArray + TableNumber - 1))).orders;
	tempTable = TableArray; 

	if (!tempDish)																	/*If the table has not orders (closed)*/
	{
		fprintf(output, "\nThe table number %d is not ordered yet.", TableNumber);
		return 1;
	}

	while (tempDish)																/*while tempDish is pointing to an order, print the dishes name and the amount orderd*/
	{
		if (flagPrint)
		{
			flagPrint = 0;
			fprintf(output, "\n");
		}
		fprintf(output, "%d %s. ", tempDish->qty, tempDish->name);
																					
		if (tempDish->next)															/*If tempDish.next is NOT null go into if and free the memory of the prev order*/			
		{
			tempDish = tempDish->next;
			free(tempDish->prev->name);
			free(tempDish->prev);
		}
		else																		/*If tempDish.next is NULL , go into else and free the memory of the current order*/		
		{
			free(tempDish->name);
			free(tempDish);
			tempDish = NULL;
		}
	}
	(*(*(tempTable + TableNumber - 1))).orders = 0;
	fprintf(output, "%.2f nis, please!", (*(*(TableArray + TableNumber - 1))).bill);	/*Prints the bill of the table closed*/
	(*(*(TableArray + TableNumber - 1))).bill = 0;
	for (i = 0; i < TableAmount; i++)												/*Loop checks to see how many tables still have orders (are open)*/
	{
		if ((*(*(tempTable + i))).orders != 0)
		{
			count++;
		}
	}
	if (count == 0)																	/*If no tables are open, find and print the MAXDISH (dish orderd the most amount of times)*/
	{
		while (checkDish)
		{
			if (MaxDish->order_amount < checkDish->order_amount)
			{
				MaxDish = checkDish;
			}
			checkDish = checkDish->next;
		}
		fprintf(output, "\nThe most popular dish today is %s! (was orderd %d times)", MaxDish->name, MaxDish->order_amount);
	}
	return 1;
}

/*
~HELPER FUNCTION FOR CreateProducts funtion~
Function Name: IfProduct
Input: Adr to Kitch from main, a string
Output: returns 1/0 to function CreateProducts
Explination: This functions checks if the dish already exist in the kitchen. The function returns1 if the dish already exist and 0 if not
*/
int IfProduct(Kitchen* Kitch, char str[])
{
	Dish* temp;

	temp = Kitch->manot;
	while (temp)
	{
		if (!strcmp(temp->name, str))
		{
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}