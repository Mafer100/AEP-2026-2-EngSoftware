#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef DEBUGMSG
#define DEBUGMSG false
#endif

void iniciarMenu(){
    if(DEBUGMSG == true){
        printf("[DEBUGMSG]: Iniciando tela de Menu!\n");
    }

    return;
}