var wasmPronto = false;

Module.onRuntimeInitialized = function() {
    wasmPronto = true;
    console.log("WebAssembly Carregado e Pronto");
    const btn = document.querySelector('button');
    if(btn) {
        btn.innerText = "Calcular";
        btn.style.backgroundColor = "#a6e3a1"; 
    }
};

function calcular() {
    if (!wasmPronto) {
        alert("Aguarde o carregamento do módulo C++.");
        return;
    }

    let ptrA = null;
    let ptrB = null;
    let ptrResultado = null; // Ponteiro para a string de resposta

    try {
        // --- ETAPA 1: CAPTURA DE DADOS ---
        const n = parseInt(document.getElementById('dimensao').value);
        const erro = parseFloat(document.getElementById('precisao').value);
        const limiar = parseFloat(document.getElementById('limiar').value);
        const metodo = parseInt(document.getElementById('metodo').value);
        const strA = document.getElementById('matrizA').value;
        const arrayA = strA.trim().split(/\s+/).map(Number);
        const strB = document.getElementById('vetorB').value;
        const arrayB = strB.trim().split(/\s+/).map(Number);

        if (arrayA.length !== n * n || arrayB.length !== n) {
            alert(`Erro de Dimensão!`);
            return;
        }

        ptrA = Module._malloc(arrayA.length * 8);
        ptrB = Module._malloc(arrayB.length * 8);

        Module.HEAPF64.set(arrayA, ptrA >> 3);
        Module.HEAPF64.set(arrayB, ptrB >> 3);

        const calcularFunc = Module.cwrap('calcularSistemaSismico', 'number', 
            ['number', 'number', 'number', 'number', 'number', 'number']
        );

        ptrResultado = calcularFunc(n, ptrA, ptrB, erro, metodo, limiar);

        const jsonString = Module.UTF8ToString(ptrResultado);

        const resultado = JSON.parse(jsonString);
        
        exibirResultados(resultado, limiar);

    } catch (e) {
        console.error("Erro:", e);
        alert("Erro no processamento.");
    } finally {
        if (ptrA !== null) Module._free(ptrA);
        if (ptrB !== null) Module._free(ptrB);
        
        if (ptrResultado !== null) {
            const liberarFunc = Module.cwrap('liberarMemoria', null, ['number']);
            liberarFunc(ptrResultado);
        }
    }
}

function exibirResultados(res, limiar) {
    const divRes = document.getElementById('resultado');
    const divContent = document.getElementById('output-content');
    divRes.classList.remove('hidden');

    // Verifica erro crítico do C++
    if (!res.sucesso) {
        divRes.className = 'danger';
        divContent.innerHTML = `<h3>Erro Matemático</h3><p >${res.erro_critico}</p>`;
        return;
    }

    // Configura cores baseado no perigo
    divRes.className = res.perigo ? 'danger' : 'safe'; 

    let html = `<h3>Status: ${res.perigo ? "🚨 Estrutura em perigo" : "✅ Estrutura segura"}</h3>`;
    
    // Aviso de Convergência (Teoria)
    if (!res.diagonal_dominante) {
        html += `<div style="background: #fff3cd; color: #856404; padding: 10px; border-radius: 5px; margin-bottom: 10px;">
                    <strong>Aviso :</strong> O critério de convergência (Diagonal/Sassenfeld) não foi satisfeito. 
                    A convergência não é garantida para métodos iterativos, mas o cálculo será tentado.
                 </div>`;
    }

    html += `<p><strong>Método:</strong> ${res.metodo} | <strong>Iterações Totais:</strong> ${res.iteracoes}</p>`;

    html += `<h4>Matriz Inversa (A⁻¹)</h4><pre>`;
    res.inversa.forEach(linha => {
        html += linha.map(v => v.toFixed(6).padStart(10)).join("  ") + "\n";
    });
    html += `</pre>`;

    html += `<h4>Deslocamentos (d)</h4><ul>`;
    res.d.forEach((val, i) => {
        // Agora compara com a variável 'limiar' e não mais 0.4 fixo
        const critico = Math.abs(val) > limiar;
        const style = critico ? "color: red; font-weight:bold;" : "";
        
        html += `<li style="${style}">
                    d${i+1} = ${val.toFixed(6)} cm 
                    ${critico ? "(CRÍTICO)" : ""}
                 </li>`;
    });
    html += `</ul>`;

    divContent.innerHTML = html;
}