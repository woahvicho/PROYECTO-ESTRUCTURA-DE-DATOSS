#PARA COMPILAR Y EJECUTAR EL PROGRAMA SE USA PRIMERO : "gcc main.c tda.c archivo.c -o gestor" Y LUEGO ".\gestor.exe"

//Antonio 06/11, 19:43//
hola cabros, cree los esqueletos del programa, el main, los tdas y las estructuras, de momento compila y ejecuta todo(estoy en vsc), pero al elegir las opciones
solo imprime lo mismo de vuelta ya que aun no les asigno nada, seguire avanzando y tratare de terminar al menos las funciones de "registrar usuario" y "crear proyecto",
si hacen algun avance dejenlo registrado aqui para saber con que seguir y que esta listo :j

//Antonio 06/11, 20:32//
cree la funcion de crear proyecto y de registrar usuarios, tambien arregle unos errores que no dejaban poner la fecha bien, pero ya esta ok, tambien aca abajo
dejare una tabla de las cosas que ya estan listas y las que faltan: 

//Antonio 07/11, 19:13//
Añadí la función 2 en menuProyectos, la que modifica los proyectos ya existentes, hice que buscara el proyecto en la tabla hash, deja actualizar la descripción y las fechas de inicio-fin del proyecto, ahora tratare de hacer la función 4, la que es para ver el progreso, terminando eso quedaria lista la primera función(menuProyectos) y habría que seguir con la de tareas, usuarios, consultas y archivos, si alguno avanza recomiendo hacer la de archivos ya que es la mas facil de las que quedan.

//Antonio 07/11 , 23:13//
Hola, termine el menú de los proyectos, ya esta al 100%, la probé hartas veces antes de subirla aquí pero ya funciona todo bien, me demore ya que cuando estaba haciéndola pensé en que no tenia sentido programar el progreso si aun ni siquiera podemos crear tareas o asignarlas, así que deje la interfaz lista, con las funciones total_tareas y tareas_completadas, ambas parten en 0 por ahora ya que aun no se crean los casos para crear, asignar o modificar tareas, para que cuando hagamos el modulo de las tareas solo debamos actualizar esas variables y funcionara igual, de todas formas si tienen alguna duda o quieran saber como funciona me hablan nomas, faltaría los módulos: tareas, usuarios, consultas y archivos, yo puedo hacer el de archivos ya que no toma mucho tiempo, pero traten de avanzar los demas.


//Antonio 15-11, 16:10//
termine el modulo 3(usuarios) y el modulo 5(archivos), añadi el archivo.h y archivo.c para el modulo 5, para cargar o exportar los datos, y tambien cree un CSV de ejemplo, se ve aca atras en el github, se llama datos.csv, solo falta el modulo 2(tareas), el modulo 4(consultas) y el README, recuerden que se envia hoy para que puedan terminarlo...(porfavor), tambien añadi en el tda.c y tda.h las funciones para eliminar listas o mapas en caso de pedirlo el usuario(modulo 2) ya que no la habia agregado antes.

//Vicente 15-11, 18:09//
**avance readme** se da por terminado el readme junto a franco, agregamos funcionalidades, guía de como correr el código y un ejemplo de uso.

//Nicolas 15-11, 21:40// termine el modulo 4(consultas), la opcion "N tareas proximas a vencer". Hice la funcion recorrerTareasPendientes, que recorre todos los proyectos y las tareas de cada uno (mapa_tareas_id), se salta las que estan en estado "Completada" y carga el resto en el monticulo minimo ordenado por fecha_limite. Despues menuConsultas valida que N sea un numero mayor a 0, y saca las N tareas mas proximas a vencer con extraerMinimoMonticulo, mostrando ID, descripcion, fecha y estado. Tambien cubri los casos borde: si no hay tareas pendientes lo avisa, y si N es mayor que la cantidad de pendientes avisa y muestra todas. Como el modulo 2 aun no esta, para probarlo hay que cargar las tareas desde el CSV (Archivos -> Cargar CSV), con datos.csv funciona bien. Con esto solo falta el modulo 2(tareas), que es el que alimenta esta consulta cuando se creen tareas desde el programa. Aviso: reescribi mis ultimos commits en main (push forzado) para dividirlos mejor, si alguien tiene algo raro al hacer pull me avisa.

## Estado del Proyecto
## 1. Módulo de Proyectos  
- [X] Crear proyecto
- [X] Modificar proyecto
- [X] Mostrar proyectos
- [x] Ver progreso

## 2. Módulo de Tareas
- [ ] Agregar tarea/subtarea
- [ ] Modificar tarea
- [ ] Eliminar tarea
- [ ] Listar tareas
- [ ] Ver árbol

## 3. Módulo de Usuarios
- [X] Registrar usuario
- [X] Agregar a proyecto
- [X] Ver tareas

## 4. Módulo de Consultas
- [X] N tareas próximas a vencer

## 5. Módulo de Archivos
- [X] Guardar/Cargar CSV
