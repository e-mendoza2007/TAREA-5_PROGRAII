#include <iostream>
#include <vector>
#include <string>

using namespace std;


class Voto {
private:
    string idVotante;
    string opcionElegida;

public:

};


class Block {
private:
    int indice;
    string previousHash;
    vector<Voto> votos;
    int nonce;
    string currentHash;

public:

};


string generarHash();


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

    return 0;
}