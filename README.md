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

### Forma de compilación del código

1. En la terminal integrada de Visual Studio Code, ubicada en la carpeta del proyecto, compile:

   ```powershell
   gcc -Wall -Wextra main.c tda.c archivo.c -o gestor.exe
   ```

2. Si la compilación termina correctamente, ejecute:

   ```powershell
   .\gestor.exe
   ```

Visual Studio Code necesita un compilador GCC instalado por separado. Si la terminal no reconoce `gcc`, revise su instalación y configuración del PATH.

No omita `archivo.c`: contiene las funciones de persistencia. Las cabeceras no se compilan por separado.

**Nota:** `datos.csv` no es necesario para compilar. Debe existir para cargar datos; guardar lo crea o sobrescribe en la carpeta desde la que se ejecuta el programa.

## Funcionalidades

### Implementadas en el código

- **Crear proyecto:** solicita nombre, descripción y fechas. Rechaza nombres repetidos y fecha final anterior a la inicial.
- **Modificar proyecto:** actualiza descripción y fechas; no permite cambiar el nombre.
- **Mostrar proyectos:** lista nombre, fechas y porcentaje de progreso.
- **Ver progreso:** muestra todos los proyectos con descripción, fechas, tareas completadas y total. El porcentaje se calcula con división entera y no incluye desglose por cada estado.
- **Registrar usuario:** solicita nombre y comprueba duplicados.
- **Agregar a proyecto:** comprueba que usuario y proyecto existan y evita membresías repetidas.
- **Ver tareas de usuario:** recorre sus tareas asignadas. En esta versión, esas tareas pueden incorporarse mediante carga CSV.
- **Guardar/Cargar CSV:** conserva usuarios, proyectos, miembros y tareas en `datos.csv`.

## Ejemplo de uso

**inicio:** El sistema muestra el menú principal, donde se puede acceder a los distintos módulos del sistema.

```text
=== SISTEMA DE TAREAS Y PROYECTOS ===
1. Proyectos
2. Tareas
3. Usuarios
4. Consultas
5. Archivos
0. Salir
Opcion:
```

**Paso 1:** Se registra un usuario nuevo en el sistema validando que su nombre no esté duplicado.

```text
Opcion: 3

--- 3. GESTION DE USUARIOS ---
1. Registrar usuario
2. Agregar a proyecto
3. Ver tareas
0. Volver
Opcion: 1

--- REGISTRAR USUARIO ---
Ingrese el nombre del usuario: Antonio
[EXITO] Usuario 'Antonio' registrado correctamente.
```

**Paso 2:** Se crea un proyecto ingresando su nombre, descripción y fechas de inicio y término.

```text
Opcion: 1

--- 1. GESTION DE PROYECTOS ---
1. Crear proyecto
2. Modificar proyecto
3. Mostrar lista de proyectos
4. Ver progreso
0. Volver
Opcion: 1

--- CREAR PROYECTO ---
Ingrese nombre del proyecto: SistemaGestion
Ingrese descripcion: Desarrollo de plataforma
Fecha de inicio (DD-MM-YYYY): 01-11-2026
Fecha de fin (DD-MM-YYYY): 30-11-2026
[EXITO] Proyecto 'SistemaGestion' creado correctamente.
```

