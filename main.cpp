#include <iostream>
#include <vector>
#include <string>

using namespace std;


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

    void setNonce(int nuevoNonce) { //Si gustan lo mueven a public, no sé si este junto a SetCurrentHash deberain estar aqui (lo digo por el contexto de que son votaciones), de igual se puede acceder con una función amiga
        nonce=nuevoNonce;
    }

    void setCurrentHash(string hash) {
        current_hash=hash;
    }

public:
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

    return to_string(funcionHash(datos));
}


class Blockchain {
private:
    vector<Block> cadena;

public:

    string mineBlock(int dificultad) {
        string hash;
        while (hash != "") {
            hash = generarHash();
        }
    };

};


class MesaElectoralObserver {
public:
    virtual void update(Block nuevoBloque) = 0;
};


class MesaElectoral : public MesaElectoralObserver {
private:
    string nombreMesa;
    Blockchain blockchainLocal;

public:

};


class CentroElectoralSubject {
private:
    vector<MesaElectoralObserver*> observers;

public:

};

int main() {
    //Puse esto en el main para que puedan probar la parte dle integrante 1 y vean si les funciona, normal lo pueden borrar
    //========================================================
    vector<Voto> votos;

    votos.push_back(Voto("ID_01", "Candidato A"));
    votos.push_back(Voto("ID_02", "Candidato B"));
    votos.push_back(Voto("ID_03", "Candidato A"));

    Block bloque1(0, "0", votos);

    cout<<"======== BLOQUE CREADO ========\n";
    cout<<"Index: "<<bloque1.getIndex()<<endl;
    cout<<"Previous Hash: "<<bloque1.getPreviousHash()<<endl;
    cout<<"Nonce: "<<bloque1.getNonce()<<endl;
    cout<<"Current Hash: "<<bloque1.getCurrentHash()<<endl;

    cout<<"\n======== VOTOS ========\n";

    for (const Voto& voto : bloque1.getVotos()) {
        cout<<voto.getIdVotante()<<" -> "<<voto.getOpcionElegida()<<endl;
    }
    //=========================================================
    
    
    return 0;
}
