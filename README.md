# ALSA Monitoring ![build status](https://api.travis-ci.org/illegalprime/alsa-monitor-node.svg)

A simple library to notify users when ALSA properties change.

### Currently

Currently the only implemented feature is triggering events when the volume
changes.

## API

The API is straightforward:

```javascript
const alsa_monitor = require("alsa-monitor");

alsa_monitor.volume.on("change", () => {
    console.log("Someone changed the volume!");
});
```
