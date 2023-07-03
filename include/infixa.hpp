#ifndef INFIXA_H
#define INFIXA_H

#include <iostream>
#include <stdexcept>

#include "expressao.hpp"
#include "pilha.hpp"

// Herda da classe base Expressao
class Infixa : public Expressao {
public:
  Infixa(std::string expr);
  ~Infixa();
  
  bool verificaParenteses() const;
  bool verificaUltimoCaractere() const;

  int obterPrecedencia(char op);

  void aplicarOperador(Pilha<Node*>& operandos, Pilha<char>& operadores);
  
  // Metodos sobrescritos da classe Pai
  bool verificaCaracteresValidos() override;
  bool avaliaExpressao() override;
  bool lerExpressao() override;  
  std::string converteExpressao() override;
};


#endif