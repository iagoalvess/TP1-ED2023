#ifndef POSFIXA_H
#define POSFIXA_H

#include <iostream>
#include <stdexcept>

#include "pilha.hpp"
#include "expressao.hpp"

// Herda da classe base Expressao
class Posfixa : public Expressao {
private:
  // Expressão infixa correspondente
  std::string expressaoInfixa;

  // Converte a árvore de expressão para expressão infixa
  void coverteRecursivo(Node* p, bool operador);
public:
  Posfixa(std::string expr);
  ~Posfixa();

  bool verificaOrdem() const;

  // Métodos sobrescritos da classe Pai
  bool verificaCaracteresValidos() override;
  bool avaliaExpressao() override;
  bool lerExpressao() override;
  std::string converteExpressao() override;
};

#endif  