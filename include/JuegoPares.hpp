#include<stack>
#include "Entidades.hpp"
#include<iostream>

class JuegoPares {
private:
    //pila para genstionar los objetos seleccionados
    std::stack<ObjetoID> pila_seleccionados;

    std::vector<EntidadObjeto> objetos_juego;

    bool verificar_par(ObjetoID obj1, ObjetoID obj2) const;

public:
    JuegoPares();

    void seleccionar_objeto(ObjetoID id_objeto);

    void procesar_seleccion();
    bool juego_completo() const;

    const EntidadObjeto* obtener_objeto(ObjetoID id) const;

    const std::stack<ObjetoID>& obtener_seleccionados() const {
        return pila_seleccionados;
    }

    void reiniciar_juego();
};
