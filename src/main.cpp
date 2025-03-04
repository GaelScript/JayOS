#include "bios.h"
#include "JayNIX.h"
#include "JS-DOS.h"

//USERNAME IS "username" AND PASSWORD IS "password"

int main(){
    srand(static_cast<unsigned int>(time(NULL)));
    const int OS_CHOICE = BIOS_Interface();
    printf("OS_CHOICE: %d\n", OS_CHOICE);

    switch (OS_CHOICE) {
        case 1:
            JayNIX_Interface(1);
            break;
        case 2:
            break;
        case 3:
            JayNIX_Interface(2);
            break;
        default:
            break;
    }
    return 0;
}