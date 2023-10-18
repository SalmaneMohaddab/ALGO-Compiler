#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int nbr_inst=0;//variable globale qui stocke le nombre des instructions
int nbr_declaration=0; // variable globale qui stocke le nombre des declarations effectuées
int nbr_erreur=0;
 typedef struct
 {
    char* ident;//identificateur de la variable
    char * type; // le type de la variable
    int valeur ; // la valeur de la variable

 }variable;/* la structure variable */


/* la liste chainee mot_p qui range les mot d'un fichier*/
typedef struct mots_fichier
{
  char * mot ; /*le mot */
  int ligne ;/*la ligne ou se trouve le mot */
  struct mots_fichier * next ; /* l'adresse du prochain mot  */
}mots_fichier;

/* declaration des listes de type mot_p pour le fichier dictionnaire et pour le fichier programme*/
 mots_fichier* liste_dict;
 mots_fichier* liste_mot_programme;
/* la fonction qui fait l'insertion des mots a la fin de la liste */
mots_fichier* insert_end(  mots_fichier * L , char * word, int ligne  )
{
  mots_fichier* p ;
  mots_fichier* temp;
  p=(mots_fichier*)malloc(sizeof(mots_fichier));/*on fait l'allocation d'un nouveau element  de la liste  */
  p->mot=word;/*on insere le mot dans le champ mot  */
  p->ligne=ligne;/*on stocke l ligne ou il se trouve */
  p->next=NULL;/*et on donne l'adresse NULL pour le champ next */
  temp=L;
  /*on test si la liste est vide ou non */
  if ( L==NULL)/*si oui */
  {
    L=p;/*on ajout l'element */
  }
  else/*si non */
  {
   while (temp->next!=NULL)/*on parcour jusqe'a la fin de la liste des mot */
   {
     temp=temp->next;
   }
    temp->next=p;/*et on ajoute le nouveau mot (element)*/
   }
  return L;/*on retourne L*/
}

mots_fichier* fonction(char * nom_fichier)/*la fonction qui convertit un fichier des mots à une liste des mots*/
{
  char c;/*variable pour stocker les caracteres */
  mots_fichier* liste_mot=NULL;/*liste des mots*/
  char * word ; /*la variable dont on va stocker chaque fois les mots du fichier*/
  char vide='\0' ; /*le mot vide */
  int ligne=1; /*compteur des ligne */
  int i=0;
  FILE* fp; /*pointeur pour le fichier*/
  word=(char*)malloc(sizeof(char));/*on commence par l'allocation de la varable word*/
  fp=fopen(nom_fichier,"r");/*on ouvre le fichier en mode ecriture*/

    if (fp==NULL)/*le cas si le fichier ne s'ouvre pas */
    {
    printf("erreur");/*on affiche un message d'erreur*/
    exit(0);/*on sort du programme*/
    }
    else/*le cas dont  le fichier s'ouvre sans problemes */
    {
      do/*on utilise une boucle pour parcourir le ficher jusqu'a sa fin */
      {
      c=(char)fgetc(fp);/* a chaque iteration  on récupère un caractere et on le stocke dans la variable "c" */
        if (c!=' ' && c!='\n') /*on test si le caractere n'est pas le mot vide  ou le saut a la ligne*/
          {
            word[i]=c;/*a chaque fois on stocke dans les cases de 'word' les caracteres d'une maniere successive*/
            i++;/*on incremente 'i' pour passer a la prochaine case*/
            word=realloc(word,i+1);/*et on ajoute une case pour le prochain caractere*/
            if ( c==';')/*on teste si le caractere est ";" */
            {
             nbr_inst++;/*donc on incrémente le nombre des instructions car chaque instruction se termine par ";" */
            }
          }
          else/* si le caractere est le mot vide  ou le saut a la ligne */
          {
            word[i]='\0'; /*on stocke a la derniere case le '\0' car le mot est terminé*/
                if (strcmp(word,&vide))/*on test si la variable word contient seulement le mot vide*/
                {
                /*si oui , on insere à la fin dans une liste des mots le mot et la ligne ou il se trouve */
                liste_mot=insert_end(liste_mot, word,ligne );
                }
            if (c=='\n')/* si le caractere est un saut à la ligne */
            {
            ligne++;/* on incrémente la variable ligne */
            }
            i=0;
            word=(char*)malloc(sizeof(char));/*puis la fin on fait une nouvelle allocation pour le prochain mot */
          }


      }while(c!=EOF);/* la boucle va se repéter jusqu'à le dernier caractère du fichier */
      fclose(fp);/*on ferme le fichier à la fin*/

    }
    return liste_mot; /*  on retourne la liste des mots du fichier*/
}


 void analyse_lix()/*fonction qui traite l'analyse lexicale*/
{
mots_fichier* p= liste_mot_programme;/*declaration du variable p qui contient liste des mots de programme*/
char * motP, *motD;/*deux variable l'un pour stocker les mots du programme l'autre pour stocker les mots du dictionnaire*/
int flag =0;/*flag pour pour faire le test si les mots du programme appartient au dictionnaire*/
int nbr_ereur=0;/*compteur des erreurs*/
FILE * pf ;
pf =fopen("/home/lynx/workspace/projet3/erreur.txt","a");
if (pf!=NULL)
{
 while ( p!=NULL)/* on fait le parcour de toute la liste des mots du programme */
 {
     motP =p->mot;/*a chaque fois on donne motP un mot de ficher programme , mot par mot*/
     mots_fichier* d = liste_dict ;/*on stocke dans la variable d la liste des mots du dictionnaire*/
     flag=0;/*au depart on donne flag la valeur 0*/
     while( d!=NULL && flag==0) /*puis on fait le parcour de tous les mots du dictionnaire*/
     {
       motD=d->mot;/*a chaque fois on stocke un mot du dictionnaire pour la comparer avec le mot du programme */
       if ( strcmp(motP,motD) )/*on test si le mot du programme appartient au dictionnaire par la comparer avec les mot du dict*/
       {d=d->next;}/*si non on passe a le prochain mot du dictionnaire*/
       else
       {
        flag=1;/*si oui on donne flag la valeur 1*/
       }
     }
     if(flag==0)/*on fait un test sur flag */
     {/*si flag egale 0 , c'est a dire que le mot du programme ne se trouve pas au dictionnaire*/
         printf("\n##erreur de type 1 dans le mot '%s' ligne %d##\n",p->mot,p->ligne);/*on affiche un message d'erreur*/
         fprintf(pf, "##erreur de type 1 dans le mot '%s' ligne %d##",p->mot,p->ligne);
         nbr_ereur++;/*puis on incremente le nombre des erreurs*/
     }
    p=p->next;/*puis on passe a le prochain  mot du programme et refaire le meme traitement jusqu'a le dernier mot*/
 }
}
fclose(pf);
}

mots_fichier** table_instruction(int nombre_inst)/*fonction qui rendre la liste des mots du programme à un tableau des instruction */
 {
 char str[]=";";/*le caractère ";" */
 int indx =0;/*un compteur pour indiquer à chaque fois la case où on va stocker l'instruction*/
 mots_fichier*  p= liste_mot_programme;/*declaration du variable p qui contient liste des mots de programme*/
 mots_fichier* q=NULL;/*declaration du variable q * qui va à chaque foit stocke l'instruction pour la stocker dans le tableau  */
 mots_fichier** tab;/*le tableau qui contient à chaque case une instructions du programme  */
 tab=(mots_fichier**)malloc(sizeof(mots_fichier*)*nombre_inst);/*on fait l'allocation du tableau dans la mémoire*/
 int test; /*la variable test pour tester si le mot est point-virgule ou non */
  while (p!=NULL)/*on fait le parcour des mots du programme jusqu'à la le dernier mot */
  {
     q=insert_end(q,p->mot,p->ligne);/*on insere chaque mot du programme a la fin dans la liste tant que le mot et différent de ";"*/
     test=strcmp(p->mot,str);/*on test si le mot est ";"*/
     if (test)/*si le mot n'est pas le mot ";" on passe a le prochain mot du programme*/
     {
      p=p->next;
     }
     else/*si le mot est le mot point-virgule*/
     {
     tab[indx]=q;/*on stocke l'instruction dans le tableau à la case indx */
     indx++;/*on incremente la variable indx*/
     q=NULL;/*puis on donne q à nouveau la valeur NULL pour refaire le même traitement pour la prochaine instruction */
     p=p->next;/*puis on passe au prochain mot du programme */
     }

  }
  return tab;/*à la fin la fonction retourne le tableau des instruction , chaque case contient une instruction*/

}
/*la fonction analyse_syntaxe qui fait l'analyse syntaxique du programme et il affiche l'erreur*/
 void analyse_syntaxe ( mots_fichier** tab , int taille)/*la fonction analyse_syntaxe qui fait l'analyse syntaxique du programme */
 {/*argument tab c'est le tableau des instructions */  /* *argument taille c'est le nombre des instructions dans le tableau */
 FILE * pf ;
 pf =fopen("/home/lynx/workspace/projet3/erreur.txt","a");
if (pf!=NULL)
{
   for (int i =0 ; i< taille; i++)/*on fait le parcour de toutes les instructions */
 {
   mots_fichier* p =tab[i];/*à chaque itération on stocke l'instruction dans le variable p */
   mots_fichier* q=NULL;/*et on donne la liste q la valeur NULL*/
   /*on test si le premier mot de l'instruction que on q stocker est une variable pour faire le traitement syntaxique sur l'affectation*/
   if (*p->mot >=(int)'a' && *p->mot<=(int)'z' && strlen(p->mot)==1 )
      {
           /*si oui */

          q=p;/*on stocke dans q le premier mot de l'instruction */
          p=p->next;/*et on passe au deuxieme mot de l'instruction*/
          /*on test si la deuxieme mot de l'instruction est le symbole d'affectation*/
          if ( strcmp(p->mot,":=")==0 )/*si oui */
           {
             p=p->next;/*on stocke dans q le deuxieme mot de l'instruction */
             q=q->next;/*et on passe aù troisième mot de l'instruction*/
             /*on test si le troisième mot de l'instruction est une variable ou bien une valeur de 0 jusqu'à 9*/
               if ( (*p->mot  >=(int)'a' && *p->mot <=(int)'z') || (*p->mot >=(int)'0' && *p->mot<=(int)'9') )/*si oui */
               {
                 p=p->next;/*on stocke dans q le troisième mot de l'instruction */
                 q=q->next;/*et on passe aù quatrième  mot de l'instruction*/
                  /*on test si le quatrième mot de l'instruction est point virgule ou bien une operation (+*-/)*/
                 if (strcmp(p->mot,";")==0 )/*si le mot et une point-virgule on saute vers la prochaine instruction sans continue
                    c'est le cas où l'instruction s'ecrit par exemple " a=1; " ou " a=v; "*/
                 {
                   continue;

                 }
                 /*dans le cas où le quatrième mot de l'instruction est une opération (+,*,-,/) */
                 else if (strcmp(p->mot,"+")==0 || strcmp(p->mot,"-")==0 || strcmp(p->mot,"*")==0 || strcmp(p->mot,"/")==0)
                 {
                    p=p->next;/*on stocke dans q le quatrième mot de l'instruction */
                    q=q->next;/*et on passe aù cinquième  mot de l'instruction*/
                    /*on test si le cinquième mot de l'instruction est un variable ou bien une valeur de 0 jusqu'à 9*/
                     if ((*p->mot  >=(int)'a' && *p->mot <=(int)'z') || (*p->mot >=(int)'0' && *p->mot<=(int)'9'))/*si oui */
                     {

                      p=p->next;/*on stocke dans q le cinquième mot de l'instruction */
                      q=q->next;/*et on passe aù sixième  mot de l'instruction*/
                      /*on test si le cinquième mot de l'instruction est different du mot point-virgule*/
                      if ( strcmp(p->mot,";")!=0 )/*si oui */
                          {
                          /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                             printf("##une ';' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                             /*on stocke lemessage d'erreur dans le fichier erreur.txt*/
                             fprintf(pf, "##une ';' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                             nbr_erreur++;
                          }
                     }
                     else/*si non*/
                     {
                       /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                       printf("##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                       /*on stocke lemessage d'erreur dans le fichier erreur.txt*/
                       fprintf(pf, "##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                       nbr_erreur++;
                     }
                 }
                 else/*si non*/
                   {/*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                      printf("##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                      /*on stocke lemessage d'erreur dans le fichier erreur.txt*/
                      fprintf(pf, "##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                      nbr_erreur++;
                   }
               }
               else/*si non*/
               {

                 {/*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                   printf("##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                   /*on stocke lemessage d'erreur dans le fichier erreur.txt*/
                   fprintf(pf, "##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                   nbr_erreur++;
                 }
               }
               }
           else/*si non*/
           {
           /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
            printf("##une ':=' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
            /*on stocke lemessage d'erreur dans le fichier erreur.txt*/
            fprintf(pf,"##une ':=' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
            nbr_erreur++;
           }
      }

     /*########## TRAITEMENT SYNTAXIQUE DES FONCTIONS LIRE ET ECRIRE  #############*/
/* si le cas où le premier mot de l'instruction que q stocker est le mot "lire" le mot "ecrire"*/
    else if ( strcmp(p->mot,"lire")==0 || strcmp(p->mot,"ecrire")==0)
        {
          q=p;/*on stocke dans q le premier mot de l'instruction */
          p=p->next;/*et on passe au deuxieme mot de l'instruction*/
          /*on test si la deuxieme mot de l'instruction est le symbole "("*/
            if ( strcmp(p->mot,"(")==0 )/*si oui */
           {
             p=p->next;/*on stocke dans q le deuxieme mot de l'instruction */
             q=q->next;/*et on passe aù troisième mot de l'instruction*/
             /*on test si le troisième mot de l'instruction est une variable */
             if ( *p->mot >=(int)'a' && *p->mot<=(int)'z' )/*si oui */
             {
               p=p->next;/*on stocke dans q le troisième mot de l'instruction */
               q=q->next;/*et on passe aù quatrième  mot de l'instruction*/
               /*on test si le quatrième mot de l'instruction est le symbole ")"*/
                if ( strcmp(p->mot,")")==0 )
                {
                  p=p->next;/*on stocke dans q le quatrième mot de l'instruction */
                  q=q->next;/*et on passe aù cinquième  mot de l'instruction*/
                  /*on test si le cinquième mot de l'instruction est different du mot point-virgule*/
                  if ( strcmp(p->mot,";")!=0 )
                  {
                   /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                   printf("##une ';' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                   /*on stocke lemessage d'erreur dans le fichier erreur.txt*/
                   fprintf(pf,"##une ';' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                   nbr_erreur++;
                  }
                }
                else/*si non */
                {
                 /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                 printf("##une ')' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                 /*on stocke lemessage d'erreur dans le fichier erreur.txt*/
                 fprintf(pf,"##une ')' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                 nbr_erreur++;
                }
             }
             else/*si non */
              {
                /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                printf("##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                /*on stocke lemessage d'erreur dans le fichier erreur.txt*/
                fprintf(pf,"##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                nbr_erreur++;
              }
        }
        else/*si non */
        {
          /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
          printf("##une '(' manque apres '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
          /*on stocke lemessage d'erreur dans le fichier erreur.txt*/
          fprintf(pf,"##une '(' manque apres '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
          nbr_erreur++;
        }
    }

          /*###########FIN DE TRAITEMENT SYNTAXIQUE POUR LES FONCTIONS LIRE ET ECRIRE ########## */
           /*########## TRAITEMENT SYNTAXIQUE DE LA PARTIE DECLARATION  #############*/
          /*########### TRAITEMENT SYNTAXIQUE POUR  LES INSTRUCTIONS DE DECLARATION##########*/
          /*on test si l'Instruction commence par le mot "variable"*/
    else if (strcmp(p->mot,"variable")==0)
    {
     /*  a chaque fois on trouve le mot variable on incremente le nombre des declarations dans un variable globale*/
             nbr_declaration++;
             q=p;/*on stocke dans q le premier mot de l'instruction */
             p=p->next;/*et on passe au deuxieme mot de l'instruction*/
             /*on test si la deuxieme mot de l'instruction est une variable*/
            if ( *p->mot >=(int)'a' && *p->mot<=(int)'z' )/*si oui */
            {
                  p=p->next;/*on stocke dans q le deuxieme mot de l'instruction */
                  q=q->next;/*et on passe aù troisième mot de l'instruction*/
                  /*on test si le troisième mot de l'instruction est le symbole ':' */
                  if (strcmp(p->mot,":")==0)/*si oui */
                  {
                    p=p->next;/*on stocke dans q le troisième mot de l'instruction */
                    q=q->next;/*et on passe aù quatrième  mot de l'instruction*/
                    /*on test si la quatrième mot de l'instruction est le mot "entier" ou"reel"*/
                     if (strcmp(p->mot,"entier")==0 || strcmp(p->mot,"reel")==0)/*si oui */
                     {
                       p=p->next;/*on stocke dans q le quatrième mot de l'instruction */
                       q=q->next;/*et on passe aù quatrième  mot de l'instruction*/
                       /*on test si le cinquième mot de l'instruction est different du mot point-virgule*/
                       if (strcmp(p->mot,";")!=0 )/*si oui*/
                       {
                        /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                         printf("##une ';' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                         /*on stocke lemessage d'erreur dans le fichier erreur.txt*/
                         fprintf(pf,"##une ';' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                         nbr_erreur++;
                       }

                     }
                     else/*si non*/
                     {
                          /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                           printf("##une erreur syntaxique apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                           /*on stocke lemessage d'erreur dans le fichier erreur.txt*/
                           fprintf(pf,"##une erreur syntaxique apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                           nbr_erreur++;
                     }
                  }
                  else/*si non */
                  {
                        /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                        printf("##une ':' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                        /*on stocke le message d'erreur dans le fichier erreur.txt*/
                        fprintf(pf,"##une ':' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                        nbr_erreur++;
                  }
            }
            else/*si non*/
            {
                 /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                 printf("##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                 /*on stocke le message d'erreur dans le fichier erreur.txt*/
                 fprintf(pf,"##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                 nbr_erreur++;
            }
    }

             /*########### FIN TRAITEMENT SYNTAXIQUE POUR  LES INSTRUCTIONS DE DECLARATION##########*/
             /*########### TRAITEMENT SYNTAXIQUE POUR  LES INSTRUCTIONS CONDITIONNELLE*/
   /*on test si l'Instruction commence par le mot "variable"*/
    else if ( strcmp(p->mot,"si")==0 )/*si oui*/
    {
             q=p;/*on stocke le mot actuel dans q */
             p=p->next;/*et on passe au prochain mot de l'instruction*/
             /*on test si le prochain mot est le symbole "("*/
            if ( strcmp(p->mot,"(")==0 )/*si oui */
           {
             p=p->next;/*on stocke le mot actuel dans q */
             q=q->next;/*et on passe au prochain mot de l'instruction*/
             /*on test si le prochain mot est une variable */
             if ( *p->mot>=(int)'a' && *p->mot<=(int)'z' )/*si oui */
             {
               p=p->next;/*on stocke le mot actuel dans q */
               q=q->next;/*et on passe au prochain mot de l'instruction*/
               /*on test si le prochain mot est un symbole de comparison <,>,=*/
                if ( strcmp(p->mot,">")==0 || strcmp(p->mot,"<")==0 || strcmp(p->mot,"=")==0)/*si oui*/
                {
                  p=p->next;/*on stocke le mot actuel dans q */
                  q=q->next;/*et on passe au prochain mot de l'instruction*/
                  /*on test si le prochain mot est un chiffre*/
                  if ( *p->mot >=(int)'0' && *p->mot<=(int)'9' )/*si oui */
                  {
                  p=p->next;/*on stocke le mot actuel dans q */
                  q=q->next;/*et on passe au prochain mot de l'instruction*/
                  /*on test si le prochain mot est le symbole ")"*/
                     if (strcmp(p->mot,")")==0)/*si oui */
                     {
                       p=p->next;/*on stocke le mot actuel dans q */
                       q=q->next ;/*et on passe au prochain mot de l'instruction*/
                       /*on test si le prochain mot est le mot "alors"*/
                         if (strcmp(p->mot,"alors")==0)/*si oui */
                         {
                         p=p->next;/*on stocke le mot actuel dans q */
                         q=q->next;/*et on passe au prochain mot de l'instruction*/
                          /*on test si le prochain mot est une variable */
                         if ( *p->mot >=(int)'a' && *p->mot<=(int)'z' )/*si oui */
                         {
                         p=p->next;/*on stocke le mot actuel dans q */
                         q=q->next;/*et on passe au prochain mot de l'instruction*/
                         /*on test si le prochain mot est le symbole ":="*/
                           if (strcmp(p->mot,":=")==0)/*si oui*/
                           {
                           p=p->next;/*on stocke le mot actuel dans q */
                           q=q->next;/*et on passe au prochain mot de l'instruction*/
                           /*on test si le prochain mot est un chiffre*/
                            if (*p->mot >=(int)'0' && *p->mot<=(int)'9')/*si oui*/
                            {
                            p=p->next;/*on stocke le mot actuel dans q */
                            q=q->next;/*et on passe au prochain mot de l'instruction*/
                            /*on test si le prochain mot est le mot "sinon"*/
                             if (strcmp(p->mot,"sinon")==0)/*si oui*/
                             {
                             p=p->next;/*on stocke le mot actuel dans q */
                             q=q->next;/*et on passe au prochain mot de l'instruction*/
                               /*on test si le prochain mot est une variable */
                               if (*p->mot >=(int)'a' && *p->mot<=(int)'z' )/*si oui*/
                               {
                                 p=p->next;/*on stocke le mot actuel dans q */
                                 q=q->next;/*et on passe au prochain mot de l'instruction*/
                                 /*on test si le prochain mot est le symbole ":="*/
                                  if (strcmp(p->mot,":=")==0)/*si oui */
                                  {
                                  p=p->next;/*on stocke le mot actuel dans q */
                                  q=q->next;/*et on passe au prochain mot de l'instruction*/
                                  /*on test si le prochain mot est un chiffre*/
                                      if (*p->mot >=(int)'0' && *p->mot<=(int)'9')/*si oui */
                                      {
                                      p=p->next;/*on stocke le mot actuel dans q */
                                      q=q->next;/*et on passe au prochain mot de l'instruction*/
                                      /*on test si le prochain mot est le mot "finsi"*/
                                      if (strcmp(p->mot,"finsi")==0)/*si oui */
                                      {
                                        p=p->next;/*on stocke le mot actuel dans q */
                                        q=q->next;/*et on passe au prochain mot de l'instruction*/
                                        /*on test si le prochain mot de l'instruction est different du mot point-virgule*/
                                        if (strcmp(p->mot,";")!=0)/*si oui */
                                        {
                                        /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                                          printf("##une ';' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                                          /*on stocke le message d'erreur dans le fichier erreur.txt*/
                                          fprintf(pf,"##une ';' manque apres '%s' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                                          nbr_erreur++;
                                        }
                                      }
                                      else/*si non*/
                                      {
                                      /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                                              printf("##une 'finsi' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                                              /*on stocke le message d'erreur dans le fichier erreur.txt*/
                                              fprintf(pf,"##une 'finsi' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                                              nbr_erreur++;
                                      }

                                      }
                                      else/*si non*/
                                      {
                                          /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                                           printf("##erreur  apres '%s' 'entrez un chifre' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                                           /*on stocke le message d'erreur dans le fichier erreur.txt*/
                                           fprintf(pf,"##erreur  apres '%s' 'entrez un chifre' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                                           nbr_erreur++;
                                      }
                                  }
                                  else/*si non*/
                                  {
                                     /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                                     printf("##une ':=' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                                     /*on stocke le message d'erreur dans le fichier erreur.txt*/
                                     fprintf(pf,"##une ':=' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                                     nbr_erreur++;
                                  }
                               }
                               else/*si non*/
                               {
                                   /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                                   printf("##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                                   /*on stocke le message d'erreur dans le fichier erreur.txt*/
                                   fprintf(pf,"##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                                   nbr_erreur++;
                               }
                             }
                             else/*si non*/
                             {
                                /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                                printf("##une 'sinon' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                                /*on stocke le message d'erreur dans le fichier erreur.txt*/
                                fprintf(pf,"##une 'sinon' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                                nbr_erreur++;
                             }
                            }
                            else/*si non*/
                            {
                              /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                              printf("##erreur  apres '%s' 'entrez un chifre' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                              /*on stocke le message d'erreur dans le fichier erreur.txt*/
                              fprintf(pf,"##erreur  apres '%s' 'entrez un chifre' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                              nbr_erreur++;
                            }

                           }
                           else/*si non*/
                           {
                               /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                               printf("##une ':=' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                               /*on stocke le message d'erreur dans le fichier erreur.txt*/
                               fprintf(pf,"##une ':=' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                               nbr_erreur++;
                           }

                         }
                         else/*si non*/
                         {
                            /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                            printf("##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                            /*on stocke le message d'erreur dans le fichier erreur.txt*/
                            fprintf(pf,"##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                            nbr_erreur++;
                         }
                         }
                         else/*si non*/
                         {
                           /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                           printf("##une 'alors' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                           /*on stocke le message d'erreur dans le fichier erreur.txt*/
                           fprintf(pf,"##une 'alors' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                           nbr_erreur++;
                         }
                     }
                     else/*si non*/
                     {
                      /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                      printf("##une ')' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                      /*on stocke le message d'erreur dans le fichier erreur.txt*/
                      fprintf(pf,"##une ')' manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                      nbr_erreur++;
                     }
                  }
                  else/*si non*/
                  {
                    /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                   printf("##erreur  apres '%s' 'entrez un chifre' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                   /*on stocke le message d'erreur dans le fichier erreur.txt*/
                   fprintf(pf,"##erreur  apres '%s' 'entrez un chifre' ligne: %d type erreur 2##\n  ",q->mot,q->ligne);
                   nbr_erreur++;
                  }

                }
                else/*si non*/
                {
                 /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                printf("##un symbole de comparaison qui  manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                /*on stocke le message d'erreur dans le fichier erreur.txt*/
                fprintf(pf,"##un symbole de comparaison qui  manque apres '%s' ligne: %d  type erreur 2##\n",q->mot,q->ligne);
                nbr_erreur++;
                }
             }
             else/*si non*/
             {
                /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
                printf("##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                /*on stocke le message d'erreur dans le fichier erreur.txt*/
                fprintf(pf,"##erreur apres le mot '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
                nbr_erreur++;
             }
        }
        else/*si non*/
        {
           /*on affiche un message d'erreur , pour indiquer l'erreur et où il se trouve */
          printf("##une '(' manque apres '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
          /*on stocke le message d'erreur dans le fichier erreur.txt*/
          fprintf(pf,"##une '(' manque apres '%s' ligne: %d type erreur 2##\n",q->mot,q->ligne);
          nbr_erreur++;
        }
    }
        /*###########FIN DU TRAITEMENT SYNTAXIQUE POUR  LES INSTRUCTIONS CONDITIONNELLE*/
 }
 }
 fclose(pf);
 }

 void analyse_sementique(mots_fichier** tab , variable* variables)/*la fonction analyse_sementique qui fait l'analyse sementique*/
 {/*l'argument tab est un tableau des instructions du programme*/ /*variables un un tableau qui stocke les variables du programme */
           /*#### ASSURER QUE LE PROGRAMME COMMENCE PAR DEBUT ET SE TERMINE PAR FIN */
 FILE * pf ;
 pf =fopen("/home/lynx/workspace/projet3/erreur.txt","a");
if (pf!=NULL)
{
 for (int i =0 ;i<nbr_inst-1;i++)/*on fait le parcour instruction par instruction*/
   {
       /*on test si la premiere instruction du programme est different du  mot "debut"*/
     if (i==0 && strcmp(tab[0]->mot,"debut")!=0)/*si oui */
      {
       /*on affiche un erreur sementique*/
       printf("##erreur de type 3 ligne %d le programme doit commencer par 'debut'##\n",tab[0]->ligne);
       /*on stocke le message d'erreur dans le fichier erreur.txt*/
        fprintf(pf,"##erreur de type 3 ligne %d le programme doit commencer par 'debut'##\n",tab[0]->ligne);
       nbr_erreur++;
      }
       /*on test si la derniere instruction du programme est different du  mot "fin"*/
     if (i==nbr_inst-2 && strcmp(tab[i]->mot,"fin")!=0)/*si oui */
      {
        /*on affiche un erreur sementique*/
        printf("##erreur de type 3 ligne %d le programme doit finir par 'fin'##\n",tab[i]->ligne);
        /*on stocke le message d'erreur dans le fichier erreur.txt*/
        fprintf(pf,"##erreur de type 3 ligne %d le programme doit finir par 'fin'##\n",tab[i]->ligne);
        nbr_erreur++;
      }
  }
                 /*#######FIN TRAITEMENT ###########*/
         /*###### TRAITEMENT DE LA DOUBLE DECLARATION POUR UNE VARIABLE##########*/

for ( int i =0 ;i<nbr_declaration;i++)/* ON PARCOUR LA TABLE DES VARIABLES , A CHAQUE CASE EN TRAITE UN VARIABLE*/
  {
     /*ON COMMENCE DE LA PROCHAINE VARIABLE DECLARER EST ON COMPARER TOUT LES CASE AVEC LA VARIABLE PRECEDENTE*/
     /* EX: ON COMPARE LA CASE[0] AVEC TOUT LES AUTRES CASE JUSQU'A LA DERNIERE VARIABLE ET ON PASSE A LA CASE[1] ET ANSI DE SUITE*/
      for ( int j =i+1;j<nbr_declaration;j++)
       { /* a chaque iteration on compare l'identificateur de la variable comparee avec la variable comparant*/
           if ( strcmp(variables[i].ident,variables[j].ident)==0)/*si l'identificateur est identique*/
           { /* on affiche un erreur sementique ,indique que la meme variable est declarer deux fois*/
               printf("erreur de type 3 le variable '%s' est deja declarer\n",variables[i].ident);
                /*on stocke le message d'erreur dans le fichier erreur.txt*/
                fprintf(pf,"erreur de type 3 le variable '%s' est deja declarer\n",variables[i].ident);
               nbr_erreur++;
           }
       }
  }
      /*###### FIN DU TRAITEMENT DE LA DOUBLE DECLARATION POUR UNE VARIABLE##########*/
      /*###### TRAITEMENT DE LA VERIFICATION DE LA DECLARATION DES VARIABLE(est ce que une variable est declaree?)##########*/
     mots_fichier* p =liste_mot_programme;/*on va utiliser la liste chainne des mots du fichier programme */
     while (p!=NULL) /*parcourir tous les mots du programme  */
     {
       /*on veut faire le test seulement sur les variables de 'a' jusqu'a 'z'  qui ont comme longeur 1*/
      if ( (*p->mot  >=(int)'a' && *p->mot <=(int)'z') && strlen(p->mot)==1)
      {
          int flag =0;/*la variable flag pour tester l'existence   des variables */
         for (int i=0;i<nbr_declaration;i++)
         /*a chaque fois quand on trouve un variable on va le comparer avec touts les variables declarees*/
         {
             /*on teste si l'identificateur du variable existe dans la table des variables*/
            if (strcmp(variables[i].ident,p->mot)==0 )/* si oui */
            {
              flag=1;/*on donne flag la valeur 1 c'est a dire que la variable est declarer*/
              break;/*et on sort de la boucle*/
            }
            else/* si non */
            {
            flag=0;/*flag reste 0 et on passe a la prochaine variable du tableau variables */
            }
         }
          /*on test si la variable flag egale 0 */
         if ( flag ==0)/*si oui */
         {/*alors la variable n'est pas declaree , on affiche un erreur*/
           printf("erreur de type 3 variable '%s' nest pas declarer\n",p->mot);
            /*on stocke le message d'erreur dans le fichier erreur.txt*/
            fprintf(pf,"erreur de type 3 variable '%s' nest pas declarer\n",p->mot);
           nbr_erreur++;
         }
      }
      p=p->next;/*et on passer a la prochaine variable a comparer*/
      }
     /*###### FIN TRAITEMENT DE LA VERIFICATION DE LA DECLARATION DES VARIABLE*/
     /*######TRAITEMENT DE LA VERIFICATION DE LA homogénéité DE TYPE DES VARIABLES AVEC LA VALEUR AFFECTER#######*/
     /*juste le cas des entie !!!!!!!!!!*/
for (int i =0 ;i<nbr_inst-1;i++)
{

  mots_fichier* p =tab[i]; /* a chaque iteration on affecte a la variable p une instruction */
   /*on test si l'instruction commence par une variable (cas d'affectation)*/
  if ((*p->mot  >=(int)'a' && *p->mot <=(int)'z') && strlen(p->mot)==1)/*si oui */
  {
  for (int i=0;i<nbr_declaration;i++)/*on parcour la table des variables declarees*/

         {   /*on cherche dans toute la table sur la variable identique au variable de l'instruction*/
            if (strcmp(variables[i].ident,p->mot)==0 )/*si on a le trouver on le memorise par l'index "i"*/
            {
               /*et on test si la variable qu'on traite est de type entier*/
               if ( strcmp(variables[i].type,"entier")==0)/* si oui , il est de type entier */
               {
                 /*apres on test si l'instruction est une simple affectation, ex ; a=b ; ou a=1 ;*/
                 /* on test si l'instruction est composee de juste 3 mot */
                  if (strcmp(p->next->next->next->mot ,";")==0 )/*si oui */
                  {
                  /*puisque on traite le cas des entier on test si la valeur affectee a la variable est un chiffre*/
                     if( !(*p->next->next->mot >=(int)'1' && *p->next->next->mot <=(int)'9') )/* si e pas le cas */
                    {  /* on affiche un erreur sementique*/
                      printf("erreur de type 3 ligne %d , la variable %s est un entier\n",p->ligne,p->mot);
                      /*on stocke le message d'erreur dans le fichier erreur.txt*/
                        fprintf(pf,"erreur de type 3 ligne %d , la variable %s est un entier\n",p->ligne,p->mot);
                      nbr_erreur++;
                    }
                  }
                 /* si non , on test si l'instruction est une affectation avec une operation */
                 /* exemple : a= 1+2 ;*/
                 /* on suppose que a chaque instruction on peut faire une operation entre exactement deux operants*/
                  else if (p->next->next->next->next->mot!=NULL )
                  /* le cas ou  l'instruction est une instruction  d'affectation avec operation*/
                  {
                     if (! ((*p->next->next->mot >=(int)'1' && *p->next->next->mot <=(int)'9') &&
                      (*p->next->next->next->next->mot >=(int)'1' && *p->next->next->next->next->mot <=(int)'9')) )
                      /*on fait un test si les opérants sont des entier*/
                      /*s'il ne sont pas on affiche un message d'erreur sementique */
                           {printf("erreur de type 3 ligne %d , la variable %s est un entier\n",p->ligne,p->mot);
                            /*on stocke le message d'erreur dans le fichier erreur.txt*/
                            fprintf(pf,"erreur de type 3 ligne %d , la variable %s est un entier\n",p->ligne,p->mot);
                            nbr_erreur++;
                           }
                  }
               }
            }
         }
   }

}
        /*####FIN TRAITEMENT DE LA VERIFICATION DE LA homogénéité DE TYPE DES VARIABLES AVEC LA VALEUR AFFECTER######*/
        }
 fclose(pf)  ;
 }
/*la fonction creation_variables sert a creé des variables qui se trouvent au programme*/
 variable * creation_variables( mots_fichier**  tab  )
 {/*argument tab c'est un  tableau des instruction du programme */
 /*on déclare un tableau des variable de type variable ,chaque case contient un variable du programme avec le type et la valeur*/
 variable *variables;
 variables=(variable*)malloc(sizeof(variable)*nbr_declaration);

 int j =0;/*un compteur des cases */
for (int i=0;i<nbr_inst-1;i++)/*on parcour le tableau des instruction */
{
  mots_fichier* p =tab[i];/*a chaque itération on stocke l'instruction dans la variable p */
  /*on test si le premier mot de l'instruction est le mot "variable", c'est à dire on traite une instruction de déclaration */
 if (strcmp(p->mot,"variable")==0)/*si oui */
 {
     variables[j].ident=p->next->mot;/*on stocke dans la case j le nom du variable */ /* exp :variable a : entier ; */
     variables[j].type=p->next->next->next->mot;/*on stocke dans la case j le type  du variable */
     j++;/*et on increment j pour passer  à la prochaine case */
 }
 }
 return variables;/*et on retourne le tableau des variables */
}
/*la fonction execution qui fait l'interpretation d ucode du programme*/
 void execution (int nbr_inst,mots_fichier** tab ,variable * variables )
 {/*nbr_inst: est le nombre des instruction*//*tab :tableau des instruction*/
 /* variables : tableau des variables du programme*/
   for (int i=0;i<nbr_inst-1;i++)/*on parcour la table des instruction */
  {
    mots_fichier* p =tab[i];/*a chaque iteration on stocke l'instruction qui se trouve dans la case i */
    if (strcmp(p->mot,"ecrire")==0)/*dans le cas dont l'instruction commence par "ecrire"*/
    {
       char* c =p->next->next->mot;/*on stocke la variable a afficher sa valeur ex ecrire(a); on stocke a  */
       for (int j=0;j<nbr_declaration;j++)/*on cherche la variable a afficher dans la table des variables*/
        {
           if (strcmp(c,variables[j].ident)==0)/*si on le touve on memorise la case ou il se trouve avec j */
           {
             /*on accede a la valeur de la variable a l'aide de l'indice j puis on affiche la valeur au terminal */
             printf("la valeur de %s est %d\n",variables[j].ident,variables[j].valeur);
             break;
             /*on sort de la boucle puisque on a traite le cas de double declaration ,donc
              la variable est declarer une seule fois*/
           }
       }
   }

 else if (strcmp(p->mot,"lire")==0)/*si l'instruction commence par le mot "lire" */
 {
     char* c =p->next->next->mot;/*on stocke comme la fonction ecrire , la variable a lire */
     for (int j=0;j<nbr_declaration;j++)/*on parcour la table des variable*/
       {
          if (strcmp(c,variables[j].ident)==0)/*si on trouve la variable  correspondante */
           {
              printf("donner le valeur de %s :",variables[j].ident);/*on affiche un message indicatif*/
              scanf("%d",&variables[j].valeur);/*et on scan la valeur dans le champ valeur pour la variable
              correspondante */
              break;
              /*on sort de la boucle puisque on a traite le cas de double declaration ,donc
              la variable est declarer une seule fois*/
           }
      }
 }
/*dans le cas ou on a une instruction d'affectation*/
 else if ( (*p->mot  >=(int)'a' && *p->mot <=(int)'z') && strlen(p->mot)==1 )
 {
      if (strcmp(p->next->next->next->mot ,";")==0)/*si on a une simple affectation */
       {
          for (int j=0;j<nbr_declaration;j++)/*on cherche la variable dans la table des variables*/
             {
                 if (strcmp(p->mot,variables[j].ident)==0)/*apres touver la variable correspondante */
               {
                  /* on fait une conversion de la valeur de char vers int */
                   variables[j].valeur=atoi(p->next->next->mot);
                   /*et on stocke la valeur convertit dans le chanp valeur pour la vriable correspondante */
                   break;
                   /*on sort de la boucle puisque on a traite le cas de double declaration ,donc
                     la variable est declarer une seule fois*/
               }
             }
      }
 /*dans le cas ou on a une instruction d'affectation avec operation */
      else if (p->next->next->next->next->mot!=NULL)
      {
         for (int j=0;j<nbr_declaration;j++)/*on cherche la variable correspondante */
            {
                if (strcmp(p->mot,variables[j].ident)==0)/*apres trouver la case ou elle se trouve */
              {
                    if (strcmp(p->next->next->next->mot,"+")==0)/*on test si l'operation est "+"*/
                     {
                     /*si oui on fait la conversion des valeurs de "char" vers "int"*/
                     /*et on stocke la valeur apres l'addition des deux valeurs*/
                       int val =atoi(p->next->next->mot)+atoi(p->next->next->next->next->mot);
                       variables[j].valeur=val ;
                     }
                     /*on test si l'operation est "-"*/
                    else if (strcmp(p->next->next->next->mot,"-")==0)
                     {
                     /*si oui on fait la conversion des valeurs de "char" vers "int"*/
                     /*et on stocke la valeur apres la soustraction des deux valeurs*/
                        int val =atoi(p->next->next->mot)-atoi(p->next->next->next->next->mot);
                       variables[j].valeur=val ;
                     }
                     /*on test si l'operation est "*"*/
                    else if (strcmp(p->next->next->next->mot,"*")==0)
                    {
                      /*si oui on fait la conversion des valeurs de "char" vers "int"*/
                     /*et on stocke la valeur apres la multiplication  des deux valeurs*/
                      int val =atoi(p->next->next->mot)*atoi(p->next->next->next->next->mot);
                       variables[j].valeur=val ;
                    }
              }
            }
      }
 }
 }
 }

int main ()
{
 int nombre_inst_prog;/*variables pour stocker juste le nombre des instruction du code du programme*/
 mots_fichier**  tab;/*on declare un tableau des instructions*/
 variable * variables;/*on declare un tableau des variables */
 liste_mot_programme=fonction("programme.txt");
 /*on stocke la liste des mots du programme dans la variable globale liste_mot_programme */
 nombre_inst_prog=nbr_inst;/*et on stocke le nombre des instruction du programme avant passer au dictionnaire*/
 liste_dict=fonction("dictionnaire.txt");
 /*on stocke la liste des mots du dictionnaire dans la variable globale liste_dict*/
 analyse_lix();/*on passe a l'analyse lixecale du programme*/
 tab=table_instruction(nombre_inst_prog);
 /*on fait la conversion de la liste des mots du programme vers un tableau des instructions */
 analyse_syntaxe(tab,nombre_inst_prog);/*on passe a l'analyse syntaxique du programme*/
 variables=creation_variables(tab);/*on cree les variables qui se trouvent au programme */
 analyse_sementique( tab ,variables);/*on passe a l'analyse sementique du programme*/
if ( nbr_erreur==0)/*on test si le nombre des erreur est 0 ou non */
{/*si oui on passe a l'execution*/
execution (nombre_inst_prog , tab ,variables );
}
 return 0;
}
