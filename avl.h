// ============================================================
// SIS210 - Semana 10: Arboles AVL
// avl.h  (C++17)  -  Actividad 1 y Actividad 2A
// ============================================================
#pragma once

#include <memory>
#include <algorithm>
#include <string>
#include <utility>

// Nodo del arbol: guarda la clave, el valor, sus dos hijos y su altura.
// Una hoja tiene altura 1 y un puntero nulo tiene altura 0.
template<typename K, typename V = K>
struct NodoBST {
    K clave; V valor;
    std::unique_ptr<NodoBST> izq, der;
    int altura;
    explicit NodoBST(K k, V v = V{})
        : clave(std::move(k)), valor(std::move(v)),
          izq(nullptr), der(nullptr), altura(1) {}
};

// Altura de un subarbol (0 si esta vacio)
template<typename K, typename V>
int altura(const std::unique_ptr<NodoBST<K,V>>& n)
{ return n ? n->altura : 0; }

// Factor de balance = altura(derecho) - altura(izquierdo)
// En un AVL siempre debe valer -1, 0 o 1
template<typename K, typename V>
int factor_balance(const std::unique_ptr<NodoBST<K,V>>& n)
{ return n ? altura(n->der) - altura(n->izq) : 0; }

// Recalcula la altura de un nodo a partir de la de sus hijos
template<typename K, typename V>
void actualizar_altura(std::unique_ptr<NodoBST<K,V>>& n)
{ if (n) n->altura = 1 + std::max(altura(n->izq), altura(n->der)); }

// TODO 1: rotar_derecha(z)
// Pre:  z y su hijo izquierdo existen.
// Post: el hijo izquierdo y pasa a ser la raiz del subarbol; z queda como su hijo derecho.
//       Se conserva el orden BST y se actualizan las alturas. Costo: O(1)
/*
         z              y
        / \            / \
       y   C   -->    A   z
      / \                / \
     A   B              B   C
*/
template<typename K, typename V>
void rotar_derecha(std::unique_ptr<NodoBST<K,V>>& z) {
    std::unique_ptr<NodoBST<K,V>> y = std::move(z->izq);  // y = z->izq
    z->izq = std::move(y->der);                           // B pasa a ser hijo izquierdo de z
    actualizar_altura(z);                                 // z ya es hijo: actualizar antes de subirlo
    y->der = std::move(z);                                // z baja a la derecha de y
    actualizar_altura(y);
    z = std::move(y);                                     // y ocupa el lugar de z
}

// TODO 2: rotar_izquierda(z)  (simetrica a la anterior)
/*
       z                  y
      / \                / \
     A   y      -->     z   C
        / \            / \
       B   C          A   B
*/
template<typename K, typename V>
void rotar_izquierda(std::unique_ptr<NodoBST<K,V>>& z) {
    std::unique_ptr<NodoBST<K,V>> y = std::move(z->der);  // y = z->der
    z->der = std::move(y->izq);                           // B pasa a ser hijo derecho de z
    actualizar_altura(z);
    y->izq = std::move(z);                                // z baja a la izquierda de y
    actualizar_altura(y);
    z = std::move(y);
}

// TODO 3: balancear(n)
// Pre:  los subarboles de n ya son AVL; n puede tener factor de balance -2 o +2.
// Post: n queda como AVL con altura actualizada. Costo: O(1)
template<typename K, typename V>
void balancear(std::unique_ptr<NodoBST<K,V>>& n) {
    actualizar_altura(n);
    int fb = factor_balance(n);

    if (fb < -1) {                                  // pesa mas a la izquierda
        if (factor_balance(n->izq) > 0)             // caso LR: primero rotar el hijo
            rotar_izquierda(n->izq);
        rotar_derecha(n);                           // caso LL (o LR ya preparado)
    } else if (fb > 1) {                            // pesa mas a la derecha
        if (factor_balance(n->der) < 0)             // caso RL: primero rotar el hijo
            rotar_derecha(n->der);
        rotar_izquierda(n);                         // caso RR (o RL ya preparado)
    }
}

// TODO 4: insertar_avl(raiz, clave, valor)
// Igual que insertar en un BST, pero al volver de cada llamada se llama a balancear.
// Pre: raiz es un AVL valido (o nulo). Post: clave insertada y arbol AVL. Costo: O(log n)
template<typename K, typename V>
void insertar_avl(std::unique_ptr<NodoBST<K,V>>& raiz,
                  K clave, V valor = V{}) {
    if (!raiz) {                                    // lugar vacio: crear el nodo
        raiz = std::make_unique<NodoBST<K,V>>(std::move(clave), std::move(valor));
        return;
    }
    if (clave < raiz->clave)
        insertar_avl(raiz->izq, std::move(clave), std::move(valor));
    else if (clave > raiz->clave)
        insertar_avl(raiz->der, std::move(clave), std::move(valor));
    else {                                          // clave repetida: solo se actualiza el valor
        raiz->valor = std::move(valor);
        return;
    }
    balancear(raiz);                                // reequilibrar al regresar
}

// YA IMPLEMENTADA - no modificar
template<typename K, typename V>
NodoBST<K,V>* buscar(const std::unique_ptr<NodoBST<K,V>>& raiz,
                     const K& clave) {
    if (!raiz) return nullptr;
    if (clave == raiz->clave) return raiz.get();
    if (clave < raiz->clave) return buscar(raiz->izq, clave);
    return buscar(raiz->der, clave);
}

// ------------------------------------------------------------
// ACTIVIDAD 2A: eliminacion AVL
// ------------------------------------------------------------

// Nodo con la clave mas pequena de un subarbol (el sucesor inorden)
template<typename K, typename V>
NodoBST<K,V>* minimo(const std::unique_ptr<NodoBST<K,V>>& n) {
    return n->izq ? minimo(n->izq) : n.get();
}

// eliminar_avl(raiz, clave)
// Pre: raiz es un AVL valido. Post: la clave ya no esta y el arbol sigue siendo AVL.
// Costo: O(log n)
template<typename K, typename V>
void eliminar_avl(std::unique_ptr<NodoBST<K,V>>& raiz,
                  const K& clave) {
    if (!raiz) return;
    if (clave < raiz->clave)
        eliminar_avl(raiz->izq, clave);
    else if (clave > raiz->clave)
        eliminar_avl(raiz->der, clave);
    else {
        if (!raiz->izq && !raiz->der) {             // Caso 1: sin hijos
            raiz = nullptr;
        } else if (!raiz->izq) {                    // Caso 2: solo hijo derecho
            raiz = std::move(raiz->der);
        } else if (!raiz->der) {                    // Caso 2: solo hijo izquierdo
            raiz = std::move(raiz->izq);
        } else {                                    // Caso 3: dos hijos
            NodoBST<K,V>* suc = minimo(raiz->der);  //   sucesor inorden
            raiz->clave = suc->clave;               //   copiar clave y valor
            raiz->valor = suc->valor;
            K clave_suc = raiz->clave;              //   (copia: el nodo sucesor se va a borrar)
            eliminar_avl(raiz->der, clave_suc);     //   eliminar el sucesor
        }
    }
    if (raiz) balancear(raiz);                      // reequilibrar
}
