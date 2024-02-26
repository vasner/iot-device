function loadSettings() {
    fetch('/api/info')
        .then(r => r.json())
        .then(json => {
            main.appendChild(json)
        })
        .catch(err => console.log(err));
}

window.onload = loadSettings;
