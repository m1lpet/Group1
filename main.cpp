#include "system.h"

#include <locale>



int main() {

    setlocale(LC_ALL, "Russian");

    WorkTimeSystem system;

    system.runMenu();

    return 0;

}

