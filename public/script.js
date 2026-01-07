// Variável para controlar se o Wasm está pronto
var wasmPronto = false;

// Função chamada automaticamente quando o C++ termina de carregar
Module.onRuntimeInitialized = function() {
    wasmPronto = true;
    console.log("✅ WebAssembly Carregado e Pronto!");
    
    // Habilita o botão visualmente
    const btn = document.querySelector('button');
    if(btn) {
        btn.innerHTML = "<i class='fas fa-check-circle'></i> CALCULAR ESTRUTURA (PRONTO)";
        btn.style.background = "linear-gradient(135deg, #a6e3a1, #89b4fa)";
    }
};

function calcular() {
    // 1. Verificação de Segurança
    if (!wasmPronto) {
        alert("Aguarde! O sistema ainda está carregando os módulos C++...");
        return;
    }

    console.log("Botão clicado! Iniciando cálculo...");

    // Declara ponteiros fora do try para poder liberar no finally
    let ptrA = null;
    let ptrB = null;

    try {
        // --- ETAPA 1: CAPTURA DE DADOS ---
        const n = parseInt(document.getElementById('dimensao').value);
        const erro = parseFloat(document.getElementById('precisao').value);
        const metodo = parseInt(document.getElementById('metodo').value);

        // Converte texto da matriz em array de números
        const strA = document.getElementById('matrizA').value;
        const arrayA = strA.trim().split(/\s+/).map(Number);

        // Converte texto do vetor b
        const strB = document.getElementById('vetorB').value;
        const arrayB = strB.trim().split(/\s+/).map(Number);

        // Validação básica
        if (arrayA.length !== n * n || arrayB.length !== n) {
            alert(`Erro de Dimensão! \nEsperado Matriz A: ${n*n} números.\nEsperado Vetor B: ${n} números.`);
            return;
        }

        // --- ETAPA 2: ALOCAÇÃO DE MEMÓRIA (C++) ---
        console.log("Dados capturados com sucesso! Iniciando alocação de memória...");

        // Double usa 8 bytes. Multiplicamos o tamanho por 8.
        ptrA = Module._malloc(arrayA.length * 8);
        ptrB = Module._malloc(arrayB.length * 8);

        // --- ETAPA 3: CÓPIA DE DADOS (JS -> C++) ---
        // Aqui ocorria o erro: HEAPF64 precisa existir
        Module.HEAPF64.set(arrayA, ptrA >> 3);
        Module.HEAPF64.set(arrayB, ptrB >> 3);

        // --- ETAPA 4: EXECUÇÃO ---
        const calcularFunc = Module.cwrap('calcularSistemaSismico', 'string', 
            ['number', 'number', 'number', 'number', 'number']
        );

        const jsonString = calcularFunc(n, ptrA, ptrB, erro, metodo);
        const resultado = JSON.parse(jsonString);
        
        exibirResultados(resultado);

    } catch (e) {
        console.error("Erro no processamento:", e);
        alert("Ocorreu um erro ao processar os dados. Verifique o console (F12).");
    } finally {
        // --- ETAPA 5: LIMPEZA DE MEMÓRIA ---
        // Libera memória apenas se ela foi alocada
        if (ptrA !== null) Module._free(ptrA);
        if (ptrB !== null) Module._free(ptrB);
    }
}

function exibirResultados(res) {
    const divRes = document.getElementById('resultado');
    const divContent = document.getElementById('output-content');
    
    divRes.classList.remove('hidden');
    // Ajuste seguro caso a classe css mude
    divRes.className = res.perigo ? 'danger' : 'safe'; 

    let html = `<h3>Status: ${res.perigo ? "🚨 PERIGO DETECTADO" : "✅ ESTRUTURA SEGURA"}</h3>`;
    html += `<p><strong>Método Usado:</strong> ${res.metodo} (${res.iteracoes} iterações totais)</p>`;

    // Formatar Matriz Inversa
    html += `<h4>Matriz Inversa (A⁻¹)</h4><pre>`;
    res.inversa.forEach(linha => {
        html += linha.map(v => v.toFixed(6).padStart(10)).join("  ") + "\n";
    });
    html += `</pre>`;

    // Formatar Deslocamentos
    html += `<h4>Deslocamentos Calculados (d)</h4><ul>`;
    res.d.forEach((val, i) => {
        const alertStyle = Math.abs(val) > 0.4 ? "color: #f38ba8; font-weight:bold;" : "";
        html += `<li style="${alertStyle}">d${i+1} = ${val.toFixed(6)} cm</li>`;
    });
    html += `</ul>`;

    divContent.innerHTML = html;
}