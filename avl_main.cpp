// ============================================================
// SIS210 - Semana 10: Arboles AVL
// avl_main.cpp  -  Casos de prueba P1 a P6
// ============================================================
#include <iostream>
#include <cassert>
#include "avl.h"

// Funcion auxiliar: retorna la altura del arbol
template<typename K, typename V>
int h(const std::unique_ptr<NodoBST<K,V>>& r) { return r ? r->altura : 0; }

// Verifica que todos los nodos tengan factor de balance -1, 0 o 1
template<typename K, typename V>
bool es_avl(const std::unique_ptr<NodoBST<K,V>>& n) {
    if (!n) return true;
    int fb = factor_balance(n);
    return fb >= -1 && fb <= 1 && es_avl(n->izq) && es_avl(n->der);
}

void test_P1_insercion_simple() {
    // Insertar 3 claves ordenadas: en BST seria h=3; en AVL h=2
    std::unique_ptr<NodoBST<int,int>> r = nullptr;
    insertar_avl(r, 10, 10);
    insertar_avl(r, 20, 20);
    insertar_avl(r, 30, 30); // provoca rotacion RR (simple a la izquierda)
    assert(h(r) == 2 && "P1: altura debe ser 2 tras RR rotation");
    assert(r->clave == 20 && "P1: raiz debe ser 20");
    std::cout << "[P1 OK] insercion_simple" << std::endl;
}

void test_P2_caso_LR() {
    std::unique_ptr<NodoBST<int,int>> r = nullptr;
    insertar_avl(r, 30, 30);
    insertar_avl(r, 10, 10);
    insertar_avl(r, 20, 20); // provoca rotacion LR (doble)
    assert(r->clave == 20 && "P2: raiz debe ser 20");
    assert(h(r) == 2 && "P2: altura debe ser 2");
    std::cout << "[P2 OK] caso_LR" << std::endl;
}

void test_P3_insercion_ordenada_grande() {
    // Insertar 1..15 en orden: un BST degeneraria con h=15
    std::unique_ptr<NodoBST<int,int>> r = nullptr;
    for (int i = 1; i <= 15; i++) insertar_avl(r, i, i);
    assert(h(r) <= 4 && "P3: altura debe ser <= 4 para 15 nodos AVL");
    assert(buscar(r, 8) && "P3: nodo 8 debe existir");
    std::cout << "[P3 OK] insercion_ordenada_grande h=" << h(r) << std::endl;
}

void test_P4_eliminar_hoja() {
    std::unique_ptr<NodoBST<int,int>> r = nullptr;
    for (int x : {20, 10, 30}) insertar_avl(r, x, x);
    eliminar_avl(r, 10); // hoja izquierda
    assert(!buscar(r, 10) && "P4: nodo 10 debe haberse eliminado");
    assert(h(r) <= 2 && "P4: arbol sigue balanceado");
    std::cout << "[P4 OK] eliminar_hoja" << std::endl;
}

void test_P5_eliminar_raiz_dos_hijos() {
    std::unique_ptr<NodoBST<int,int>> r = nullptr;
    for (int x : {20, 10, 30, 25, 35}) insertar_avl(r, x, x);
    int raiz_orig = r->clave;
    eliminar_avl(r, 20); // nodo raiz con 2 hijos
    assert(!buscar(r, 20) && "P5: nodo 20 eliminado");
    assert(r->clave != raiz_orig && "P5: la raiz fue reemplazada por su sucesor");
    assert(h(r) <= 3 && "P5: arbol sigue balanceado");
    std::cout << "[P5 OK] eliminar_raiz_dos_hijos" << std::endl;
}

void test_P6_eliminar_con_rebalanceo() {
    // Insertar 1..7, eliminar 1 y verificar el balanceo
    std::unique_ptr<NodoBST<int,int>> r = nullptr;
    for (int i = 1; i <= 7; i++) insertar_avl(r, i, i);
    int h_antes = h(r);
    eliminar_avl(r, 1);
    // Verificar que el arbol sigue siendo AVL
    assert(h(r) <= h_antes && "P6: altura no aumento tras eliminacion");
    assert(es_avl(r) && "P6: todos los factores de balance estan en -1, 0, 1");
    std::cout << "[P6 OK] eliminar_con_rebalanceo" << std::endl;
}

int main() {
    test_P1_insercion_simple();
    test_P2_caso_LR();
    test_P3_insercion_ordenada_grande();
    test_P4_eliminar_hoja();
    test_P5_eliminar_raiz_dos_hijos();
    test_P6_eliminar_con_rebalanceo();
    std::cout << "=== Todos los casos P1-P6 pasaron ===" << std::endl;
    return 0;
}
