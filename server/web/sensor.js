class Sensor {
  name = null;
  value = null;
  unit = null;
  container = null;

  fromJson(json) {
    this.name = json.name;
    this.value = json.value;
    this.unit = json.unit;
    return this;
  }

  toUI() {
    const value_label = createDiv(["label", "value"]);
    value_label.innerHTML = this.value;
    const value_unit = createDiv(["label", "unit"]);
    value_unit.innerHTML = this.unit;
    this.container = createDiv(["sensor-container"], [value_label, value_unit]);
    return this.container;
  }
}
