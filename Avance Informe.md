# Sistema de Gestión de Tareas y Proyectos en C

Avance de informe del proyecto · Asignatura ICI1242 · Escuela de Ingeniería Informática, PUCV · Noviembre 2026

Ignacio Aracena · Nicolas Campos · Franco Gallo · Vicente Gonzalez · Antonio Martínez

## 1. Introducción

En proyectos donde participan varias personas, la información sobre el trabajo suele quedar dispersa: quién es responsable de cada actividad, qué plazos existen y cuánto se ha avanzado. Sin una vista centralizada, es difícil coordinar al equipo y detectar atrasos a tiempo.

Este informe presenta el avance del proyecto : un gestor de tareas por consola. La aplicación organiza el trabajo en proyectos, que contienen tareas divisibles en subtareas. Cada tarea tiene descripción, responsable, fecha límite y estado (Por hacer, En proceso o Completado), lo que permite saber quién debe hacer qué, para cuándo y qué porcentaje del proyecto está terminado.

El usuario interactúa mediante un menú principal que ofrece gestión de proyectos, de tareas y de usuarios, consultas de tareas próximas a vencer y almacenamiento de datos en archivos CSV. La aplicación valida los datos ingresados y mantiene reglas de consistencia, como no permitir completar una tarea con subtareas pendientes.

Técnicamente, el proyecto aplica los contenidos de estructuras de datos de la asignatura, eligiendo cada TDA según las operaciones más frecuentes: una tabla hash para búsquedas en tiempo promedio constante, listas enlazadas para las colecciones que se recorren completas, un árbol general para la jerarquía de tareas y un montículo mínimo para obtener las tareas con vencimiento más cercano. Su principal desafío es integrar estas estructuras de forma consistente y administrar memoria dinámica en C, aunque se considera realizable al poder construirse por etapas.

El informe se organiza así: la sección 2 describe la aplicación y sus funcionalidades; la sección 3, la solución propuesta (tipos de datos, TDA e implementación con su complejidad); y la sección 4, los aspectos desafiantes del proyecto.

## 2. Descripción de la aplicación

### 2.1. Descripción general

La aplicación es un gestor de tareas por consola. Cada proyecto tiene nombre, descripción y fechas de inicio y fin, y contiene tareas que pueden dividirse en subtareas. Cada tarea tiene descripción, responsable, fecha límite y un estado (Por hacer, En proceso o Completado). La aplicación resuelve la falta de una vista centralizada del trabajo: permite saber quién debe hacer qué, para cuándo y qué porcentaje del proyecto está terminado.

### 2.2. Funcionalidades, menús e interacción con el usuario

La aplicación se maneja con un menú principal. El usuario escoge una opción ingresando su número y, al terminar la operación, la aplicación vuelve al menú. Si se ingresa un dato inválido (proyecto o tarea inexistente, fecha incorrecta u opción fuera de rango), se muestra un mensaje de error y se vuelve a pedir el dato, sin cerrar el programa. Las opciones son las siguientes:

**1. Gestión de proyectos**

- **Crear proyecto:** solicita nombre, descripción y fechas de inicio y fin. Valida que el nombre no exista y que la fecha de fin no sea anterior a la de inicio.
- **Modificar proyecto:** permite editar el nombre, la descripción o las fechas de un proyecto.
- **Listar proyectos:** muestra el nombre, las fechas y el porcentaje de progreso de cada proyecto.
- **Ver progreso:** muestra el porcentaje de tareas completadas y cuántas tareas hay en cada estado.

**2. Gestión de tareas**

- **Agregar tarea o subtarea:** solicita el proyecto, la tarea padre (si es subtarea), la descripción, el responsable y la fecha límite. La tarea se crea con estado Por hacer.
- **Modificar tarea:** permite cambiar la descripción, el responsable, la fecha límite o el estado. No permite marcar como completada una tarea que tiene subtareas pendientes.
- **Eliminar tarea:** elimina la tarea y sus subtareas, previa confirmación del usuario.
- **Listar tareas de un proyecto:** las muestra ordenadas por fecha límite, estado o responsable, según elija el usuario.
- **Ver árbol de tareas:** muestra la jerarquía de tareas y subtareas con sangría.

**3. Gestión de usuarios**

- Registrar usuario con un nombre único, agregar un usuario a un proyecto (para poder asignarle tareas) y ver las tareas de un usuario en todos sus proyectos.

**4. Consultas**

- **Tareas próximas a vencer:** el usuario indica un número N y se muestran las N tareas no completadas con la fecha límite más cercana, considerando todos los proyectos.

**5. Archivos**

- **Guardar y cargar datos:** exporta e importa usuarios, proyectos y tareas desde archivos CSV.

**0. Salir**

## 3. Descripción de la solución

### 3.1. Tipos de datos

La solución utiliza las siguientes estructuras de datos:

- **Usuario:** nombre (string) y una lista con las tareas que tiene asignadas.
- **Tarea:** id, descripción, responsable (Usuario), fecha límite, estado, puntero a su tarea padre y lista de subtareas.
- **Proyecto:** nombre, descripción, fechas de inicio y fin, lista de tareas del primer nivel, mapa de tareas por id, mapa de miembros, y contadores con el total de tareas y las tareas completadas.

Además, se mantiene un mapa global de proyectos (clave: nombre) y un mapa global de usuarios (clave: nombre).

### 3.2. TDAs utilizados

Se utilizarán cuatro TDA. En lo que sigue, n es la cantidad de tareas de un proyecto.

- **Mapa (tabla hash con encadenamiento).** Se usa para los proyectos, usuarios, miembros de un proyecto y tareas por id. Casi todas las opciones del menú parten buscando un proyecto o una tarea, y la tabla hash permite buscar, insertar y eliminar en O(1) promedio, frente a O(n) en una lista. Se redimensiona cuando el factor de carga supera 0,75 para mantener ese rendimiento.
- **Lista (enlazada simple con puntero al último nodo).** Se usa para las subtareas, las tareas de un usuario y los resultados de consultas, que casi siempre se recorren completas. Insertar al final cuesta O(1).
- **Árbol general (cada tarea con una lista de hijos).** Representa de forma natural la relación entre tareas y subtareas. Agregar una subtarea es O(1) y recorrer la jerarquía (recorrido en profundidad) es O(n). Como buscar un nodo en un árbol cuesta O(n), cada proyecto tiene además un mapa de tareas por id que permite ubicar cualquier tarea en O(1) promedio.
- **Montículo mínimo (cola de prioridad).** Se usa en la consulta de tareas próximas a vencer, con la fecha límite como prioridad. Construirlo cuesta O(n) y extraer cada tarea O(log n), lo que es mejor que ordenar todo cuando N es pequeño.

### 3.3. Implementación de las funcionalidades

Para cada funcionalidad se explica brevemente su implementación y su complejidad temporal. Se usa la notación p (cantidad de proyectos), u (usuarios), k (tareas de un usuario), c (subtareas directas de una tarea), N (tareas pedidas en la consulta) y T (tareas totales de todos los proyectos).

- **Crear y modificar proyecto, registrar usuario y agregar miembro:** se buscan los elementos en los mapas y se insertan o actualizan. O(1) promedio.
- **Agregar tarea o subtarea:** se busca el proyecto y la tarea padre en los mapas, se crea la tarea y se inserta en la lista de subtareas del padre (o del proyecto), en el mapa de tareas por id y en la lista de su responsable, y se actualiza el contador de tareas. O(1) promedio.
- **Modificar tarea:** se busca por id y se actualiza el campo. Al completar una tarea se verifica que todas sus subtareas estén completadas, O(c), y se actualiza el contador de completadas. Al cambiar el responsable se retira la tarea de la lista del anterior, O(k).
- **Eliminar tarea:** se recorre su subárbol liberando cada nodo y quitándolo del mapa y de la lista de su responsable, y luego se retira de la lista de su padre. O(m·k), con m el tamaño del subárbol.
- **Listar tareas con orden:** se recorre el árbol copiando las tareas a un arreglo y se ordena con mergesort según el criterio elegido. O(n log n).
- **Ver árbol de tareas:** recorrido en profundidad que imprime cada tarea con sangría según su nivel. O(n).
- **Listar proyectos y ver progreso:** el porcentaje se calcula con los contadores de tareas totales y completadas, sin recorrer el árbol. O(p) para listar todos los proyectos y O(1) para el progreso de uno (el desglose por estado requiere recorrer el árbol, O(n)).
- **Tareas de un usuario:** se busca el usuario en el mapa y se recorre su lista de tareas. O(k).
- **Tareas próximas a vencer:** se insertan en el montículo las tareas no completadas de todos los proyectos y se extraen las N con menor fecha. O(T + N log T).
- **Guardar y cargar:** se escribe una línea por registro, incluyendo el id del padre de cada tarea; al cargar se reconstruye el árbol usando el mapa de ids. O(u + p + T) promedio.

## 4. Aspecto desafiante

La propuesta es desafiante para el equipo por los siguientes motivos:

1. **Integrar varias estructuras de forma consistente.** Una misma tarea está a la vez en el árbol del proyecto, en el mapa de tareas por id y en la lista de su responsable. Cada operación (crear, cambiar de responsable, eliminar) debe actualizar todas estas referencias sin dejar punteros colgantes ni datos desincronizados.
2. **Trabajar con un árbol y memoria dinámica en C.** Eliminar una tarea implica liberar un subárbol completo, y el estado de una tarea depende del de sus subtareas. Los errores de memoria son fáciles de cometer y difíciles de depurar.
3. **Implementar los TDA con buen rendimiento y reglas consistentes.** La tabla hash requiere una buena función de hash y redimensionamiento, el montículo debe mantener su orden, y el progreso se mantiene con contadores que deben actualizarse en cada cambio de estado, cumpliendo reglas como no completar una tarea con subtareas pendientes. Además, guardar y cargar el árbol desde CSV exige un formato que preserve la jerarquía.

A pesar de esto, la propuesta es realizable porque puede construirse por etapas: primero mapas y listas con tareas simples, luego el árbol de subtareas y finalmente el montículo y los archivos.
