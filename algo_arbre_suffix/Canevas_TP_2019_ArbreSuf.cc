/*========================================================================
Fichier Canevas_TP_2019_ArbreSuf.cc 
Cours d'algorithmique du texte HMIN121M - Université de Montpellier
Année 2019-20

Créé par Sèverine Bérard, modifié par VOTRE_NOM_ICI
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
   string etiquette; //l'étiquette de l'arc entrant sur ce noeud
   int numero; // dans le cas des feuilles >0 et 0 pour les noeuds internes 
   struct Noeud * fils[ALPHABET+1]; //arcs sortant, rangés selon la 1re lettre de l'étiquette
};

struct Noeud * ArbreSuf(string T,int n){
   cout<<"\t Fonction ArbreSuf"<<endl;

   int cpt=0;

   //Création d'un premier noeud racine
   struct Noeud * Racine= new struct Noeud;
   Racine->id=cpt;cpt++;
   Racine->etiquette="RACINE";
   Racine->numero=0;
   for(int i=0; i<=ALPHABET;i++) Racine->fils[i]=NULL;

   //À compléter
   
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
   //On lit le fichier caractère par caractère
   while(IfFic.get(c)){
	    S += c;
   }
   //On élimine l'éventuel retour à la ligne en fin de fichier
   if (S.size() >1 && (int)S[S.size()-1]==10)
      S=S.substr(0,S.size()-1);
   
   S='#'+S+'$'; //On ajoute le caractère sentinelle à la fin et un caractère # pour que la première lettre soit à la pos 1
   t=S.size()-1;
   return(1);
}

void Affichage (struct Noeud * R, int i){
   for(int k=0;k<i;k++)
      cout<<"\t";
   if (R->numero==0)
      cout<<"Noeud";
   else
      cout<<"Feuille n°"<<R->numero;
   cout<<" id="<<R->id<<" etiq=|"<<R->etiquette<<"|";
   if (R->numero==0){
      cout<<" de fils :"<<endl;
      for(int j=0;j<ALPHABET+1;j++)
	 if (R->fils[j]!=NULL){
	    for(int k=0;k<=i;k++)
	       cout<<"\t";
	    cout<<(char)j<<endl;
	    Affichage (R->fils[j],i+1);
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
   if (R->numero!=0) //c'est une feuille, on affiche son numéro dans un cercle
      {
	 dot+=s_id.str();
	 dot+=" [label=\"";
	 s_num<<R->numero;
	 dot+=s_num.str();
	 dot+="\", shape=circle, color=blue, fontcolor=black]\n";
      }
   else //c'est un neoud interne, son label est mis à *
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
      cout << "Impossible d'ouvrir le fichier "<<nom_dot<<" en écriture !" << endl;
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

      /* Création de l'arbre des suffixe */
      struct Noeud * Racine;
      Racine=ArbreSuf(T,n);

      /* Affichage et création du fichier dot à utiliser : dot -Tpdf monfic.dot -o monfic.pdf*/
      Affichage (Racine, 0);
      Dot(Racine, nomFicTexte);
   }
}
