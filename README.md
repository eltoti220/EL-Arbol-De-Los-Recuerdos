# EL-Arbol-De-Los-Recuerdos
Un videojuego desarrollado en C++ utilizando SFML, con un sistema de estados, inventario, mapa, combate y administración de entidades.

Características principales:

1.Sistema de StateManager con múltiples estados del juego (menú, juego, inventario).

2.Sistema de entidades mediante EntityManager.

3.Mapa con colisiones, Quadtree y datos estructurados en MapaDataTypes.

4.Inventario funcional con slots e items renderizados.

5.Enemigos, ataques cuerpo a cuerpo y habilidades.

6.Manejo centralizado de texturas con TextureManager.

Estructura del Proyecto
EL-ARBOL-DE-LOS-RECUERDOS/
│
├── assets/             # Recursos gráficos, fuentes, metadata
├── include/            # Archivos .hpp del proyecto
├── src/                # Implementaciones .cpp
├── build/              # Archivos generados por CMake
├── CMakeLists.txt      # Configuración del proyecto
└── README.md

Compilación y ejecución
Desde la carpeta del proyecto:
Se bebe borrar la carpeta build:
rm -rf build
procedemos a volverla a crear y continuar con los pasos a continuación:
mkdir build
cd build
cmake ..
cmake --build .
./mi_juego.exe


Funcionalidades implementadas

-Sistema de menús (MainMenuState)

-Jugador con movimiento y ataques

-Enemigos básicos

-Inventario interactivo (InventoryState)

-Carga y gestión de texturas con TextureManager

-Mapa y Quadtree

-Sistema de comportamiento para NPCs (BehaviorTree y BTNodes)


Equipo de desarrollo

Proyecto desarrollado por :
-Zadkiel García
-Andres Zerpa
-María Lobo
-Angel Balza