#include <stdio.h>
#include <stdlib.h>
// Step 1: Define structure for vehicles and for RentalTransaction
struct Vehicle {
    int vehicleID; // number of the vehicle
    char make[50]; // the manufacturer
    char model[100]; // model
    int year; // year of production
    float pricePerDay; // price of rent per day
    int isAvailable; // 1 if available, 0 if not
};

struct RentalTransaction {
    int transactionID;
    int vehicleID;
    char customerName[50];
    int rentalDays;
    float totalCost;
};

// Calculating total rent
float calculateTotalCost(float pricePerDay, int rentalDays) {
    return pricePerDay * rentalDays;
}

int main() {
    struct Vehicle vehicles[100];
    struct RentalTransaction transactions[100];
    int vehicleCount = 0;
    int transactionCount = 0;

    // Main menu
    while (1) {
	// options of the user to choose from will run in while loop
        printf("Vehicle Rental Management System\n");
        printf("Which action would you like to do: \n");
        printf("1. Add Vehicle\n");
        printf("2. Rent Vehicle\n");
        printf("3. Return Vehicle\n");
        printf("4. Search for Vehicle by ID\n");
        printf("5. Display Vehicles\n");
        printf("6. Exit\n");

        int choice;
        scanf("%d", &choice); // whatever option the user chooses will be stored in variable choice

        if (choice == 1) {
	    // step 2: adding vehicle
            FILE *inventory;
            inventory = fopen("inventory.txt", "a"); // Use append mode to add vehicles to file
            if (inventory == NULL) {
                printf("Error opening file for writing.\n");
            }

            struct Vehicle add;
            printf("Enter the Vehicle ID: ");
            scanf("%d", &add.vehicleID);
            printf("Enter the make: ");
            scanf("%s", add.make);
            printf("Enter the model: ");
            scanf("%s", add.model);
            printf("Year of Production: ");
            scanf("%d", &add.year);
            printf("Price of rent per day: ");
            scanf("%f", &add.pricePerDay);
            printf("Availability (1 if available, 0 if not): ");
            scanf("%d", &add.isAvailable);

            fprintf(inventory, "%d\n%s\n%s\n%d\n%.2f\n%d\n", add.vehicleID, add.make, add.model, add.year, add.pricePerDay, add.isAvailable);
            fclose(inventory);  // Close the file
        }
	else if (choice == 2) {
		// step 3: Rent a Vehicle
            FILE *inventory = fopen("inventory.txt", "r"); //use read mode to read from the file.
            if (inventory == NULL) {
                printf("Error opening file for reading.\n");
                return EXIT_FAILURE;
            }
            vehicleCount = 0;//setting vehicle count to zero initially.
		//fetching data from file.
            while (fscanf(inventory, "%d\n%s\n%s\n%d\n%f\n%d\n", &vehicles[vehicleCount].vehicleID, vehicles[vehicleCount].make, vehicles[vehicleCount].model, &vehicles[vehicleCount].year, &vehicles[vehicleCount].pricePerDay, &vehicles[vehicleCount].isAvailable) != EOF) {
                vehicleCount++;//incrementing vehicle count after each fetch.
            }
            fclose(inventory);
		// function for renting a vehicle
            void rentVehicle(struct Vehicle vehicles[], int v, struct RentalTransaction transactions[], int *counter) {
                int vehicleID;
		//asking for vehicle ID
                printf("Enter vehicle ID: ");
                scanf("%d", &vehicleID);
		//initially setting index as -1 and will change if the vehicle ID matched with one in the file.
                int index = -1;
		//searching for the match of vehicle ID and will exit loop once found.
                for (int i = 0; i < v; i++) {
                    if (vehicles[i].vehicleID == vehicleID) {
                        index = i;
                        break;
                    }
                }
		//checking if vehicleID is invalid or unavailable for rent.
                if (index == -1){   
                    printf("Invalid ID.\n");
                    return;
                }
		if(vehicles[index].isAvailable == 0){
		    printf("vehicle unavailable for rent.\n");
		}
		//asking for customer details for new transaction.
                transactions[*counter].transactionID = *counter + 1;
                transactions[*counter].vehicleID = vehicleID;
                printf("Enter customer name: ");
                scanf("%s", transactions[*counter].customerName);
                printf("Enter rental days: ");
                scanf("%d", &transactions[*counter].rentalDays);
		//calculating total cost.
                transactions[*counter].totalCost = calculateTotalCost(vehicles[index].pricePerDay, transactions[*counter].rentalDays);
                //changing vehicle's availability to unavailable.
		vehicles[index].isAvailable = 0;
                printf("The cost will be %.2f\n", transactions[*counter].totalCost);
                FILE *RentalTransactions = fopen("Rentaltransactions.txt", "a");//opened the transaction file to append.
                if (RentalTransactions != NULL) {
                    fprintf(RentalTransactions, "Transaction ID:%d\n", transactions[*counter].transactionID);
                    fclose(RentalTransactions);
                }
                (*counter)++;//incrementing the transaction counter
            }

            rentVehicle(vehicles, vehicleCount, transactions, &transactionCount);
        }

	else if(choice == 3) {
		// step 4: return a vehicle
            void returnVehicle(struct Vehicle vehicles[], int v, struct RentalTransaction transactions[], int *counter) {
                int vehicleID;
                printf("Enter vehicle ID: ");
                scanf("%d", &vehicleID);
                int index = -1;
                for (int i = 0; i < v; i++) {
                    if (vehicles[i].vehicleID == vehicleID) {
                        index = i;
                        break;
                    }
                }
                if (index == -1){ 
                    printf("Invalid ID \n");
                    return;
                }
		if( vehicles[index].isAvailable == 1){
		    printf("The vehicle is already available.\n");
		    return;
		}
                vehicles[index].isAvailable = 1;
                printf("Vehicle successfully returned.\n");
            }

            returnVehicle(vehicles, vehicleCount, transactions, &transactionCount);
        }

	else if (choice == 4) {
            // Step 5: Search for vehicle by ID
            FILE *inventory;
            inventory = fopen("inventory.txt", "r");  // open file for read.
            if (inventory == NULL) {
                printf("Error opening file for reading.\n");
                return EXIT_FAILURE;
            }
		//asking for vehicle ID for search.
            int searchV;
            printf("\nEnter the Vehicle ID to search: ");
            scanf("%d", &searchV);

		// A vehicle will be displayed only if it exists.
            struct Vehicle display;
            while (fscanf(inventory, "%d\n%s\n%s\n%d\n%f\n%d\n", &display.vehicleID, display.make, display.model, &display.year, &display.pricePerDay, &display.isAvailable) != EOF) {
                if (display.vehicleID == searchV) {
                    printf("ID: %d, Make: %s, Model: %s, Year: %d, Price per Day: %.2f, Available: %s\n", display.vehicleID, display .make, display.model, display.year, display.pricePerDay, (display.isAvailable == 1 ? "Yes" : "No"));
                    break; // exit loop once the vehicle is found
                }
           else {
		printf("Invalid ID number.");

	}
	}

            fclose(inventory); // close the file

        }

        else if (choice == 5) {
            // Step 6: Display vehicles availability
            FILE *inventory;
            inventory = fopen("inventory.txt", "r"); // open file to read
            if (inventory == NULL) {
                printf("Error opening file for reading.\n");
                return EXIT_FAILURE;
            }

            printf("\nAvailable Vehicles:\n");

            struct Vehicle display; 

            // while loop will go through all vehicles and if availability is = 1, then it will display, otherwise it will skip it
            while (fscanf(inventory, "%d\n%s\n%s\n%d\n%f\n%d\n", &display.vehicleID, display.make, display.model, &display.year, &display.pricePerDay, &display.isAvailable) != EOF) {
                if (display.isAvailable == 1) {
                    printf("ID: %d\nMake: %s\nModel: %s\nYear: %d\nPrice per Day: %.2f\n\n", display.vehicleID, display.make, display.model, display.year, display.pricePerDay);
                }
            }

            fclose(inventory);  // close the file

        }

        else if (choice == 6) {
            printf("Exiting the system. Goodbye!\n");
            break;
        }

        else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
