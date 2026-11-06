# Práctica Semana 10: Árboles AVL
**Curso:** Algoritmos y Estructuras de Datos (SIS210)  
**Semestre:** 2026-II  
**Universidad:** Universidad Nacional del Altiplano (UNAP), Puno  

---

## Datos del Estudiante
* **Nombre:** Stefany Marie Pampa Yucra  
* **Código:** 251273  
* **Docente:** Dr. Aldo Hernán Zanabria Gálvez  

---

## Contenido de la Carpeta

* **avl.h**: Implementación en C++17 de la estructura del Árbol AVL. Incluye funciones clave como rotar_derecha, rotar_izquierda, balancear, insertar_avl (Actividad 1) y eliminar_avl (Actividad 2A).
* **avl_main.cpp**: Programa principal en C++ que contiene y ejecuta los casos de prueba automatizados de la P1 a la P6.
* **avl_python.py**: Script en Python 3.8+ para la comparación empírica entre BST y AVL (Actividad 2B) y el sistema de menú interactivo SIGA-UNAP-AVL (Actividad 3).
* **README.md**: Este archivo con la documentación del proyecto, instrucciones de ejecución y respuestas teóricas.

---

## Instrucciones de Ejecución

### Componente en C++ (Actividades 1 y 2A)
Para compilar y ejecutar las pruebas de árboles AVL utilizando el estándar C++17, elije el comando adecuado para tu sistema operativo:

* **Compilación general (Linux / macOS / Bash):**
  ```bash
  g++ -std=c++17 -Wall -Wextra -o avl_main avl_main.cpp
  ./avl_main
  ```

* **Ejecución limpia en Windows (PowerShell):**
  ```powershell
  g++ -std=c++17 -Wall -Wextra -o temporal_eliminar avl_main.cpp; ./temporal_eliminar; Remove-Item temporal_eliminar.exe
  ```

### Componente en Python (Actividades 2B y 3)
Requires Python 3.8 o superior. Tienes tres formas de ejecutarlo mediante argumentos en la terminal:

1. **Ejecución Completa (Tabla + Menú):**
   ```bash
   python avl_python.py
   ```
2. **Solo Tabla Comparativa (Análisis Empírico):**
   ```bash
   python avl_python.py tabla
   ```
3. **Solo Menú Interactivo (SIGA-UNAP-AVL):**
   ```bash
   python avl_python.py menu
   ```

Nota para Google Colab: Si trabajas en un cuaderno de Colab, ejecuta las celdas usando los comandos mágicos correspondientes:
```python
%run avl_python.py tabla
%run avl_python.py menu
```

---

## Respuestas Teóricas y Resultados

### Actividad 1: Análisis de Rotaciones y Alturas

* **P1-A. Inserción de 10, 20, 30:** Ocurre 1 rotación. El desbalance generado es de tipo RR (el nodo 30 se inserta a la derecha del hijo derecho de 10). Se corrige mediante una rotación simple a la izquierda sobre el nodo 10. Tras el rebalanceo, la nueva raíz del árbol pasa a ser el 20.
* **P1-B. Inserción de 30, 10, 20:** Ocurren 2 rotaciones simples que en conjunto forman una rotación doble de tipo LR. Primero se realiza una rotación a la izquierda sobre el nodo 10 (dejando al 20 como hijo izquierdo de 30) y posteriormente una rotación a la derecha sobre el nodo 30. La raíz final del árbol pasa a ser el 20.
* **P1-C. Altura Máxima con 15 nodos:** La altura máxima posible de un Árbol AVL con 15 nodos es 5. Estrictamente, un AVL de altura 5 requiere como mínimo 12 nodos, mientras que uno de altura 6 requiere al menos 20 nodos (según la cota superior analítica h <= 1.44 * log2(n+2) - 0.33, que para 15 nodos da aproximadamente 5.6). La prueba P3 restringe deliberadamente la altura a h <= 4 debido a que inserta los elementos de forma ordenada del 1 al 15, resultando en un árbol perfectamente balanceado de altura 4 (15 = 2^4 - 1), que representa el mejor caso posible.

---

### Actividad 2B: Comparación Empírica (BST vs AVL)

A continuación se muestran los resultados de las alturas obtenidas tras una ejecución real del script comparativo:

| Número de Nodos (n) | BST (Ordenado) | AVL (Ordenado) | Límite Teórico | BST (Aleatorio) | AVL (Aleatorio) |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **100** | 100 | 7 | 9 | 17 | 8 |
| **500** | 500 | 9 | 12 | 18 | 11 |
| **1000** | 1000 | 10 | 14 | 25 | 12 |
| **5000** | 5000 | 13 | 17 | 31 | 15 |
| **10000** | 10000 | 14 | 19 | 31 | 16 |

*Los valores de las columnas aleatorias pueden variar ligeramente en cada ejecución debido a la naturaleza estocástica de los datos.*

#### Conclusiones del Análisis:
* **P2B-A.** La altura del BST con inserción ordenada es exactamente igual al número de nodos (h = n). Al no poseer mecanismos de balanceo, el árbol se degenera completamente convirtiéndose en una lista enlazada. El costo de búsqueda en este escenario crítico se degrada de un comportamiento logarítmico ideal a uno lineal: O(n).
* **P2B-B.** El Árbol AVL no supera en ningún caso el límite teórico establecido. Para un volumen de n = 10000 nodos, la cota superior estricta es 1.44 * log2(10001) aprox. 19.13 (19 niveles al truncar). El AVL real alcanzó una altura de 14 con inserción ordenada y 16 con inserción aleatoria, manteniéndose convenientemente 5 y 3 niveles por debajo del peor caso teórico respectivamente.
* **P2B-C.** Las alturas generadas por datos ordenados y aleatorios en el AVL no son idénticas, pero se mantienen sumamente cercanas (ej. 14 frente a 16 para n = 10000). Esto demuestra la robustez del AVL: al aplicar rotaciones correctoras tras cada inserción, la altura queda asintóticamente acotada entre log2(n+1) y 1.44 * log2(n+2). La inserción ordenada genera estructuras casi perfectas, mientras que la aleatoria introduce pequeñas asimetrías permitidas dentro del factor de balanceo en {-1, 0, 1}.

---

### Actividad 3: Sistema SIGA-UNAP-AVL
Este módulo simula un entorno interactivo de gestión académica. Al iniciar, precarga automáticamente 10 estudiantes de muestra y despliega un menú interactivo con las siguientes capacidades optimizadas mediante la estructura AVL:
1. **Matricular Estudiante:** Inserta un nuevo registro en el Árbol AVL, validando estrictamente que no existan códigos duplicados y calculando el tiempo de ejecución de la operación en microsegundos (us).
2. **Buscar por Código:** Localiza instantáneamente a un estudiante devolviendo sus datos y la eficiencia temporal del proceso.
3. **Listar Estudiantes:** Despliega los alumnos en orden ascendente según su código mediante un recorrido en Inorden.
4. **Estadísticas de la Estructura:** Muestra métricas de control como el total de alumnos matriculados, la altura actual del AVL, su límite teórico esperado y el porcentaje de eficiencia de balanceo.
