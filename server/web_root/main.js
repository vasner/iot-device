// TODO: Add js/css/html code formatter https://github.com/prettier/prettier and https://prettier.io/docs/en/precommit.html

function loadSensors() {
  fetch("/api/info")
    .then((r) => r.json())
    .then((json) => {
      main.appendChild(json);
    })
    .catch((err) => console.log(err));
}

window.onload = loadSensors;
