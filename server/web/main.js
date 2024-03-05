function loadSensors() {
  fetch("/api/info")
    .then((r) => r.json())
    .then((json) => {
      const label = createDiv(["label"]);
      label.innerHTML = `server: v${json["server_version"]} device: v${json["device_version"]}`;
      footer.append(createDiv(["footer-label"], [label]));
    })
    .catch((err) => console.log(err));
}

function createElement(type, classes = [], children = []) {
  const element = document.createElement(type);
  children.forEach((c) => element.appendChild(c));
  classes.forEach((c) => element.classList.add(c));
  return element;
}

function createDiv(classes = [], children = []) {
  return createElement("div", classes, children);
}

window.onload = loadSensors;
