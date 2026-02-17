const backendUrl = "http://127.0.0.1:5000";
let selectedStationIndex = ""; 
let selectedPartyIndex = null;
let myChart = null;

function showPage(pageId) {
    document.querySelectorAll('section').forEach(s => s.classList.add('hidden'));
    document.getElementById(pageId).classList.remove('hidden');
}

function validateAndProceed() {
    const stationSelect = document.getElementById("stationSelect");
    if (stationSelect.value === "") {
        alert("Wait! Please select a Polling Station location first.");
        return;
    }
    selectedStationIndex = stationSelect.value;
    document.getElementById("currentStationHead").innerText = stationSelect.options[stationSelect.selectedIndex].text;
    showPage('page-menu');
}

function testBackend() {
    fetch(`${backendUrl}/test`).then(() => showPage('page-welcome')).catch(() => alert("C++ Server Offline!"));
}

function showVotePage() {
    selectedPartyIndex = null;
    document.querySelectorAll('.symbol-item').forEach(i => i.classList.remove('selected'));
    showPage('page-vote');
}

function selectParty(idx, el) {
    document.querySelectorAll('.symbol-item').forEach(i => i.classList.remove('selected'));
    el.classList.add('selected');
    selectedPartyIndex = idx;
}

function liveStatus() {
    fetch(`${backendUrl}/results?stationIndex=${selectedStationIndex}`)
        .then(res => res.text())
        .then(data => {
            showPage('page-data');
            document.getElementById('graphWrapper').classList.remove('hidden');
            const nums = data.match(/\d+/g);
            if(nums) {
                const votes = nums.slice(0, 4);
                drawChart(['PTI', 'PML-N', 'PPP', 'IND'], votes);
                renderResults(votes, true);
            }
        });
}

function viewResults() {
    fetch(`${backendUrl}/results?stationIndex=${selectedStationIndex}`)
        .then(res => res.text())
        .then(data => {
            showPage('page-data');
            document.getElementById('graphWrapper').classList.add('hidden');
            const nums = data.match(/\d+/g);
            if(nums) renderResults(nums.slice(0, 4), false);
        });
}

function drawChart(labels, values) {
    const ctx = document.getElementById('voteChart').getContext('2d');
    if (myChart) myChart.destroy();
    myChart = new Chart(ctx, {
        type: 'bar',
        data: {
            labels: labels,
            datasets: [{ 
                label: 'Votes Polled', 
                data: values, 
                backgroundColor: ['#FFD700', '#0056b3', '#ff0000', '#28a745'] 
            }]
        },
        options: { 
            maintainAspectRatio: false,
            plugins: {
                legend: { display: false } // Saves more vertical space
            }
        }
    });
}

function renderResults(votes, isLive) {
    const parties = ['PTI', 'PML-N', 'PPP', 'IND'];
    const styles = ['row-pti', 'row-pmln', 'row-ppp', 'row-ind'];
    const board = document.getElementById('resultsBoard');
    board.innerHTML = "";
    let max = -1, winner = "";
    
    parties.forEach((p, i) => {
        const v = parseInt(votes[i]);
        if(v > max) { max = v; winner = p; }
        board.innerHTML += `<div class="row ${styles[i]}"><span>${p}</span><span>${v} Votes</span></div>`;
    });
    
    const msg = document.getElementById('leaderMessage');
    if(isLive) {
        msg.innerText = "Current Live Vote Distribution Data";
    } else {
        msg.innerText = `${winner} is currently leading with ${max} votes. Wait for full counting!`;
    }
}

function exitSystem() {
    const name = document.getElementById("voterName").value || "Authorized User";
    alert(`Thank you for using EVM, ${name}!`);
    location.reload();
}

function castVote() {
    const vID = document.getElementById("voterID").value;
    const vName = document.getElementById("voterName").value;
    if(!vID || selectedPartyIndex === null) return alert("Fill all details first!");

    const params = new URLSearchParams();
    params.append("voterID", vID); params.append("voterName", vName);
    params.append("partyIndex", selectedPartyIndex); params.append("stationIndex", selectedStationIndex);

    fetch(`${backendUrl}/vote`, { method: "POST", body: params })
        .then(res => res.text())
        .then(data => {
            alert(data);
            if(data.includes("successfully")) showPage('page-menu');
        });
}