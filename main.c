#include "main.h"
#define TRUE 1 //Definiendo macros para manejar booleanos fácilmente.
#define FALSE 0
// No hagas #include aquí, hazlos en main.h

int main(int argc, char const *argv[])
{
    // Verifica si hay al menos de 3 parametros
    // sino entonces se imprimen las instrucciones
    // y se para la ejecución del programa
    if (argc < 3)
    {
        imprime_instrucciones(); 
    } 
    else {
        FILE* salida= fopen(argv[argc-1], "w"); //Apertura del archivo para escritura; si no existe el archivo, lo crea.
        int indice = 1; //Empezamos a manjar desde argv[1] ya que argv[0] es el nombre del programa.
        char const* nombre_salida = argv[argc-1]; //Vemos cuál es el nombre del archivo de salida, que es el último argumento en la línea de comandos.
        FILE *archivo; //Necesitamos un apuntador para manejar cada archivo que será leído.
        printf("%s\t\t%s\t\t%s\n", "Nombre del archivo", "Palabra más repetida", "Número de repeticiones");
        /*fprintf() manda una cadena con formato a un flujo en específico, en este caso, al archivo de salida.*/
        fprintf(salida, "%s\t\t%s\t\t%s\n", "Nombre del archivo", "Palabra más repetida", "Número de repeticiones");
        for(indice;indice<argc-1;indice++){
            Lista palabras = crea_lista(); 
            char const* nombre = argv[indice];
            archivo = fopen(argv[indice], "a+"); //Abrimos el archivo en turno.
            analiza(palabras, archivo); //Creamos la lista de Elementos de palabras que corresponden al archivo.
            /*Y tanto como en el archivo de salida como en la consola imprimimos la palabra con más apariciones
            en el archivo actual de esta iteración, así como el archivo que corresponde la palabra y el número de 
            apariciones que posee.*/
            printf("     %s\t\t\t\t%s\t\t\t\t%d\n", nombre, (char*)mayor(palabras)->contenido, mayor(palabras)->num_apariciones );
            fprintf(salida, "     %s\t\t\t\t\t\t%s\t\t\t\t\t\t%d\n", nombre, (char*)mayor(palabras)->contenido, mayor(palabras)->num_apariciones );
            fclose(archivo); //No nos olvidamos de cerrar el archivo,
            borra_lista(palabras); //así como de liberar el espacio que fue asignado en el heap al crear la lista de Elementos.
        }
        printf("Se guardó el resultado en el archivo %s\n", nombre_salida); //En consola mencionamos en qué archivo quedó guardada la información.
     }
     return 0; //El estado de salida del programa es satisfactorio.
}

/* ------ Aquí va la implementación de tus funciones ------ */


/* INSTRUCCIONES EN CASO DE ERROR.
* ----------------------------------
* Le notifica al usuario del programa 
* acerca del correcto uso del mismo al 
* pasarle los archivos a ser analizados.
*/
void imprime_instrucciones()
{
	printf("%s\n", "Error: El formato de entrada es incorrecto");
    printf("%s\n", "Uso: nombre_programa <archivo> [<archivo> [<archivo> ...]] <archivo>");
}   

/*
* CREA LISTA DE ELEMENTOS DE PALABRAS.
* ------------------------------------
* Se encarga de reservar el espacio en el heap dinámicamente para una Lista donde almancenaremos 
* los Elementos que son contenedores de Palabras. Regresa un apuntador a la primera posición en 
* memoria de la lista.
*/
Lista crea_lista()
{
    Lista lista = calloc(1, sizeof(Lista)); //calloc() asegura que la posición en memoria va a estar limpia.
    return lista;
}

/* AGREGA PALABRA A LISTA DE ELEMENTOS
* ------------------------------------
* Dada una Palabra, se verifica si ya hay un Elemento que forme parte de 
* la lista que apunta a una palabra cuyo contenido sea el mismo. En caso de ya estar, 
* aumentamos el número de apariciones que corresponden a esta Palabra; en otro caso, 
* se crea un nuevo Elemento, cuyo apuntador a Palabra apunta a la Palabra pasada como parámentro.
*/
void nuevo_elemento(Lista lista, Palabra *palabra)
{
    Elemento *actual = *lista; //Apuntamos a la cabeza de la Lista.
    int ya_esta_palabra = FALSE; //Al inicio decimos que la Palabra no "está" en la Lista.
    while(actual != NULL){
        //Checamos si algún Elemento ya tiene la Palabra con la que estamos comparando.
        if(strcmp(actual->palabra->contenido, palabra->contenido) == 0){
            actual->palabra->num_apariciones++; //Si sí, aumentamos su contador de apariciones.
            ya_esta_palabra = TRUE; //Decimos que la Palabra ya estaba y salimos del ciclo.
            break;
        }
        actual = actual->siguiente; //En otro caso, vamos recorriendo la estructura al apuntar cada vez al siguiente elemento.
    }
    if(!ya_esta_palabra){
        Elemento *nuevo_elemento = malloc(sizeof(Elemento)); //Si nunca coincidió ninguna palabra, reservamos memoria para un nuevo Elemento.
        nuevo_elemento->siguiente = *lista; //Dicho Elemento tendrá como siguiente Elemento a la cabeza de la Lista.
        nuevo_elemento->palabra = palabra; //Le decimos que la Palabra a la que apunta es aquella dada como parámetro a la función.
        *lista = nuevo_elemento; //Y la nueva cabeza de la Lista es éste Elemento que acabamos de agregar.
    }
}

/*
* MANEJA LAS PALABRAS DE UN ARCHIVO.
* -----------------------------------
* Lee las líneas del archivo dado como parámetro, en donde se encarga de tokenizar 
* a partir de ciertoas delimitadores. Para cada token, se crea una nueva Palabra que se 
* agrega a la LIsta de Palabras que también es dada como parámetro. La función nuevo_elemento()
* se encarga de checar si la Palabra es nueva o se debe incrementar su contadore de número de apariciones.
*/
void analiza(Lista palabras, FILE *archivo){
    char carga[501];
    while(fgets(carga, 500, archivo)){ //Se leen lo del renglón (500 carácteres).
        char *token = strtok(carga, " ,.-\n\r"); //Construimos el primer token a partir de los delimitadores.
        while(token != NULL) { //Ahora, mientras el token regresado por strtok() no sea NULO (no haya terminado), seguimos tokenizando.
            Palabra *nueva_palabra = malloc(sizeof(Palabra)); //Reservamos memoria dinámicamente para una nueva Palabra.
            nueva_palabra->num_apariciones = 1; //Ya la palabra apareció una vez.
            char*  respaldo = malloc(strlen(token)+1);
            strcpy(respaldo,token); //Hacemos un respaldo del token y esta copia es justamente el contneido que le damos a la Palabra que habíamos creado.
            nueva_palabra->contenido = respaldo;
            nuevo_elemento(palabras, nueva_palabra); //Y metemos la nueva Palabra a la lista de Palabras.
            token = strtok (NULL, " ,.-\n\r"); //Ahora, seguimos tokenizando desde donde se había quedado.
        }
    }
}

/*
* ENCUENTRA PALABRA CON MÁS APARICIONES.
* -------------------------------------
* Regresa un apuntador a la Palabra con mayor número de apariciones.
*/
Palabra* mayor(Lista lista)
{
    ordena_lista(lista, *cmp_int); //Ordenamos la Lista a partir de una función comparadora que la ordena descendentemente.
    Elemento *cabeza = *lista; //Conseguimos un apuntador a la cabeza de la Lista, que es un Elemento.
    return (cabeza->palabra); //Y devolvemos la Palabra a la que apunta dicho Elemento.
}

/*
* NÚMERO DE ELEMENTOS EN LA LISTA.
* --------------------------------
* Itera la lista para contar el número de 
* elementos que la componen.
*/
size_t longitud(Lista lista)
{
    size_t length = 0; 
    Elemento* current_element = *lista; //Comenzando por la cabeza,
    while(current_element != NULL){  //Iteramos en tanto que haya siguiente Elemento en la Lista.
        current_element = current_element -> siguiente; //El elemento apunta a su siguiente.
        length++; //E incrementamos el contador de la longitud.
    }
    return length; 
}

/*
* FUNCIÓN COMPARADORA PARA ORDENAR LISTA.
* --------------------------------------
* Devuelve un entero positivo si la segunda palabra tiene más apariciones que la primera, un 
* entero negativo en caso contrario; devuelve el valor de cero en caso de que ambas posean 
* el mismo número de apariciones. Esto es porque la lista será ordenada de forma descendente
* en cuanto al número de palabras para tener luego a la Palabra con mayor número de apariciones 
* como la cabeza de la lista.
*/
int cmp_int(const void* a, const void* b)
{
    /*Se convierten los apuntadores a void en apuntadores de Elemento y luego se accede el número de 
    apariciones de la palabra en cada caso, para efectuar la resta.*/
    return ((((Elemento*)b)->palabra->num_apariciones)-(((Elemento*)a)->palabra->num_apariciones));
}

/* 
* QSORT DE LA LISTA DE ELEMENTOS.
* ------------------------------
* Ordena la lista de Elementos de Palabras a partir de una función comparadora. 
* Como en la práctica previa, para poder llamar a qsort(), el contenido de la lista 
* tiene que ser copiado a un arreglo. Luego de ordenado este último, su contenido 
* se vuelve a copiar a la Lista original.
*/
void ordena_lista(Lista lista, int(*cmp)(const void*, const void*))
{
    size_t tam = longitud(lista); //Almacenamos esta variable para saber hasta donde iterar. También para saber qué tan grande será el arreglo.
    Elemento* next= *lista; //Apuntamos a la cabeza de la lista.
    Elemento* arreglo = malloc(sizeof(Elemento) * tam); //Creamos un arreglo de Elementos. Es necesario un arreglo para poder llamar a la función qsort().
    for(int i= 0; i < tam; i++){
        arreglo[i]= *next; //En cada posición del arreglo metemos el Elemento. Como no queremos el apuntador al Elemento, tenemos que redeferenciar.
        next= next -> siguiente; //Y vamos haciendo que apunte cada vez al siguiente Elemento de la Lista.
    }
    qsort(arreglo, tam, sizeof(Elemento), *cmp); /*Mandamos llamar a qsort() con la dirección de la cabeza del arreglo (podemos pasar 
                                                equivalentemente el arreglo), el número de elementos del arreglo (que es el mismo 
                                                que de Elementos en la lista), el tamaño de los saltos que tendrán que efectuarse a partir de
                                                los tipos contenidos en el arreglo, y una función comparadora.*/
    Elemento* next_2= *lista; //Ahora, volvemos a apuntar a la cabeza de la Lista.
    for(int i= 0; i < tam; i++){
        next_2 -> palabra = arreglo[i].palabra;  //Y cada valor de los Elementos del arreglo los ponemos como campo de valor a los apuntadores de Elemento de la Lista.
                                                //En este caso, los "valores" de los Elementos son las Palabras a las que apuntan.
        next_2= next_2 -> siguiente; //Y vamos recorriendo toda la estructura.
    }
}

/*
* LIBERA ESPACIO DE LISTA
* ------------------------
* Se encarga de recuperar la memoria que fue asignada dinámicamente 
* en el heap para no tener esta memoria desperdiciada luego de que
* ya haya sido usada.
*/
void borra_lista(Lista lista)
{
    size_t length = longitud(lista); //Vemos cuántas veces vamos a tener que repetir la instrucción.
    int i;
    for(i = 0; i < length; i++){ //Incrementamos el contador.
        Elemento *curr_removed_element = quita_elemento(lista, 0); /*Quitamos la cabeza de la vista
                                                                    tantas veces como sea necesario
                                                                    hasta que ya no haya elementos en ella.
                                                                    Cada elemento que está siendo removido 
                                                                    lo podemos referenciar con un apuntador a Elemento.*/
        if (curr_removed_element != NULL){ //Ahora, si dicho Elemento es no nulo: 
            free(curr_removed_element->palabra); //Liberamos la memoria que ocupa su valor.
            free(curr_removed_element); //Y el elemento en sí.
        }
    }
    free(lista); //Al final de todo, liberamos la memoria que ocupada la Lista (apuntador a apuntador de Elemento).
}

/*
* ELIMINA NODO DE LISTA.
* -----------------------
* Dado un índice de la Lista, el Elemento en dicha posición 
* es eliminado, cuidando que los apuntadores siguiente de los elementos que
* le precedían e iban después de éste Elemento queden bien después de los cambios.
*/
Elemento *quita_elemento(Lista lista, size_t posicion)
{   
    /*Verificamos que la lista no sea Nula (el apuntador al apuntador de Elemento).
    Tampoco queremos que la posición sea negativa o mayor al número de elementos en la lista
    porque no podríamos acceder a ésta. Es inválido y regresamos NULL.*/
    if(*lista == NULL || posicion < 0 || posicion >= longitud(lista)){
        return NULL;
    }
    /*QUITAR LA CABEZA.*/
    if(posicion == 0){ 
        Elemento* head = *lista; //Apuntamos al primer elemento de la lista: la cabeza.
        Elemento* second_node = head->siguiente; //Apuntamos al segundo elemento.
        *lista = second_node; //Decimos que el primer elemento de la lista es el segundo elemento.
        return head; //Devolvemos a la función el elemento que fue quitado, i.e., la cabeza. 
    }
    /*QUITAR LA COLA.*/
    else if(posicion == longitud(lista)-1){
        Elemento *curr = *lista;
        int i;
        for(i = 0; i<longitud(lista)-2; i++){ //Con un apuntador de elementos recorremos la lista hasta llegar al elemento previo a la cola.
            curr = curr-> siguiente;
        }
        Elemento *temp2 = curr->siguiente; //Apuntamos a la cola.
        Elemento *adevolver = temp2; //Almacenamos una referencia al Elemento que será eliminado.
        curr->siguiente = NULL;  //El siguiente del que previamente era el penúltimo elemento ahora es la cola, por lo que su siguiente Elemento es NULL.
        return adevolver; //Devolvemos la cola, pues fue el Elemento eliminado.
    }
    /*QUITAR UN ELEMENTO ENMEDIO DE LA LISTA (CASO GENERAL).*/
    else{
        Elemento *curr = *lista;
        int i;
        for(i = 0; i<posicion-1; i++){ //Con un apuntador de elementos recorremos la lista hasta llegar al elemento que queremos quitar.
            curr = curr-> siguiente;
        }
        Elemento *temp2 = curr->siguiente; //Apuntamos al que le sigue al elemento actual, que es el que será removido.
        Elemento *adevolver = temp2; //Almacenamos un apuntador al elemento que será quitado.
        curr->siguiente = temp2->siguiente; //El siguiente elemento del anterior al que está siendo quitado apunta al siguiente Elemento del que está siendo quitado.
        return adevolver; //Devolvemos el elemento que fue removido de la Lista.
    }
}