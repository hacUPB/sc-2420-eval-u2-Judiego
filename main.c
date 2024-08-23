#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[])
{
    float estatura, peso, IMC;
    char nombre[30];

    printf("ingrese su nombre: \n");
    fgets(nombre, 30, stdin);     //LEE TEXO DE ARCHIVO

    printf("ingrese su peso; \n");
    scanf("%f",&peso);      //LEE DEL TECLADO Y LO CONVIERTE EN FORMATO

    printf("ingrese su estatura: \n");
    scanf("%f" ,&estatura); //%F ES FLOAT

    //imc = peso / estatura
    IMC = peso / pow(estatura, 2);

    printf("&s su imc = %f\n", nombre,IMC);

    return 0;
}

float calcula_imc( float peso, float estatura)
{
    float imc;
    //imc: peso / estatura2
    imc = peso / pow(estatura);
}
