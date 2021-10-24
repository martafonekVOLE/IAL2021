
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
                                //Inicializace seznamu (listu)
    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
                        //Pokud seznam existuje, odstranuj a uvolnuj jeden prvek za druhym, dokud neodstranis cely seznam
    if(list != NULL){
        DLLElementPtr temp = list->firstElement;
        DLLElementPtr removeElement;

        while(temp != NULL){
            removeElement = temp->nextElement;
            free(temp);
            temp = removeElement;
        }

        list->firstElement = NULL;
        list->activeElement = NULL;
        list->lastElement = NULL;
    }
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

    DLLElementPtr temp = malloc(sizeof(struct DLLElement));
    DLLElementPtr firstInList = list->firstElement; 

    if(temp == NULL){       //Zkouska, ze se alokovalo pozadovane misto
        DLL_Error();
    }
    else{                   //Pokud nedoslo k chybe vloz prvek na zacatek seznamu
        temp->data = data;
        temp->previousElement = NULL;
        temp->nextElement = firstInList;

        if(firstInList == NULL){            //Pokud již v seznamu prvek je, uložím nový prvek před něj a nastavím previousElement ukazatel starého prvního prvku na nový první prvek
            list->lastElement = temp;
        }
        else{
            firstInList->previousElement = temp;
        }
        list->firstElement = temp;
    }


}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {

    DLLElementPtr temp = malloc(sizeof(struct DLLElement));
    DLLElementPtr lastInList = list->lastElement; 

    if(temp == NULL){       //Podobny postup - kontrola alokace, nasledne nahrani obsahu na posledni pozici v seznamu
        DLL_Error();
    }
    else{
        temp->data = data;
        temp->previousElement = lastInList;
        temp->nextElement = NULL;

        if(list->firstElement == NULL){
            list->firstElement = temp;
        }
        else{
            lastInList->nextElement = temp;
        }
        list->lastElement = temp;
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {

    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
                                    //Pokud neni seznam prazdny, vrati pomoci ukazatele hodnotu prvniho prvku
    if(list->firstElement != NULL){
        *dataPtr = list->firstElement->data;
    }
    else{
        DLL_Error();
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
                                    //Pokud neni seznam prazdny, vrati pomoci ukazatele hodnotu posledniho prvku
    if(list->firstElement != NULL){
        *dataPtr = list->lastElement->data; //Errno - test 37 - solved 
    }
    else{
        DLL_Error();
    }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

    if(list == NULL||list->firstElement==NULL){

    }
    else{                                                       
        DLLElementPtr temp = list->firstElement->nextElement;
        if(list->firstElement == list->activeElement){      //Overeni, zdali je aktivni element zaroven prvnim
            list->activeElement = NULL;
        }
        if(temp == NULL){
            free(list->firstElement);   //Pokud existuje druhy element, uvolni prvni a puvodne druhy element prirad jako prvni v seznamu
            list->firstElement = temp;
        }
        else{
            temp->previousElement = NULL;
            free(list->firstElement);
            list->firstElement = temp;
        }
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    if(list==NULL || list->firstElement==NULL){
            
    }
    else{
        if(list->lastElement == list->activeElement){       //Overeni, zdali je aktivni element zaroven poslednim
            list->activeElement = NULL;
        }
        DLLElementPtr temp = list->lastElement;
        if(list->lastElement != list->firstElement){
            list->lastElement = list->lastElement->previousElement;
            list->lastElement->nextElement = NULL;
        }
        else{                                               //Pokud existuje jen jeden prvek seznamu, smaž jej a všechny ukazatele nastav na NULL
            list->firstElement = NULL;
            list->lastElement = NULL;
            list->activeElement = NULL;
        }
        free(temp);
        
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {

    if(list->firstElement == list->activeElement || list == NULL || list->activeElement == NULL || list->activeElement->nextElement == NULL){

    }
    else{       
        DLLElementPtr temp = list->activeElement->nextElement; //Pokud se nesplni predchozi podminky, uvolni element po aktivnim 
        
        list->activeElement->nextElement = temp->nextElement;
        if(temp->nextElement == NULL){
            list->lastElement = list->activeElement;
        }
        else{
            temp->nextElement->previousElement = list->activeElement;
        }
        free(temp); 
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {

    if(list->firstElement == list->activeElement || list == NULL || list->activeElement == NULL || list->activeElement->previousElement == NULL){

    }
    else{
        DLLElementPtr temp = list->activeElement->previousElement;
        
        list->activeElement->previousElement = temp->previousElement;
        if(temp->previousElement == NULL){             //Pokud existuje prvek pred cilenym prvkem, tak ho prirad na pozici a nasledne smaz cilovy prvek
            list->firstElement = list->activeElement;
        }
        else{
            temp->previousElement->nextElement = list->activeElement;
        }
        free(temp);
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

    if(list == NULL || list->activeElement == NULL){

    }
    else{
        DLLElementPtr temp = list->activeElement;
        DLLElementPtr newInput = malloc(sizeof(struct DLLElement));

        if(newInput == NULL){
            DLL_Error();
        }
        else{                       //Pokud se zdari alokace, vloz na misto noveho prvku (za aktivni prvek) dana data
            newInput->data = data;
            newInput->previousElement = list->activeElement;
            
            if(temp->nextElement != NULL){
                temp->nextElement->previousElement = newInput;
                newInput->nextElement = list->activeElement->nextElement;
            }
            else{
                list->lastElement = newInput;
                newInput->nextElement = NULL;
            }
            list->activeElement->nextElement = newInput;
        }
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

    if(list == NULL || list->activeElement == NULL){

    }   
    else{
        DLLElementPtr temp = list->activeElement;
        DLLElementPtr newInput = malloc(sizeof(struct DLLElement));

        if(newInput == NULL){
            DLL_Error();
        }
        else{                       //Pokud se zdari alokace, vloz na misto noveho prvku (pred aktivni prvek) dana data //Check ze je seznam propojeny (nechybi ukazatele)
            newInput->data = data;
            newInput->nextElement = list->activeElement;
            if(temp->previousElement != NULL){
                list->activeElement->previousElement->nextElement = list->activeElement;
                //temp->previousElement = newInput;   //Error here
                newInput->previousElement = list->activeElement->previousElement;
            }
            else{
                list->firstElement = newInput;
                newInput->previousElement = NULL;
            }
            list->activeElement->previousElement = newInput; //Error -> position of this line - solved

        }
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

    if(list == NULL||list->activeElement == NULL){
        DLL_Error();
    }
    else{                       //Ulozi hodnotu aktivniho prvku na danou adresu
        *dataPtr = list->activeElement->data;
    }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {

    if(list == NULL||list->activeElement==NULL){

    }
    else{                       //Ulozi dane data na misto aktivniho prvku
        list->activeElement->data = data;
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {

    if(list == NULL || list->activeElement == NULL){

    }
    else if(list->activeElement == list->lastElement){ //Pokud je aktivnim prvkem posledni prvek, aktivita se ztrati
        list->activeElement = NULL;
    }
    else{
        list->activeElement = list->activeElement->nextElement; //Jinak se aktivita posune na nasledujici prvek
    }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

    if(list == NULL || list->activeElement == NULL){

    }
    else if(list->activeElement == list->firstElement){ //Pokud je aktivnim prvkem prvni prvek, aktivita se ztrati
        list->activeElement = NULL;
    }
    else{
        list->activeElement = list->activeElement->previousElement; //Jinak se aktivita posune na predchozi prvek
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {

    return (list->activeElement != NULL) ? 1 : 0;
}

/* Konec c206.c */
