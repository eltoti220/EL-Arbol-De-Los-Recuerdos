#include "JuegoPares.hpp"
#include <algorithm>
#include <iterator>

//definiciond e la  lista de onjetos con sus rutas de png ajustadas

std::vector<EntidadObjeto> LISTA_OBJETOS = {

    // PARES DEL PROTAGONISTA (A)
    {ObjetoID::PETALO_MARCHITO, "Pétalo Marchito de Diente de León", "../assets/graphics/pares/figura1.png", "La Fragilidad que intenta ocultar."},
    {ObjetoID::HOJAS_SECAS, "Manojo de Hojas Secas y Calladas", "../assets/graphics/pares/figura2.png", "El Silencio y la represión de su voz."},
    {ObjetoID::CHARCO_OSCURO, "Charco de Agua Estancada y Oscura", "../assets/graphics/pares/complemento3.png", "Las Emociones Reprimidas que lo bloquean."}, 
    {ObjetoID::CORTEZA_DANADA, "Corteza de Árbol Dañada (Cicatríz)", "../assets/graphics/pares/figura4.png", "El Dolor o la herida emocional pasada."},
    {ObjetoID::HUELLA_SOLITARIA, "Huella Solitaria y Borrosa", "../assets/graphics/pares/figura5.png", "La Ausencia y la falta de pertenencia."},
    
    // PARES DE LA AMIGA (B)
    {ObjetoID::RAIZ_HELECHO, "Raíz Firme de un Helecho", "../assets/graphics/pares/complemento1.png", "La Fortaleza que ella le ofrecía."},
    {ObjetoID::GRILLO_ESCONDIDO, "Canto de un Grillo Escondido", "../assets/graphics/pares/complemento2.png", "La Voz que ella intentaba liberar en él."},
    {ObjetoID::GOTAS_ROCIO, "Gotas de Rocío Puras sobre la Piedra", "../assets/graphics/pares/figura3.png", "La Claridad que ella le ayudaba a ver."}, 
    {ObjetoID::SAVIA_NUEVA, "Savia Nueva y Pegajosa", "../assets/graphics/pares/complemento4.png", "La Curación y el acto de reparar el daño."},
    {ObjetoID::CAMINO_HORMIGAS, "Camino de Hormigas en Marcha", "../assets/graphics/pares/complemento5.png", "La Compañía y el sentido de comunidad."}
};

JuegoPares::JuegoPares() : objetos_juego(LISTA_OBJETOS) {
    std::cout <<"Juego de Pares Simbólicos Inicializado.\n";
}

const EntidadObjeto* JuegoPares::obtener_objeto(ObjetoID id) const {
    auto it = std::find_if(objetos_juego.begin(), objetos_juego.end(),
        [&](const EntidadObjeto& obj) { return obj.id == id; });

    if (it != objetos_juego.end()) {
        return &(*it);
    }   
    return nullptr;
}

bool JuegoPares::verificar_par(ObjetoID obj1, ObjetoID obj2) const {
    
    for(int i = 0; i < FILAS_PARES; ++i) {
        ObjetoID a = MATRIZ_PARES_CORRECTOS[i].objetoA;
        ObjetoID b = MATRIZ_PARES_CORRECTOS[i].objetoB; 
        
        if((obj1 == a && obj2 == b) || (obj1 == b && obj2 == a)) {
            return true;
        }

    }
    return false;
}

void JuegoPares::seleccionar_objeto(ObjetoID id_objeto) {
    const EntidadObjeto* obj = obtener_objeto(id_objeto);

    if(obj == nullptr || obj->encontrado) {
        if(obj && obj->encontrado) std::cout<<"Objeto ya encontrado previamente.\n";
        return;
    }

    if(!pila_seleccionados.empty() && pila_seleccionados.top() == id_objeto) {
        std::cout<<"Objeto ya seleccionado actualmente.\n";
        return;
    }

    pila_seleccionados.push(id_objeto);
    std::cout<<"Objeto seleccionado: "<< obtener_objeto(id_objeto)->nombre <<"' seleccionado. Pila.size() = "<< pila_seleccionados.size() <<"\n";

    if(pila_seleccionados.size() == COLUMNAS_PARES) {
        procesar_seleccion();
    }
}

void JuegoPares::procesar_seleccion() {

    ObjetoID obj2 = pila_seleccionados.top();
    pila_seleccionados.pop();

    ObjetoID obj1 = pila_seleccionados.top();
    pila_seleccionados.pop();

    std::cout<<"Procesando selección de objetos: "<< obtener_objeto(obj1)->nombre <<" y "<< obtener_objeto(obj2)->nombre <<"\n";
    return;

    if(verificar_par(obj1, obj2)) {
        
        auto actualizar_estado = [&](ObjetoID id_p) {
            auto it = std::find_if(objetos_juego.begin(), objetos_juego.end(),
                [&](const EntidadObjeto& obj) { return obj.id == id_p; });
            if(it != objetos_juego.end()) {
                it->encontrado = true;
            }
        };

        actualizar_estado(obj1);
        actualizar_estado(obj2);

        std::cout<<"¡Par correcto! Objetos marcados como encontrados.\n" <<obtener_objeto(obj1)->simbolismo <<"\n" <<obtener_objeto(obj2)->simbolismo <<"\n";
    
    } else {

        std::cout<<"Par incorrecto. Intenta de nuevo.\n";
    }
        
}

bool JuegoPares::juego_completo() const {
    
    int pares_completados = 0;

    for(const auto& obj : objetos_juego) {
        
        if(obj.encontrado) {
            pares_completados++;
        }
    }

    return pares_completados == (int)ObjetoID::NUM_OBJETOS;
}

void JuegoPares::reiniciar_juego() {

    std::cout<<"Renderizando estado actual del juego de pares:\n";
} 