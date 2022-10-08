#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10
#define NUM_CARTAS 5

typedef struct node{
    char *value;
    struct node* next;
}Node;

int* GeneradorRandom(int * numeros);
FILE* AbrirArchivo(char *nombreArchivo, char *toDo);
Node* GenerarRandom(int *randoms, char *nombreArchivo);
Node* new_node(char * value);
Node * add_value_to_list(Node *head, char * value_to_add);
void print_cadenita(Node *head);
Node* card_remove_top(Node *head);
Node* card_down(Node *head);
void cards_save(Node* head);
void print_instrucciones();


int main() {

    int numeros[NUM_CARTAS];
    int * numerosRandon = GeneradorRandom(numeros);
    Node *head = GenerarRandom(numerosRandon, "../Cartas");
    print_instrucciones();
    print_cadenita(head);   // imprime cartas en orden aleatorio

    char opc;
    do {
        fflush(stdin);
        scanf("%c",&opc);

        if(opc != 'e' && opc != 'p' && opc != 's' && opc != 'l')
        {
            printf("La tecla ingresada no es valida\n");
        }

        if (opc == 'p')
        {
            if (head->next == NULL)
                printf("La carta que tomo es la ultima carta\n");

            fflush(stdin);
            scanf("%c",&opc);

            if (opc != 'd' && opc != 'b' && opc != 'u')
                printf("Movimiento invalido, la carta se a regresado a su lugar\n");
            else if (opc == 'd')
                head = card_remove_top(head);
            else if (opc == 'b')
                head = card_down(head);

        }

        if (opc == 's')
        {
            cards_save(head);
            printf("Se ha guardado el estado actual de tu juego\n");
        }
        if (opc == 'l')
        {
            head = GenerarRandom(NULL, "../LastGame");
        }

    }while(opc != 'e' && head != NULL);

    printf("Gracias por jugar\n");

    if ( head != NULL ) {
        print_cadenita(head);//imprime estado final del juego
    } else {
        printf("Todas cartas han sido removidas\n");
    }
    return 0;
}

void print_instrucciones()
{
    FILE *readMe = AbrirArchivo("../Instrucciones", "r");
    char *line = malloc(sizeof(char)*MAX_SIZE);

    line = fgets(line, MAX_SIZE, readMe);
    while (line != NULL ) {
        printf("%s", line);
        line = fgets(line, MAX_SIZE, readMe);
    }
    fclose(readMe);
}

void cards_save(Node* head)
{
    FILE *LastGame = AbrirArchivo("../LastGame", "w");
    char *line = malloc(sizeof(char)*10);

    Node *current = head;
    while (current != NULL)
    {
        fputs(current->value, LastGame);
        current = current->next;

    }

    fclose(LastGame);
}

Node* card_down(Node *head)
{
    if(head->next == NULL)
    {
        return head;
    }
    Node *current = head;
    while(current->next->next != NULL)
    {
        current = current->next;
    }
    Node *final = current->next;
    current->next = NULL;
    final->next = head;
    Node *new_head = final;
    return new_head;

}

Node* card_remove_top(Node *head){
    if ( head->next == NULL ) {
        free(head);
        head = NULL;
    } else {
        Node *current = head;
        while(current->next->next != NULL)
        {
            current = current->next;
        }
        free(current->next);
        current->next = NULL;
    }
    return head;
}


int* GeneradorRandom(int *numeros) {

    srand(time(NULL));
    for (int i = 0; i < NUM_CARTAS; ++i) {
        int temp = rand() % NUM_CARTAS + 1;
        int existe = 0;
        for (int j = 0; j < i; ++j) {
            if (numeros[j] == temp) {
                existe = 1;
                break;
            }
        }
        if (existe == 0 ) {
            numeros[i] = temp;
        } else {
            --i; // Intenta de nuevo
        }
    }
    return numeros;
}

FILE* AbrirArchivo(char *nombreArchivo, char *toDo){
    // Abrir Archivo Original
    FILE *mi_descriptor = NULL;
    mi_descriptor = fopen(nombreArchivo, toDo);
    int line_count= 0;

    // Checar que no sea NULL
    if (mi_descriptor == NULL){
        printf("No se pudo abrir el archivo\n");
        //return 0xF0;
    }
    return mi_descriptor;
}

Node* GenerarRandom(int *randoms, char *nombreArchivo) {

    FILE *Copia = AbrirArchivo(nombreArchivo, "r");
    Node *current = NULL;

    for (int i = 0; i < NUM_CARTAS ; i++ ){
        char *line = malloc(sizeof(char)*MAX_SIZE);

        if (randoms == NULL ) { // se usa cuando se quiere leer las cartas (el archivo) en orden
            line = fgets(line, MAX_SIZE, Copia);
            if ( line == NULL) {
                break;
            }
        } else { // se usa cuando se quiere leer las cartas (el archivo) en modo random
            int random = randoms[i];
            rewind (Copia);
            for (int j = 0; j < random; j++) {
                fgets(line, MAX_SIZE, Copia);
            }
        }

        if(current == NULL){
            // Asegura guardar el head
            current = add_value_to_list(current, line);
        }else{
            add_value_to_list(current, line);
        }
    }
    return current;
}

Node* new_node(char * value)
{
    Node *new = malloc(sizeof(Node));
    new->value = value;
    new->next = NULL;
    return new;
}

Node * add_value_to_list(Node *head, char * value_to_add)
{
    {
        if (head == NULL) {
            // obtine un node de la memoroia con mallo
            head = new_node(value_to_add);
            return head;
        } else {
            Node *current = head;
            while(current->next != NULL)
                current = current->next;
            current->next = new_node(value_to_add);
            return head;
        }
    }
}

void print_cadenita(Node *head)
{
    Node *current = head;
    do
    {
        printf("Carta: %s", current->value);
        current = current->next;
    } while (current != NULL);
}



