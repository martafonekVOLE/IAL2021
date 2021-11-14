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

  while(temp != NULL){        //prohledávací algoritmus bst
    if(temp->key == key){     //prvek nalezen
      return true;
    }

    else{                     //hledání prvku
      if(key < temp->key){
        temp = temp->left;
      }
      else{
        temp = temp->right;
      }
      if(temp == NULL){
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

  if(temp == NULL){           //Vložení prvního prvku do bst
    bst_node_t *new = malloc(sizeof(bst_node_t));
  
    if(new == NULL){          //Ověření chybné alokace
      return;
    }

    new->value = value;       //hodnota a klíč prvku
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

          new->value = value; //Vkládání hodnoty
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

        new->value = value;   //vkládání hodnoty
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

  if((*tree)->right == NULL){   //Nalezení nejpravějšího prvku
    bst_node_t *temp = *tree;
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    
    *tree = temp->left;
    free(temp);
  }
  else{                       //Hledání nejpravějšího prvku
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
void bst_delete(bst_node_t **tree, char key) {    //Aktuálně vypisuje jen napojovaný podstrom - mohlo by způsobit segfault
  /*if(*tree == NULL){
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
  }*/
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
	if (*tree == NULL){
    return;
  }

	stack_bst_t* stack = malloc(sizeof(stack_bst_t));     //vytvoření zásobníku
	if (stack == NULL){
    return;
  }
	stack_bst_init(stack);    //inicializace

	bst_node_t *temp = *tree;

	while (temp != NULL || !stack_bst_empty(stack)) {   //dokud není zásobník prázdný nebo nejsme nakonci stromu loopuj
		if (temp == NULL){
      temp = stack_bst_pop(stack);
    }

		if (temp->right != NULL){
      stack_bst_push(stack, temp->right);
    }

		bst_node_t *tempF = temp;
		temp = temp->left;
		free(tempF);
	}

	*tree = NULL;
	free(stack);

	return;

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
    stack_bst_push(to_visit, tree);
    bst_print_node(tree);
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

  stack_bst_t* stack = malloc(sizeof(stack_bst_t));   //práce se zásobníkem (jako u předchozí funkce)
  if(stack == NULL){
    return;
  }
  stack_bst_init(stack);

  bst_leftmost_preorder(tree, stack);

  while(!stack_bst_empty(stack)){                     //loopování zásobníkem (jako u předchozí funkce)
    tree = stack_bst_top(stack);
    tree = stack_bst_pop(stack);
    bst_leftmost_preorder(tree->right, stack);
  }

  free(stack);
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
    stack_bst_push(to_visit, tree);
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
  
  stack_bst_t* stack = malloc(sizeof(stack_bst_t));     //(tvorba, init - jako u předchozího orderu)
  if(stack == NULL){
    return;
  }
  stack_bst_init(stack);

  bst_leftmost_inorder(tree, stack);

  while(!stack_bst_empty(stack)){                     //same loopování, jiné pořadí
    tree = stack_bst_top(stack);
    tree = stack_bst_pop(stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, stack);
  }

  free(stack);
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
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
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
    return;
  }

  bool isLeft;
  stack_bst_t* stack = malloc(sizeof(stack_bst_t));         //tvorba dvou zásobníku - jeden klasicky jako prve, druhý bool
  stack_bool_t* boolStack = malloc(sizeof(stack_bool_t));   // bool použijeme pro získání přehledu kde v zásobníku se pohybujeme
  if(stack == NULL || boolStack == NULL){
    return;
  }
  stack_bst_init(stack);
  stack_bool_init(boolStack);

  bst_leftmost_postorder(tree, stack, boolStack);

  while(!stack_bst_empty(stack)){
    tree = stack_bst_top(stack);
    isLeft = stack_bool_top(boolStack);
    stack_bool_pop(boolStack);

    if(isLeft){
      stack_bool_push(boolStack, false);
      bst_leftmost_postorder(tree->right, stack, boolStack);
    }
    else{
      stack_bst_pop(stack);
      bst_print_node(tree);
    }
  }
  free(stack);
  free(boolStack);
  return;
}
