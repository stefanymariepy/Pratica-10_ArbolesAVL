# ============================================================
# SIS210 - Semana 10: Arboles AVL
# avl_python.py  -  Actividad 2B (BST vs AVL) y Actividad 3 (SIGA-UNAP-AVL)
#
# Uso:  python avl_python.py          -> tabla 2B y luego el menu
#       python avl_python.py tabla    -> solo la tabla comparativa (2B)
#       python avl_python.py menu     -> solo el menu interactivo (3)
# ============================================================
import sys
import random
import math
from time import perf_counter
from dataclasses import dataclass

sys.setrecursionlimit(30000)   # el BST ordenado con n=10000 llega a 10000 niveles


class Nodo:
    def __init__(self, clave, valor=None):
        self.clave = clave
        self.valor = valor          # dato asociado (en la Actividad 3 es el estudiante)
        self.izq = self.der = None
        self.altura = 1             # una hoja tiene altura 1


# ---------- Funciones del AVL ----------
def height(n):
    """Altura de un subarbol (0 si esta vacio)."""
    return n.altura if n else 0


def fb(n):
    """Factor de balance = altura(der) - altura(izq). En un AVL vale -1, 0 o 1."""
    return height(n.der) - height(n.izq) if n else 0


def actualizar(n):
    """Recalcula la altura de n a partir de sus hijos."""
    n.altura = 1 + max(height(n.izq), height(n.der))


def rotar_der(z):
    """Rotacion simple a la derecha. Devuelve la nueva raiz del subarbol (y)."""
    y = z.izq
    z.izq = y.der       # el subarbol B pasa a ser hijo izquierdo de z
    y.der = z           # z baja a la derecha de y
    actualizar(z)       # primero z (ahora es hijo) y luego y
    actualizar(y)
    return y


def rotar_izq(z):
    """Rotacion simple a la izquierda. Devuelve la nueva raiz del subarbol (y)."""
    y = z.der
    z.der = y.izq
    y.izq = z
    actualizar(z)
    actualizar(y)
    return y


def balancear(n):
    """Corrige los 4 casos (LL, RR, LR, RL) y devuelve la raiz del subarbol."""
    actualizar(n)
    f = fb(n)
    if f < -1:                          # pesa a la izquierda
        if fb(n.izq) > 0:               # caso LR: rotar primero el hijo
            n.izq = rotar_izq(n.izq)
        return rotar_der(n)             # caso LL
    if f > 1:                           # pesa a la derecha
        if fb(n.der) < 0:               # caso RL: rotar primero el hijo
            n.der = rotar_der(n.der)
        return rotar_izq(n)             # caso RR
    return n


def insertar_avl(raiz, clave, valor=None):
    """Inserta como en un BST y balancea al regresar. Devuelve la nueva raiz."""
    if raiz is None:
        return Nodo(clave, valor)
    if clave < raiz.clave:
        raiz.izq = insertar_avl(raiz.izq, clave, valor)
    elif clave > raiz.clave:
        raiz.der = insertar_avl(raiz.der, clave, valor)
    else:                               # clave repetida: solo se actualiza el valor
        raiz.valor = valor
        return raiz
    return balancear(raiz)


def buscar(raiz, clave):
    """Busca una clave. Devuelve el nodo o None. Costo O(log n) en un AVL."""
    while raiz is not None:
        if clave == raiz.clave:
            return raiz
        raiz = raiz.izq if clave < raiz.clave else raiz.der
    return None


def inorden(raiz, resultado):
    """Recorrido inorden: llena 'resultado' con los nodos en orden de clave."""
    if raiz is None:
        return
    inorden(raiz.izq, resultado)
    resultado.append(raiz)
    inorden(raiz.der, resultado)


# ---------- BST simple (sin balanceo) ----------
def insertar_bst(raiz, clave):  # BST simple sin balanceo
    if raiz is None:
        return Nodo(clave)
    if clave < raiz.clave:
        raiz.izq = insertar_bst(raiz.izq, clave)
    elif clave > raiz.clave:
        raiz.der = insertar_bst(raiz.der, clave)
    return raiz


def altura_arbol(n):
    if n is None:
        return 0
    return 1 + max(altura_arbol(n.izq), altura_arbol(n.der))


# ---------- Actividad 2B: experimento BST vs AVL ----------
def experimento(n, orden='random'):
    claves = list(range(1, n + 1))
    if orden == 'random':
        random.shuffle(claves)
    # BST simple
    raiz_bst = None
    for c in claves:
        raiz_bst = insertar_bst(raiz_bst, c)
    h_bst = altura_arbol(raiz_bst)
    # AVL
    raiz_avl = None
    for c in claves:
        raiz_avl = insertar_avl(raiz_avl, c)
    h_avl = raiz_avl.altura if raiz_avl else 0
    limite_teorico = int(1.44 * math.log2(n + 1))
    return h_bst, h_avl, limite_teorico


def tabla_comparativa():
    print(f"{'n':>8} | {'BST (ord)':>10} | {'AVL (ord)':>10} | {'Teorico':>10} | {'BST (rnd)':>10} | {'AVL (rnd)':>10}")
    print('-' * 68)
    for n in [100, 500, 1000, 5000, 10000]:
        h_bst_o, h_avl_o, teo = experimento(n, 'ordered')
        h_bst_r, h_avl_r, _ = experimento(n, 'random')
        print(f'{n:>8} | {h_bst_o:>10} | {h_avl_o:>10} | {teo:>10} | {h_bst_r:>10} | {h_avl_r:>10}')


# ---------- Actividad 3: SIGA-UNAP con AVL ----------
@dataclass
class EstudianteUNAP:
    codigo: str
    nombre: str
    escuela: str
    promedio: float


class SIGA_AVL:
    def __init__(self):
        self.raiz = None
        self.total = 0

    def matricular(self, est: EstudianteUNAP):
        if buscar(self.raiz, est.codigo):       # no se permiten codigos repetidos
            print(f'  El codigo {est.codigo} ya esta matriculado')
            return
        t0 = perf_counter()
        # se inserta con el codigo como clave y el estudiante como valor
        self.raiz = insertar_avl(self.raiz, est.codigo, est)
        self.total += 1
        t1 = perf_counter()
        print(f'  Matriculado {est.nombre} en {(t1 - t0) * 1e6:.2f} µs')

    def buscar_estudiante(self, codigo: str):
        t0 = perf_counter()
        nodo = buscar(self.raiz, codigo)
        t1 = perf_counter()
        if nodo:
            print(f'  Encontrado: {nodo.valor} ({(t1 - t0) * 1e6:.2f} µs)')
        else:
            print(f'  Código {codigo} no encontrado')

    def listar_ordenado(self):
        # recorrido inorden del AVL => lista ordenada por codigo
        nodos = []
        inorden(self.raiz, nodos)
        if not nodos:
            print('  (no hay estudiantes)')
        for n in nodos:
            e = n.valor
            print(f'  {e.codigo} | {e.nombre:<14} | {e.escuela:<9} | {e.promedio:>5.1f}')

    def estadisticas(self):
        h = self.raiz.altura if self.raiz else 0
        teo = int(1.44 * math.log2(self.total + 1)) if self.total > 0 else 0
        print(f'  Total estudiantes: {self.total}')
        print(f'  Altura AVL actual: {h}')
        print(f'  Altura teórica max: {teo}')
        print(f'  Eficiencia: {"ÓPTIMO" if h <= teo else "SUBÓPTIMO"}')


def menu():
    siga = SIGA_AVL()
    # Cargar 10 estudiantes de muestra
    muestra = [
        EstudianteUNAP('2026100001', 'Ana Quispe', 'Sistemas', 15.5),
        EstudianteUNAP('2026100002', 'Luis Mamani', 'Sistemas', 14.0),
        EstudianteUNAP('2026100003', 'Rosa Condori', 'IA&CD', 17.2),
        EstudianteUNAP('2026100004', 'Ivan Huanca', 'Sistemas', 13.8),
        EstudianteUNAP('2026100005', 'Maria Apaza', 'IA&CD', 16.9),
        EstudianteUNAP('2026100006', 'Carlos Tito', 'Sistemas', 12.1),
        EstudianteUNAP('2026100007', 'Lucia Ramos', 'IA&CD', 18.0),
        EstudianteUNAP('2026100008', 'Pedro Vargas', 'Sistemas', 15.0),
        EstudianteUNAP('2026100009', 'Sofia Luna', 'IA&CD', 16.4),
        EstudianteUNAP('2026100010', 'Juan Puma', 'Sistemas', 14.7),
    ]
    print('Cargando 10 estudiantes...')
    for e in muestra:
        siga.matricular(e)

    while True:
        print('\n=== SIGA-UNAP-AVL ===')
        print('1. Matricular nuevo estudiante')
        print('2. Buscar por código')
        print('3. Listar todos (inorden)')
        print('4. Estadísticas del árbol')
        print('5. Salir')
        op = input('Opción: ').strip()
        if op == '1':
            cod = input('  Código: ').strip()
            nom = input('  Nombre: ')
            esc = input('  Escuela: ')
            try:
                prom = float(input('  Promedio: '))
            except ValueError:
                print('  Promedio no válido: debe ser un número')
                continue
            siga.matricular(EstudianteUNAP(cod, nom, esc, prom))
        elif op == '2':
            siga.buscar_estudiante(input('  Código: ').strip())
        elif op == '3':
            siga.listar_ordenado()
        elif op == '4':
            siga.estadisticas()
        elif op == '5':
            break
        else:
            print('  Opción no válida')


if __name__ == '__main__':
    modo = sys.argv[1] if len(sys.argv) > 1 else 'todo'
    if modo in ('tabla', 'todo'):
        tabla_comparativa()
    if modo in ('menu', 'todo'):
        menu()
