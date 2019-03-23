#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
*                                   STRUCTURE D'UNE NODE
*   La node contient un symbole (char) et une fréquence d'apparition dans le code
*/
typedef struct node_s{
    char symbol;
    int freq;
    struct node_s* fg;
    struct node_s* fd;
}node;


/*
*                                   STRUCTURE DU TAS MIN
*   int n permet de connaître le nombre d'arguments dans le tas
*/
typedef struct tas_s{
    int max;
    int n;
    node** tab;
}tas;


#define TRUE 1

#define FALSE 0

/*
*                                    PROTOTYPES DE FONCTIONS
*/
node* creer_node(int, char);

tas* init_tas(int);

int est_vide_tas(tas*);

int est_feuille(node*);

tas* saisie_alphabet();

tas* saisie_alphabet2();

void inserer_tas(tas*, node*);

node* supprimer_tas(tas*);

node* creer_arbre(tas*);

void imprimer_arbre(node*);

void imprimer_codes(node*, char*, int i);

void supprimer_arbre(node*);

void liberer_memoire_tas(tas*);

void codage_manuel();

void codage_fichier();

void afficher_tas(tas*);

tas* nombre_caractere_fichier(FILE*);


/*
*                                           FONCTIONS
*/



/*
*   Fonction permettant d'allouer la mémoire pour un node. les valeurs pour le caractère et la fréquence
*   passées en paramètre sont ensuite attribuées aux variables contenues dans le node (char symbol et int freq)
*/
node* creer_node(int f, char s){
    //  Allocation de mémoire
    node* res = (node*)malloc(sizeof(node));
    
    res->freq = f;
    res->symbol = s;
    res->fd = NULL;
    res->fg = NULL;
    
    return res;
}



/*
*   Cette fonction permet d'afficher de manière claire tous les éléments contenus dans
*   le tableau de *node du tas.
*/
void afficher_tas(tas* T){
    //  Si on veut afficher un tas ne contenant aucun élément
    if(est_vide_tas(T)){
        printf("Le tas est vide.\n");
        
        return ;
    }
    
    int i = 0;
    
    //  affichage de tous les éléments du tas
    while(i <= T->n-1){
        printf(" %d : (%c %d) \n", i, T->tab[i]->symbol, T->tab[i]->freq);
        i++;
    }
    printf("\n");
}



/*
*   Fonction permettant d'initialiser un pointeur sur un tas en allouant exactement la quantité de
*   mémoire requise. 
*/
tas* init_tas(int nombre){
    //  Allocation du tas
    tas* res = (tas*)malloc(sizeof(tas));
    
    res->max = nombre;
    res->n = 0;
    
    //  Allocation du tableau de pointeurs de node contenant le nombre "nombre" passé en paramètre de *node
    res->tab = (node**)malloc(nombre*sizeof(node*));
    
    return res;
}



/*
*   Fonction permettant de tester si un tas est vide, c'est à dire que le nombre d'élément T->n est 0
*/
int est_vide_tas(tas* T){
    if(T->n == 0){
        return TRUE;
    }
    //  Si le tas n'est pas vide
    return FALSE;
}



/*
*   Fonction permettant de tester si un node est une feuille
*/
int est_feuille(node* A){
    if((A->fd == NULL) && (A->fg == NULL)){
        return TRUE;
    }
    //  Si ce n'est pas une feuille
    return FALSE;
}



/*
*   Cette fonction récursive permet d'imprimer l'arbre de manière infixe, c'est à dire qu'on va à 
*   gauche, ensuite on effectue le traitement pour ensuite aller à droite
*/
void imprimer_arbre(node* A){
    // Tant que le node n'est pas nul on effectue les instructions dans le bloc ci dessous
    if(A != NULL){
        imprimer_arbre(A->fg);
        
        // TRAITEMENT INFIXE
        if(est_feuille(A)){
            printf(" ( %c : %d ) ", A->symbol, A->freq);
        }
        else if(!est_feuille(A)){
            printf(" ( %d ) ", A->freq);
        }
        
        imprimer_arbre(A->fd);
    }
}



/*
*   Fonction permettant d'inserer un node dans un tas min. L'insertion respecte les caractéristiques
*   du tas min. 
*/
void inserer_tas(tas* T, node* A){
    //  initialisation de variable temporaire
    node* temp;
    
    //  On récupère le nombre d'éléments
    int i = T->n;
    //  On insère l'élément à ajouter à la fin du tas
    T->tab[i] = A;
    
    /*
    *   Tant que le père du node que l'on ajoute a un fréquence plus élevée que celle de celui-ci,
    *   on réalise un swap entre le père en (i-1)/2 et le node que l'on ajoute
    */
    while(T->tab[i]->freq < T->tab[(i-1)/2]->freq){
        temp = T->tab[i];
        T->tab[i] = T->tab[(i-1)/2];
        T->tab[(i-1)/2] = temp;
        i = ((i-1)/2);
    }
    
    //  On incrémente le nombre d'éléments contenus dans le tas
    T->n++;
}



/*
*   Cette fonction permet de récuperer les éléments du tas min pour ensuite contruire l'arbre
*   qui permettra de coder les éléments. Le pseudo code fourni nous à aidé à coder cette fonction
*/
node* creer_arbre(tas* T){
    //  On récupère le nombre d'arguments
    int C = T->n-1;
    //  On crée un nouveau tas avec le nombre d'arguments
    tas* F = init_tas(C);
    
    //  Variable permettant de créer l'arbre en ajoutant les fils droits et gauches
    node* tempGauche;
    node* tempDroit;
   
    //  Insertion dans le nouveau tas
    for(int j = 1; j <= C; j++){
        inserer_tas(F, T->tab[j]);
    }
    
    for(int i = 1; i <= C; i++){
        //  On crée un noeud interne ayant ' ' pour caractère
        node* racine = creer_node(0, ' ');
        
        //  On récupère les valeurs à insérer à gauche et à droite du noeud interne
        tempGauche = supprimer_tas(T);
        tempDroit = supprimer_tas(T);
        //  On attribut les noeuds au noeud interne/ à la racine
        racine->fg = tempGauche;
        racine->fd = tempDroit;
        //  On additionne les fréquences
        racine->freq = tempDroit->freq + tempGauche->freq;
        //  On insére ensuite la racine/ le noeud interne dans l'arbre et affiche le tas
        inserer_tas(T, racine);
        afficher_tas(T);
    }
    //  On renvoit la racine de l'arbre
    return T->tab[0];
}



/*
*   Fonction permettant de supprimer le plus petit élément du tas, qui est à la racine 
*   Le tas est ensuite retrié pour que le plus petit élément soit de nouveau au début
*/
node* supprimer_tas(tas* T)
{
    //  Si le tas n'est pas vide
    if(est_vide_tas(T)){
        printf("\nLe tas est vide, impossible de supprimer un élément\n");
        return NULL;
    }
        
        //  Création de deux variables temporaires
        node* temp = T->tab[0];
        node* temp2;
    
        //  Le dernier element du tableau est mis à la fin
        T->tab[0] = T->tab[T->n-1];
    
        //  Décrémente le nombre d'éléments
        T->n--;
        int n = T->n;
        
        int succes = TRUE;
        int k;
        // compteiur
        int i = 0;
        
        while( (2*i + 1 < n ) && succes)
        {
            if(2*i + 2 < n)
            {
                if(T->tab[2*i + 1]->freq <= T->tab[2*i + 2]->freq)
                    k = 2*i + 1;
                else
                    k = 2*i + 2;
            }
            else
                k = 2*i + 1;
            
            if(T->tab[i]->freq > T->tab[k]->freq)
            {
                //  Réalisation du swap
                temp2 = T->tab[i];
                T->tab[i] = T->tab[k];
                T->tab[k] = temp2;
                i=k;
            }
            else
                succes = FALSE;
        }
        return temp;
}



/*
*   Cette fonction nous permet de savoir si le caractère que nous avons entré existe déja dans le tas
*/
int existe(tas* T, char c){
    for(int i = 0; i < T->n; i++){
        
        //  Si le caractère est déjà présent, on renvoie l'indice d'où il se trouve
        if(c == T->tab[i]->symbol){
            return i;
        }
    }
    //  Sinon on return -1
    return -1;
}



/*
*   Fonction permettant à l'utilisateur de choisir le nombre de caractères qu'il veut
*   Cette fonction va également lui permettre d'entrer un par un les caractères et leurs
*   fréquence qui seront insérés un à un dans le tas
*/
tas* saisie_alphabet2(){
    printf("\nVous avez choisis de prendre en compte les caractères identiques\n\n");
    int nombre_elements;
    char carac;
    int frequence;
    
    int indice;
    
    printf("Combien de caractères différents contient votre code ? : \n");
    scanf("%d", &nombre_elements);
    
    node* temp; // node qui sera utiliser pour inserer les données dans le tas
    tas* T = init_tas(nombre_elements); 
    
    for(int i = 0; i < nombre_elements; i++){
        
        //  Tant que le caractère n'est pas une lettre minuscule entre a et z
        do{
        printf("%d caractere : \n", i+1);
        scanf(" %c", &carac);
        }while(carac < 97 || carac > 122);
        
        //  Tant que la fréquence entrée est inférieur à 0
        do{
        printf("frequence du %d caractère : ", i+1);
        scanf("%d", &frequence);
        }while(frequence <= 0);
        
        indice = existe(T, carac);
        
        if(indice >= 0){
            T->tab[indice]->freq = frequence + T->tab[indice]->freq;
        }
        else{
        
        //  On crée un node avec la fréquence et le caractère choisi
        temp = creer_node(frequence, carac);
        
        //  On insére le node dans le tas
        inserer_tas(T, temp);
        }
    }
    
    printf("\n");
    //  On retourne le tas
    return T;
}



/*
*   Fonction permettant à l'utilisateur de choisir le nombre de caractères qu'il veut
*   Cette fonction va également lui permettre d'entrer un par un les caractères et leurs
*   fréquence qui seront insérés un à un dans le tas
*/
tas* saisie_alphabet(){
    printf("\nVous avez choisis de ne pas prendre en compte les caractères identiques\n\n");
    int nombre_elements;
    char carac;
    int frequence;
    
    printf("Combien de caractères différents contient votre code ? : \n");
    scanf("%d", &nombre_elements);
    
    node* temp; // node qui sera utiliser pour inserer les données dans le tas
    tas* new = init_tas(nombre_elements); 
    
    for(int i = 0; i < nombre_elements; i++){
        
        //  Tant que le caractère n'est pas une lettre minuscule entre a et z
        do{
        printf("%d caractere : \n", i+1);
        scanf(" %c", &carac);
        }while((carac < 97 || carac > 122));
        
        //  Tant que la fréquence entrée est inférieur à 0
        do{
        printf("frequence du %d caractère : ", i+1);
        scanf("%d", &frequence);
        }while(frequence <= 0);
        
        //  On crée un node avec la fréquence et le caractère choisi
        temp = creer_node(frequence, carac);
        
        //  On insére le node dans le tas
        inserer_tas(new, temp);
    }
    printf("\n");
    //  On retourne le tas
    return new;
}



/*
*   Fonction récursive permettant de parcourir l'arbre et de coder chaque caractère entrés par l'utilisateur
*   Lorsque que l'on visite le fils gauche on ajoute 0 à la chaine de caractère, 1 quand on visite le fils droit
*/
void imprimer_codes(node* A, char* string, int i){
    if(A != NULL){
        //  TRAITEMENT PREFIXE
        if(est_feuille(A)){
            string[i] = '\0';        
            printf("%c = %s ; frequence = %d\n",A->symbol, string, A->freq);
        }
        //  Va à gauche si possible et ajoute 1 à string
        if(A->fg != NULL){
            string[i] = '0';
            imprimer_codes(A->fg, string, i+1);
        }
        //  Va à droite si possible et ajoute 0 à string
        if(A->fd != NULL){
            string[i] = '1';
            imprimer_codes(A->fd, string, i+1);
        }
    }
}



/*
*   Fonction récursive permettant de libérer les cases mémoire allouées pour créer l'arbre
*/
void supprimer_arbre(node* A){
    if(A != NULL){
        supprimer_arbre(A->fg);
        supprimer_arbre(A->fd);
        // TRAITEMENT POSTFIXE
        free(A);
    }
}



/*
*   Fonction récursive permettant de libérer un tas allouée, on supprimer l'arbre crée avec ce tas
*   puis on libère le tableau et ensuite le tas
*/
void liberer_memoire_tas(tas* T){

    //  On supprime tous les éléments du tas
    for(int i = T->n-1; i > 0; i--){
        free(T->tab[i]);
    }
    //  On supprime tous les éléments de l'arbre car la racine de l'arbre est également le premier élément
    supprimer_arbre(T->tab[0]);
    
    //  Suppression du tableau
    free(T->tab);
    free(T);
    // free le tas
}



/*
*   Cette fonction va récuperer tous les caractères, les mettre dans un tableau puis les inserer dans le tas de manière
*   ordonnée
*/
tas* nombre_caractere_fichier(FILE* stream){
    // Tab qui va récuperer tous les éléments
    node** tab = (node**)malloc(sizeof(node)*26);
    // Tas que l'on va renvoyer
    tas* T;
    
    char symbole;
    int succes = FALSE;
    int distinct = 0;
    
    fseek(stream, 0, SEEK_SET);
    
    printf("Voici la chaine de caractère présente dans le fichier : \n\n");
    
    // Je préfère récupérer le nombre de caractère tout d'abord
    int n = fseek(stream, 0, SEEK_END);
    n = ftell(stream);
    
    
    printf("Ce fichier contient %ld caractères au total\n\n", ftell(stream));
    fseek(stream, 0, SEEK_SET);
    
    //  Tant qu'il y a des caractères
    while((symbole = fgetc(stream)) != EOF){
        
        succes = FALSE;
        printf("%c", symbole);
        
        // On récupère les éléments et les mets dans le tableau
        if(tab[0] == NULL){
            tab[0] = creer_node(1, symbole);
            distinct++;
        }
        else{
            for(int i = 0; tab[i] != NULL && succes == FALSE; i++){
                if(tab[i]->symbol == symbole){
                    tab[i]->freq+=1;
                    succes = TRUE;
                }
                else if(tab[i+1] == NULL){
                    tab[i+1] = creer_node(1, symbole);
                    distinct++;
                    succes = TRUE;
                }
            }
        }
    }
    
    //  Init le tas avec exactement le bon nombre d'élément
    T = init_tas(distinct);
    
    printf("\n\n");
    printf("Le fichier contient %d caractères distincts.\n\n", distinct);
    
    //  On insère un à un dans le tas
    for(int i = 0; i < distinct; i++){
        inserer_tas(T, tab[i]);
    }
    
    printf("\n\nFin de l'insertion\n\n");
    afficher_tas(T);
    
    return T;
}



/*
*   Fonction principale permettant l'ouverture du fichier, elle fait appelle a la fonction qui va récuperer les
*   caractères et les mettre dans le tas, le tas va être récupéré ici et l'arbre sera crée
*/
void codage_fichier(){
    FILE* stream = NULL;
    
    //  Déclaration pour ouverture de fichiers
    char nom_fich[50];
    char nom_final[100] = "fichiers/";
    char* fich = (char*)malloc(150*sizeof(char));
    
    
    //  FORMALITE
    printf("\nQuel fichier souhaitez vous ouvrir ? (Le fichier doit être contenu dans le répertoire fichiers)\n\n");
    
    scanf("%s", nom_fich);
    
    //  On concat pour avoir le chemin et aller au fichier
    fich = strcat(nom_final, nom_fich);
    printf("\nVous avez choisi l'ouverture du fichier %s .\n", fich);
    
    printf("\nOuverture du fichier \n\n");
    
    //  Ouverture du fichier
    stream = fopen(fich, "r");
    

    //  Si le fichier existe et s'ouvre
    if(stream != NULL){
        tas* T = nombre_caractere_fichier(stream);
    
        //  Récupération de la racine de l'arbre
        node* racine = creer_arbre(T);

        //  Initialisation de la chaîne de caractères permettant de récupérer les fréquences des caractères
        char* str = (char*)malloc(T->n+1*sizeof(char));

        afficher_tas(T);

        printf("----------------------------------------------------------------------------\n\n");
        printf("                                 IMPRIMER ARBRE                             \n\n");

        //  L'affichage du tas se fait de manière infixe
        imprimer_arbre(racine);

        printf("\n\n");

        printf("----------------------------------------------------------------------------\n\n");
        printf("                                 IMPRIMER CODES                             \n\n");

        //  On affiche les caractères et leurs fréquences
        imprimer_codes(racine, str, 0);

        printf("\n\n");
        
        //  On libère la chaîne et ferme le fichier
        liberer_memoire_tas(T);
        free(str);
        fclose(stream);
    }
    
    //  Si le fichier ne s'ouvre pas
    else if(stream == NULL){
        printf("\n\nERREUR LORS DE L'OUVERTURE DU FICHIER OU FICHIER INEXISTANT\n\n");
    }
}



/*
*   Si l'utilisateur choisit le codage manuel, cette fonction va permettre de réaliser 
*   l'intégralité des opérations afin de créer l'arbre et
*   de générer le code des caractères entrés par l'utilisteur
*/
void codage_manuel(){
    int choix;
    tas* Tas;
    
    do{
        printf("Quelle type d'insertion voulez-vous effectuer ?\n\n");
        printf("1) Insertion sans prise en compte de plusieurs occurences du même élément\n");
        printf("2) Insertion avec la prise en compte des éléments distincts\n\n");
        scanf("%d", &choix);
        printf("\n");
        
    }while(choix > 2 || choix < 1);
    
    
    //  On récupère les caractères et leurs fréquence 
    if(choix == 1){
        Tas = saisie_alphabet();
    }
    else{
        Tas = saisie_alphabet2();
    }
    
    
    //  Premier affichage du tas
    afficher_tas(Tas);
    
    //  Récupération de la racine de l'arbre
    node* racine = creer_arbre(Tas);
    
    //  Initialisation de la chaîne de caractères permettant de récupérer les fréquences des caractères
    char* str = (char*)malloc(Tas->n+1*sizeof(char));
    
    afficher_tas(Tas);
    
    printf("----------------------------------------------------------------------------\n\n");
    printf("                                 IMPRIMER ARBRE                             \n\n");
    
    //  L'affichage du tas se fait de manière infixe
    imprimer_arbre(racine);
    
    printf("\n\n");
    
    printf("----------------------------------------------------------------------------\n\n");
    printf("                                 IMPRIMER CODES                             \n\n");
    
    //  On affiche les caractères et leurs fréquences
    imprimer_codes(racine, str, 0);
    
    printf("\n\n");
    
    //  On libère la mémoire allouée pour la chaîne de caractère
    free(str);
    //  On libère le tas qui va également libérer l'arbre
    liberer_memoire_tas(Tas);
    
}



/*
*   FONCTION MAIN 
*   Lance le programme et continue tant que l'utilisateur ne choisit pas de quitter
*/
int main(){
    
    
    //  Variable permettant à l'utilisateur de choisir ce qu'il veut faire
    int choix;
    int exit = FALSE;
    
    printf("\n--------------------------------------------------------------------------------\n");
    printf("\nBIENVENUE DANS LE PROGRAMME DE CODAGE REALISE PAR THOMAS CIMAN ET ARTHUR LEGOFF\n");
    printf("\n--------------------------------------------------------------------------------\n");
    //  Le programme continu tant que l'utilisateur ne choisit pas de quitter
    while(exit == FALSE){
        do{

            printf("\n\t\t\tQue voulez-vous faire ?\n\n");
            printf("1) Codage par saisie manuelle\n");
            printf("2) Codage par fichier\n");
            printf("3) Quitter\n\n");

            scanf("%d", &choix);
            if(choix == 1){
                printf("\n\t\t\tVOUS AVEZ CHOISI LE CODAGE MANUEL\n\n\n");
                codage_manuel();
                printf("\n\n\n\n");
            }
            if(choix == 2){
                printf("\n\t\t\tVOUS AVEZ CHOISI LE CODAGE PAR FICHIER\n\n\n");
                codage_fichier();
                printf("\n\n\n\n");
            }
            if(choix == 3){
                printf("\nNous vous remercions d'avoir utilisé notre programme\n\n");
                exit = TRUE;
            }
            //  Le panel de choix que peut réaliser l'utilisateur est compris entre 1 et 3 inclus
        }while((choix > 3) || (choix < 1));
    }
    return EXIT_SUCCESS;
}




/*
*                                           RAPPORT
*/

/*
    Nous avons implémenté les principales fonctions demandées. Nous avons également crée un script compiler.bash afin de
    rendre plus rapide et pratique la compilation et l'execution du programme. 
    La fonction alphabet2 a été implémenté afin de gérer l'insertion de plusieurs caractères identiques mais l'utilisateur
    garde le choix de ne pas utiliser ce mode d'insertion, plus gourmand
    
    La fonction existe() va renvoyer l'indice ou se trouve le caractère déjà présent dans le tableau
    
    La fonction codage_manuel() va être lancé depuis le main et va permettre à l'utilisateur de rentrer manuellement les 
    caractères et leurs fréquence tout en gardant le choix du mode d'insertion
    
    La fonction codage_fichier() va permettre à l'utilisateur de récupérer les codes de caractères dans un fichier et
    de les inserer dans un tas min à l'aide de la fonction nombre_caractere_fichier()
    Un amélioration possible serait de recoder en binaire le fichier en entré dans un autre fichier
    Pour cela il faudrait réecrir un équivalent d'imprimer code qui retourne chaque chaine de caractère
    
    La fonction afficher_tas(tas*) permet d'afficher le tas dans toutes les étapes de la création. Cela nous permet de 
    contrôler de vérifier ce qu'il se passe à chaque étape (et en plus c'est jolie)
    
    Nous nous sommes posé beaucoup de questions lors de la réalisation de ce projet par rapport à la complexité de certaines
    fonction, nous avons notamment rencontré certains dilemmes comme le choix entre allouer de la mémoire peut-être inutile
    ou augmenter la complexité de notre programme mais en controlant les allocation de mémoire.
    
    Nous aimerions vous en faire part de manière plus approfondie lors de la soutenance de projet
    
*/