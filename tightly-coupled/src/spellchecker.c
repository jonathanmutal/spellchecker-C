#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//tamaño maximo de una palabra
#define MAX_WORD_SIZE 30

/* diccionario principal */
char **dict_main;
/*tamaño inicial del diccionario principal*/
int main_size = 10;

/* diccionario de palabras ignoradas */
char **dict_ignored;
/*tamaño inicial del diccionario de palabras ignoradas*/
int ignored_size = 0;

/* Documento de entrada */
FILE *doc_in;
/* Documento de salida */
FILE *doc_out;

/*******************************************************************
* NAME :            void dict_load(char *fname)
*
* DESCRIPTION :     Carga en memoria el diccionario principal desde 
*                   el archivo fname
*
* PARAMETERS:
*      INPUT:
*           char    *fname       Nombre del archivo que contiene el 
*                                diccionario
* RETURN :
*           Type: void
*
* OBSERVATIONS :
*    1) El tamaño de la lista que contiene representa al diccionario 
*       debe coincidir con el nro de palabras en el diccionario. 
*       Esto implica que si es necesario hay que "agrandar" la lista, 
*       y si sobra espacio en la lista hay que "achicarla".
*    2) Recordar borrar el \n y \r al final de cada palabra y que los 
*       strings en C terminan en \0.
*******************************************************************/
void dict_load(char *fname) {
    int pos = 0;
    FILE *fd = NULL;
    char word[MAX_WORD_SIZE];

    dict_main = calloc(main_size, sizeof(char *));

    if(!(fd = fopen(fname, "r")))
      printf("ERROR AL ABRIR ARCHIVO %s\n", fname);

    while (fscanf(fd, "%s" , word) > 0) {
        if (main_size <= pos) {
            main_size += 10;
            dict_main = realloc(dict_main, main_size*sizeof(char *));
        }
        dict_main[pos] = calloc(MAX_WORD_SIZE, sizeof(char));
        strcpy(dict_main[pos], word);
        pos++;
    }
    if (pos < main_size) {
        main_size = pos;
        dict_main = realloc(dict_main, main_size*sizeof(char *));
    }
    fclose(fd);
}

/*******************************************************************
* NAME :            void dict_save(char *fname)
*
* DESCRIPTION :     Guarda el diccionario principal en el archivo 
*                   fname
*
* PARAMETERS:
*      INPUT:
*           char    *fname       Nombre del archivo donde se guardara
*                                el diccionario
* RETURN :
*           Type: void
*******************************************************************/
void dict_save(char *fname){
    int position = 0;
    FILE *fd = NULL;
    fd = fopen(fname, "w");

    for(position = 0; position < main_size; position++){
        fprintf(fd,"%s\n",dict_main[position]);
    }

    fclose(fd);
}

/*******************************************************************
* NAME :            void dict_add(char *word)
*
* DESCRIPTION :     Agrega una palabra al diccionario principal.
*
* PARAMETERS:
*      INPUT:
*           char    *word       Palabra a ser agregada.
*
* RETURN :
*           Type: void
*
* OBSERVATIONS :
*    1) Recordar que la lista que representa el diccionario no tiene 
*       lugar "de sobra".
*******************************************************************/
void dict_add(char *word){

    main_size += 1;
    dict_main = realloc(dict_main, main_size*sizeof(char *));
    dict_main[main_size-1] = calloc(MAX_WORD_SIZE, sizeof(char));
    strcpy(dict_main[main_size-1], word);
}

/*******************************************************************
* NAME :            void ignored_add(char *word)
*
* DESCRIPTION :     Agrega una palabra al diccionario de palabras 
*                   ignoradas.
*
* PARAMETERS:
*      INPUT:
*           char    *word       Palabra a ser agregada.

* RETURN :
*           Type: void
* OBSERVATIONS :
*    1) La lista que representa el diccionario de palabras ignoradas 
*       debe agrandarse a medida que se agregan palabras.
*******************************************************************/
void ignored_add(char *word){
 
    ignored_size += 1;
    dict_ignored = realloc(dict_ignored, ignored_size*sizeof(char *));     
    dict_ignored[ignored_size - 1] = calloc(MAX_WORD_SIZE, sizeof(char));
    strcpy(dict_ignored[ignored_size - 1], word);
}

/*******************************************************************
* NAME :            int is_known(char *word)
*
* DESCRIPTION :     Verifica si una palabra es "conocida", ya sea 
*                   porque esta en el diccionario principal o en el 
*                   diccionario de palabras ignoradas. 
*
* PARAMETERS:
*      INPUT:
*           char    *word       Palabra a verificar.
*
* RETURN :
*           Type: int
*           Values: 1 si la palabra es conocida
*                   0 si la palabra no es conocida
*******************************************************************/
int is_known(char *word){
    int known = 0;

    for( int position = 0; position < ignored_size; position++){
        if(strcmp(word, dict_ignored[position]) == 0){
            known = 1;
            break;
        }
    }
    if(!known){
      for(int position = 0; position < main_size; position++){
        if(strcmp(word, dict_main[position]) == 0){
            known = 1;
            break;
            }
        }
    }
    return known;
} 


/*******************************************************************
* NAME :            int get_word(char *w)
*
* DESCRIPTION :     Lee una palabra del archivo de entrada, copiando 
*                   todo caracter de puntuacion precedente al archivo
*                   de salida.
* PARAMETERS:
*      OUTPUT:
*           char    *word       Palabra que se lee desde el archivo.
*
* RETURN :
*           Type: int
*           Values: 0 si no hay mas palabras para leer.  
*                   1 si hay mas palabras para leer.
*******************************************************************/
int get_word(char *word){
    int c;
    int r = 1;

    while(fscanf(doc_in, "%[a-z|A-Z]", word) == 0 && !feof(doc_in)){
        c = fgetc(doc_in);
        fputc(c, doc_out);
        printf("%c \n", c);
    }
    if(feof(doc_in)) {
        r = 0;
    }
    return r;
}

/*******************************************************************
* NAME :            void put_word(char *word)
*
* DESCRIPTION :     Escribe la palabra w al archivo de salida.
*
* PARAMETERS:
*      INPUT:
*           char    *word       Palabra a escribir.
*
* RETURN :
*           Type: void
*******************************************************************/
void put_word(char *word){
     fputs(word, doc_out);
 }

/*******************************************************************
* NAME :            void consult_user(char *word)
*
* DESCRIPTION :     Consulta al usuario sobre que accion realizar 
*                   (aceptar, ignorar o reemplazar) con la palabra w.
*                   Una vez que el usuario elige, realiza la accion 
*                   elegida.
*
* PARAMETERS:
*      INPUT:
*           char    *word       Palabra sobre la cual se consulta la 
*                            accion a realizar.
*
* RETURN :
*           Type: void
*******************************************************************/
void consult_user(char *word){
  char ans[2];
  do{
    printf("Palabra no reconocida: %s\n Aceptar (a) - Ignorar (i) - Reemplazar (r): ", word);
    scanf("%s", ans);
  }while((strcmp(ans,"r") != 0) && (strcmp(ans,"a") != 0) && (strcmp(ans,"i") != 0));
  switch(ans[0]){
    case 'a':
        dict_add(word);
        put_word(word);
        break;
    case 'i':
        ignored_add(word);
        put_word(word);
        break;
    case 'r':
        printf("Ingrese una palabra\n");
        scanf("%s", word);
        put_word(word);
   }

}
/*******************************************************************
* NAME :            void process_document(char *fname)
*
* DESCRIPTION :     Procesa el documento fname, palabra por palabra, 
*                   consultando al usuario sobre la accion a realizar 
*                   si la palabra no es conocida.
* PARAMETERS:
*      INPUT:
*           char    *fname   Nombre del archivo a procesar.
*
* RETURN :
*           Type: void
*******************************************************************/
void process_document(char *fname){
    char current_word[MAX_WORD_SIZE];

    doc_in = fopen(fname,"r");
    doc_out = fopen("out.txt","w");

    while(get_word(current_word)){

        if(is_known(current_word)){
            put_word(current_word);
        } else {
            consult_user(current_word);
        }
    }
    fclose(doc_in);
    fclose(doc_out);

 }

/*******************************************************************
* NAME :            int main(int argc, char **argv)
*
* DESCRIPTION :     Punto de entrada principal. Abre el diccionario 
*                   principal, procesa el archivo especificado y 
*                   guarda los cambios realizados en el diccionario 
*                   principal.
*******************************************************************/
int main(int argc, char **argv){
   char *dict;
   char *text;
   /* Verificamos el nro de argumentos. */
   if (argc < 2)
     {
       printf("spellchecker.c: nro de argumentos erroneo. Deben ser <documento> [<diccionario>].\n");
       return (1);
     }
   /* si se especifico un diccionario lo usamos,  */
   /* caso contrario usamos el diccionario por defecto */
   dict = (argc >=3) ? argv[2] : "dict.txt";
   text = argv[1];

   dict_load(dict);
   process_document(text);

   dict_save(dict);

   for(int i = 0; i < main_size; i++){
       free(dict_main[i]);
   }
   free(dict_main);

   for(int i = 0; i < ignored_size; i++){
       free(dict_ignored[i]);
   }
   free(dict_ignored);

   printf("El documento %s ha sido procesado. Resultados en out.txt\n", argv[1]);

   return 0;
}

