# Overview
The Vehicle Rental Management System helps a rental company manage its fleet through a simple text-menu interface. Each vehicle is stored with an ID, make, model, year, price per day, and availability status. Renting a vehicle calculates the total cost based on rental days and logs a transaction; returning a vehicle restores its availability.

The project was chosen to practice structures in C - the professor recommended struct types because they let multiple related data fields (ID, make, model, year, price, availability) be grouped and passed around as a single unit, which suited a first C project well

# Tech Stack Used
- Language: C; compiled with GCC
- Data Storage: Plain text files (inventory.txt, Rentaltransactions.txt); no database
- Core Concepts Used: struct, file I/O (fopen/fscanf/fprintf/fclose), arrays, functions, while/for loops, conditionals
- Planning tools used: GanttProject (.gan) for scheduling, and draw.io for flowcharts for logic design
- Environment: Developed/tested on Linux (WSL/terminal) and Windows using gcc

# Features
1. Add Vehicle: Appends a new vehicle record (ID, make, model, year, price/day, availability) to inventory.txt
2. Rent Vehicle: Loads inventory, looks up a vehicle by ID, checks availability, collects customer name and rental days, calculates total cost, marks the vehicle unavailable, and logs the transaction to Rentaltransactions.txt
3. Return Vehicle – Looks up a vehicle by ID and, if currently rented, marks it available again
4. Search Vehicle by ID – Scans the inventory file and prints full details for a matching vehicle
5. Display Vehicles – Lists all currently available vehicles with their details
6. Exit – Cleanly exits the program

* All options are driven from a single numbered menu that loops until the user chooses to exit

# Short Explanation of the Files

ProjectFinal.c → Main C source code for the system\
inventory.txt → Sample vehicle inventory like ID, make, model, year, price/day,  and availability \
Rentaltransactions.txt → Log of completed rental transactions

# System Design
 
The team designed the logic with flowcharts before coding, covering four core flows:
 
- *Add Vehicle* - open file → input details → output confirmation → add another? → close file
- *Rent Vehicle* - open inventory (read) → display available vehicles → search ID → check availability → input customer & rental days → calculate cost → log transaction → close file
- *Return Vehicle* - input vehicle ID → open inventory (read) → search ID → check if currently rented → update availability → close file
- *Search Vehicle* - open inventory → search by ID → display match or error

# Development notes
Implementation went through several rounds of debugging - early attempts to read the inventory file with fgets and fscanf produced type-mismatch and argument errors, and an early attempt to split code into a vehicles.h header file surfaced macro/preprocessor errors like naming the header the same as a variable, #ifndef vehicles.h. These were fixed by correcting format specifiers, matching function signatures, and renaming identifiers. After the fixes, the team tested the program end-to-end by adding vehicles and completing a rental, confirming that adding, renting, and displaying vehicles worked as expected, with the return-vehicle flow left as the main outstanding issue 

# What Could Be Improved
 
- *Fixed-size arrays* Vehicles and transactions are stored in fixed arrays (`vehicles[100]`, `transactions[100]`). Switching to dynamic memory allocation (`malloc`/`calloc`/`realloc`) would remove the hard cap and was already identified by the team as a planned next step
- *Return Vehicle bug* Returning a vehicle relies on the in-memory `vehicles[]` array from `main()`, which is only populated when option 2 (Rent) has been run first, and any changes are never written back to `inventory.txt`. As a result, returns don't reliably persist, and the fleet can appear stuck as "rented" on the next run. Fixing this means having Return and Search/Display, load the file itself, and re-writing the full inventory back to disk after any change
- *Nested functions* `rentVehicle()` and `returnVehicle()` are currently defined inside the `main()` `if` blocks. This compiles under GCC's nested-function extension but is not standard C and won't build with most other compilers. Moving them to top-level functions, ideally in a separate `.c`/`.h` pair, would make the code portable and easier to test
- *Incomplete transaction records* `Rentaltransactions.txt` only logs the transaction ID, not the vehicle ID, customer name, rental days, or total cost - worth expanding for a real audit trail
- *Input validation* `scanf("%s", ...)` for names/models has no bounds checking, risk of buffer overflow, and there's no validation for negative years, prices, or rental days
- *No update/delete vehicle options*, and no persistent transaction history search
- *No unit tests* - testing was manual like adding vehicles, renting one, checking the output files

# How to Run
 

    gcc ProjectFinal.c -o rental_system
    ./rental_system

 
Make sure `inventory.txt` and `Rentaltransactions.txt` are in the same directory as the executable, the program reads from and writes to them directly
