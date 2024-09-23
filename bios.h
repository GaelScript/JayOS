#ifndef bios_h
#define bios_h

#include <stdio.h>
#include <unistd.h>

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
            break;
        case 2:
            printf("Booting into JS-DOS...\n");
            // Add code to boot into JS-DOS
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