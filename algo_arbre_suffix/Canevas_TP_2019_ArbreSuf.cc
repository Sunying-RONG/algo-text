/*========================================================================
Fichier Canevas_TP_2019_ArbreSuf.cc 
Cours d'algorithmique du texte HMIN121M - Universit� de Montpellier
Ann�e 2019-20

Cr�� par S�verine B�rard, modifi� par VOTRE_NOM_ICI
=========================================================================*/
#include <iostream>
#include <fstream> //pour lire les fichiers
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>

#define ALPHABET 255

using namespace std;


struct Noeud{
   int id; // un identifiant
   string etiquette; //l'�tiquette de l'arc entrant sur ce noeud
   int numero; // dans le cas des feuilles >0 et 0 pour les noeuds internes 
   struct Noeud * fils[ALPHABET+1]; //arcs sortant, rang�s selon la 1re lettre de l'�tiquette
};

struct Noeud * ArbreSuf(string T,int n){
   cout<<"\t Fonction ArbreSuf"<<endl;

   int cpt=0;

   //Cr�ation d'un premier noeud racine
   struct Noeud * Racine= new struct Noeud;
   Racine->id=cpt;
   cpt++;
   Racine->etiquette="RACINE";
   Racine->numero=0;
   for(int i=0; i<=ALPHABET;i++) {
      Racine->fils[i]=NULL;
   }
   //� compl�ter
   struct Noeud * N1 = new struct Noeud;
   N1 -> id = cpt;
   N1 -> etiquette = T.substr(cpt-1); // "ananas$"
   N1 -> numero = cpt;
   for(int i=0; i<=ALPHABET;i++) {
      N1 -> fils[i] = NULL;
   }
   Racine -> fils[int(N1 -> etiquette[0])] = N1;
   cpt++;

   struct Noeud * N2 = new struct Noeud;
   N2 -> id = cpt;
   N2 -> etiquette = T.substr(cpt-1); // "nanas$"
   N2 -> numero = cpt;
   for(int i=0; i<=ALPHABET;i++) {
      N2 -> fils[i] = NULL;
   }
   Racine -> fils[int(N2 -> etiquette[0])] = N2;
   cpt++;

   struct Noeud * N3 = new struct Noeud;
   N3 -> id = cpt;
   N3 -> etiquette = "ana";
   N3 -> numero = 0;
   for(int i=0; i<=ALPHABET;i++) {
      N3 -> fils[i] = NULL;
   }
   Racine -> fils[int(N3 -> etiquette[0])] = N3;
   cpt++;

   N1->etiquette = "nas$";
   N3->fils[int(N1->etiquette[0])]=N1;

   struct Noeud * N4 = new struct Noeud;
   N4 -> id = cpt;
   N4 -> etiquette = "s$";
   N4 -> numero = cpt;
   for(int i=0; i<=ALPHABET;i++) {
      N4 -> fils[i] = NULL;
   }
   N3 -> fils[int(N4->etiquette[0])] = N4;
   
   cpt++;
   // struct Noeud * N3 = new struct Noeud;
   // N3 -> id = cpt;
   // N3 -> etiquette = T.substr(cpt);

   // for(int i=0; i<=N1->etiquette.length(); i++) {
   //    if(N3->etiquette[i].compare(N1->etiquette[i]) == 0) {

   //    }
   // }
   // N3 -> numero = cpt;
   // for(int i=0; i<=ALPHABET;i++) {
   //    N3 -> fils[i] = NULL;
   // }
   // Racine -> fils[int(N3 -> etiquette[1])] = N3;
   // cpt++;





   return Racine;
}
   
   
void mauvaiseUtilisation(string NomProg)
{
   cout<<"Mauvaise utilisation du programme "<<NomProg<<", on attend :"<<endl;
   cout<<NomProg<<" ficTexte"<<endl;
   exit(1);
}

int lectureFic(string nomFic, string &S, int &t)
{
   ifstream IfFic (nomFic.c_str(), ios::in);
   if (!IfFic)
      {
	 cout<<endl<<"Impossible d'ouvrir le fichier : "<<nomFic<<endl<<endl;
	 return(0);
      }
   char c;
   //On lit le fichier caract�re par caract�re
   while(IfFic.get(c)){
	    S += c;
   }
   //On �limine l'�ventuel retour � la ligne en fin de fichier
   if (S.size() >1 && (int)S[S.size()-1]==10)
      S=S.substr(0,S.size()-1);
   
   S=S+'$'; //On ajoute le caract�re sentinelle � la fin et un caract�re # pour que la premi�re lettre soit � la pos 1
   t=S.size()-1;
   return(1);
}

void Affichage (struct Noeud * R, int i){
   for(int k=0;k<i;k++) {
      cout<<"\t";
   }
   if (R->numero==0) {
      cout<<"Noeud";
   }
   else {
      cout<<"Feuille n�"<<R->numero;
   }
   cout<<" id="<<R->id<<" etiq=|"<<R->etiquette<<"|";
   if (R->numero==0){
      cout<<" de fils :"<<endl;
      for(int j=0;j<ALPHABET+1;j++) {
         if (R->fils[j]!=NULL){
            for(int k=0;k<=i;k++) {
               cout<<"\t";
            }
            cout<<(char)j<<endl;
            Affichage (R->fils[j],i+1);
         }
      }
   }
   else cout<<endl;
}

string ajouteDot(int id, struct Noeud * N){
   //Arc id -> N
   string dot="";
   stringstream sa,sb;
   sa<<id;
   dot+=sa.str();
   dot+=" -> ";
   sb<<N->id;
   dot+=sb.str();
   dot+=" [label=\"";
   dot+=N->etiquette;
   dot+="\"]\n";
   return dot;
}

string dotRec (struct Noeud * R){
   string dot="";
   stringstream s_id,s_num;
   s_id<<R->id;
   if (R->numero!=0) //c'est une feuille, on affiche son num�ro dans un cercle
      {
	 dot+=s_id.str();
	 dot+=" [label=\"";
	 s_num<<R->numero;
	 dot+=s_num.str();
	 dot+="\", shape=circle, color=blue, fontcolor=black]\n";
      }
   else //c'est un neoud interne, son label est mis � *
      {
	 dot+=s_id.str();
	 dot+=" [label=\"*\", shape=square, color=black, fontcolor=black]\n";
	 for(int j=0;j<ALPHABET+1;j++)
	 if (R->fils[j]!=NULL){
	    dot+=dotRec(R->fils[j]);
	    dot+=ajouteDot(R->id,R->fils[j]);
	 }
      }
   return dot;
}

void Dot (struct Noeud * R, string nom){
   string dot="digraph G {\n";
   dot+=dotRec(R);
   dot+="}";
   string nom_dot = nom.append(".dot");
   ofstream f (nom_dot.c_str());
   
   if (!f.is_open())
      cout << "Impossible d'ouvrir le fichier "<<nom_dot<<" en �criture !" << endl;
   else
      f << dot << endl;
   f.close();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// main ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
   string T;
   int n;
   if (argc != 2){
      mauvaiseUtilisation(argv[0]);
   }
   else{
      /* Lecture du texte */
      string nomFicTexte(argv[1]);
      lectureFic(nomFicTexte,T,n);

      /* Cr�ation de l'arbre des suffixe */
      struct Noeud * Racine;
      Racine=ArbreSuf(T,n);

      /* Affichage et cr�ation du fichier dot � utiliser : dot -Tpdf monfic.dot -o monfic.pdf*/
      Affichage (Racine, 0);
      Dot(Racine, nomFicTexte);
   }
}
