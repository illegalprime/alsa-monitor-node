#!/usr/bin/env node

var alsa = require("./index.js");

alsa.monitor(function() {
    console.log("trigger");
});
