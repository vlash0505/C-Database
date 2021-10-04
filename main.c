#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//prototypes of function
int get_m();
int get_s();
void del_m();
void del_s();
void update_m();
void update_s();
int count_m();
int count_s();
void util_m();
void util_s();
void util_s_m();
void menu_bar();

/**
 * Structure to define master entity.
 */

struct bank_branch {
    int branch_id;
    int capitalization;
    char director_name[256];
    char address[256];

    int is_deleted;
};

/**
 * Structure to define slave entity.
 */

struct bank_account {
    int account_number;
    int connected_branch_id;
    long ITN;
    char full_name[256];
    char account_type[256];
    long balance;

    int is_deleted;
};

/**
 *  Function that explores whether master entity with the given
 *  id exists.
 *
 *  @return 1 if exists, otherwise - 0.
 */

int get_m(int id) {
    //Opening file master.dat that contains all master records.
    FILE* fpM = fopen("E:\\Programming\\DB\\master.dat", "rb");

    //if we have problems opening file, stop the program from running further.
    if(fpM == NULL) {
        printf("Error opening file. \n");
        getchar();
        exit(1);
    }
    struct bank_branch bank_branch_data;

    //traversing file to find record with the given ID
    while (fread(&bank_branch_data, sizeof(struct bank_branch), 1, fpM) == 1) {
        //verifying that record is not deleted.
        if(bank_branch_data.branch_id == id && bank_branch_data.is_deleted == 0) {
            fclose(fpM);
            return 1;
        }
    }
    fclose(fpM);
    printf("\n");
    return 0;
}

/**
 *  Function that explores whether master entity with the given
 *  id exists.
 *
 *  @return 1 if exists, otherwise - 0.
 */
int get_s(int number) {
    //Opening file slave.dat that contains all slave records.
    FILE* fpS = fopen("E:\\Programming\\DB\\slave.dat", "r");

    //if we have problems opening file, stop the program from running further.
    if(fpS == NULL) {
        printf("Error opening file. \n");
        getchar();
        exit(1);
    }

    struct bank_account bank_account_data;

    //traversing file to find record with the given ID
    while (fread(&bank_account_data, sizeof(struct bank_account), 1, fpS) == 1) {
        //verifying that record is not deleted.
        if(bank_account_data.account_number == number && bank_account_data.is_deleted == 0) {
            fclose(fpS);
            return 1;
        }
    }
    fclose(fpS);
    printf("\n");
    return 0;
}

/**
 * Function that logically deletes record in master table
 * by the given id.
 *
 * @param id - id of the master entity to be deleted.
 */

void del_m(int id) {
    //opening file with all master records
    FILE *fpM = fopen("E:\\Programming\\DB\\master.dat", "rb+");
    //opening file with all slave entities to deleted ones that are connected
    //to the given master entity.
    FILE *fpS = fopen("E:\\Programming\\DB\\slave.dat", "rb+");

    //if we have problems opening file, stop the program from running further.
    if (fpM == NULL || fpS == NULL) {
        printf("File not found.");
        getchar();
        exit(1);
    }

    struct bank_branch bank_branch_data;
    struct bank_account bank_account_data;

    bank_branch_data.branch_id = id;
    //marking entity as logically deleted
    bank_branch_data.is_deleted = 1;

    fseek(fpM, (id - 1) * sizeof(struct bank_branch), SEEK_SET);
    fwrite(&bank_branch_data, sizeof(struct bank_branch),1, fpM);
    fclose(fpM);

    //deleting slaves connected to the branch.
    while (fread(&bank_account_data, sizeof(struct bank_account), 1, fpS) == 1) {
        //verifying that record is not already deleted.
        if(bank_account_data.connected_branch_id == id && bank_account_data.is_deleted == 0) {
            bank_account_data.is_deleted = 1;
            fseek(fpS, (bank_account_data.account_number - 1) * sizeof(struct bank_account), SEEK_SET);
            fwrite(&bank_account_data, sizeof(struct bank_account),1, fpS);
        }
        //avoiding infinite loop
        fseek(fpS, (bank_account_data.account_number) * sizeof(struct bank_account), SEEK_SET);
    }

    fclose(fpS);
    printf("\n");
}

/**
 * Function that logically deletes record in slave table
 * by the given id(in our case bank account's number).
 *
 * @param id - id of the slave entity to be deleted.
 */

void del_s(int number) {
    FILE* fpS = fopen("E:\\Programming\\DB\\slave.dat", "rb+");

    //if we have problems opening file, stop the program from running further.
    if (fpS == NULL) {
        printf("File not found.");
        getchar();
        exit(1);
    }
    //structure to store information to be inserted to the table
    struct bank_account bank_account_data;
    bank_account_data.account_number = number;

    //marking entity as logically deleted
    bank_account_data.is_deleted = 1;

    fseek(fpS, (number - 1) * sizeof(struct bank_account), SEEK_SET);
    fwrite(&bank_account_data, sizeof(struct bank_account),1, fpS);
    fclose(fpS);
}

/**
 * Function to modify master entity.
 */

void update_m(int id) {
    FILE* fpM = fopen("E:\\Programming\\DB\\master.dat", "rb+");

    //if we have problems opening file, stop the program from running further.
    if (fpM == NULL) {
        printf("File not found.");
        getchar();
        exit(1);
    }

    struct bank_branch bank_branch_data;
    bank_branch_data.branch_id = id;

    //getting information from the user
    printf("Enter the capitalization of the branch: ");
    scanf(" %d", &bank_branch_data.capitalization);
    printf("Enter director's name: ");
    scanf(" %[^\n]", bank_branch_data.director_name);
    printf("%s", "Enter branch's address: ");
    scanf(" %[^\n]", bank_branch_data.address);
    bank_branch_data.is_deleted = 0;

    fseek(fpM, (id - 1) * sizeof(struct bank_branch), SEEK_SET);
    fwrite(&bank_branch_data, sizeof(struct bank_branch),1, fpM);
    fclose(fpM);
}

/**
 * Function to modify slave entity.
 */

void update_s(int number) {
    FILE* fpS = fopen("E:\\Programming\\DB\\slave.dat", "rb+");

    //if we have problems opening file, stop the program from running further.
    if (fpS == NULL) {
        printf("File not found.");
        getchar();
        exit(1);
    }
    //structure to store information to be inserted to the table
    struct bank_account bank_account_data;
    bank_account_data.account_number = number;

    //get data from the user.
    printf("%s", "Enter ITN connected to the account: ");
    scanf(" %ld", &bank_account_data.ITN);
    printf("%s", "Enter bank branch to which you want to connect account: ");
    scanf(" %d", &bank_account_data.connected_branch_id);
    if(get_m(bank_account_data.connected_branch_id) == 0) {
        printf("No  such branch found\n");
        fclose(fpS);
        return;
    }
    printf("%s", "Enter customer's full name(assigned to bank account): ");
    scanf(" %[^\n]", bank_account_data.full_name);
    printf("%s", "Enter account's type: ");
    scanf(" %s", bank_account_data.account_type);
    printf("%s", "Enter account's balance: ");
    scanf(" %ld", &bank_account_data.balance);
    bank_account_data.is_deleted = 0;

    fseek(fpS, (number - 1) * sizeof(struct bank_account), SEEK_SET);
    fwrite(&bank_account_data, sizeof(struct bank_account),1, fpS);
    fclose(fpS);
}

/**
 * Function that counts number of records in a file
 * with master entities.
 *
 * @return  number of records in the master file.
 */

int count_m() {
    //opening file master.dat with all master entities
    FILE *fpM;
    fpM = fopen("E:\\Programming\\DB\\master.dat","rb");

    //if we have problems opening file, stop the program from running further.
    if(fpM == NULL) {
        printf("Error opening file. \n");
        getchar();
        exit(1);
    }
    int num = 0;
    struct bank_branch traverse;

    //counting number of records while traversing the file.
    while (fread(&traverse, sizeof(struct bank_branch), 1, fpM) == 1) {
        num++;
    }
    fclose(fpM);
    return num;
}

/**
 * Function that counts number of records in a file
 * with slave entities.
 *
 * @return  number of records in the slave file.
 */

int count_s() {
    //opening file slave.dat with all slave entities
    FILE *fpS;
    fpS = fopen("E:\\Programming\\DB\\slave.dat","rb");

    //if we have problems opening file, stop the program from running further.
    if(fpS == NULL) {
        printf("Error opening file. \n");
        getchar();
        exit(1);
    }
    int num = 0;
    struct bank_account traverse;

    //counting number of records while traversing the file.
    while (fread(&traverse, sizeof(struct bank_account), 1, fpS) == 1) {
        num++;
    }
    fclose(fpS);
    return num;
}

/**
 * Function to add master entity to the file.
 */

void insert_m() {
    //AUTOINCREMENT (automatically give a record it's ID)
    int id = (count_m() + 1);
    //opening file master.dat with all master entities
    FILE *fpM;
    fpM = fopen("E:\\Programming\\DB\\master.dat", "rb+");

    //if we have problems opening file, stop the program from running further.
    if(fpM == NULL) {
        printf("Error opening file. \n");
        getchar();
        exit(-1);
    }
    struct bank_branch bank_branch_data;

    //if we have deleted elements, take ID of the firs one and rewrite it.
    while (fread(&bank_branch_data, sizeof(struct bank_branch), 1, fpM) == 1) {
        if(bank_branch_data.is_deleted == 1) {
            id = bank_branch_data.branch_id;
            break;
        }
    }
    bank_branch_data.branch_id = id;
    printf("ID of the new branch: %d \n", id);

    //getting data from the user
    printf("Enter the capitalization of the branch: ");
    scanf(" %d", &bank_branch_data.capitalization);
    printf("Enter director's name: ");
    scanf(" %[^\n]", bank_branch_data.director_name);
    printf("%s", "Enter branch's address: ");
    scanf(" %[^\n]", bank_branch_data.address);
    bank_branch_data.is_deleted = 0;
    //searching position to insert new record.
    fseek(fpM, (id - 1) * sizeof(struct bank_branch), SEEK_SET);
    fwrite(&bank_branch_data, sizeof(struct bank_branch),1, fpM);
    fclose(fpM);
    printf("\n");
}

/**
 * Function to add a slave entity.
 *
 * If the master entity to which we assign the
 * slave entity to does not exist, we are not
 * inserting the slave entity.
 */

void insert_s() {
    //AUTOINCREMENT (automatically give a record it's ID)
    int id = (count_s() + 1);
    /**
     * opening file master.dat with all master entities to verify
     * that there is a master entity with given by the user ID.
     *
     * opening file slave.dat with all slave entities.
    **/
    FILE *fpS, *fpM;
    fpS = fopen("E:\\Programming\\DB\\slave.dat", "rb+");
    fpM = fopen("E:\\Programming\\DB\\master.dat", "rb");

    //if we have deleted elements, rewrite first of them with new data.
    if(fpS == NULL) {
        printf("Error opening file. \n");
        getchar();
        exit(-1);
    }

    struct bank_account bank_account_data;
    struct bank_branch traverse;
    int has_branch = 0;

    //if we have deleted elements, take ID of the firs one and rewrite it.
    while (fread(&bank_account_data, sizeof(struct bank_account), 1, fpS) == 1) {
        if(bank_account_data.is_deleted == 1) {
            id = bank_account_data.account_number;
            break;
        }
    }
    bank_account_data.account_number = id;
    printf("Number of the new account: %d \n", id);

    //get data from the user.
    printf("%s", "Enter ITN connected to the account: ");
    scanf(" %ld", &bank_account_data.ITN);
    printf("%s", "Enter branch to which account is connected: ");
    scanf(" %d", &bank_account_data.connected_branch_id);
    //check if the master entity with the given by the user ID exists.
    while (fread(&traverse, sizeof(struct bank_branch), 1, fpM) == 1) {
        //verify the master's record is not deleted also.
        if (traverse.branch_id == bank_account_data.connected_branch_id &&
            traverse.is_deleted == 0) {
            printf("%s", "Enter customer's full name(assigned to bank account): ");
            scanf(" %[^\n]", bank_account_data.full_name);
            printf("%s", "Enter account's type: ");
            scanf(" %s", bank_account_data.account_type);
            printf("%s", "Enter account's balance: ");
            scanf(" %ld", &bank_account_data.balance);
            bank_account_data.is_deleted = 0;
            has_branch = 1;
            break;
        }
    }
    if(has_branch == 0) {
        printf("No such branch found. \n");
        fclose(fpM);
        fclose(fpS);
        return;
    }

    //seek for the position to insert new record
    fseek(fpS, (id - 1) * sizeof(struct bank_account), SEEK_SET);
    fwrite(&bank_account_data, sizeof(struct bank_account),1, fpS);
    fclose(fpS);
    fclose(fpM);
    printf("\n");
}

/**
 * Function to display all master entities.
 */

void util_m() {
    //opening file master.dat with all master entities
    FILE *fpM = fopen ("E:\\Programming\\DB\\master.dat","rb");
    struct bank_branch show_master;

    //if we have deleted elements, rewrite first of them with new data.
    if (fpM == NULL) {
        printf("Error opening file. \n");
        getchar();
        exit (1);
    }

    printf("\t \t \t \t \t | All master entities: | \n \n");
    printf("%20s | %20s | %20s | %20s |\n", "Branch ID", "Capital", "Director's Name", "Address");
    printf("__________________________________________________________________________________");
    printf("______________________\n");

    //traversing all file and outputting each master entity that is not deleted.
    while (fread(&show_master, sizeof(struct bank_branch), 1, fpM) == 1) {
        //verifying the record is not deleted.
        if(show_master.is_deleted == 0) {
            printf("%20d | %20d | %20s | %20s | \n", show_master.branch_id, show_master.capitalization,
                   show_master.director_name, show_master.address);
        }
    }
    printf("\n");
    fclose(fpM);
}

/**
 * Function to display all slave entities.
 */

void util_s() {
    //opening file slave.dat with all slave entities
    FILE *fpS = fopen ("E:\\Programming\\DB\\slave.dat","rb");

    //if we have deleted elements, rewrite first of them with new data.
    if (fpS == NULL) {
        printf("Error opening file. \n");
        getchar();
        exit (1);
    }
    struct bank_account show_slave;

    printf("\t \t \t \t \t | All slave entities: | \n \n");
    printf("%15s | %15s | %15s | %15s | %15s | %15s |\n", "Account Number", "ITN", "Connected branch",
           "Full Name", "Account type", "Balance");
    printf("__________________________________________________________________________________");
    printf("___________________________\n");

    //traversing all file and outputting each slave entity that is not deleted.
    while (fread(&show_slave, sizeof(struct bank_account), 1, fpS) == 1) {
        //verify the record is not deleted.
        if(show_slave.is_deleted == 0) {
            printf("%15d | %15ld | %15d | %15s | %15s | %15ld |\n", show_slave.account_number, show_slave.ITN,
                   show_slave.connected_branch_id, show_slave.full_name, show_slave.account_type,
                   show_slave.balance);
        }
    }
    printf("\n");
    fclose(fpS);
}

/**
 * Function to display all slaves related to the master's
 * ID which we require from user.
 */

void util_s_m() {
    //opening file master.dat with all master entities
    //opening file slave.dat with all slave entities
    FILE *fpS = fopen("E:\\Programming\\DB\\slave.dat", "rb");

    //if we have deleted elements, rewrite first of them with new data.
    if(fpS == NULL) {
        printf("Error opening file. \n");
        getchar();
        exit(1);
    }
    struct bank_account bank_account_data;

    int id;
    printf("Enter ID of the branch to see accounts attached to it: ");
    scanf("%d", &id);

    printf("\t \t \t \t \t | Slave entities connected to the given id: | \n \n");
    printf("%15s | %15s | %15s | %15s | %15s | %15s |\n", "Account Number", "ITN", "Connected branch",
           "Full Name", "Account type", "Balance");
    printf("__________________________________________________________________________________");
    printf("______________________________\n");

    //traversing the file with slave entities and displaying
    //all the instances connected to the given master's id
    while (fread(&bank_account_data, sizeof(struct bank_account), 1, fpS) == 1) {
        //verifying the record is not deleted.
        if (bank_account_data.connected_branch_id == id && bank_account_data.is_deleted == 0) {
            printf("%15d | %15ld | %15d | %15s | %15s | %15ld |\n", bank_account_data.account_number,
                   bank_account_data.ITN, bank_account_data.connected_branch_id, bank_account_data.full_name,
                   bank_account_data.account_type, bank_account_data.balance);
        }
    }

    fclose(fpS);
    printf("\n");
}

/**
 * Function to reset the file system.
 * Clears all the files.
 */

void reset() {
    fclose(fopen("E:\\Programming\\DB\\master.dat", "wb"));
    fclose(fopen("E:\\Programming\\DB\\slave.dat", "wb"));
    fclose(fopen("E:\\Programming\\DB\\master_index.dat", "wb"));
}

/**
 * Function to display user interface(menu) to interact
 * with the file system.
 */

void menu_bar() {
    char command[100] = "";
    printf("\n \t \t \t | Menu: | \n \n");
    printf("insert-m : insert bank branch to the database. \n");
    printf("insert-s : insert bank account to the database. \n");
    printf("util-m : display all bank branches in a database. \n");
    printf("util-s : display all bank accounts in a database. \n");
    printf("util-s-m : display all bank accounts connected to the specified branch.\n");
    //printf("get-m : get information on specified bank branch. \n");
    //printf("get-s : get information on specified bank account. \n");
    printf("update-m : modify information in a specific bank branch.\n");
    printf("update-s : modify information in a specific bank account.\n");
    printf("delete-m : delete a record in a master table. \n");
    printf("delete-s : delete a record in a slave table. \n");
    printf("reset-all : delete all records in all tables. \n");
    printf("exit : exit the program.\n \n");
    while(1) {
        printf("Your command > ");
        scanf("%s", command);
        if(strcmp(command, "exit") == 0) break;
        else if(strcmp(command, "util-s-m") == 0) util_s_m();
        else if(strcmp(command, "util-m") == 0) util_m();
        //else if(strcmp(command, "get-m") == 0)  get_m();
        else if(strcmp(command, "insert-m") == 0) insert_m();
        else if(strcmp(command, "util-s") == 0) util_s();
            //else if(strcmp(command, "get-s") == 0) get_s();
        else if(strcmp(command, "insert-s") == 0) insert_s();
        else if(strcmp(command, "update-m") == 0) {
            int id_u;
            printf("Enter the ID of the branch you want to modify:");
            scanf("%d", &id_u);
            //validating the user input
            if(get_m(id_u) == 0) {
                printf("No such branch found \n");
            } else {
                update_m(id_u);
            }
        }
        else if(strcmp(command, "delete-m") == 0){
            int id_d;
            printf("Enter the ID of the branch you want to delete:");
            scanf("%d", &id_d);
            //validating the user input
            if(get_m(id_d) == 0) {
                printf("No such branch found \n");
            } else {
                del_m(id_d);
            }
        }
        else if(strcmp(command, "update-s") == 0) {
            int number_u;
            printf("Enter the number of the account you want to modify:");
            scanf("%d", &number_u);
            //validating the user input
            if(get_s(number_u) == 0) {
                printf("No such account found.\n");
            } else {
                update_s(number_u);
            }
        }
        else if(strcmp(command, "delete-s") == 0) {
            int number_d;
            printf("Enter the number of the account you want to delete:");
            scanf("%d", &number_d);
            //validating the user input
            if(get_s(number_d) == 0) {
                printf("No such account found.\n");
            } else {
                del_s(number_d);
            }
        }
        else if(strcmp(command, "reset-all") == 0) reset();
        else printf("Wrong command !\n");
    }
}

/**
 * Driver function to run our code,
 * starting point of the program.
 */

int main() {
    printf("Program started successfully! \n \n");
    //initialising menu bar to allow user interact with the
    //file system.
    menu_bar();
    return 0;
}