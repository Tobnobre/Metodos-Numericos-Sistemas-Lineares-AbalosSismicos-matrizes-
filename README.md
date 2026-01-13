##  Pré-requisitos

Para compilar e rodar este projeto, você precisará de:

1. **Emscripten SDK (emsdk):** Para compilar C++ em WebAssembly.
2. **Python 3:** Para rodar o servidor local HTTP.

##  Compilação (Build)

Para gerar os arquivos `.wasm` e `.js` atualizados, execute o comando abaixo na raiz do projeto (certifique-se de que o ambiente `emsdk` esteja carregado):

```bash
emcc src/main.cpp src/utils.cpp src/jacobi.cpp src/seidel.cpp src/sismic.cpp -I include -o public/trabalho2.js -s WASM=1 -s "EXPORTED_RUNTIME_METHODS=['cwrap', 'getValue', 'setValue', 'HEAPF64', 'UTF8ToString']" -s "EXPORTED_FUNCTIONS=['_malloc', '_free', '_calcularSistemaSismico', '_liberarMemoria']" -O2