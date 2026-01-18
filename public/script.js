var wasmPronto = false;

Module.onRuntimeInitialized = function() {
    wasmPronto = true;
    console.log("WebAssembly Carregado e Pronto");
    const btn = document.querySelector('button');
    if(btn) btn.innerText = "Calcular Comparativo";
};

function calcular() {
    if (!wasmPronto) return alert("Aguarde o carregamento do módulo C++.");

    let ptrA = null;
    let ptrB = null;
    let ptrResJacobi = null;
    let ptrResSeidel = null;

    try {
        const n = parseInt(document.getElementById('dimensao').value);
        
        const erro = parseFloat(document.getElementById('precisao').value.replace(',', '.'));
        const limiar = parseFloat(document.getElementById('limiar').value.replace(',', '.'));
        
        const strA = document.getElementById('matrizA').value.replace(/,/g, '.');
        const arrayA = strA.trim().split(/\s+/).map(Number);
        
        const strB = document.getElementById('vetorB').value.replace(/,/g, '.');
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
        
        ptrResJacobi = calcularFunc(n, ptrA, ptrB, erro, 0, limiar);
        const jsonJacobi = Module.UTF8ToString(ptrResJacobi);
        gerarHtmlResultado(JSON.parse(jsonJacobi), limiar, 'res-jacobi');

        ptrResSeidel = calcularFunc(n, ptrA, ptrB, erro, 1, limiar);
        const jsonSeidel = Module.UTF8ToString(ptrResSeidel);
        gerarHtmlResultado(JSON.parse(jsonSeidel), limiar, 'res-seidel');

        document.getElementById('painel-resultados').classList.remove('hidden');

    } catch (e) {
        console.error("Erro:", e);
        alert("Erro no processamento.");
    } finally {
        if (ptrA !== null) Module._free(ptrA);
        if (ptrB !== null) Module._free(ptrB);
        
        const liberarFunc = Module.cwrap('liberarMemoria', null, ['number']);
        if (ptrResJacobi !== null) liberarFunc(ptrResJacobi);
        if (ptrResSeidel !== null) liberarFunc(ptrResSeidel);
    }
}

function gerarHtmlResultado(res, limiar, elementId) {
    const divTarget = document.getElementById(elementId);
    
    if (!res.sucesso) {
        divTarget.className = 'result-box danger';
        divTarget.innerHTML = `<h3 style="color:#f38ba8">Erro</h3><p>${res.erro_critico}</p>`;
        return;
    }

    divTarget.className = res.perigo ? 'result-box danger' : 'result-box safe'; 

    let html = `<h4>Status: ${res.perigo ? "🚨 Perigo" : "✅ Seguro"}</h4>`;
    
    if (!res.diagonal_dominante) {
        const nomeCriterio = res.metodo.includes("Jacobi") ? "Critério de linhas" : "Sassenfeld";

        html += `<div style="font-size: 0.9em; background: #fff3cd; color: #856404; padding: 10px; border-radius: 4px; margin-bottom: 10px;">
                    Aviso: O critério de convergência (<strong>${nomeCriterio}</strong>) não foi satisfeito. 
                    <br><span style="font-size: 0.9em; opacity: 0.9">A convergência não é garantida, mas o cálculo foi tentado.</span>
                 </div>`;
    }
    html += `<p><strong>Iterações:</strong> ${res.iteracoes}</p>`;

    html += `<h5>Matriz Inversa (A⁻¹)</h5><pre style="font-size: 0.75em;">`;
    if(res.inversa) {
        res.inversa.forEach(linha => {
            html += linha.map(v => v.toFixed(6).padStart(8)).join(" ") + "\n";
        });
    }
    html += `</pre>`;

    html += `<h5>Deslocamentos (d)</h5><ul style="margin: 0; padding-left: 15px;">`;
    res.d.forEach((val, i) => {
        const critico = Math.abs(val) > limiar;
        const style = critico ? "color: #f38ba8; font-weight:bold;" : ""; 
        html += `<li style="${style}">d${i+1} = ${val.toFixed(6)} cm</li>`;
    });
    html += `</ul>`;    

    let erroVisual = "N/A";
    if (res.erro_final) {
        const strCientifica = res.erro_final.toExponential(4);
        const partes = strCientifica.split('e');
        erroVisual = `${partes[0]} × 10<sup>${partes[1]}</sup>`;
    }
    
    html += `<div style="margin-top:15px; padding-top:10px; border-top: 1px solid #45475a;">
                <p style="text-align: left; font-weight:700; font-size:14px; color:#cdd6f4;">Erro Relativo Final:</p>
                <span style="color: #fab387;">
                    ${erroVisual}
                </span>
             </div>`;

    divTarget.innerHTML = html;
}