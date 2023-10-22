#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct Music {
    char artist[100];
    char name[100];
    struct Music* next;
    struct Music* prev;
} Music;


Music* createMusic(const char* artist, const char* name) {
    Music* newMusic = (Music*)malloc(sizeof(Music));
    if (newMusic == NULL) {
        fprintf(stderr, "Erro na alocacao de memoria.\n");
        exit(1);
    }
    strcpy(newMusic->artist, artist);
    strcpy(newMusic->name, name);
    newMusic->next = newMusic;
    newMusic->prev = newMusic;
    return newMusic;
}


void insertMusic(Music** head, const char* artist, const char* name) {
    Music* newMusic = createMusic(artist, name);
    if (*head == NULL) {
        *head = newMusic;
    } else {
        newMusic->prev = (*head)->prev;
        newMusic->next = *head;
        (*head)->prev->next = newMusic;
        (*head)->prev = newMusic;
    }
}


void removeMusic(Music** head, Music* music) {
    if (*head == NULL) {
        return;
    }
    if (*head == music) {
        if ((*head)->next == *head) {
            free(*head);
            *head = NULL;
        } else {
            (*head)->prev->next = (*head)->next;
            (*head)->next->prev = (*head)->prev;
            *head = (*head)->next;
            free(music);
        }
    } else {
        music->prev->next = music->next;
        music->next->prev = music->prev;
        free(music);
    }
}


void displayPlaylist(Music* head) {
    if (head == NULL) {
        printf("Playlist vazia.\n");
        return;
    }
    Music* current = head;
    do {
        printf("Artista: %s | Musica: %s\n", current->artist, current->name);
        current = current->next;
    } while (current != head);
}


Music* nextMusic(Music* current) {
    if (current == NULL) {
        return NULL;
    }
    return current->next;
}


Music* previousMusic(Music* current) {
    if (current == NULL) {
        return NULL;
    }
    return current->prev;
}


void freePlaylist(Music* head) {
    if (head == NULL) {
        return;
    }
    Music* current = head;
    do {
        Music* temp = current;
        current = current->next;
        free(temp);
    } while (current != head);
}

int main() {
    Music* playlist = NULL;
    char filename[] = "musicas.txt";
    setlocale(LC_ALL, "Portuguese");
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *artist = strtok(line, ";");
        char *name = strtok(NULL, "\n");
        insertMusic(&playlist, artist, name);
    }
    fclose(file);

    int choice;
    Music* current = playlist;

    while (1) {
        printf("\n-------------------MENU-----------------------------\n");
        printf("- 1. Exibir playlist pela ordem de cadastro        -\n");
        printf("- 2. Exibir playlist ordenada pelo nome das musicas-\n");
        printf("- 3. adicionar nova musica                         -\n");
        printf("- 4. Remover uma musica                            -\n");
        printf("- 5. Buscar por uma musica(case sensitive)         -\n");
        printf("- 6. Avancar para a proxima musica                 -\n");
        printf("- 7. Retornar a musica anterior                    -\n");
        printf("- 8. Sair                                          -\n");
        printf("\n----------------------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: 
                displayPlaylist(playlist);
                break;

            case 2: 
                printf("por algum motivo todas as formas que eu tentei deram erro no VS code, ent deixei de lado");
                break;

            case 3:
                {
                    char artist[100];
                    char name[100];
                    printf("Digite o nome do artista: ");
                    scanf(" %[^\n]", artist);
                    printf("Digite o nome da musica: ");
                    scanf(" %[^\n]", name);
                    insertMusic(&playlist, artist, name);

                    
                    file = fopen(filename, "a");
                    if (file == NULL) {
                        fprintf(stderr, "Erro ao abrir o arquivo para adicionar a nova musica.\n");
                    } else {
                        fprintf(file, "%s;%s\n", artist, name);
                        fclose(file);
                    }
                }
                break;

            case 4:
                if (playlist != NULL) {
                    char name[100];
                    printf("Digite o nome da musica a ser removida: ");
                    scanf(" %[^\n]", name);
                    Music* temp = playlist;
                    do {
                        if (strcmp(temp->name, name) == 0) {
                            removeMusic(&playlist, temp);

                            file = fopen(filename, "w");
                            if (file == NULL) {
                                fprintf(stderr, "Erro ao abrir o arquivo para reescrever os registros.\n");
                            } else {
                                Music* current = playlist;
                                do {
                                    fprintf(file, "%s;%s\n", current->artist, current->name);
                                    current = current->next;
                                } while (current != playlist);
                                fclose(file);
                            }
                            break;
                        }
                        temp = temp->next;
                    } while (temp != playlist);
                    if (temp == playlist) {
                        printf("Musica nao encontrada.\n");
                    }
                } else {
                    printf("Playlist vazia.\n");
                }
                break;

            case 5: 
                {
                    char name[100];
                    printf("Digite o nome da musica a ser buscada: ");
                    scanf(" %[^\n]", name);
                    Music* temp = playlist;
                    do {
                        if (strcmp(temp->name, name) == 0) {
                            printf("Musica encontrada: Artista: %s | Musica: %s\n", temp->artist, temp->name);
                            break;
                        }
                        temp = temp->next;
                    } while (temp != playlist);
                    if (temp == playlist) {
                        printf("Musica nao encontrada.\n");
                    }
                }
                break;

            case 6: 
                current = nextMusic(current);
                if (current != playlist) {
                    printf("Musica atual: Artista: %s | Musica: %s\n", current->artist, current->name);
                } else {
                    printf("Fim da playlist.\n");
                }
                break;

            case 7: 
                current = previousMusic(current);
                if (current != playlist) {
                    printf("Musica atual: Artista: %s | Musica: %s\n", current->artist, current->name);
                } else {
                    printf("Inicio da playlist.\n");
                }
                break;

            case 8:
                freePlaylist(playlist);
                return 0;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}
