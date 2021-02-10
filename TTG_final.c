#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <MLV/MLV_all.h>

//Création de la structure permettant de stocker un token
typedef struct token{

	int couleur;
	int forme; 
	struct token* suivant;
	struct token* suivant_couleur;
	struct token* precedant_couleur;
	struct token* suivant_forme;
	struct token* precedant_forme;

}Tokens, *Liste;


//Alloue la mémoire pour un token dont les valeurs (couleur/forme) sont initialisées avec les arguments fournis
Liste alloue_cellule(int forme_alea, int couleur_alea){ 
	Liste tmp;
	tmp = (Tokens *)malloc(sizeof(Tokens));
	if (tmp != NULL){
		tmp->couleur = couleur_alea;
		tmp->forme = forme_alea;
		tmp->suivant=NULL;
		tmp->suivant_couleur=NULL;
		tmp->precedant_couleur=NULL;
		tmp->suivant_forme=NULL;
		tmp->precedant_forme=NULL;
	}
	return tmp;
}

//Génère un token aléatoire
Liste token_aleatoire(){
	int forme_alea = rand() %4;
	int couleur_alea = rand() %4;
	Liste tmp = alloue_cellule(forme_alea,couleur_alea);
	return tmp;
}

//Insère un token en fin de liste si position vaut 1, sinon, en début de liste
Liste insere_simple(Liste *liste_token, Liste nouveau_token){
	if (nouveau_token != NULL){
		nouveau_token->suivant = *liste_token;
		*liste_token = nouveau_token;
	}
	return nouveau_token;
}

//Affiche la liste chainée des tokens sur le terminal
void affiche_liste(Liste liste_token){

	char couleur = '!';
	char forme = '!';

	for(;liste_token != NULL; liste_token = liste_token->suivant){

		switch(liste_token->forme){
			case 0: forme = 'O';
			break;
			case 1: forme = 'C';
			break;
			case 2: forme = 'T';
			break;
			case 3: forme = 'D';
			break;
		}

		switch(liste_token->couleur){
			case 0: couleur = 'r';
			break;
			case 1: couleur = 'v';
			break;
			case 2: couleur = 'b';
			break;
			case 3: couleur = 'j';
			break;
		}

		printf(" %c %c -->",forme, couleur);
	}
	printf(">\n");
}

//Affiche la liste chainée des tokens sur le terminal
void affiche_liste_circ(Liste liste_token){

	char couleur = '!';
	char forme = '!';

	Liste index = liste_token;
	for(;liste_token->suivant != index; liste_token = liste_token->suivant){

		switch(liste_token->forme){
			case 0: forme = 'O';
			break;
			case 1: forme = 'C';
			break;
			case 2: forme = 'T';
			break;
			case 3: forme = 'D';
			break;
		}

		switch(liste_token->couleur){
			case 0: couleur = 'r';
			break;
			case 1: couleur = 'v';
			break;
			case 2: couleur = 'b';
			break;
			case 3: couleur = 'j';
			break;
		}

		printf(" %c %c -->",forme, couleur);
	}
	switch(liste_token->forme){
			case 0: forme = 'O';
			break;
			case 1: forme = 'C';
			break;
			case 2: forme = 'T';
			break;
			case 3: forme = 'D';
			break;
	}

	switch(liste_token->couleur){
		case 0: couleur = 'r';
		break;
		case 1: couleur = 'v';
		break;
		case 2: couleur = 'b';
		break;
		case 3: couleur = 'j';
		break;
	}
	printf(" %c %c -->>\n",forme,couleur);
}

// Compte le nombre d'éléments dans une listes
int compteListe(Liste lst){
	if (lst == NULL)
		return 0;
	int compteur=0;
	Liste index = lst;
	do{
		index=index->suivant;
		compteur+=1;
	}
	while(index!=lst);

	return compteur;
}

//Créé une liste de (taille_liste) tokens aléatoires au début de la partie
Liste cree_liste_aleatoire(int taille_liste){
	Liste liste_token = token_aleatoire();//premier élément de la liste chainée
	int i = 0;

	for(i=0; i<taille_liste-1; i++){
		Liste nouveau_token = token_aleatoire();
		insere_simple(&liste_token, nouveau_token);
	}
	return liste_token;
}

//Prend le derniere élément de liste_token et le deporte à la fin ou au debut de liste_token_jeu en foncion de dir
Liste insere_jeu(Liste *liste_token, Liste *liste_token_jeu, int dir){

	Liste index = *liste_token;
	for(;index->suivant->suivant!=NULL;index=index->suivant){
	}

	if (*liste_token_jeu != NULL){
		Liste index_jeu = *liste_token_jeu;
		for(;index_jeu->suivant!=*liste_token_jeu;index_jeu=index_jeu->suivant){
		}

		(index->suivant)->suivant = *liste_token_jeu; // element extrait pointe sur premier de liste_token_jeu
		index_jeu ->suivant = index->suivant; // Dernier element de liste_token_jeu pointe vers l'élément extrait

		if(dir == 0){ //insere en debut de liste
			*liste_token_jeu = index->suivant;
		}
	}

	else{
		index->suivant->suivant = index->suivant;
		*liste_token_jeu = index->suivant;
	}

	index->suivant=NULL;//(avant)dernier element de liste_token pointe sur null
	return *liste_token_jeu;
}


void supprime_combo(Liste *premier, int compteur, int n){
	Liste index=*premier;
	if(n<compteur){
		supprime_combo(&(index->suivant), compteur, n+=1);
		free(index);
	}
}


int repere_combo_couleur(Liste *liste_token_jeu){
	if (*liste_token_jeu==NULL)
		return 0;
	int couleur = 0;
	for (couleur=0; couleur<4; couleur++){ //Teste pour les 4 couleurs

		Liste index = *liste_token_jeu;
		Liste premier = NULL;
		Liste dernier  = NULL;
		int compteur = 0;
		for(;index->suivant!=*liste_token_jeu;index=index->suivant){
			if(index->couleur!=couleur && compteur<3){ // Cas 1&3
				compteur = 0;
				premier=index;
			}
			if(index->couleur==couleur){
				compteur += 1;
			}
			if((index->couleur!=couleur && compteur>=3 && premier!=NULL) || (index->suivant->suivant==*liste_token_jeu && index->suivant->couleur!=couleur && compteur>=3 && premier!=NULL)) {//Fin de triplet ou plus
				dernier = index;
				supprime_combo(&premier->suivant,compteur,0);//On supprime le combo
				premier->suivant = dernier;//On raccorde les éléments entre eux
				return 1;
				//break;
			}
			if(index->couleur!=couleur && compteur>=3 && premier==NULL){ //Fin de triplet ou plus
				premier = index;
				for (;index->suivant!=*liste_token_jeu;index=index->suivant){
				}
				dernier = index;
				supprime_combo(&dernier->suivant,compteur,0);//On supprime le combo
				*liste_token_jeu = premier;//On redefinit le premier element de la liste
				dernier->suivant = premier;//On raccorde le dernier élément au premier
				return 1;
				//break;
			}
			if (index->suivant->suivant==*liste_token_jeu && compteur>=3 && premier==NULL){
				dernier = index->suivant;
				supprime_combo(&dernier->suivant,compteur,0);//On supprime le combo
				dernier->suivant = dernier;//On raccorde le dernier élément au premier
				*liste_token_jeu = dernier;//On redefinit le premier element de la liste
				return 1;
				//break;

			}
		}
		if (compteur>=2 && index->couleur==couleur){
			compteur+=1;
			if (premier == NULL && dernier == NULL){
				Liste cas4 = *liste_token_jeu;
				supprime_combo(&cas4,compteur,0);
				*liste_token_jeu=NULL;
				return 1;
			}
			else{
				supprime_combo(&premier->suivant,compteur,0);
				premier->suivant=*liste_token_jeu;
				return 1;
			}

		}
	}
	return 0;
}

int repere_combo_forme(Liste *liste_token_jeu){
	if (*liste_token_jeu==NULL)
		return 0;

	int forme = 0;
	for (forme=0; forme<4; forme++){ //Teste pour les 4 couleurs

		Liste index = *liste_token_jeu;
		Liste premier = NULL;
		Liste dernier  = NULL;
		int compteur = 0;
		for(;index->suivant!=*liste_token_jeu;index=index->suivant){
			if(index->forme!=forme && compteur<3){ // Cas 1&3
				compteur = 0;
				premier=index;
			}
			if(index->forme==forme){
				compteur += 1;
			}
			if((index->forme!=forme && compteur>=3 && premier!=NULL) || (index->suivant->suivant==*liste_token_jeu && index->suivant->forme!=forme && compteur>=3 && premier!=NULL)) {//Fin de triplet ou plus
				dernier = index;
				supprime_combo(&premier->suivant,compteur,0);//On supprime le combo
				premier->suivant = dernier;//On raccorde les éléments entre eux
				return 1;
				//break;
			}
			if(index->forme!=forme && compteur>=3 && premier==NULL){ //Fin de triplet ou plus
				premier = index;
				for (;index->suivant!=*liste_token_jeu;index=index->suivant){
				}
				dernier = index;
				supprime_combo(&dernier->suivant,compteur,0);//On supprime le combo
				*liste_token_jeu = premier;//On redefinit le premier element de la liste
				dernier->suivant = premier;//On raccorde le dernier élément au premier
				return 1;
				//break;
			}
			if (index->suivant->suivant==*liste_token_jeu && compteur>=3 && premier==NULL){
				dernier = index->suivant;
				supprime_combo(&dernier->suivant,compteur,0);//On supprime le combo
				dernier->suivant = dernier;//On raccorde le dernier élément au premier
				*liste_token_jeu = dernier;//On redefinit le premier element de la liste
				return 1;
				//break;

			}
		}
		if (compteur>=2 && index->forme==forme){
			compteur+=1;
			if (premier == NULL && dernier == NULL){
				Liste cas4 = *liste_token_jeu;
				supprime_combo(&cas4,compteur,0);
				*liste_token_jeu=NULL;
				return 1;
			}
			else{
				supprime_combo(&premier->suivant,compteur,0);
				premier->suivant=*liste_token_jeu;
				return 1;
			}

		}
	}
	return 0;
}


int chainage_forme_couleur(Liste *liste_token_jeu){
	if(*liste_token_jeu==NULL){
		return 0;
	}

	Liste p1=*liste_token_jeu; //on initialise tout nos pointeurs sur la premiere position de la liste
	Liste p2=*liste_token_jeu;
	Liste p3=*liste_token_jeu;
	Liste index=*liste_token_jeu;
	int trouve=0;
	int forme=0;
	int couleur=0;

	for(forme=0;forme<4;forme++){ //On verifie pour toutes les formes
		p1=*liste_token_jeu; //on initialise tout nos pointeurs sur la premiere position de la liste
		p2=*liste_token_jeu;
		p3=*liste_token_jeu;
		index=*liste_token_jeu;
		trouve=0;

		do{
			if(index->forme==forme){
				trouve=1;
				p1=index;
				break;
			}
			index=index->suivant;
		}while(index!=*liste_token_jeu);
		
		if(trouve==1){
			p2=p1;
			p3=p1;
			do{
				p2=p2->suivant;
				if(p2->forme==forme){
					p1->suivant_forme=p2;
					p2->precedant_forme=p1;
					p1=p2;
				}
			}while(p2!=p3);
		}
	}

	//boucle de couleur
	for(couleur=0;couleur<4;couleur++){ //On verifie pour toutes les couleur
		p1=*liste_token_jeu; //on initialise tout nos pointeurs sur la premiere position de la liste
		p2=*liste_token_jeu;
		p3=*liste_token_jeu;
		index=*liste_token_jeu;
		trouve=0;

		do{
			if(index->couleur==couleur){
				trouve=1;
				p1=index;
				break;
			}
			index=index->suivant;
		}while(index!=*liste_token_jeu);
		
		if(trouve==1){
			p2=p1;
			p3=p1;
			do{
				p2=p2->suivant;
				if(p2->couleur==couleur){
					p1->suivant_couleur=p2;
					p2->precedant_couleur=p1;
					p1=p2;
				}
			}while(p2!=p3);
		}
	}
	return 0;
}

void echange_forme(Liste * liste_token_jeu, int forme_token){
	Liste p1=*liste_token_jeu;
	Liste index=*liste_token_jeu;
	int couleur_token;

	while(p1->forme != forme_token){
		p1 = p1->suivant;
	}

	index = p1;
	couleur_token = p1->couleur;
	while( p1 != index->suivant_forme){
		p1->couleur = (p1->precedant_forme)->couleur;
		p1 = p1->precedant_forme;
	}
	p1->couleur=couleur_token;
	chainage_forme_couleur(&(*liste_token_jeu));
}



void echange_couleur(Liste * liste_token_jeu, int couleur_token){
	Liste p1=*liste_token_jeu;
	Liste index=*liste_token_jeu;
	int forme_token;

	while(p1->couleur != couleur_token){
		p1 = p1->suivant;
	}

	index = p1;
	forme_token = p1->forme;
	while( p1 != index->suivant_couleur){
		p1->forme = (p1->precedant_couleur)->forme;
		p1 = p1->precedant_couleur;
	}
	p1->forme=forme_token;
	chainage_forme_couleur(&(*liste_token_jeu));
}


int verif_parametre(int argc,char *argv[]){

	if(argc>2){ //si trop d'argument return erreur 
		return 0;
	}

	if(*argv[1]=='a'){ //si ascii retourne ascii
		return 1;
	}

	else if(*argv[1]=='g'){ //si graphique, retourne graphique
		return 2;
	}

	else{ //sinon, retourne erreur
		return 0;
	}
}


int affiche_liste_g(Liste liste_token, int hauteur, int largeur, int mode, int nb_max){

	if (liste_token == NULL){
		return 0;
	}

    int vert[4] = {98,193,125,255};
    int rouge[4] = {230,115,62,255};
    int bleu[4] = {116,161,212,255};
    int jaune[4] = {223,220,124,255};
    int *couleur = NULL;
    int ecart; 
    int rayon = (hauteur+largeur)/60;
	int nb_token = 1;
	int hauteur_t = hauteur/3;
	int ecart_token=largeur/10;
	int espace;

	if(nb_max%2==0){
		espace = (nb_max/2)*-(ecart_token/2);
	}
	else{
		espace = (nb_max/2)*-ecart_token;
	}

	Liste arret = NULL;

	if (mode!=0){//detecte que l'on veut afficher une liste circulaire
		hauteur_t*=2;
		arret = liste_token;
	}

	do{

		if (mode == 0)
			//ecart = largeur/((hauteur+largeur)/250)+(3*(nb_token*((hauteur+largeur)/60)));
			ecart = nb_token*((largeur/2)/5);
		else{
			if (nb_token == 1 && liste_token->suivant==liste_token)
				ecart = largeur/2;
			else 
				if(nb_max%2==0)
					ecart = largeur/2-espace+((ecart_token/2)*((nb_max/2)-1));
				else
					ecart = largeur/2-espace;
		}

		if (nb_token==5 && mode ==0){
			rayon*=1.5;
		}

		int x_t[3] = {ecart-rayon,ecart,ecart+rayon};
		int y_t[3] = {hauteur_t+rayon,hauteur_t-rayon,hauteur_t+rayon};
		int x_d[4] = {ecart,ecart+rayon,ecart,ecart-rayon};
		int y_d[4] = {hauteur_t-rayon,hauteur_t,hauteur_t+rayon,hauteur_t};

		switch(liste_token->couleur){
			case 0: couleur = rouge;
				break;
			case 1:  couleur = vert;
				break;
			case 2: couleur = bleu;
				break;
			case 3: couleur = jaune;
				break;
		}

		switch(liste_token->forme){

			case 0: MLV_draw_filled_circle(ecart,hauteur_t,rayon,MLV_rgba(couleur[0],couleur[1],couleur[2],couleur[3]));
				break;
			case 1: MLV_draw_filled_rectangle(ecart-rayon,hauteur_t-rayon,2*rayon,2*rayon,MLV_rgba(couleur[0],couleur[1],couleur[2],couleur[3]));
				break;
			case 2: MLV_draw_filled_polygon(x_t,y_t,3,MLV_rgba(couleur[0],couleur[1],couleur[2],couleur[3]));
				break;
			case 3: MLV_draw_filled_polygon(x_d,y_d,4,MLV_rgba(couleur[0],couleur[1],couleur[2],couleur[3]));
				break;
		}

		nb_token+=1;
		liste_token = liste_token->suivant;

		espace+=ecart_token;


	}while(liste_token != arret);


	MLV_actualise_window();
	return 0;
}

void cree_bouton(int largeur, int hauteur,int *x1_hg,int *y1_hg,int *x1_bd, int *y1_bd,int *x2_hg,int *y2_hg,int *x2_bd, int *y2_bd){
	*x1_hg = ((largeur/2)-2*(hauteur+largeur)/60)-((largeur+hauteur)/50);
	*y1_hg = (hauteur/2)-(hauteur+largeur)/60;
	*x1_bd = (largeur/2)-2*(hauteur+largeur)/60+(hauteur+largeur)/60;
	*y1_bd = (hauteur/2)-(hauteur+largeur)/60+(hauteur+largeur)/60;

	*x2_hg = (largeur/2)+1*(hauteur+largeur)/60;
	*y2_hg = hauteur/2-(hauteur+largeur)/60;
	*x2_bd = ((largeur/2)+1*(hauteur+largeur)/60 +(hauteur+largeur)/60)+((largeur+hauteur)/50);
	*y2_bd = hauteur/2-(hauteur+largeur)/60 +(hauteur+largeur)/60;


	MLV_Font *font = MLV_load_font("images2/subnose.otf" , 36);
	char fleche_d[] = "->";
	char fleche_g[] = "<-";
	
	int taille = ((hauteur+largeur)/60)+((largeur+hauteur)/50);
	int taille2 = (hauteur+largeur)/60;
	int taille3 = ((hauteur+largeur)/60)+((largeur+hauteur)/50);
	int taille4 = (hauteur+largeur)/60;


	MLV_draw_text_box_with_font(*x1_hg, *y1_hg, taille, taille2, fleche_g, font, 0,
		MLV_COLOR_BLACK,
		MLV_COLOR_BLACK,
		MLV_rgba(255,255,190,255),
		MLV_TEXT_LEFT,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER
	);


	MLV_draw_text_box_with_font(*x2_hg, *y2_hg, taille3, taille4, fleche_d, font, 0,
		MLV_COLOR_BLACK,
		MLV_COLOR_BLACK,
		MLV_rgba(255,255,190,255),
		MLV_TEXT_LEFT,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER
	);

	MLV_actualise_window();
}


void detecte_clic_forme(int pos_x, int pos_y, int nb_token, int largeur, int hauteur, Liste *liste_token_jeu){
	int tableau[nb_token][4];
	int hauteur_t = 2*(hauteur/3);
	int ecart; 
    int rayon = (hauteur+largeur)/60;
	int i;
	int ecart_token=largeur/10;
	int espace;


	if(nb_token%2==0){
		espace = (nb_token/2)*-(ecart_token/2);
	}
	else{
		espace = (nb_token/2)*-ecart_token;
	}

	for(i=0;i<nb_token;i++){
		if (nb_token == 1){
			tableau[0][0] = largeur/2-rayon;
			tableau[0][1] = hauteur_t-rayon;
			tableau[0][2] = largeur/2-rayon+2*rayon;
			tableau[0][3] = hauteur_t-rayon+2*rayon;
			break;
		}

		else{
			if(nb_token%2==0)
				ecart = largeur/2-espace+((ecart_token/2)*((nb_token/2)-1));
			else
				ecart = largeur/2-espace;

			tableau[i][0] = ecart-rayon;
			tableau[i][1] = hauteur_t-rayon;
			tableau[i][2] = ecart-rayon+2*rayon;
			tableau[i][3] = hauteur_t-rayon+2*rayon;

		}
		espace+=ecart_token;
	}

	int x;
	for(x=0; x<nb_token; x++){
		if(pos_x >= tableau[x][0] && pos_x <= tableau[x][2] && pos_y >= tableau[x][1] && pos_y <= tableau[x][3] ){ //le joueur à cliqué sur un token (x=num du token)
			MLV_Font *font = MLV_load_font("images2/subnose.otf" , 50);

			MLV_draw_text_box_with_font(tableau[x][0], tableau[x][1]-(2*rayon)-3, 2*rayon, 2*rayon, "C", font, 0,
			MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_rgba(255,255,190,255), MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);

		   	MLV_draw_text_box_with_font(tableau[x][0],tableau[x][1]+(2*rayon)+3,2*rayon,2*rayon, "F", font, 0,
			MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_rgba(255,255,190,255), MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);


			//MLV_draw_rectangle(tableau[x][0], tableau[x][1]-(2*rayon)-3, 2*rayon, 2*rayon,MLV_COLOR_WHITE);
			//MLV_draw_filled_rectangle(tableau[x][0],tableau[x][1]+(2*rayon)+3,2*rayon,2*rayon,MLV_COLOR_WHITE);
			MLV_actualise_window();
			int x_pos,y_pos;
			MLV_wait_mouse(&x_pos,&y_pos);
			if(x_pos >= tableau[x][0] && x_pos <= tableau[x][0]+(2*rayon) && y_pos >= tableau[x][1]+(2*rayon)+3 && y_pos <= (tableau[x][1]+(2*rayon)+3)+(2*rayon)) {
				int numero_token = 0;
				Liste index = *liste_token_jeu;
				int forme;
				for(numero_token=0; numero_token!=x; numero_token++){
					index=index->suivant;
				}
				forme = index->forme;
				echange_forme(&(*liste_token_jeu),forme);
				break;
			}
			if(x_pos >= tableau[x][0] && x_pos <= tableau[x][0]+(2*rayon) && y_pos >= tableau[x][1]-(2*rayon)-3 && y_pos <= (tableau[x][1]-(2*rayon)-3)+(2*rayon)) {
				int numero_token = 0;
				Liste index = *liste_token_jeu;
				int couleur;
				for(numero_token=0; numero_token!=x; numero_token++){
					index=index->suivant;
				}
				couleur = index->couleur;
				echange_couleur(&(*liste_token_jeu),couleur);
				break;
			}
		}

	}

}

void cree_ecran(int largeur,int hauteur){
	MLV_create_window("THREE TO GO !","THREE TO GO !",largeur,hauteur);
}

void affiche_anim_debut(){
	int i = 97;
	char num_im[2]="";
	MLV_Image *image;
	num_im[0] = (char)i;
	for (i=97;i<118;i++){
		num_im[0] = (char)i;
		num_im[1] = '\0';
		char chemin[13] = "images/";
		strcat(chemin,num_im);
		strcat(chemin,".png");
		image = MLV_load_image( chemin );
		
		MLV_draw_image( image, 0, 0 );
		MLV_actualise_window();
		MLV_wait_milliseconds(10);
		MLV_free_image( image );

		memset(chemin,0,sizeof(chemin));

	}
	memset(num_im,0,sizeof(num_im));

	int a =0;
	int x=-1;
	int y=-1;
	for(a=0;a<9000;a++){
		if(a%2==0){
			image = MLV_load_image( "images/u.png" );
		}
		else{
			image = MLV_load_image( "images/t.png" );
		}
		MLV_draw_image( image, 0, 0 );
		MLV_actualise_window();
		MLV_wait_mouse_or_seconds(&x,&y,1);	
		if(x!=-1){
			break;
		}
		MLV_free_image( image );	
	}



}

void affiche_anim_apres(){
	int i = 97;
	char num_im[2]="";
	MLV_Image *image;
	num_im[0] = (char)i;
	for (i=97;i<112;i++){
		num_im[0] = (char)i;
		char chemin[13] ="images2/"; 
		strcat(chemin,num_im);
		strcat(chemin,".png");
		image = MLV_load_image( chemin );
		MLV_draw_image( image, 0, 0 );
		MLV_actualise_window();
		MLV_wait_milliseconds(8);
		MLV_free_image( image );
		memset(chemin,0,sizeof(chemin));

	}
	memset(num_im,0,sizeof(num_im));
}

void affiche_ecran_jeu(int largeur, int hauteur){
	largeur+=1;
	hauteur+=1;
	MLV_Image *image; 
	image = MLV_load_image( "images/fond.png" );
	MLV_draw_image( image, 0, 0 );
	MLV_actualise_window();
}


void affiche_score(int score, int largeur, int hauteur, int fin){
	char* centaine_txt;
	char* dixaine_txt;
	char* unite_txt;
	char* score_txt="score:";
	int centaine;
	int dixaine;
	int unite;

	if(score>=999){
		centaine=9;
		dixaine=9;
		unite=9;
	}
	centaine=score/100;
	dixaine=score/10;
	if(dixaine>=10){
		dixaine=dixaine/10;
	}
	unite=score%10;
	
	centaine_txt=MLV_convert_unicode_to_string(48+centaine);
	dixaine_txt=MLV_convert_unicode_to_string(48+dixaine);
	unite_txt=MLV_convert_unicode_to_string(48+unite);
	if(fin==0){
		MLV_draw_text(largeur/12,hauteur/9,score_txt,MLV_COLOR_WHITE);
		MLV_draw_text(largeur/12,hauteur/8,centaine_txt,MLV_COLOR_WHITE);
		MLV_draw_text(largeur/11,hauteur/8,dixaine_txt,MLV_COLOR_WHITE);
		MLV_draw_text(largeur/10,hauteur/8,unite_txt,MLV_COLOR_WHITE);
	}
	else{
		MLV_Font *font = MLV_load_font("images2/subnose.otf" , 50);
		MLV_draw_text_with_font((largeur/15)*8.5,hauteur/2.3,"%d",font,MLV_COLOR_WHITE,score); //centaine_txt
	}
}

void affiche_temps(int largeur, int hauteur, int dixaine, int minute, int unite){

	char* chrono_txt_unite; //mise en place du chronometre
	char* chrono_txt_dixaine;
	char* chrono_txt_minute;
	char* deux_point;
	char* chrono="temps:";
	chrono_txt_unite=MLV_convert_unicode_to_string(48+unite);
	chrono_txt_dixaine=MLV_convert_unicode_to_string(48+dixaine);
	chrono_txt_minute=MLV_convert_unicode_to_string(48+minute);
	deux_point=MLV_convert_unicode_to_string(46);

	MLV_draw_text(largeur/12,hauteur/15,chrono,MLV_COLOR_WHITE);
	MLV_draw_text(largeur/12,hauteur/12,chrono_txt_minute,MLV_COLOR_WHITE);
	MLV_draw_text(largeur/11,hauteur/12,deux_point,MLV_COLOR_WHITE);
	MLV_draw_text(largeur/10.5,hauteur/12,chrono_txt_dixaine,MLV_COLOR_WHITE);
	MLV_draw_text(largeur/10,hauteur/12,chrono_txt_unite,MLV_COLOR_WHITE);
}

int ecran_fin(int *menu_jeu, int score, int largeur, int hauteur){
	MLV_Image *image;
	image = MLV_load_image( "images2/fin2.png" );
	MLV_draw_image( image, 0, 0 );
	affiche_score(score, largeur, hauteur,1);
	MLV_actualise_window();
	MLV_Keyboard_button touche;
	while(touche!=97 || touche!=98){
		MLV_wait_keyboard( &touche, NULL, NULL);
		if (touche==98)
			return 0;
		if (touche==97)
			return 1;

	}

}

void bouton_quitter(int largeur, int hauteur){
	MLV_Font *font = MLV_load_font("images2/subnose.otf" , 36);
	char quit[] = "->>";
	MLV_draw_text_box_with_font(largeur-(hauteur+largeur)/50, hauteur-(hauteur+largeur)/50, (hauteur+largeur)/50,(hauteur+largeur)/50, quit, font, 0,
	MLV_COLOR_BLACK,
	MLV_COLOR_BLACK,
	MLV_rgba(255,255,190,255),
	MLV_TEXT_LEFT,
	MLV_HORIZONTAL_CENTER,
	MLV_VERTICAL_CENTER
	);
	MLV_actualise_window();
}




void supprime_liste(Liste *liste_jeu,int nombre){
	if(nombre!=0){
		supprime_liste(&(*liste_jeu)->suivant,nombre-1);
		free(*liste_jeu);
	}
}
//############################################ BOUCLE #########################################
int main(int argc, char *argv[]){

	int jeu=verif_parametre(argc,&(*argv));
	srand(time(NULL));//Pour l'aléatoire
	Liste liste_token = cree_liste_aleatoire(5); //Initialisationd de la liste du haut
	Liste liste_token_jeu = NULL;               //Initialisation de la liste du bas

	if(jeu==0){
		printf("erreur, choisissez 'a' ou 'g'\n");
		return 0;
	}

	else if(jeu==1){ //Boucle de jeu en ascii
		affiche_liste(liste_token);
		affiche_liste(liste_token_jeu);
		int choix=0;
		while(choix<2){
			int supprime_couleur=1;
			int supprime_forme=1;
			printf("Ou voulez vous insérer l'élément ? : ");
			scanf("%d",&choix);

			insere_jeu(&liste_token, &liste_token_jeu,choix);
			
			Liste nouveau_token = token_aleatoire();
			nouveau_token->suivant=liste_token;
			liste_token = nouveau_token;
			affiche_liste(liste_token);
			while(supprime_couleur==1){
				supprime_couleur = repere_combo_couleur(&liste_token_jeu);
			}
			while(supprime_forme==1){
				supprime_forme = repere_combo_forme(&liste_token_jeu);
			}
			if (liste_token_jeu==NULL)
				affiche_liste(liste_token_jeu);
			else
				affiche_liste_circ(liste_token_jeu);

		}
		int nombre_elem=compteListe(liste_token_jeu);
		supprime_liste(&liste_token,5);
		supprime_liste(&liste_token_jeu,nombre_elem);
	}

	else if(jeu==2){ // version graphique du jeu
		int game = 1;
		int score=0;
		int combo=0;
		int largeur = 1280;
		int hauteur = 720;
		int pos_x,pos_y;
		int x1_hg, y1_hg, x1_bd,y1_bd;
		int x2_hg, y2_hg, x2_bd,y2_bd; // valeurs permettant de detecter les clics sur les boutons
		int nb_token=0;

		int unite=0;
		int dixaine=0;
		int minute=0;
		int verif1;
		int verif2;

		cree_ecran(largeur,hauteur);

		int menu_jeu=1;
		
		affiche_anim_debut();
        affiche_anim_apres();
        MLV_init_audio();
        MLV_Music *music = MLV_load_music("sons/musique.mp3");
        MLV_Sound* clic = MLV_load_sound("sons/clic.aiff");
        MLV_Sound* efface = MLV_load_sound("sons/efface.wav");

        MLV_play_music (music, 30, 1);


		while(menu_jeu==1){ //boucle de menu
			game=1;
			score=0;
			combo=0;
			pos_x =0;
			pos_y=0;
			nb_token=0;
			unite=0;
			dixaine=0;
			minute=0;
			Liste liste_token = cree_liste_aleatoire(5); //Initialisationd de la liste du haut
			Liste liste_token_jeu = NULL;               //Initialisation de la liste du bas

			while (game==1){ // Partie lancée !

				verif1=time(NULL);
				int supprime_couleur=1; // vos 1 si un triplet de couleur à été effacée, 0 sinon
				int supprime_forme=1;

				affiche_ecran_jeu(largeur,hauteur);
				cree_bouton(largeur,hauteur,&x1_hg,&y1_hg,&x1_bd,&y1_bd,&x2_hg,&y2_hg,&x2_bd,&y2_bd);
				bouton_quitter(largeur,hauteur);
				affiche_liste_g(liste_token, hauteur, largeur,0,5);
				nb_token = compteListe(liste_token_jeu);
				affiche_liste_g(liste_token_jeu, hauteur, largeur,1,nb_token);
				chainage_forme_couleur(&liste_token_jeu);

				if(unite==10){
					unite=0;
					dixaine+=1;
				}
				if(dixaine==6){
					minute+=1;
					dixaine=0;
				}

				
				affiche_temps(largeur, hauteur, dixaine, minute, unite);
				affiche_score(score, largeur, hauteur,0);
				MLV_actualise_window();

				pos_x=0;
				pos_y=0;
				MLV_wait_mouse_or_seconds(&pos_x, &pos_y,1);

				detecte_clic_forme(pos_x, pos_y, nb_token, largeur, hauteur, &liste_token_jeu);

				if(nb_token<10){

					if (pos_x >= x1_hg && pos_x<= x1_bd && pos_y >= y1_hg && pos_y<= y1_bd){ //clic sur le bouton gauche
						MLV_play_sound(clic, 30);
						insere_jeu(&liste_token, &liste_token_jeu,1);
						chainage_forme_couleur(&liste_token_jeu);
						Liste nouveau_token = token_aleatoire();
						nouveau_token->suivant=liste_token;
						liste_token = nouveau_token;
					}
					else if(pos_x >= x2_hg && pos_x<= x2_bd && pos_y >= y2_hg && pos_y<= y2_bd){ //clic sur le bouton droit
						MLV_play_sound(clic, 30);
						insere_jeu(&liste_token, &liste_token_jeu,0);
						chainage_forme_couleur(&liste_token_jeu);
						Liste nouveau_token = token_aleatoire();
						nouveau_token->suivant=liste_token;
						liste_token = nouveau_token;
					}
				}

				if (pos_x >= largeur-(hauteur+largeur)/50 && pos_y >= hauteur-(hauteur+largeur)/50)
					game=0;

				while(supprime_couleur==1){
					supprime_couleur = repere_combo_couleur(&liste_token_jeu); //boucle permettant de gérer les combo de couleur
					if(supprime_couleur==1){
						MLV_play_sound(efface, 5);
						combo+=1;
					}
				}
				while(supprime_forme==1){
					supprime_forme = repere_combo_forme(&liste_token_jeu); //boucle permettant de gérer les combo de forme
					if(supprime_forme==1){
						MLV_play_sound(efface, 5);
						combo+=1;
					}
				}
				if(combo>=2){
					score+=combo*combo;
				}
				else if(combo==1){
					score+=1;
				}

				combo=0;
				if (minute>=2)
					game=0;
				verif2=time(NULL);
				if(verif1!=verif2){
					unite+=1;
				}


				MLV_clear_window(MLV_COLOR_BLACK);

			}
			menu_jeu=ecran_fin(&menu_jeu, score, largeur, hauteur);
			MLV_clear_window(MLV_COLOR_BLACK);

			int nombre_elem=compteListe(liste_token_jeu);
			supprime_liste(&liste_token,5);               //suppresion des listes de tokens
			supprime_liste(&liste_token_jeu,nombre_elem);
		}

	}
	
	return 0;
}











