#ifndef _PROJET_H_
#define _PROJET_H_

#include "raylib.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MENU_CHOICE_FONT_SIZE 50
#define SNARE_COUNT 30
#define SQUARE_SIZE 31

static char *CitationsSeneque[] = {
"Le travail est l'aliment des âmes nobles.",
"C'est pendant l'orage qu'on connait le pilote.",
"Toute la vie n'est qu'un voyage vers la mort.",
"Hâte toi de bien vivre et songe que chaque jour est à lui seul une vie.",
"Je préfère modérer mes joies que réprimer mes douleurs.",
"Il ne vaut mieux ne pas commencer que de cesser",
"Un grand exemple ne nait que de la mauvaise fortune.",
"Il est vaincu sans gloire celui qui est vaincu sans péril.",
"La parole reflète l'âme.",
"A quoi perd-on la plus grand partie de sa vie ? à différer.",
"On est nulle part quand on est partout.",
"Pendant qu'on la diffère, la vie passe en courant.",
"Ils vomissent pour manger, ils mangent pour vomir.",
"L'erreur n'est pas un crime."};

typedef enum GAMESTATE
{
    GS_MENU,

    GS_NORMAL,

    GS_SENEQUE,

    GS_SNARE,

    GS_SNACK

} GAMESTATE;

typedef struct
{
    Texture2D SenequeHeadImage;

    bool isCitation;

    int indexCitation;

    int LastCitationFrame;

} GAME_SENEQUE;

typedef enum 
{
    SNARE_START,

    SNARE_LOADING,

    SNARE_CHARGED

} SNARE_STATE;

typedef struct Snare
{
    Vector2 position;

    Vector2 size;

    bool active;

    Color color;

    SNARE_STATE state;

    unsigned int nSeconds;

} Snare;

typedef struct 
{
    Snare snares[SNARE_COUNT];

    unsigned int nbCurrentCount;

} GAME_SNARE;

typedef Snare Element;

struct cellule_s {
	Element val;
	struct cellule_s* suiv;
};

typedef struct cellule_s Cellule;

typedef Cellule* Liste;

static GAME_SNARE GameSnare = { 0 };

void InitProjetAddOn(GAME_SENEQUE *gameSeneque, Liste *snares);
void CloseProjetAddOn(GAME_SENEQUE *gameSeneque, Liste *snares);

void DrawMenu(void);
void IfCollisionSendCitation(GAME_SENEQUE *GameSeneque, int currentFrameNumber);
void displayCitation(GAME_SENEQUE *GameSeneque, int seconds, int currentFrame);
void DrawSeneque(void);
void DrawSnare(void);
void UpdateSnares(Liste *snares, unsigned int waitForNext, unsigned int lifeSpanSnare, Vector2 fruitPosition);
void DrawSnares(Liste snares);
bool SnareAlreadyAtPosition(Liste snares, Vector2 position);
bool SnareCollision(Liste snares, Vector2 position);

// ajout d'une partie du code de la liste_chainée que nous avons fait en TP
// je peux réutiliser le tout en changeant juste le typedef Snare Element

bool estVide(Liste l);

Liste creer(Element v);

Liste cherche(Element v,Liste l);

Liste ajoutTete(Element v, Liste l);

void detruire(Liste *l);

Liste ajoutFin(Element v, Liste l);

void retirePremierElement(Liste *l);

#endif