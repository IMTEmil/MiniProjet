#include "projet.h"

// Continue replacing GAME_SNARE with snares

void SnareColorUpdate(Snare *snare)
{
    if (snare->state == SNARE_START) snare->color = LIGHTGRAY;
    if (snare->state == SNARE_LOADING) snare->color = PURPLE;
    if (snare->state == SNARE_CHARGED) snare->color = DARKPURPLE;
}

int InitSnare(Liste snares, Snare *snare)
{
    Vector2 offset = {0};
    int j = 0;

    offset.x = GetScreenWidth() % SQUARE_SIZE;
    offset.y = GetScreenHeight() % SQUARE_SIZE;

    snare->size = (Vector2) { SQUARE_SIZE, SQUARE_SIZE };    

    snare->active = true;

    snare->state = SNARE_START;

    snare->nSeconds = 0;

    snare->color = LIGHTGRAY;

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
}

void snareStateIteration(Liste snares, unsigned int nbCalls)
{
    Liste currentList = snares;
    while (currentList != NULL)
    {
        if (currentList->val.state != SNARE_CHARGED)
        {
            currentList->val.state++;
            SnareColorUpdate(&currentList->val);
        }
       
        currentList = currentList->suiv;
    }
}

void UpdateSnares(Liste snares, unsigned int waitForNext, unsigned int lifeSpanSnare)
{
    static unsigned int nbCalls = 1;

    Snare snare = { 0 };

    if (nbCalls % (60 * waitForNext) == 0)
    {
        snareStateIteration(snares, nbCalls);

        InitSnare(snares, &snare);

        snare.nSeconds = nbCalls / 60;

        ajoutFin(snare ,snares);
    }

    if (nbCalls == 60) 
        nbCalls = 60;

    if (((nbCalls / 60) - snares->val.nSeconds) == lifeSpanSnare)
    {
        retirePremierElement(snares);
    }

    nbCalls++;
}

void DrawSnares(Liste snares)
{
    Liste currentList = snares;
    while (currentList != NULL)
    {
        DrawRectangleV(currentList->val.position, currentList->val.size, currentList->val.color);
        currentList = currentList->suiv;
    }
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
	if (newList != NULL && !estVide(l))
	{
		while (lastElement->suiv != NULL)
        {
            lastElement = lastElement->suiv;
        }
	}
	lastElement->suiv = newList;
	newList->val = v;
	newList->suiv = NULL;
	return l;
}

Liste retirePremierElement(Liste l) 
{
    Liste premierElement = { 0 };
	if (!estVide(l))
    {
        premierElement = l;
        l = l->suiv;
        premierElement->suiv = NULL;
        free(premierElement);
        premierElement = NULL;
    }
    return l;
}

void InitProjetAddOn(GAME_SENEQUE *gameSeneque, Liste *snares)
{
    Image TempImage = { 0 };
    Snare snare = { 0 };

    TempImage = LoadImage("assets/seneque.png");
    ImageResize(&TempImage, SQUARE_SIZE, SQUARE_SIZE);
    gameSeneque->SenequeHeadImage = LoadTextureFromImage(TempImage);
    UnloadImage(TempImage);

    InitSnare(*snares, &snare);
    *snares = creer(snare);
}

void CloseProjetAddOn(GAME_SENEQUE *gameSeneque, Liste snares)
{
    UnloadTexture(gameSeneque->SenequeHeadImage);

    detruire(snares);
}