#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <sstream>
#include <iomanip>

using namespace std;

// INTEGRANTE 1 

class Voto {
    string idVotante;
    string opcionElegida;

public:
    Voto(string id, string opcion) {
        idVotante=id;
        opcionElegida=opcion;
    }

    string getIdVotante() const {
        return idVotante;
    }

    string getOpcionElegida() const {
        return opcionElegida;
    }
};

class Block;

string generarHash(const Block& bloque);

class Block {
    int index;
    string previous_hash;
    vector<Voto> votos;
    int nonce;
    string current_hash;
public:

    void setNonce(int nuevoNonce) { 
        nonce=nuevoNonce;
    }

    void setCurrentHash(string hash) {
        current_hash=hash;
    }

    Block(int indiceBloque, string hashAnterior, vector<Voto> listaVotos) {
        index=indiceBloque;
        previous_hash=hashAnterior;
        votos=listaVotos;
        nonce=0;
        current_hash=generarHash(*this);
    }

    int getIndex() const {
        return index;
    }

    string getPreviousHash() const {
        return previous_hash;
    }

    const vector<Voto>& getVotos() const {
        return votos;
    }

    int getNonce() const {
        return nonce;
    }

    string getCurrentHash() const {
        return current_hash;
    }
};

string generarHash(const Block& bloque){
    string datos;

    datos+=to_string(bloque.getIndex());
    datos+=bloque.getPreviousHash();

    for (const Voto& voto : bloque.getVotos()) {
        datos+=voto.getIdVotante();
        datos+=voto.getOpcionElegida();
    }

    datos+=to_string(bloque.getNonce());

    hash<string> funcionHash;
    stringstream ss;

    ss<<setw(16)<<setfill('0')<<hex<<funcionHash(datos);

    return ss.str();
}


//  INTEGRANTE 2 (incluido puente para el integrante 3)

class Blockchain {
private:
    vector<Block> cadena;

    Blockchain() {}

public:
    static Blockchain& getInstance() {
        static Blockchain instancia;
        return instancia;
    }

    void insertar_bloque(const Block& bloque) {
        cadena.push_back(bloque);
    }

    void mineBlock(int dificultad) {
        if (cadena.empty()) {
            return;
        }

        Block& bloque = cadena.back();
        string objetivo(dificultad, '0');

        while (bloque.getCurrentHash().substr(0, dificultad) != objetivo) {
            bloque.setNonce(bloque.getNonce() + 1);
            string nuevoHash = generarHash(bloque);
            bloque.setCurrentHash(nuevoHash);
        }

        cout << "Bloque minado: " << bloque.getCurrentHash()<< endl;
    }

    bool isChainValid() {
        for (int i = 1; i < cadena.size(); i++) {
            Block& actual = cadena[i];
            Block& anterior = cadena[i - 1];

            if (actual.getCurrentHash() != generarHash(actual)) {
                return false;
            }

            if (actual.getPreviousHash() != anterior.getCurrentHash()) {
                return false;
            }
        }
        return true;
    }

    // Puente añadido para la parte del integrante 
    // Permite obtener el bloque que acaba de ser modificado/minado
    Block getUltimoBloque() {
        return cadena.back();
    }
    
    
    void actualizarUltimoBloque(const Block& bloqueMined) { // permite actualizar la cadena desde las mesas
        if (!cadena.empty() && cadena.back().getIndex() == bloqueMined.getIndex()) {
            cadena.back() = bloqueMined;
        }
    }
};

class MesaElectoralObserver {
public:
    virtual void update(Block nuevoBloque) = 0;
    virtual ~MesaElectoralObserver() = default;
};

class MesaElectoral : public MesaElectoralObserver {
private:
    string nombreMesa;
    Blockchain& blockchain = Blockchain::getInstance();

public:
    MesaElectoral(string nombre) : nombreMesa(nombre) {}

    void update(Block nuevoBloque) override {
        cout << " -> [" << nombreMesa << "] Verificando bloque #" << nuevoBloque.getIndex() << "...\n";
        
        string hashCalculado = generarHash(nuevoBloque);
        
        if (nuevoBloque.getCurrentHash() == hashCalculado) {
            cout << "    [V] Valido. Actualizando registro local en " << nombreMesa << ".\n";
            blockchain.actualizarUltimoBloque(nuevoBloque);
        } else {
            cout << "    [X] ALERTA: Bloque rechazado. Hash invalido.\n";
        }
    }
};

class CentroElectoralSubject {
private:
    vector<MesaElectoralObserver*> observers;

public:
    void attach(MesaElectoralObserver* observer) {
        observers.push_back(observer);
    }

    void notificarNuevoBloque(Block b) {
        cout << "\n========================================================\n";
        cout << " [RED] TRANSMITIENDO NUEVO BLOQUE A TODAS LAS MESAS...\n";
        cout << "========================================================\n";
        for (MesaElectoralObserver* obs : observers) {
            obs->update(b);
        }
        cout << "========================================================\n\n";
    }
};


int main() {
    // instanciar la red y las mesas
    CentroElectoralSubject redElectoral;
    MesaElectoral mesa1("Norte");
    MesaElectoral mesa2("Sur");
    MesaElectoral mesa3("Centro");

    // conectar mesas a la red
    redElectoral.attach(&mesa1);
    redElectoral.attach(&mesa2);
    redElectoral.attach(&mesa3);

    // creamos los votos y el bloque
    vector<Voto> votos;
    votos.push_back(Voto("ID_01", "Candidato A"));
    votos.push_back(Voto("ID_02", "Candidato B"));
    
    Block bloque1(0, "0", votos);

    // 4. Utilizar la Blockchain del Integrante 2
    Blockchain& blockchain = Blockchain::getInstance();
    blockchain.insertar_bloque(bloque1);

    cout << "Iniciando mineria (Dificultad 2)...\n";
    blockchain.mineBlock(2);

    // extraer el bloque ya minado (con el nonce y hash resueltos)
    Block bloqueMinado = blockchain.getUltimoBloque();

    // notificamos a las mesas usando el Patrón Observer
    redElectoral.notificarNuevoBloque(bloqueMinado);

    // Comprobación final
    cout << "Validacion global del sistema: " << (blockchain.isChainValid() ? "OK" : "ERROR") << endl;

    return 0;
}