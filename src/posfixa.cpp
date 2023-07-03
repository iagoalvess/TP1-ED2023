#include "posfixa.hpp"

Posfixa::Posfixa(std::string expr) : Expressao(expr), expressaoInfixa("") {}



Posfixa::~Posfixa() {}



bool Posfixa::verificaCaracteresValidos() {
  int n = expressao.length();

  for (int i = 0; i < n; i++) {
    char caractereAtual = expressao[i];

    if (ehDigito(caractereAtual)) {
      continue;
    }
    else if (caractereAtual == ' ') {
      continue;
    }
    else if (ehOperador(caractereAtual)) {
      continue;
    }
    else {
      return false;
    }
  }

  return true;
}



// Verifica se a ordem dos operadores e operandos está correta
// em relação ao formato posfixo
bool Posfixa::verificaOrdem() const {
  Pilha<double> pilha;

  unsigned int i = 0;

  while (i < expressao.size()) {
    if (isdigit(expressao[i])) {
      // Se o caractere atual for um dígito, converte o número
      double num = 0;
      while (i < expressao.size() && (ehDigito(expressao[i]))) {
        // Converte os dígitos antes do ponto decimal
        if (expressao[i] == '.') {
          i++;
          double frac = 0.1; // Representa a fração decimal
          while (i < expressao.size() && isdigit(expressao[i])) {
            // Converte os dígitos após o ponto decimal
            num += (expressao[i] - '0') * frac;
            frac /= 10;
            i++;
          }
        }
        else {
          num = num * 10 + (expressao[i] - '0');
          i++;
        }
      }
      // Empilha o número na pilha de operandos
      pilha.empilhar(num);
    }
    else if (ehOperador(expressao[i])) {
      // Se o caractere atual for um operador
      if (pilha.tamanho() < 2) return false;
      // Desempilha os dois operandos
      double b = pilha.valorTopo();
      pilha.desempilhar();
      double a = pilha.valorTopo();
      pilha.desempilhar();
      // Realiza a operação com base no operador
      switch (expressao[i]) {
      case '+':
        pilha.empilhar(a + b);
        break;
      case '-':
        pilha.empilhar(a - b);
        break;
      case '*':
        pilha.empilhar(a * b);
        break;
      case '/':
        pilha.empilhar(a / b);
        break;
      }
      i++;
    }
    else {
      // Se o caractere atual não for um dígito nem um operador, avança para o próximo caractere
      i++;
    }
  }

  // Retorna true se houver exatamente um valor na pilha de operandos
  return pilha.tamanho() == 1;
}



// Avalia a validade da expressão pos-fixa
bool Posfixa::avaliaExpressao() {
  if (!verificaTamanhoMaximo()) {
    return false;
  }
  else if (!contagemExpressao()) {
    return false;
  }
  else if (!verificaOrdem()) {
    return false;
  }
  else if (!verificaCaracteresValidos()) {
    return false;
  }

  return true;
}



// Método para ler uma expressão posfixa e armazená-la em uma árvore binária
bool Posfixa::lerExpressao() {
  // Verifica se a expressão é válida e se não excede o tamanho máximo permitido
  if (!avaliaExpressao()) {
    throw std::invalid_argument("ERRO");
  }

  // Cria uma pilha para armazenar os nós da árvore
  Pilha<Node *> pilha;

  int n = expressao.length();
  // Percorre cada caractere da expressão
  for (int i = 0; i < n; i++) {
    char caractereAtual = expressao[i];

    // Se for um operador, desempilha os dois últimos nós da pilha
    if (ehOperador(caractereAtual)) {
      Node *direita = pilha.valorTopo();
      pilha.desempilhar();
      Node *esquerda = pilha.valorTopo();
      pilha.desempilhar();

      // Cria um novo nó com o operador como valor
      Node *node = new Node(std::string(1, caractereAtual));

      // Define os filhos do nó como os dois nós desempilhados
      node->setEsq(esquerda);
      node->setDir(direita);

      // Empilha o novo nó na pilha
      pilha.empilhar(node);
    }
    else if (ehDigito(caractereAtual)) {
      std::string num;
      bool isFractional = false;

      // Se for um dígito fracionário, lê todos os dígitos consecutivos para formar um número
      while (ehDigito(caractereAtual) || (caractereAtual == '.' && !isFractional)) {
        num += caractereAtual;
        if (caractereAtual == '.') {
          isFractional = true;
        }
        i++;
        if (i >= n) {
          break;
        }
        caractereAtual = expressao[i];
      }
      i--;

      // Cria um novo nó com o número como valor e empilha na pilha
      pilha.empilhar(new Node(num));
    }
    // Se o caractere não for um operador nem um dígito, ignora e continua para o próximo caractere
    else {
      continue;
    }
  }
  // Desempilha o último nó da pilha e define como raiz da árvore
  raiz = pilha.desempilhar();

  return true;
}



// Converte a expressão posfixa caminhando a árvore
void Posfixa::coverteRecursivo(Node* p, bool operador) {
  if (p == nullptr) return;

  // Se o nó é um operador, adiciona um parêntese aberto antes de percorrer os filhos
  if (operador) expressaoInfixa += "(";

  coverteRecursivo(p->getEsq(), ehOperador(p->getValor()[0])); // Verifica se o nó esquerdo é operador
  expressaoInfixa += (' ');
  expressaoInfixa += (p->getValor()); // Adiciona o valor do nó atual à expressão infixa
  expressaoInfixa += (' ');
  coverteRecursivo(p->getDir(), ehOperador(p->getValor()[0])); // Verifica se o nó direito é operador

  // Se o nó é um operador, adiciona um parêntese fechado após percorrer os filhos
  if (operador) expressaoInfixa += ")";
}



// Método para converter a expressão posfixa armazenada na árvore para uma expressão infixa
std::string Posfixa::converteExpressao() {
  coverteRecursivo(raiz, raiz && raiz->getValor() != "");

  // Retorna a expressão infixa gerada
  return expressaoInfixa;
}