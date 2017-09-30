#include "document.h"

struct _document_{
    FILE *doc_in;
    FILE *doc_out;
};


int doc_get_word(Document doc, char *word){
    int c;
    int r = 1;

    while(fscanf(doc->doc_in, "%[a-z|A-Z]", word) == 0 && !feof(doc->doc_in)){
        c = fgetc(doc->doc_in);
        fputc(c, doc->doc_out);
    }
    if(feof(doc->doc_in)) {
        r = 0;
    }

    return r;
}


void doc_put_word(Document doc, char *word){
    fputs(word, doc->doc_out);
}


Document doc_open(char *name_doc_in, char *name_doc_out){
    Document doc = calloc(1, sizeof(struct _document_));

    doc->doc_in = fopen(name_doc_in, "r");
    doc->doc_out = fopen(name_doc_out, "w");

    return doc;
}


void doc_close(Document doc){
    fclose(doc->doc_in);
    fclose(doc->doc_out);
    free(doc);
}
