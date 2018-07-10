#include "scanner.h"
#include "grammar.h"
using namespace std;

int main() {
	SCANNER hex("in.java");
	hex.run();
	/*GRAMMAR_ANALYSIS six;
	six.set_tokens(hex.tokens);
	six.run();*/
	return 0;
}