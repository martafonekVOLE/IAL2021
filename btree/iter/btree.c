/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  if(tree == NULL){
    return;
  }
  *tree = NULL;
  return;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if(tree == NULL){
    return false;
  }
  else{
    if(tree->key == key){
      *value = tree->value;
      return true;
    }

    else if(tree->key > key){
      bst_node_t *temp = tree;
      while(temp != NULL){
        if(temp->key == key){
          return true;
        }
        else if(temp->key < key){
          while(temp != NULL){
            if(temp->key == key){
              return true;
            }
            else{
              temp = temp->right;
            }
          }
        }
        else{
          temp = temp->left;
        }
      }
    }

    else{
      bst_node_t *temp = tree;
      while(temp != NULL){
        if(temp->key == key){
          return true;
        }
        else if(temp->key > key){
          while(temp != NULL){
            if(temp->key == key){
              return true;
            }
            else{
              temp = temp->left;
            }
          }
        }
        else{
          temp = temp->right;
        }
      }
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *temp = *tree;

  if(temp == NULL){
    bst_node_t *new = malloc(sizeof(bst_node_t));

    if(new == NULL){          //Chyba alokace
      return;
    }

    new->value = value;
    new->key = key;
    new->left = NULL;
    new->right = NULL;
    (*tree) = new;
  }
  else{
    while(temp != NULL){

      if(key == temp->key){     //Nalezení jíž existujícího prvku a přepsání hodnoty
        temp->value = value;
        temp->key = key;
        return;
      }
      
      if(key < temp->key){      //Hledaná položka je v levé části podstromu
        if(temp->left == NULL){
          bst_node_t *new = malloc(sizeof(bst_node_t));

          if(new == NULL){    //Chyba alokace
            return;
          }

          new->value = value;
          new->key = key;
          new->left = NULL;
          new->right = NULL;
          temp->left = new;

          return;
        }
        else{
          temp = temp->left;
          continue;
        }
      }

      else if(key > temp->key){ //Hledaná položka je v pravé části podstromu
        if(temp->right == NULL){
        bst_node_t *new = malloc(sizeof(bst_node_t));

        if(new == NULL){      //Chyba alokace
            return;
        }

        new->value = value;
        new->key = key;
        new->left = NULL;
        new->right = NULL;
        temp->right = new;

        return; 
        }
        else{
          temp = temp->right;
          continue;
        }
      }
    }
  }


  return;
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if(*tree == NULL || target == NULL){
    return;
  }

  if((*tree)->right == NULL){
    bst_node_t *temp = *tree;
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    
    *tree = temp->left;
    free(temp);
  }
  else{
    bst_node_t *temp = *tree;
    while(temp->right != NULL){
      printf("xDoA3C");
      temp = temp->right;
    }
    target->key = temp->key;
    target->value = temp->value;
    *tree = temp->left;
    free(temp->right);
  }

}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if(*tree == NULL){
    return;
  }

  bst_node_t *temp = (*tree);

  while(key != temp->key && temp != NULL){
    if(key < temp->key){
      if(temp->left == NULL || temp->key == key){
        break;
      }
      temp = temp->left;
    }
    if(key > temp->key){
      if(temp->right == NULL || temp->key == key){
        break;
      }
      temp = temp->right;
    }
    if (key == temp->key){
      break;
    }
  }
  if(key == temp->key){

    if((temp->left == NULL) && (temp->right == NULL)){
      free(temp);
      temp = NULL;
    }
  
    else if(temp->right != NULL && temp->left != NULL){
      bst_replace_by_rightmost(temp, &temp->left);
    }
    else{
      return;
    }
    
  }
  return;
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if((*tree) == NULL){
    return;
  }
  bst 

}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
}
