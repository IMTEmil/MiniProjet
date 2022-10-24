#ifndef _PROJET_H_
#define _PROJET_H_

#include "raylib.h"
#include <stdint.h>

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

typedef struct BufferCirculaire
{
    Snare snares[SNARE_COUNT];

    uint16_t indexLecture;

    uint16_t indexEcriture;
} BufferCirculaire ;

static GAME_SNARE GameSnare = { 0 };

void DrawMenu(void);
void IfCollisionSendCitation(GAME_SENEQUE *GameSeneque, int currentFrameNumber);
void displayCitation(GAME_SENEQUE *GameSeneque, int seconds, int currentFrame);
void DrawSeneque(void);
void DrawSnare(void);
int InitSnares(GAME_SNARE *gameSnare, unsigned int firstIndex, unsigned int lastIndex);
void SnareColorUpdate(Snare *snare);

#endif