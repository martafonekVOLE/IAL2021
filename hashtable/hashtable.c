/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  if (table == NULL){     /*Kontrola, že existuje pointer*/
      return;
  }
  
  for(int i = 0; i < HT_SIZE; i++){
    (*table)[i] = NULL;
  }
  return;
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  if (table == NULL){
    return NULL;
  }
  
  ht_item_t *temp = (*table)[get_hash(key)];
  while(temp != NULL){
    if(temp->key == key){
      return temp;
    }
    else{
      temp = temp->next;
    }
  }
  return NULL; //return temp?
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  if(table == NULL){
    return;
  }

  ht_item_t *temp = ht_search(table, key);
  if(temp != NULL){
    temp->value = value;
    return;
  }
  else{
    temp = malloc(sizeof(ht_item_t));
    if(temp == NULL){
      //Alokace selhala
      return;
    }
    temp->key = key;
    temp->value = value;
    temp->next = (*table)[get_hash(key)];
    (*table)[get_hash(key)] = temp;
    return;
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if(table == NULL){
    return NULL;
  }

  ht_item_t *temp = ht_search(table, key);
  if (temp != NULL){
    return &temp->value;
  }
  else{
    return NULL;
  }
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  ht_item_t *temp = (*table)[get_hash(key)];

  if (temp == NULL || table == NULL){
    return;                           /*Ověření prádzné tabulky & neinicializované tabulky*/
  }

  if(temp->key != key){               /*Prohledávací algoritmus*/
    ht_item_t *previous;
    while(temp != NULL){

      if(temp->key == key){
        previous->next = temp->next;
        free(temp);
        break;
      }

      previous = temp;
      temp = temp->next;
    }
  }

  else{                               /*První prvek je hledaný*/
    if(temp->next == NULL){           /*Je zároveň jediný prvek*/
      temp = NULL;
    }
    else{                             /*Má následníka*/
      (*table)[get_hash(key)] = temp->next;
    }
    free(temp);
  }
  return;
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  if(table == NULL){
    return;                           /*Prevence memory leaku*/
  }
  ht_item_t *temp;
  for(int i = 0; i < HT_SIZE; i++){
    while((*table)[i] != NULL){
      temp = (*table)[i]->next;
      free((*table)[i]);
      (*table)[i] = temp;
    }
  }
  return;
}
