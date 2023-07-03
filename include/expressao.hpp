#ifndef EXPRESSOES_H
#define EXPRESSOES_H

#include <string>
#include <iostream>

constexpr int MAX_TAM_EXPRESSAO = 1000; // Tamanho máximo da expressão

// Classe Node para construção da árvore binária
class Node {
public:
  Node(const std::string& value);
  ~Node();

  const std::string& getValor() const;
  void setValor(const std::string& value);

  Node* getEsq() const;
  void setEsq(Node* novaEsq);

  Node* getDir() const;
  void setDir(Node* novaDir);

  friend class Expressao;

private:
  std::string valor;
  Node* esq;
  Node* dir;
};

class Expressao {
public:
  virtual ~Expressao(); // Destrutor virtual
  explicit Expressao(const std::string& expr);

  static bool ehDigito(char c);
  static bool ehOperador(char c);

  bool verificaTamanhoMaximo() const;
  bool contagemExpressao() const;

  // Metodos virtuais implementados por posfixa e infixa
  virtual bool verificaCaracteresValidos() = 0;
  virtual bool lerExpressao() = 0;
  virtual bool avaliaExpressao() = 0;
  virtual std::string converteExpressao() = 0;
  
  double resolveExpressao();
  double resolveRecursivo(Node* raizAtual);

  const std::string& getExpressao() const;
  void setExpressao(const std::string& novaExpressao);
  Node* getRaiz() const;

protected:
  std::string expressao;
  Node* raiz;
};

#endif