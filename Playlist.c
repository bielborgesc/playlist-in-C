//Import libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <time.h>
#define SIZE 200

// Common music structure
typedef struct music{
    int id;
    char tittle[SIZE];
    char artist[SIZE];
    char album[SIZE];
    int duration;
} music;

// Tree node
typedef struct AVL_music{
    music *music;
    struct AVL_music *left;
    struct AVL_music *right;
    int h;//Height
} AVL_music;

typedef struct AVL_music *tree_AVL; //Tree - node pointer

// Circular list of musics that make up the playlist
typedef struct playlist_node {
	music *music;
	struct playlist_node *next;
} playlist_node;

// Simply linked list which forms the playlist list
typedef struct lplaylist_node {
	int id;
	char name[SIZE];
	playlist_node *musics;
	int amount;
	struct lplaylist_node *next;
} lplaylist_node;

// Build the root of the tree
tree_AVL *build_tree_node(void){
	tree_AVL *source = (tree_AVL*)malloc(sizeof(tree_AVL));
    if(source != NULL){
        *source = NULL;
    }
    return source;
}
// Head of playlist list
lplaylist_node head_playlist_list(void){
	lplaylist_node *head;
	head = malloc(sizeof(lplaylist_node));
	head->next = NULL;
	return *head;
}

// Measures the height of the node
int node_height(AVL_music *no){
    if(no == NULL) return -1;
    return no->h;
}

// Measures the balancing factor (difference in heights)
int balancing_factor(AVL_music *no){
    return labs(node_height(no->left) - node_height(no->right));
}


// Check which number is higher
int maximum(int x, int y){
    if(x > y){
        return x;
    }
    return y;
}

// AVL tree height
int height_AVL(tree_AVL *source){
    if(source == NULL) return 0;
    if(*source == NULL) return 0;
    int alt_left = height_AVL(&((*source)->left));
    int alt_right = height_AVL(&((*source)->right));
    if(alt_left > alt_right) return alt_left + 1;
    else{
        return alt_right + 1;
    }
    
}

// Print playlist name and id too
void print_name_playlists(lplaylist_node *start_playlist_list) {
    lplaylist_node *p;
	p = malloc(sizeof(lplaylist_node));
    p = start_playlist_list->next;
    while(p) {
        printf("\nID: %d - %s", p->id, p->name);
        p = p->next;
    }
}

// Search for a specific head of a playlist in the playlist list
lplaylist_node search_playlist(lplaylist_node *start_playlist_list){
	int opPlay;
	printf("\n\nEnter Playlist Id: ");
	scanf("%d", &opPlay);
	lplaylist_node *p;
	p = malloc(sizeof(lplaylist_node));
	p = start_playlist_list->next;
	while(p && p->id != opPlay){
		p = p->next;
	}
	return *p;
}
// Print all musics from a chosen playlist.
void print_musics_playlist(lplaylist_node *p){
	playlist_node *q;
	q = malloc(sizeof(playlist_node));
	q = p->musics->next;
	while(q != NULL){
		printf("\nID - %d - %s",q->music->id ,q->music->tittle);
		q = q->next;
	}
}

// Print the musics with id and name
void print_musics_id_name(tree_AVL *source){
    if(source == NULL){
        printf("There are no musics registered");
        return;
    }
    if(*source != NULL){
        print_musics_id_name(&((*source)->left));
        printf("%d - music: %s\n", (*source)->music->id, (*source)->music->tittle);
        print_musics_id_name(&((*source)->right));
    }

}

// Print all registered musics
void print_musics(tree_AVL *source){
    if(source == NULL){
        printf("There are no musics registered");
        return;
    }
    if(*source != NULL){// Printed in order
        print_musics(&((*source)->left));// Recursive call, print left elements
        // Conversion to hours, minutes and seconds
        int h, m, s;
		h = ((*source)->music->duration) / (60*60);
		m = ((*source)->music->duration - (h*60*60)) / 60;
		s = ((*source)->music->duration) % 60;
        printf("\n--- ID: %d ---\n", (*source)->music->id);
		printf("Tittle: %s \n",(*source)->music->tittle);
        printf("Artist: %s\n", (*source)->music->artist);
        printf("Album: %s\n", (*source)->music->album);
        printf("Duration: %02d:%02d:%02d\n", h, m, s);
        printf("Balancing Factor: %d\n", balancing_factor(*source));
		printf("----------------------\n");
        print_musics(&((*source)->right));// And last from the right
    }
}

// Do a simple right rotation
void right_rotation(tree_AVL *source){
    AVL_music *auxiliary_node;
    auxiliary_node = (*source)->left;
    (*source)->left = auxiliary_node->right;
    auxiliary_node->right = *source;

    (*source)->h = maximum(node_height((*source)->left), node_height((*source)->right)) + 1;
    auxiliary_node->h = maximum(node_height(auxiliary_node->left), (*source)->h) + 1;

    *source = auxiliary_node;
}

// Do a simple left rotation
void left_rotation(tree_AVL *source){
    AVL_music *auxiliary_node;
    auxiliary_node = (*source)->right;
    (*source)->right = auxiliary_node->left;
    auxiliary_node->left = (*source);

    (*source)->h = maximum(node_height((*source)->left), node_height((*source)->right)) + 1;
    auxiliary_node->h = maximum(node_height(auxiliary_node->right), (*source)->h) + 1;

    (*source) = auxiliary_node;
}

// Do a double rotation left and right
void left_right_rotation(tree_AVL *source){
    left_rotation(&(*source)->left);
    right_rotation(source);
}

// Do a double rotation left and right
void right_rotation_left(tree_AVL *source){
    right_rotation(&(*source)->right);
    left_rotation(source);
}

// Search for a music
AVL_music* search_music(tree_AVL *source, int id){

    if(source == NULL){
        return;
    }
    AVL_music *current = *source;
    while(current != NULL){
        if(id == current->music->id){
            return current;
        }
        if(id > current->music->id){
            current = current->right;
        } else {
            current = current->left;
        }
    }
    return;
}

// Taking the chosen musics to a playlist on the keyboard selecting the musics to create a playlist
void selected_music(tree_AVL *source, lplaylist_node *start_playlist_list, int *playlist_contactor, int *playlist_id){
	int musics_id[SIZE];
	char entrada;
	char temp[100];// It add until it finds a space or /not
	int i =0;
	int num = 0;
	int cont = 0;
	AVL_music *selected_music;
	playlist_node *playlist;
	playlist_node *iniPlay;
	iniPlay = malloc(sizeof(playlist_node));
	iniPlay->next = NULL;
	iniPlay->music = NULL;
	printf("\n\nSelecione as musics: ");
	fflush(stdin);
	do{
        scanf("%c", &entrada); 
        temp[i] = entrada;// We changed the insert part
        if (temp[i] == ' ' || temp[i] == '\n'){
            temp[i] = NULL;
            musics_id[num] = atoi(temp);
			selected_music = malloc(sizeof(AVL_music));
            selected_music = search_music(source, musics_id[num]);// Searching for music is now a function
            printf("music: %s\n", selected_music->music->tittle);
			playlist = malloc(sizeof(playlist_node));
			playlist->next = iniPlay->next;
			playlist->music = selected_music->music;
			iniPlay->next = playlist;
            num++;
			cont ++;
            i = -1;
        }
        i++;
    } while (entrada != '\n');
	criar_lista_playlist(iniPlay, playlist_contactor, start_playlist_list, playlist_id, cont);
}

// Creating the playlist list
void criar_lista_playlist(playlist_node *start_playlist, int *playlist_contactor, lplaylist_node *head, int *playlist_id, int amountmusic){
	lplaylist_node *nova_lista;
	(*playlist_contactor)++;
	(*playlist_id) ++;
	nova_lista = malloc(sizeof(lplaylist_node));
	nova_lista->id = (*playlist_id);
	printf("name da Playlist: ");
	fflush(stdin);
	gets(nova_lista->name);
	nova_lista->amount= amountmusic;
	nova_lista->musics = start_playlist;
	nova_lista->next = head->next;
	head->next = nova_lista;
}

// start of shuffle function generating a random number and a position for exchange
void function_shuffle(lplaylist_node *p){
	int i;
	int pos;
	int num;
	srand(time(NULL));
	for(i = 0; i < p->amount; i++){
		num = 1 + (rand() % ((p->amount)-1));
		trocarmusics(num,i,p);
	}
}

// We exchange the dynamic position and the index position
void trocarmusics(int randomico, int indice, lplaylist_node *head){
	int i;
	playlist_node *aux;
	aux = malloc(sizeof(playlist_node));
	playlist_node *dados;
	dados = malloc(sizeof(playlist_node));
	playlist_node *p;
	p = malloc(sizeof(playlist_node));
	playlist_node *q;
	q = malloc(sizeof(playlist_node));
	int cont;
	cont = head->amount;
	// first music playlist
	dados = head->musics->next;
	

	for(i = 0; i < cont; i ++){
		if(i == randomico){
			p = dados;
		}
		if(i == indice){
			q = dados;
		}
		dados = dados->next;
	}

	aux->music = p->music;
	p->music = q->music;
	q->music = aux->music;
}


// Search all playlists for the desired music and delete it.
void delete_music_playlist(int id, lplaylist_node *head_playlist_list){
	playlist_node *anterior;
	lplaylist_node *playlist;
	playlist_node *music_procurada;
	playlist_node *music_procurada_salva;
	music_procurada = malloc(sizeof(playlist_node));
	playlist = head_playlist_list->next;
	while(playlist){
		music_procurada = playlist->musics->next;
		anterior = playlist->musics;
		while(music_procurada){
			if(music_procurada->music->id == id){
				(playlist->amount)--;
				music_procurada_salva = music_procurada;
				anterior->next = music_procurada->next;
			}
			anterior = music_procurada;
			music_procurada = music_procurada->next;
		}
		playlist = playlist->next;
	}
}

// Insert a music into the tree
int inserir_music(tree_AVL *source, music *nova_music){
    int res;
    if(*source == NULL){
        AVL_music *novo;// When you only have the trunk
        novo = (struct AVL_music*)malloc(sizeof(AVL_music));
        if(novo == NULL){
            return 0;
        }
        novo->music = nova_music;// we put music at the root
        novo->h = 0;
        novo->right = NULL;
        novo->left = NULL;
        *source = novo;
        return 1;
    }
    AVL_music *current = *source;
    if(nova_music->id < current->music->id){
        if((res = inserir_music(&(current->left), nova_music)) == 1){
            if(balancing_factor(current) >= 2){
                if(nova_music->id < (*source)->left->music->id){
                    right_rotation(source);
                }else{
                    left_right_rotation(source);
                }
            }
        }
    }else{
        if(nova_music->id > current->music->id){
            if((res = inserir_music(&(current->right),nova_music)) == 1){
                if(balancing_factor(current) >= 2){
                    if((*source)->right->music->id < nova_music->id){
                        left_rotation(source);
                    }else{
                        right_rotation_left(source);
                    }
                }
            }
        }else{// If the value is the same, there will be no insertion
            printf("Valor duplicado");
            return 0;
        }
    }

    current->h = maximum(node_height(current->left),node_height(current->right)) + 1;
    return res;
}

// Look for the smallest in the
AVL_music *procura_menor(AVL_music *current){
    AVL_music *no1 = current;
    AVL_music *no2 = current->left;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->left;
    }
    return no1;
}

// Delete a music
int delete_music( tree_AVL *source, int id, int *music_contactor){
    if((*source) == NULL){
        printf("music nAo cadastrada\n");
        return 0;
    }
    int res;
    if(id < (*source)->music->id){
        if((res = delete_music(&(*source)->left, id, music_contactor)) == 1){
            if(balancing_factor(*source) >= 2){
                if(node_height((*source)->right->left) <= node_height((*source)->right->right)){
                    left_rotation(source);
                }else{
                    left_right_rotation(source);
                }
            }
        }
    }
    if((*source)->music->id < id){
        if((res = delete_music(&(*source)->right, id, music_contactor)) == 1){
            if(balancing_factor(*source) >= 2){
                if(node_height((*source)->left->right) <= node_height((*source)->left->left)){
                    right_rotation(source);
                }else{
                    right_rotation_left(source);
                }
            }
        }
    }
    if((*source)->music->id == id){
        if(((*source)->left == NULL || (*source)->right == NULL)){
            AVL_music *antigo  = (*source);
            if((*source)->left != NULL){
                *source = (*source)->left;
            }else{
                *source = (*source)->right;
            }
            free(antigo);
        }else{
            AVL_music *temp = procura_menor((*source)->right);
            (*source)->music = temp->music;
            res = delete_music(&(*source)->right,(*source)->music->id, music_contactor);
            if(balancing_factor(*source) >= 2){
                if(node_height((*source)->left->right) <= node_height((*source)->left->left)){
                    right_rotation(source);
                }else{
                    right_rotation_left(source);
                }
            }
        }
        return 1;
    }
    (*source)->h = maximum(node_height((*source)->left),node_height((*source)->right)) + 1;// In any situation it updates the height
    if(res == 1){
        (*music_contactor)--; // If it is eliminated we reduce the amount of musics
    }
    return res;
}
// Registering a music
void register_music(int *music_contactor, tree_AVL *source, int *music_id){
    int res;
	music *nova;
	nova = malloc(sizeof(music));
	*music_contactor += 1;
	*music_id += 1;
	nova->id = *music_id;
	printf("\ntittle: ");
	fflush(stdin);
	gets(nova->tittle);
	printf("artist: ");
	fflush(stdin);
	gets(nova->artist);
	printf("Album: ");
	fflush(stdin);
	gets(nova->album);
	printf("duration em Segundos: ");
	scanf("%d", &nova->duration);
	res = inserir_music(source, nova);// inserting music
}

// create music for testing
void create_test_musics(int *music_contactor, tree_AVL *source, int *music_id) {
    music *nova;
    nova = malloc(sizeof(music));
    (*music_contactor)++;
    (*music_id)++;
    nova->id = *music_id;
    strcpy(nova->tittle, "Girassol");
    strcpy(nova->artist, "Cidade Negra");
    strcpy(nova->album, "Acustico Cidade Negra");
    nova->duration = 111;
    inserir_music(source, nova);
   
    nova = malloc(sizeof(music));
    (*music_id)++;
    (*music_contactor)++;
    nova->id = *music_id;
    strcpy(nova->tittle, "Rindo a Toa");
    strcpy(nova->artist, "Falamanca");
    strcpy(nova->album, "Falamanca Essencial");
    nova->duration = 122;
    inserir_music(source, nova);
   
    nova = malloc(sizeof(music));
    (*music_id)++;
    (*music_contactor)++;
    nova->id = *music_id;
    strcpy(nova->tittle, "Enquanto Houver Sol");
    strcpy(nova->artist, "Titas");
    strcpy(nova->album, "Como Estao Voces?");
    nova->duration = 133;
    inserir_music(source, nova);
   
    nova = malloc(sizeof(music));
    (*music_id)++;
    (*music_contactor)++;
    nova->id = *music_id;
    strcpy(nova->tittle, "Reconvexo");
    strcpy(nova->artist, "Caetano Veloso");
    strcpy(nova->album, "Ofertario");
    nova->duration = 144;
    inserir_music(source, nova);
   
    nova = malloc(sizeof(music));
    (*music_id)++;
    (*music_contactor)++;
    nova->id = *music_id;
    strcpy(nova->tittle, "A Paz");
    strcpy(nova->artist, "Gilberto Gil");
    strcpy(nova->album, "Unplugged");
    nova->duration = 155;
    inserir_music(source, nova);

    nova = malloc(sizeof(music));
    (*music_contactor)++;
    (*music_id)++;
    nova->id = *music_id;
    strcpy(nova->tittle, "Um Sonho");
    strcpy(nova->artist, "Nacao Zumbi");
    strcpy(nova->album, "Nacao Zumbi");
    nova->duration = 111;
    inserir_music(source, nova);
   
    nova = malloc(sizeof(music));
    (*music_id)++;
    (*music_contactor)++;
    nova->id = *music_id;
    strcpy(nova->tittle, "Gente Aberta");
    strcpy(nova->artist, "Erasmo Carlos");
    strcpy(nova->album, "Carlos, Erasmo");
    nova->duration = 122;
    inserir_music(source, nova);
   
    nova = malloc(sizeof(music));
    (*music_id)++;
    (*music_contactor)++;
    nova->id = *music_id;
    strcpy(nova->tittle, "Estado de Poesia");
    strcpy(nova->artist, "Chico Cesar");
    strcpy(nova->album, "Estado de Poesia");
    nova->duration = 133;
    inserir_music(source, nova);
   
    nova = malloc(sizeof(music));
    (*music_id)++;
    (*music_contactor)++;
    nova->id = *music_id;
    strcpy(nova->tittle, "Sorte");
    strcpy(nova->artist, "Ney Matogrosso");
    strcpy(nova->album, "Sorte");
    nova->duration = 144;
    inserir_music(source, nova);
   
    nova = malloc(sizeof(music));
    (*music_id)++;
    (*music_contactor)++;
    nova->id = *music_id;
    strcpy(nova->tittle, "Bom Senso");
    strcpy(nova->artist, "Tim Maia");
    strcpy(nova->album, "Racional");
    nova->duration = 155;
    inserir_music(source, nova);
}

int main_menu() {
	int op;
	printf("\n---------------------------------------------\n"
			"                OPTIONS MENU"
			"\n---------------------------------------------\n"
			"1 - Music menu\n"
			"2 - Playlist menu\n" 
			"3 - Exit\n\n"
			"Escolha uma opcoes: ");
	fflush(stdin);
	scanf("%d", &op);
	return op;
}

int music_menu() {
	int op;
	printf("\n---------------------------------------------\n"
			"               MUSIC MENU"
			"\n---------------------------------------------\n"
			"1 - Insert music\n"
			"2 - Delete music\n"
			"3 - Print musics\n"
			"4 - Back\n\n"
			"Escolha uma opcoes: ");
	scanf("%d", &op);
	return op;
}

int menuPlaylist() {
	int op;
	printf("\n\n---------------------------------------------\n"
			"               PLAYLIST MENU"
			"\n---------------------------------------------\n"
			"1 - Insert playlist\n"
			"2 - Print playlists\n"
			"3 - Shuffle\n"
			"4 - Back\n\n"
			"Choose a option: ");
	scanf("%d", &op);
	return op;
}

//Main Application
int main(){
    int op = 0;
    int music_contactor = 0;
    int playlist_contactor = 0;
	int music_id = 0;
    int playlist_id = 0;
    music music_node;
    tree_AVL *source = build_tree_node();
    playlist_node start_playlist;
	lplaylist_node start_playlist_list;
    start_playlist_list = head_playlist_list();
    create_test_musics(&music_contactor, &(*source), &music_id);
    while (op != 3) {
		int op = main_menu();
		if (op == 1) {
			int option = 0;
			while (option != 4) {
				option = music_menu();
				if (option == 1) {
					register_music(&music_contactor, source, &music_id);
					printf("\nMusic successfully registered!\n");
				} else 
				if (option == 2) {
					if(music_contactor == 0){
						printf("There are no musics to be deleted\n");
					}else{
						int id;
						print_musics_id_name(source);
						printf("\nWhich music do you want to delete: ");
						scanf("%d", &id);
						delete_music_playlist(id, &start_playlist_list);
						int res = delete_music(source, id, &music_contactor);
					}
					
				}if (option == 3) {
					if(music_contactor == 0){
						printf("There are no musics to be shown\n");
					}else{
						print_musics(source);
					}
				}
			}
		} else
		
		if (op == 2) {
			lplaylist_node p;
			int option = 0;
			while (option != 4) {
				option = menuPlaylist();
				if (option == 1) {
					print_musics_id_name(source);
					selected_music(source, &start_playlist_list, &playlist_contactor, &playlist_id);
					printf("\nPlaylist successfully registered!\n");
				}else if (option == 2) {
					if(playlist_contactor == 0){
						printf("There is no playlist registered\n");
					}else{
						print_name_playlists(&start_playlist_list);
						p = search_playlist(&start_playlist_list);
						print_musics_playlist(&p);
					}
				} else if(option == 3){
					if(playlist_contactor == 0){
						printf("There is no playlist registered\n");
					}else{
						print_name_playlists(&start_playlist_list);
						p = search_playlist(&start_playlist_list);
                        function_shuffle(&p);
					}
				}
			}
		}	
		else {
			break;
		} 
	}
}
