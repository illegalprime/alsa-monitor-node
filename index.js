var EventEmitter = require("events").EventEmitter;
var alsa_native = require("bindings")("alsa.node");

var volume = new EventEmitter();

alsa_native.monitor(function() {
    volume.emit("change");
});

module.exports = {
    volume: volume,
};
