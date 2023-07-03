#ifndef PILHA_H
#define PILHA_H

template <typename T>
class Pilha {
public:
    Pilha();        // Construtor padrão
    ~Pilha();       // Destrutor

    bool vazia();

    void empilhar(T elemento);
    void limpar();

    T valorTopo();
    T desempilhar();

    int tamanho();
private:
    struct Nodo {
        T dado;
        Nodo *proximo;
    };
    Nodo *topo;
    int tam;
};

template <typename T>
Pilha<T>::Pilha() {
    topo = nullptr;
    tam = 0;
}

template <typename T>
Pilha<T>::~Pilha() {
    limpar();
}

template <typename T>
bool Pilha<T>::vazia() {
    return topo == nullptr;
}

// Adiciona um elemento no topo da pilha
template <typename T>
void Pilha<T>::empilhar(T elemento) {
    Nodo *novoNodo = new Nodo;
    novoNodo->dado = elemento;
    novoNodo->proximo = topo;
    topo = novoNodo;
    tam++;
}

// Remove todos os elementos da pilha e libera a memória alocada
template <typename T>
void Pilha<T>::limpar() {
    while (!vazia()) {
        desempilhar();
    }
}

// Retorna o valor do elemento no topo da pilha
template <typename T>
T Pilha<T>::valorTopo() {
    if (vazia()) {
        throw "ERRO: PILHA VAZIA!";
    }
    
    return topo->dado;
}

// Remove e retorna o elemento no topo da pilha
template <typename T>
T Pilha<T>::desempilhar() {
    if (vazia()) {
        throw "ERRO: PILHA VAZIA!";
    }

    T valor = topo->dado;
    Nodo *temp = topo;
    topo = topo->proximo;
    delete temp;
    tam--;

    return valor;
}

template <typename T>
int Pilha<T>::tamanho() {
    return tam;
}

#endif

