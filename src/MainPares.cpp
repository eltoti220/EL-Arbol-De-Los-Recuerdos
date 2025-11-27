#include "JuegoPares.hpp"
#include <iostream>

void menuSeleccion(const JuegoPares& juego) {
    std::cout << "=== Juego de Pares ===\n";
    std::cout << "Seleccione dos objetos para intentar formar un par:\n";

    for (int i = 0; i < (int)ObjetoID::NUM_OBJETOS; ++i) {
        
        ObjetoID id = static_cast<ObjetoID>(i);
        const EntidadObjeto* obj = juego.obtener_objeto(id);

        if (obj && !obj->encontrado) {
            std::cout << i << ": " << obj->nombre << "\n";
        }
    }

    std::cout << "--------------------------------------------\n";

}

int main() {
    JuegoPares juego;

    std::cout << "\nINICIO DEL JUEGO: ENOCNTRAR PARES SIMBÓLICOS\n";
    std::cout << "============================================\n";

    while (!juego.juego_completo())
    {
        menuSeleccion(juego);

        int seleccion;
        
        std::cout << "Ingrese el número del objeto a seleccionar: ";

        if(!(std::cin >> seleccion)) break;
        juego.seleccionar_objeto(static_cast<ObjetoID>(seleccion));

         int id_input;

        if(juego.obtener_seleccionados().size() == 1){
            std::cout << "Seleccione el segundo objeto: ";
            
            if(!(std::cin >> id_input)) break;
            juego.seleccionar_objeto(static_cast<ObjetoID>(id_input));
    }

    if(std::cin.fail()) {
        std::cin.clear(); 
        std::cin.ignore(10000, '\n');
    }
}

    std::cout << "¡Felicidades! Ha encontrado todos los pares simbólicos.\n";
    return 0;
}
