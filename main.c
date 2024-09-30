#include "bios.h"
#include "JayNIX.h"
#include "JS-DOS.h"

//USERNAME IS "username" AND PASSWORD IS "password"

int main(){
    const int OS_CHOICE = BIOS_Interface();
    printf("OS_CHOICE: %d\n", OS_CHOICE);

    switch (OS_CHOICE) {
        case 1:
            JayNIX_Interface();
            break;
        case 2:
            JS_DOS_Interface();
            break;
        case 3:
            break;
        default:
            break;
    }
    return 0;
}