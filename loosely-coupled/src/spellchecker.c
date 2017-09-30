#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
#include "document.h"

/* tamaño maximo de una palabra */
#define MAX_WORD_SIZE 30

/* diccionario principal */
Dictionary main_dict;
/* diccionario de palabras ignoradas */
Dictionary ignored;
/* Documento a procesar */
Document doc;


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

    if(dict_contains(main_dict, word) || dict_contains(ignored, word)){
        known = 1;
    }

    return known;
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
      main_dict = dict_add(main_dict, word);
      doc_put_word(doc, word);
      break;
    case 'i':
      ignored = dict_add(ignored, word);
      doc_put_word(doc, word);
      break;
    case 'r':
      printf("Please, enter the word:\n");
      scanf("%s", word);
      doc_put_word(doc, word);
      break;
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

  doc = doc_open(fname, "out.txt");
  while(doc_get_word(doc, current_word)){

      if(is_known(current_word)){
        doc_put_word(doc, current_word);
      } else {
        consult_user(current_word);
      }
  }

  doc_close(doc);
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
   char *document;
   /* Verificamos el nro de argumentos. */
   if (argc < 2)
     {
       printf("spellchecker.c: nro de argumentos erroneo. Deben ser <documento> [<diccionario>].\n");
       return (1);
     }
   /* si se especifico un diccionario lo usamos,  */
   /* caso contrario usamos el diccionario por defecto */
   dict = (argc >=3) ? argv[2] : "dict.txt";
   document = argv[1];
  main_dict = dict_new();
  ignored = dict_new();

  main_dict = dict_load(dict);

  process_document(document);

  dict_save(main_dict, dict);

  dict_destroy(main_dict);
  dict_destroy(ignored);

   printf("El documento %s ha sido procesado. Resultados en out.txt\n", argv[1]);
   return 0;
}

