# Versión mínima de CMake requerida
cmake_minimum_required(VERSION 3.10)

# 1. Definición del Proyecto
# Define el nombre del proyecto. Este será el nombre del ejecutable.
project(mi_juego CXX)

# Establecer el estándar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Útil para IDEs como VSCode

# 2. Búsqueda de Dependencias (SFML)
# Busca el paquete SFML. 'sfml-all' intenta encontrar todos los módulos.
# Si el paquete no está en una ubicación estándar, el usuario deberá pasar la variable SFML_DIR
# (Ejemplo en terminal: cmake -DSFML_DIR="/ruta/a/sfml" ..)
find_package(SFML 2.5 COMPONENTS graphics window system audio network REQUIRED)

# 3. Directorios de Inclusión
# Añade las carpetas 'include' y 'external' para que los headers se encuentren.
include_directories(
    include
    external
)

# 4. Recolección de Archivos Fuente
# Recolecta todos los archivos .cpp de la carpeta src/.
file(GLOB SOURCES "src/*.cpp")

# 5. Creación del Ejecutable
# Crea el ejecutable final a partir de los archivos fuente.
add_executable(${PROJECT_NAME} ${SOURCES})

# --- NUEVA SECCIÓN 6 ---

# 6. Enlazado de Bibliotecas
# Usamos el estilo moderno de CMake para las carpetas de inclusion
# Enlazamos el ejecutable con las bibliotecas de SFML encontradas.
# target_link_libraries es la fuente del error. Usaremos los nombres de las librerías directamente.

# Añadimos las rutas de inclusión de SFML (aunque find_package ya lo hace, es mas seguro asi)
target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIR})

# --------------------------------------------------------------------------------------------------
# CRÍTICO: Este bloque evita el target SFML::* y usa los nombres de librerías encontrados por find_package.
# Reemplaza la línea 37.
# --------------------------------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    sfml-network
)
# 7. Reglas de Copia (Opcional, pero recomendado para Windows/MinGW)
# Copia las DLLs de SFML (si se usan) al directorio de construcción/ejecución.
# Esto es crítico para los usuarios de Windows/MinGW.
if(SFML_STATIC_LIBRARIES)
    # Si se usan bibliotecas estáticas, no se necesita copiar DLLs.
else()
    # Copiar las DLLs de SFML a la carpeta donde se construye el ejecutable
    # get_target_property(BIN_DIR ${PROJECT_NAME} LOCATION) # En versiones recientes, el BIN_DIR es el CMAKE_CURRENT_BINARY_DIR
    
    # Busca la ruta de las DLLs de SFML y añade una regla de post-construcción para copiarlas.
    if (SFML_INCLUDE_DIR)
        get_filename_component(SFML_BIN_DIR ${SFML_INCLUDE_DIR} DIRECTORY)
        set(SFML_BIN_DIR "${SFML_BIN_DIR}/bin")
        
        # Copia las DLLs necesarias
        # Nota: La lista exacta puede variar, pero estas son las principales.
        set(SFML_DLLS 
            "${SFML_BIN_DIR}/sfml-graphics-2.dll"
            "${SFML_BIN_DIR}/sfml-window-2.dll"
            "${SFML_BIN_DIR}/sfml-system-2.dll"
            "${SFML_BIN_DIR}/sfml-audio-2.dll"
            "${SFML_BIN_DIR}/sfml-network-2.dll"
        )
        
        # Regla de Post-Build para copiar los archivos.
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_DLLS} $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copiando las DLLs de SFML al directorio de compilación..."
        )
    endif()
endif()