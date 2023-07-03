#include "expressao.hpp"

Node::Node(const std::string& value) : valor(value), esq(nullptr), dir(nullptr) {}



Node::~Node() {
  if (esq != nullptr) {
    delete esq;
  }

  if (dir != nullptr) {
    delete dir;
  }
}



const std::string& Node::getValor() const {
  return valor;
}



Node* Node::getDir() const {
  return dir;
}



Node* Node::getEsq() const {
  return esq;
}



void Node::setValor(const std::string& value) {
  valor = value;
}



void Node::setDir(Node* novaDir) {
  dir = novaDir;
}



void Node::setEsq(Node* novaEsq) {
  esq = novaEsq;
}



Expressao::Expressao(const std::string& expr) : expressao(expr), raiz(nullptr) {}



Expressao::~Expressao() {
  if (raiz != nullptr) {
    delete raiz;
  }
}



// Verifica se o caractere é um digito ou um ponto em caso de digitos fracionários
bool Expressao::ehDigito(char c) {
  return ((isdigit(c)) || c == '.');
}



// Verifica se o caractere é um operador válido
bool Expressao::ehOperador(char c) {
  return (c == '*' || c == '/' || c == '-' || c == '+');
}



bool Expressao::verificaTamanhoMaximo() const{
  //Retorna false caso a expressao exceda o tamanho maximo
  return expressao.length() <= MAX_TAM_EXPRESSAO;
}



// Compara o número de operandos e operadores da expressão
bool Expressao::contagemExpressao() const {
// A expressão não pode estar vazia
  if (expressao == "") return false;

  // Contadores para verificar se a quantidade de operadores é maior ou igual a quantidade de operandos
  int contOperador = 0, contOperando = 0;

  int n = expressao.size();
  for (int i = 0; i < n; i++) {
    char caractereAtual = expressao[i];

    switch (caractereAtual) {
    // Se o caractere atual for um dígito
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      contOperando++; // Pula os dígitos subsequentes do mesmo número
      while (ehDigito(caractereAtual)) {
        i++;
        caractereAtual = expressao[i];
      }
      i--;
      break;
    // Se o caractere atual for um operador
    case '+':
    case '-':
    case '*':
    case '/':
      contOperador++; // Incrementa o contador de operadores
      break;
    default:
      break;
   }
  }

  // Verifica se a quantidade de operadores é maior ou igual à quantidade de operandos
  if (contOperador >= contOperando) return false;

  return true;
}



// Método para resolver a expressão armazenada na árvore e retornar o resultado
// Como ambas expressoes são armazenadas em uma árvore não vi necessidade em fazer um
// método para cada
double Expressao::resolveExpressao() {
  // Chama o método resolveRecursivo
  return resolveRecursivo(raiz);
}



// Método auxiliar para percorrer a árvore e calcular o resultado da expressão
double Expressao::resolveRecursivo(Node* raizAtual) {
  // Verifica se o nó atual é nulo
  if (!raizAtual) {
    return 0;
  } 

  // Verifica se o nó atual é uma folha (não tem filhos)
  if (!raizAtual->getEsq() && !raizAtual->getDir()) {
    return std::stod(raizAtual->getValor());
  }

  // Calcula o resultado da subexpressão da esquerda
  double esquerda = resolveRecursivo(raizAtual->getEsq());
  // Calcula o resultado da subexpressão da direita
  double direita = resolveRecursivo(raizAtual->getDir());

  // Verifica qual é o operador armazenado no nó atual e
  // Aplica-o aos resultados das subexpressões da esquerda e da direita
  if (raizAtual->getValor() == "+") {
    return esquerda + direita;
  }
  else if (raizAtual->getValor() == "-") {
    return esquerda - direita;
  }
  else if (raizAtual->getValor() == "*") {
    return esquerda * direita;
  }
  else { 
    if (direita == 0) {
      throw std::invalid_argument("Divisão por zero!"); // Lança uma exceção
    }
    return esquerda / direita;
  }
} 



const std::string& Expressao::getExpressao() const{
  return expressao;
}



void Expressao::setExpressao(const std::string& novaExpressao) {
  expressao = novaExpressao;
}



Node* Expressao::getRaiz() const {
  return raiz;
}
