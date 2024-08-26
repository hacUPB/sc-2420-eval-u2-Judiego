#include "minimal.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]){
    int num1, num2, potencia;
    printf("escribe la variable1 \n");
    scanf("%i", &num1);
    
    printf("escribe la variable2 \n");
    scanf("%i", &num2)

    //pedir al usuario que ingrese dos numeros y calcule el menor de ellos 
    printf("the min value is: %d\n",minimal(num1,num2));
    //usar la funcion pow() para elevar el pirmer numero al segundo

    return 0;


}