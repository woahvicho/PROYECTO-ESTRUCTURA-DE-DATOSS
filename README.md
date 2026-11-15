# Sistema de Gestión de Proyectos y Tareas

Aplicación de consola en C para la gestión de proyectos, tareas jerárquicas (árbol general), usuarios y consultas de prioridad mediante Montículo Mínimo (Min-Heap).

## Archivos necesarios
Todos los siguientes archivos deben estar juntos en la misma carpeta para compilar y ejecutar correctamente:
* main.c, tda.c, archivo.c (código fuente)
* estructuras.h, tda.h, archivo.h (definiciones y cabeceras)
* datos.csv (archivo de datos de prueba)

## Cómo descargar y ejecutar

### Opción A: Descargando el archivo ZIP
1. Descomprima el archivo .zip en una carpeta.
2. Abra una terminal dentro de esa carpeta.

### Opción B: Clonando el repositorio
```bash
git clone <URL-DEL-REPOSITORIO>
cd <NOMBRE-DE-LA-CARPETA>
```

Reemplace los valores entre ángulos por la URL y carpeta reales.

## Descripción

La versión actual permite gestionar proyectos y usuarios, registrar membresías y guardar o cargar datos. Las tareas pueden recuperarse desde CSV y consultarse por usuario. La gestión de tareas desde el menú y la consulta de vencimientos todavía no están implementadas, aunque existen las estructuras de árbol y montículo necesarias.

## Cómo compilar y ejecutar el programa

### En Visual Studio Code

1. Descargue o clone el proyecto siguiendo las instrucciones anteriores.
2. Inicie **Visual Studio Code**.
3. Seleccione `Archivo > Abrir carpeta...` y elija la carpeta del proyecto.
4. Seleccione `Terminal > Nueva terminal` y utilice **PowerShell**.
5. Compruebe que GCC esté instalado y disponible:

   ```powershell
   gcc --version
   ```
