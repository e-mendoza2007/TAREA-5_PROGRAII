#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <sstream>
#include <iomanip>

using namespace std;

// INTEGRANTE 1
class Voto {
    string idVotante, opcionElegida;
public:
    Voto(string id, string opcion) : idVotante(id), opcionElegida(opcion) {}
    string getIdVotante() const { return idVotante; }
    string getOpcionElegida() const { return opcionElegida; }
};

class Block;
string generarHash(const Block& bloque);

class Block {
    int index, nonce;
    string previous_hash, current_hash;
    vector<Voto> votos;
public:
    void setNonce(int n) { nonce = n; }
    void setCurrentHash(string h) { current_hash = h; }
    Block(int i, string prev, vector<Voto> v) : index(i), previous_hash(prev), votos(v), nonce(0) {
        current_hash = generarHash(*this);
    }
    int getIndex() const { return index; }
    string getPreviousHash() const { return previous_hash; }
    const vector<Voto>& getVotos() const { return votos; }
    int getNonce() const { return nonce; }
    string getCurrentHash() const { return current_hash; }
};

string generarHash(const Block& b) {
    string datos = to_string(b.getIndex()) + b.getPreviousHash();
    for (const auto& v : b.getVotos()) datos += v.getIdVotante() + v.getOpcionElegida();
    datos += to_string(b.getNonce());
    stringstream ss;
    ss << hex << hash<string>{}(datos);
    return ss.str();
}

//  INTEGRANTE 2 
class Blockchain {
    vector<Block> cadena;
    Blockchain() {}
public:
    static Blockchain& getInstance() { static Blockchain instancia; return instancia; }
    void insertar_bloque(const Block& b) { cadena.push_back(b); }
    void actualizarUltimoBloque(const Block& b) { cadena.back() = b; }
    string getLatestHash() { return cadena.empty() ? "0" : cadena.back().getCurrentHash(); }
    
    void mineBlock(int dificultad) {
        string objetivo(dificultad, '0');
        Block& b = cadena.back();
        while (b.getCurrentHash().substr(0, dificultad) != objetivo) {
            b.setNonce(b.getNonce() + 1);
            b.setCurrentHash(generarHash(b));
        }
        cout << "[Mineria] Bloque " << b.getIndex() << " sellado: " << b.getCurrentHash() << endl;
    }
};

// INTEGRANTE 3
class MesaElectoralObserver {
public:
    virtual void update(Block b) = 0;
};

class MesaElectoral : public MesaElectoralObserver {
    string nombre;
public:
    MesaElectoral(string n) : nombre(n) {}
    void update(Block b) override {
        cout << ">> [" << nombre << "] Notificada. Bloque " << b.getIndex() << " validado y guardado." << endl;
    }
};

class CentroElectoralSubject {
    vector<MesaElectoralObserver*> observers;
public:
    void attach(MesaElectoralObserver* o) { observers.push_back(o); }
    void notificar(Block b) { for (auto o : observers) o->update(b); }
};


int main() {
    CentroElectoralSubject red;
    MesaElectoral mesa1("Mesa Norte"), mesa2("Mesa Sur");
    red.attach(&mesa1); red.attach(&mesa2);

    Blockchain& bc = Blockchain::getInstance();
    
    // bloque de ejemplo
    vector<Voto> v = {Voto("ID_01", "Candidato A")};
    Block nuevoBloque(0, "0", v);
    bc.insertar_bloque(nuevoBloque);
    bc.mineBlock(2); // Dificultad 2

    // Notificar a las mesas
    red.notificar(nuevoBloque);

    return 0;
}