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

  bst_node_t *temp = tree;
  while(temp != NULL){
    if(tree->key == key){
      return true;
    }
    else{
      if(key < tree->key){
        tree = tree->left;
      }
      else{
        tree = tree->right;
      }
      if(tree == NULL){
        return false;
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

  bst_node_t *temp;
  while((*tree) != NULL){

  if(key == (*tree)->key){
    if(((*tree)->left == NULL) && ((*tree)->right == NULL)){
      free(*tree);
      (*tree) = NULL;
      return;
    }
    else if((*tree)->right != NULL && (*tree)->left == NULL){
      temp = (*tree);
      (*tree) = (*tree)->right;
      free(temp);
      return;
    }
    else if((*tree)->right == NULL && (*tree)->left != NULL){
      temp = (*tree);
      (*tree) = (*tree)->left;
      free(temp);
      return;
    }
    else{
      temp = (*tree);
      bst_replace_by_rightmost(temp, &temp->left);
      return;
    }
    
  }
  else{
    if(key < (*tree)->key){
      (*tree) = (*tree)->left;
    }
    else{
      (*tree) = (*tree)->right;
    }
    if((*tree) == NULL){
      return;
    }
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

  //TAKTOO??
  STACKDEC(bst_node_t*, ptrStack);
  void stack_ptrStack_init(stack_ptrStack_t *ptrStack);

  stack_ptrStack_t *temp = malloc(sizeof(stack_ptrStack_t)); //NNEJDE
  //

  

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
  if(tree == NULL){
    return;
  }
  while(tree != NULL){
    bst_print_node(tree);
    //push na stack
    tree = tree->left;
  }
  return;
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
  if(tree == NULL){
    return;
  }
  //zásobník = malloc(sizeof(zásobník));
  //if(zasobnik == NULL) return;
  //stack_init (zasobnik);
  
  bst_leftmost_preorder(tree, //zasobnik);

  while(//!zasobnik_empty(zasobnik)){
    tree = //stack_pop_top(zasobnik);
    bst_leftmost_preorder(tree->right, //zasobnik);
  }
  free(//stack);
  return;
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
  if(tree == NULL){
    return;
  }

  while(tree != NULL){
    //stack_push(//zasobnik, tree);
    tree = tree->left;
  }
  return;
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
  if(tree == NULL){
    return;
  }
  //zasobnik = malloc(sizeof(stack));
  if(//zasobnik == NULL){
    return;
  }
  //zasobnik_init(stack);

  bst_leftmost_inorder(tree, //zasobnik);

  while(!//stack_notEmpty){
    tree = //stack_pop_top(stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, //stack);
  }

  free(//stack);
  return;
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
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  if(tree == NULL){
    return;
  }
  while(tree != NULL){
    //stack_push(//stack, tree);
    //stack_push(//stack, true);
    tree = tree->left;
  }
  return;
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
  if(tree == NULL){

  }
  //stack = malloc(sizeof(stack));
  //stack2 = malloc(sizeof(stack));
  if(//stack == NULL || stack2 == NULL){
    return;
  }
  //stack_init;
  //stack2_init;

  bst_leftmost_postorder(tree, stack, stack2);
  bool isLeft;

  while(!//stackEmpty(stack)){
    tree = //stack_popTop;
    isLeft = //stack2_popTop;

    if(isLeft){
      //stack_push(stack, tree);
      //stack2_push(stack, false);
      bst_leftmost_postorder(tree->right, //stack, //stack2);
    }
    else{
      bst_print_node(tree);
    }
  }
  free(//stack);
  free(//stack2);
  return;
}
