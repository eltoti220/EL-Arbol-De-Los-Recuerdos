#include<string>
#include<vector>

//enumeracion para identificar de forma unica a los 10 objetos de buscar su "par"
enum class ObjetoID {
    PETALO_MARCHITO = 0,
    RAIZ_HELECHO,
    HOJAS_SECAS,
    GRILLO_ESCONDIDO,
    CHARCO_OSCURO,
    GOTAS_ROCIO,
    CORTEZA_DANADA,
    SAVIA_NUEVA,
    HUELLA_SOLITARIA,
    CAMINO_HORMIGAS,
    NUM_OBJETOS
};
// Estructura para almacenar la información de un objeto individual.
struct EntidadObjeto {
    ObjetoID id;
    std::string nombre;
    std::string ruta_png; 
    std::string simbolismo;
    bool encontrado = false; 
};

//estructura para defininr los pares correctos
struct EntidadPar {
    ObjetoID objetoA;
    ObjetoID objetoB;
};

//matriz de referencia con los 10 objetos y su informacion para los pares corrctos 

const int FILAS_PARES = 5;
const int COLUMNAS_PARES = 2;

const EntidadPar MATRIZ_PARES_CORRECTOS[FILAS_PARES] = {
    {ObjetoID::PETALO_MARCHITO, ObjetoID::RAIZ_HELECHO},
    {ObjetoID::HOJAS_SECAS, ObjetoID::GRILLO_ESCONDIDO},
    {ObjetoID::CHARCO_OSCURO, ObjetoID::GOTAS_ROCIO},
    {ObjetoID::CORTEZA_DANADA, ObjetoID::SAVIA_NUEVA},
    {ObjetoID::HUELLA_SOLITARIA, ObjetoID::CAMINO_HORMIGAS}
};

//vector con la informacion de los objetos
const std::vector<EntidadObjeto> OBJETOS;   