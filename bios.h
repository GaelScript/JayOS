#ifndef bios_h
#define bios_h

#include <stdio.h> 
#include <unistd.h> //Added for sleep() function to work
#include <stdbool.h> //Added for boolean values
#include <string.h> //Added for strcmp() function to work

const char username[] = "username";
const char password[] = "password";

//Function for login authentication
bool login(){
    char usernameInput[20];
    char passwordInput[20];

    while (true){
        printf("Login:\n");

        //Get input from user
        printf("Enter your username:\n");
        scanf("%s", usernameInput);

        printf("Enter your password:\n");
        scanf("%s", passwordInput);

        //Check if login information is correct
        if (strcmp(usernameInput, username) == 0 && strcmp(passwordInput, password) == 0){
            printf("Login successful!\n");
            return true;
        } else {
            printf("Wrong username or password. Please try again.\n");
        }
    }
}

int BIOS_Interface(){
    printf("Welcome to JayOS!\n");
    printf("Loading JayOS");
    for (int i = 0; i < 5; ++i) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
    printf("JayOS loaded successfully!\n\n");

    int choice;
    printf("\n");
    printf("-------------------------\n");
    printf("| Select an option:     |\n");
    printf("| 1. Boot into JayNIX   |\n");
    printf("| 2. Boot into JS-DOS   |\n");
    printf("| 3. Shut down          |\n");
    printf("-------------------------\n");
    printf("Enter your choice: ");
    fflush(stdout);
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Booting into JayNIX...\n");
            // Add code to boot into JayNIX
            login();
            break;
        case 2:
            printf("Booting into JS-DOS...\n");
            // Add code to boot into JS-DOS
            login();
            break;
        case 3:
            printf("Shutting down...\n");
            // Add code to shut down the computer
            break;
        default:
            printf("Invalid choice. Please restart and try again.\n");
            break;
    }
    return choice;
}

#endif // bios_h
