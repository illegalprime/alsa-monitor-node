#!/usr/bin/env node
const alsa_monitor = require("alsa-monitor");

alsa_monitor.volume.on("change", () => {
    console.log("Someone changed the volume!");
});
