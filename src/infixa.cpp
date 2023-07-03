#include "infixa.hpp"

Infixa::Infixa(std::string expr) : Expressao(expr) {}



Infixa::~Infixa() {}



bool Infixa::verificaCaracteresValidos() {
  char ultimoCaractere = '\0'; 

  int n = expressao.length();

  // Percorre toda a expressao verificando a validade dos caracteres
  for (int i = 0; i < n; i++) {
    char caractereAtual = expressao[i];

    // Se o caractere atual for um dígito, atualiza o último caractere e continua
    if (ehDigito(caractereAtual)) {
      ultimoCaractere = caractereAtual;
      continue;
    }

    // Se o caractere atual for um operador
    if (ehOperador(caractereAtual)) {
      // Verifica se o último caractere também é um operador
      if (ehOperador(ultimoCaractere)) {
        // Retorna false para a ocorrencia de dois operadores em sequencia
        return false;
      }
      // Verifica se há dois operadores separados por um espaço
      else if (ultimoCaractere == ' ' && ehOperador(expressao[i - 2])) {
        return false;
      }
    
      ultimoCaractere = caractereAtual;
      continue;
    }

    if (caractereAtual == '(' || caractereAtual == ')' || caractereAtual == ' ') {
      ultimoCaractere = caractereAtual;
      continue;
    }

    // Uma vez que chega aqui retorna false pois o caractere eh invalido
    return false;
  }

  // Se todos os caracteres forem válidos, retorna true
  return true;
}



bool Infixa::verificaParenteses() const {
  int numParenteses = 0; // Contador para verificar o equilíbrio dos parênteses

  int n = expressao.length();

  // Percorre cada caractere da expressão
  for (int i = 0; i < n; i++) {
    char caractereAtual = expressao[i];

    if (caractereAtual == '(') {
      numParenteses++;
    } 

    else if (caractereAtual == ')') {
        numParenteses--;
        
        if (numParenteses < 0) {
          return false;
        }
    }
  }

  // Se o contador for zero no final, significa que todos os parênteses estão equilibrados
  return (numParenteses == 0);
}



bool Infixa::verificaUltimoCaractere() const{
  char c = expressao[expressao.length() - 1];

  // Para expressoes terminadas em um digito ou fechamento de parentese retorna true
  if (ehDigito(c) || c == ')') {
    return true;
  }

  return false;
}



// Retorna a precedência do operador op
int Infixa::obterPrecedencia(char op) {
  int precedencia = -1;
  switch(op) {
    case '+':
    case '-':
      precedencia = 1;
      break;
    case '*':
    case '/':
      precedencia = 2;
      break;
    default:
      break;
  }
  return precedencia;
}



// Calcula a subexpressão com os dois operandos e um operador no topo 
// das pilhas e empilha o resultado como um nó de subexpressão
void Infixa::aplicarOperador(Pilha<Node *> &operandos, Pilha<char> &operadores) {
  // Obtém o segundo operando e desempilha da pilha de operandos
  Node *b = operandos.valorTopo();
  operandos.desempilhar();

  // Obtém o primeiro operando e desempilha da pilha de operandos
  Node *a = operandos.valorTopo();
  operandos.desempilhar();

  // Obtém o operador e desempilha da pilha de operadores
  char op = operadores.valorTopo();
  operadores.desempilhar();

  // Cria um novo nó de subexpressão com o operador
  Node *node = new Node(std::string(1, op));

  // Define o primeiro operando como filho esquerdo do nó de subexpressão
  node->setEsq(a);

  // Define o segundo operando como filho direito do nó de subexpressão
  node->setDir(b);

  // Empilha o nó de subexpressão na pilha de operandos
  operandos.empilhar(node);
}



// Efetua vericações para validar a expressão infixa
bool Infixa::avaliaExpressao() {
  if (!verificaTamanhoMaximo()) {
    return false;
  }
  else if (!verificaParenteses()) {
    return false;
  }
  else if (!verificaUltimoCaractere()) {
    return false;
  }
  else if (!verificaCaracteresValidos()) {
    return false;
  }
  else if (!contagemExpressao()) {
    return false;
  }

  return true;
}



// Lê a expressão infixa e constrói a árvore de expressão
bool Infixa::lerExpressao() {
  // Verifica se a expressão é válida
  if (!avaliaExpressao()) {
    throw std::invalid_argument("ERRO");
  }

  // Verifica se a expressão está vazia
  if (expressao.empty()) {
    return false;
  }

  // Cria pilhas para operandos e operadores
  Pilha<Node *> operandos;
  Pilha<char> operadores;

  // Percorre cada caractere da expressão
  for (unsigned int i = 0; i < expressao.length(); i++) {
    char caractereAtual = expressao[i];

    // Ignora espaços em branco
    if (caractereAtual == ' ') {
      continue;
    }

    // Se o caractere atual for um dígito ou ponto decimal
    if (ehDigito(caractereAtual)) {
      std::string num(1, caractereAtual);

      // Obtém o número completo
      while (i + 1 < expressao.size() && (ehDigito(expressao[i + 1]) || expressao[i + 1] == '.')) {
        num += expressao[++i];
      }

      // Empilha o número como um nó de operando
      operandos.empilhar(new Node(num));
    }
    else if (caractereAtual == '(') {
      operadores.empilhar(caractereAtual);
    }
    else if (caractereAtual == ')') {
      // Desempilha os operadores até encontrar um parêntese aberto
      while (!operadores.vazia() && operadores.valorTopo() != '(') {
        aplicarOperador(operandos, operadores);
      }
      operadores.desempilhar();
    }
    else if (ehOperador(caractereAtual)) {
      // Desempilha os operadores enquanto a precedência do operador 
      //no topo da pilha for maior ou igual ao do caractere atual
      while (!operadores.vazia() && obterPrecedencia(operadores.valorTopo()) >= obterPrecedencia(caractereAtual)) {
        aplicarOperador(operandos, operadores);
      }
      operadores.empilhar(caractereAtual);
    }
  }

  // Desempilha os operadores restantes e calcula a expressão
  while (!operadores.vazia()) {
    aplicarOperador(operandos, operadores);
  }

  raiz = operandos.valorTopo();

  return true;
}



// Converte a expressão infixa para a equivalente posfixa
std::string Infixa::converteExpressao() {
  std::string posfixa = ""; // Inicializa a string que armazenará a expressão pós-fixa
  Pilha<char> pilha; // Cria uma pilha para armazenar os operadores

  int n = expressao.length();

  // Percorre cada caractere da expressão infixa
  for (int i = 0; i < n; i++) {
    char caractereAtual = expressao[i];

    // Se o caractere atual for um dígito ou uma vírgula, adiciona à expressão pós-fixa
    if (ehDigito(caractereAtual)) {
      posfixa += caractereAtual;
    } else if (caractereAtual == '.') {
      posfixa += caractereAtual;
    } 
    // Se o caractere atual for um operador
    else if (ehOperador(caractereAtual)) {
      posfixa += ' ';
      // Desempilha os operadores de maior ou igual precedência e adiciona à expressão pós-fixa
      while (!pilha.vazia() && (pilha.valorTopo() != '(') && obterPrecedencia(caractereAtual) < obterPrecedencia(pilha.valorTopo())) {
        posfixa += pilha.desempilhar();
        posfixa += ' ';
      }
      // Empilha o operador atual
      pilha.empilhar(caractereAtual);
    } 

    else if (caractereAtual == '(') {
      pilha.empilhar(caractereAtual);
    } 

    else if (caractereAtual == ')') {
      posfixa  += ' ';
      // Desempilha os operadores até encontrar um parêntese de abertura e adiciona à expressão pós-fixa
      while (!pilha.vazia() && pilha.valorTopo() != '(') {
        posfixa += pilha.desempilhar();
        posfixa += ' ';
      }
      // Desempilha o parêntese de abertura
      pilha.desempilhar();
    }
  }

  // Desempilha os operadores restantes e adiciona à expressão pós-fixa
  while (!pilha.vazia()) {
    posfixa += ' ';
    posfixa += pilha.desempilhar();
  }

  return posfixa;
}