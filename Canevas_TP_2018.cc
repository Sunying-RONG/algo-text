/*========================================================================
Fichier Canevas_TP.cc 
Cours d'algorithmique du texte HMSN116M - Universit� de Montpellier
Ann�e 2017-18

Cr�� par S�verine B�rard, modifi� par VOTRE_NOM_ICI
=========================================================================*/
#include <iostream>
#include <fstream> //pour lire les fichiers
#include <string>
#include <map> //pour les tableaux associatifs
#include <unordered_set>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// NA�F ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void naif(string P, int m, string T, int n)
{
	int pos=1;
	int nb=0;
	while (pos<=n-m+1) 
	{
		int i=1;
		while (i<=m && P[i]==T[pos+i-1])
		{
			i=i+1;
			nb=nb+1;
		}
		if (i==m+1) 
		{
			cout<<"P apparait a la position "<<pos<<endl;
		} else 
		{
			nb=nb+1;
		}
		pos=pos+1;
	}
	cout<<"le nombre de comparaisons de caracteres reellement effectuees : "<<nb<<endl;
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// MP /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int * calcule_MP_next(string P, int m)
{
	// * zone memoire pas efface
   int * MP_next = new int [m+2]; // nombre de case m+2
   MP_next[1]=0;
   int j=0;
   for (size_t i = 1; i <= m; i++)
   {
	   while (j>0 && P[i]!=P[j])
	   {
		   j=MP_next[j];
	   }
	   j++;
	   MP_next[i+1]=j;
   }
   for (size_t i = 1; i <= m+1; i++) // indice m+1 sur case m+2
   {
	   cout<<"MP_next["<<i<<"] = "<<MP_next[i]<<endl;
   }
   return MP_next;
}

void MP(string P, int m, string T, int n)
{
	int * MP_next = calcule_MP_next(P, m);
	int i=1;
	int j=1;
	int nb=0;
	while (j<=n)
	{
		while (i>0 && P[i]!=T[j])
		{
			i=MP_next[i];
			nb++;
			cout<<"non égale  "<<T[j]<<endl;
		}
		if (i>0 && P[i]==T[j]) {
			nb++;
			cout<<"égale  "<<T[j]<<endl;
		}
		i=i+1;
		j=j+1;
		
		if (i==m+1) 
		{
			cout<<"P apparait a la position "<<j-i+1<<endl;
			i=MP_next[i];
		}
		
	}
   	cout<<"le nombre de comparaisons de caracteres reellement effectuees : "<<nb<<endl;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// KMP /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int * calcule_KMP_next(string P, int m)
{
   int * KMP_next = new int [m+2];
   KMP_next[1]=0;
   int j=0;
   for (size_t i = 1; i <= m; i++)
   {
	   while (j>0 && P[i]!=P[j])
	   {
		   j=KMP_next[j];
	   }
	   j++;
	   if (i==m || P[i+1]!=P[j]) {
		   KMP_next[i+1]=j;
	   } else {
		   KMP_next[i+1]=KMP_next[j];
	   }
   }
   for (size_t i = 1; i <= m+1; i++)
   {
	   cout<<"KMP_next["<<i<<"] = "<<KMP_next[i]<<endl;
   }
   return KMP_next;
}

void KMP(string P, int m, string T, int n)
{
   int * KMP_next = calcule_KMP_next(P, m);
   int i=1;
   int j=1;
   int nb=0;
   while (j<=n)
   {
	    while (i>0 && P[i]!=T[j])
		{
			i=KMP_next[i];
			nb++;
			cout<<"non égale  "<<T[j]<<endl;
		}
		if (i>0 && P[i]==T[j])
		{
			nb++;
			cout<<"égale  "<<T[j]<<endl;
		}
		i++;
		j++;
		
		if (i==m+1) 
		{
			cout<<"P apparait a la position "<<j-i+1<<endl;
			i=KMP_next[i];
		}
   }
   cout<<"le nombre de comparaisons de caracteres reellement effectuees : "<<nb<<endl;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Boyer-Moore ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int * calcule_suff(string P, int m)
{
	int * suff = new int [m + 1];
	suff[m] = m;
	int g = m;
	int f;
	for (int i = m - 1; i >= 1; i--)
	{
		if (i > g && suff[i + m - f] != (i - g))
		{
			suff[i] = min(suff[i + m - f], i - g);
		} 
		else 
		{
			f = i;
			g = min(g, i);
			while (g > 0 && P[g] == P[g + m - f])
			{
				g = g - 1;
			}
			suff[i] = f - g;
		}
	}
	// print suff
	cout<<"tableau suff"<<endl;
	for (int i = 1; i <= m; i++)
	{
		cout<<"suff["<<i<<"] : "<<suff[i]<<endl;
	}
	return suff;
}

int * calcule_D(string P, int m)
{
	int * suff = calcule_suff(P, m);
	int * D = new int [m + 1];
	// initialisation
	for (int i = 1; i <= m; i++)
	{
		D[i] = m;
	}
	// cas 2
	int i = 1;
	for (int j = m - 1; j >= 0; j--)
	{
		if (j == 0 || suff[j] == j)
		{
			while (i <= m - j)
			{
				D[i] = m - j;
				i++;
			}
		}
	}
	// cas 1
	for (int j = 1; j <= m - 1; j++)
	{
		D[m - suff[j]] = m - j;
	}
	// print D
	cout<<"tableau D"<<endl;
	for (int i = 1; i <= m; i++)
	{
		cout<<"D["<<i<<"] : "<<D[i]<<endl;
	}
	return D;
}

map<char,int> calcule_R(string P, int m, unordered_set<char> setT)
{
	map<char,int> R;
	for (char const &c : setT)
	{
		R[c]=0;
	}
	for (size_t i = 0; i <= m; i++)
	{
		R[P[i]]=i;
	}
	// print R
	cout<<"tableau R"<<endl;
	for (auto const &pair : R)
	{
		cout<<pair.first<<" : "<<pair.second<<endl;
	}
	return R;
}

void BM(string P, int m, string T, int n)
{
   unordered_set<char> setT;
   int nb = 0;
   for (char const &c : T) 
   {
	   setT.insert(c);
   }
//    for (char const &c : setT)
//    {
// 	   cout<<c<<endl;
//    }
	map<char,int> R = calcule_R(P,m,setT);
	int * D = calcule_D(P, m);
	int pos = 1;
	int i;
	while (pos <= n - m + 1)
	{
		i = m;
		while (i > 0 && P[i] == T[pos + i - 1])
		{
			i--;
			nb++;
		}
		if (i > 0 && P[i] != T[pos + i - 1])
		{
			nb++;
		}
		if (i == 0)
		{
			cout<<"P apparait a la position "<<pos<<endl;
			pos = pos + D[1];
		}
		else
		{
			pos = pos + max(D[i], i - R[T[pos+i-1]]);
		}
	}
	cout<<"le nombre de comparaisons de caracteres reellement effectuees : "<<nb<<endl;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// main ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
   if (argc != 3)
      {
	 cout<<"Mauvaise utilisation du programme "<<argv[0]<<", on attend :"<<endl;
	 cout<<argv[0]<<" ficMotif ficTexte"<<endl;
      }
   else
      {
	 /* Lecture du motif */
	 string nomFicMotif (argv[1]);
	 ifstream IfFicMotif (nomFicMotif.c_str(), ios::in);
	 if (!IfFicMotif)
	    {
	       cout<<endl<<"Impossible d'ouvrir le fichier : "<<nomFicMotif<<endl<<endl;
	       return(0);
	    }
	 string P; char c;
	 //On lit le fichier caract�re par caract�re
	 while(IfFicMotif.get(c)){
	    P += c;
	 }
	 //On �limine l'�ventuel retour � la ligne en fin de fichier
	 if (P.size() >1 && (int)P[P.size()-1]==10)
	    P=P.substr(0,P.size()-1);

	 P='#'+P; //Pour que la premi�re lettre de P soit � l'indice 1
	 int m=P.size()-1;

	 /* Lecture du texte */
	 string nomFicTexte(argv[2]);
	 ifstream IfFicTexte (nomFicTexte.c_str(), ios::in);
	  if (!IfFicTexte)
	    {
	       cout<<endl<<"Impossible d'ouvrir le fichier : "<<nomFicTexte<<endl<<endl;
	       return(0);
	    }
	  string T;
	  //On lit le fichier caract�re par caract�re
	  while(IfFicTexte.get(c)){
	     T += c;
	  }
	  //On �limine l'�ventuel retour � la ligne en fin de fichier
	  if (T.size() >1 && (int)T[T.size()-1]==10)
	     T=T.substr(0,T.size()-1);
    
	  T='#'+T; //Pour que la premi�re lettre de T soit � l'indice 1
	  int n=T.size()-1;
	  
	  cout<<"********************************************************************"<<endl;
	  cout<<"Recherche du motif de taille "<<m<<" P="<<P.substr(1,P.size())<<endl;
	  //On n'affiche plus le texte car il peut �tre long
  	  cout<<"Dans un texte de taille "<<n<<endl;//<<" T="<<T.substr(1,T.size())<<endl;
	  cout<<"********************************************************************"<<endl<<endl<<endl;

	  cout<<"************* Recherche naive *************"<<endl;
	  naif(P,m,T,n);
	  cout<<"###############################"<<endl<<endl;

	  cout<<"************* Recherche avec MP *************"<<endl;
	  MP(P,m,T,n);
	  cout<<"################################"<<endl<<endl;

	  cout<<"************* Recherche avec KMP *************"<<endl;
	  KMP(P,m,T,n);
	  cout<<"################################"<<endl<<endl;

	  cout<<"************* Recherche avec Boyer-Moore *************"<<endl;
	  BM(P,m,T,n);
	  cout<<"################################"<<endl;
      }
}
