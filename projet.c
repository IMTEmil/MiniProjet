#include "projet.h"

void SnareColorUpdate(Snare *snare)
{
    if (snare->state == SNARE_START) snare->color = LIGHTGRAY;
    if (snare->state == SNARE_LOADING) snare->color = PURPLE;
    if (snare->state == SNARE_CHARGED) snare->color = DARKPURPLE;
}

int InitSnare(GAME_SNARE gameSnare, Snare *snare)
{
    Vector2 offset = {0};
    int j = 0;

    offset.x = GetScreenWidth() % SQUARE_SIZE;
    offset.y = GetScreenHeight() % SQUARE_SIZE;

    snare->size = (Vector2) { SQUARE_SIZE, SQUARE_SIZE };    

    snare->active = false;

    snare->state = SNARE_START;

    snare->position = (Vector2){GetRandomValue(0, (GetScreenWidth() / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (GetScreenHeight() / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};

    for (j = 0; j < GameSnare.nbCurrentCount; j++)
    {
        while ((snare->position.x == GameSnare.snares[j].position.x) 
            && (snare->position.y == GameSnare.snares[j].position.y))
        {
            snare->position = (Vector2){GetRandomValue(0, (GetScreenWidth() / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (GetScreenHeight() / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
            j = 0;
        }
    }
    
    SnareColorUpdate(&snare);
}

int InitSnares(GAME_SNARE *gameSnare, unsigned int firstIndex, unsigned int lastIndex)
{
    int i = 0, j = 0;
    Vector2 offset = {0};

    offset.x = GetScreenWidth() % SQUARE_SIZE;
    offset.y = GetScreenHeight() % SQUARE_SIZE;

    if (lastIndex > SNARE_COUNT || lastIndex > SNARE_COUNT) return -1;

    for (i = firstIndex; i < lastIndex; i++)
    {        
        gameSnare->snares[i].size = (Vector2) { SQUARE_SIZE, SQUARE_SIZE };

        SnareColorUpdate(&gameSnare->snares[i]);     

        gameSnare->snares[i].active = false;

        gameSnare->snares[i].state = SNARE_START;

        gameSnare->snares[i].position = (Vector2){GetRandomValue(0, (GetScreenWidth() / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (GetScreenHeight() / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};

        for (j = 0; j < SNARE_COUNT; j++)
        {
            if (i == j) break;
        
            while ((gameSnare->snares[i].position.x == gameSnare->snares[j].position.x) && (gameSnare->snares[i].position.y == gameSnare->snares[j].position.y))
            {
                gameSnare->snares[i].position = (Vector2){GetRandomValue(0, (GetScreenWidth() / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (GetScreenHeight() / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2};
                j = 0;
            }
        }
    }   

    return 0;
}

void IfCollisionSendCitation(GAME_SENEQUE *GameSeneque, int currentFrameNumber)
{
    GameSeneque->isCitation = true;
    GameSeneque->LastCitationFrame = currentFrameNumber;
}

void displayCitation(GAME_SENEQUE *GameSeneque, int seconds, int currentFrameNumber)
{
    if ((currentFrameNumber - GameSeneque->LastCitationFrame) > (60 * seconds))
    {
        GameSeneque->isCitation = false;
        GameSeneque->indexCitation = GetRandomValue(0, (sizeof(CitationsSeneque) / sizeof(CitationsSeneque[0])));
    }
}

#define TODO NULL;

bool estVide(Liste l) {
	return l == NULL;
}

Liste creer(Element v){
	Liste l = malloc(sizeof(Cellule));
	if (l!= NULL)
	{
		l->val = v;
		l->suiv = NULL;
	}
	return l;
}

Liste ajoutTete(Element v, Liste l) {
	Liste newList = malloc(sizeof(Cellule));
	if (newList != NULL)
	{
		newList->suiv = l;
		newList->val = v;
	}
	return newList;
}

void detruire(Liste l) 
{
	Liste nextList = NULL;
	Liste currentList = l;

	if (!(estVide(l)))
	{
		currentList->suiv;
		do {
			free(currentList);
			currentList = nextList;
			nextList = currentList->suiv;
		} while (nextList != NULL);
	}
}

Liste ajoutFin(Element v, Liste l) {
	Liste lastElement = l;
	Liste newList = malloc(sizeof(Cellule));
	if (newList != NULL)
	{
		do {
			lastElement = lastElement->suiv;
		} while (lastElement->suiv != NULL);
	}
	lastElement->suiv = newList;
	newList->val = v;
	newList->suiv = NULL;
	return l;
}

// compare deux elements
bool equalsElement(Element e1, Element e2){
	return e1 == e2;
}

Liste retirePremier(Element v, Liste l) 
{
	return TODO;
}