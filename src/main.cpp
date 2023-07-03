#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <unistd.h>

#include "posfixa.hpp"
#include "infixa.hpp"

// Verifica se a expressão existe
bool expressaoExiste(Expressao* expressao) {
	if (expressao == nullptr || expressao->getExpressao() == "") {
		std::cerr << "ERRO: EXP NAO EXISTE" << std::endl;
		exit(1);
	}

	return true;
}



// Imprime a conversão da expressão
void printConversao(Expressao* expressao, bool verificaTipoExpressao) {
	if (!expressaoExiste(expressao)) {
		return;
	}

	try {
		std::string convertida = expressao->converteExpressao();

		if (verificaTipoExpressao) {
			std::cout << "INFIXA: " << convertida << std::endl;
		}
		else {
			std::cout << "POSFIXA: " << convertida << std::endl;
		}
	}
	catch (const std::invalid_argument &e) {
		std::cerr << "ERRO NA CONVERSAO! " << e.what() << std::endl;
		exit(1);
	}
}



// Imprime a solução da expressão
void printSolucao(Expressao* expressao) {
	if (!expressaoExiste(expressao)) {
		return;
	}
	
	try {
		double resultado = expressao->resolveExpressao();
		std::cout << "VAL : " << resultado << std::endl;
	}
	catch (const std::invalid_argument &e) {
		std::cerr << "ERRO: " << e.what() << std::endl;
		exit(1);
	}
}



// Efetua a leitura da expressão
void printLerExpressao(Expressao& expressao) {
	try {
		if (expressao.lerExpressao()) {
			std::cout << "EXPRESSAO OK:" << expressao.getExpressao() << std::endl;
		}
	}
	catch (const std::invalid_argument &e) {
		std::cerr << e.what() << ":" << expressao.getExpressao() << " NAO VALIDA" << std::endl;
		exit(1);
	}
}



int main(int argc, char *argv[]) {
	std::string arquivoEntrada = "arquivo.txt"; // Valor padrão do arquivo de entrada

	// Obtém as opções de linha de comando
	int opcao;
	while ((opcao = getopt(argc, argv, "o:")) != -1) {
		switch (opcao) {
		case 'o':
			arquivoEntrada = optarg; // Altera o arquivo de entrada
			break;
		default:
			std::cerr << "Opção inválida" << std::endl;
			return 1;
		}
	}

	// Abre o arquivo de entrada
	std::ifstream arquivo(arquivoEntrada);
	if (!arquivo.is_open())
	{
		std::cerr << "Erro ao abrir o arquivo de entrada" << std::endl;
		return 1;
	}

	// Variáveis para armazenar as expressões
	Expressao *expressaoPosfixa = nullptr;
	Expressao *expressaoInfixa = nullptr;

	std::string linha;

	bool posfixa;	// Armazena true caso a expressão seja posfixa

	while (getline(arquivo, linha)) {
		if (linha.substr(0, 11) == "LER POSFIXA") {
			posfixa = true;

			linha = linha.substr(11); // Remove o prefixo "LER POSFIXA "

			if (expressaoInfixa != nullptr) {
				delete expressaoInfixa;
				expressaoInfixa = nullptr;
			}

			try {
				delete expressaoPosfixa;
				expressaoPosfixa = new Posfixa(linha);
				printLerExpressao(*expressaoPosfixa);
			}
			catch (const std::invalid_argument &e) {
				std::cerr << e.what() << ": " << linha << " NAO VALIDA" << std::endl;
			}
		}
		else if (linha.substr(0, 10) == "LER INFIXA") {
			posfixa = false;

			linha = linha.substr(10); // Remove o prefixo "LER INFIXA "

			if (expressaoPosfixa != nullptr) {
				delete expressaoPosfixa;
				expressaoPosfixa = nullptr;
			}

			try {
				delete expressaoInfixa;
				expressaoInfixa = new Infixa(linha);
				printLerExpressao(*expressaoInfixa);
			}
			catch (const std::invalid_argument &e) {
				std::cerr << e.what() << ": " << linha << " NAO VALIDA" << std::endl;
			}
		}
		else if (linha == "POSFIXA") {
			if (expressaoPosfixa != nullptr) {
				std::cerr << "ERRO: EXP JA NO FORMATO POSFIXO" << std::endl;
				exit(1);
			}

			printConversao(expressaoInfixa, posfixa);
		}
		else if (linha == "INFIXA") {
			if (expressaoInfixa != nullptr) {
				std::cerr << "ERRO: EXP JA NO FORMATO INFIXO" << std::endl;
				exit(1);
			}

			printConversao(expressaoPosfixa, posfixa);
		}
		else if (linha.substr(0, 7) == "RESOLVE") {
			printSolucao(posfixa ? expressaoPosfixa : expressaoInfixa);
		}
		else {
			std::cout << "ENTRADA INVALIDA! TENTE NOVAMENTE" << std::endl;
		}
	}

	delete expressaoInfixa;
	delete expressaoPosfixa;

	arquivo.close();
	return 0;
}