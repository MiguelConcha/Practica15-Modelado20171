#ifndef MAIN_H // Guarda para evitar include circulares
#define MAIN_H

/* ------ Se hacen todos los includes necesarios ------ */
#include <stdio.h> //Archivo de cabecera para imprimir y manejar entrada y salida.
#include <string.h> //strtok() y strlen()
#include <stdlib.h> //free(), qsort(), etc.

/* ------ Definición de tus estructuras ------ */
typedef struct{ //Desde la definición de la estructura asignamos su alias.
	int num_apariciones; /*Los dos campos que definirán a una palabra serán el número de veces
						que aparece en el archivo y su contenido en sí.*/
	char *contenido;
}Palabra; 

struct elemento{
	struct elemento *siguiente; /*Dado que es una estructura recursiva en el sentido de que tiene 
								un struct elemento para apuntar al contenedor
								de Palabra que le sigue en su cuerpo, el alias lo definimos más adelante.*/
	Palabra *palabra; //Cada Elemento almacena una dirección en memoria a una Palabra.
};

typedef struct elemento Elemento; //Definimos un alias para el struct Elemento.
typedef Elemento** Lista; /*Y para una Lista, que como en la práctica pasada, es un doble apuntador a Elemento (la dirección en memoria de la dirección en memoria de la cabeza de la lista)
							para poder estar modificando siempre la misma estructura.


/* ------ Definición de tus funciones ------ */
Lista crea_lista(); //Se pide el espacio para crear una lista de Elementos y se regresa un apuntador a dicha Lista.
void nuevo_elemento(Lista lista, Palabra *palabra); //Agrega una Palabra a una Lista.
void analiza(Lista lista, FILE *archivo); //Lee las palabras de un archivo y las va metiendo en la Lista.
int cmp_int(const void* a, const void* b); //Función comparadora genérica reusada de la práctica previa. 
size_t longitud(Lista lista); //Nos regresa el número de Elementos que componen una Lista.
void imprime_instrucciones(); //Imprime las instrucciones de uso del programa.
void borra_lista(Lista lista); //Reusé la función de la práctica anterior para liberar la memoria que estamos disponiendo con malloc y calloc.
void ordena_lista(Lista lista, int(*cmp)(const void*, const void*)); //Sirve para poder ordenar una lista de Elementos en orden descendente para poder regresar eventualmente la Palabra con más apariciones en cada archivo.
Elemento *quita_elemento(Lista lista, size_t posicion); //Se utiliza dentro de la función borra_lista() y fue reusada de la práctica anterior.
Palabra* mayor(Lista lista); //Esta función regresa la palabra con mayor número de apariciones una vez la LIsta haya sido ordenada.

#endif //MAIN_H
