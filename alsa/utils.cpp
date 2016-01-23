#include <alsa/asoundlib.h>

int open_ctl(const char *name, snd_ctl_t **ctlp) {
    snd_ctl_t *ctl;
    int err;

    err = snd_ctl_open(&ctl, name, SND_CTL_READONLY);
    if (err < 0) {
        fprintf(stderr, "Cannot open ctl %s\n", name);
        return err;
    }
    err = snd_ctl_subscribe_events(ctl, 1);
    if (err < 0) {
        fprintf(stderr, "Cannot open subscribe events to ctl %s\n", name);
        snd_ctl_close(ctl);
        return err;
    }
    *ctlp = ctl;
    return 0;
}

void close_all(snd_ctl_t* ctls[], int ncards) {
    for (ncards -= 1; ncards >= 0; --ncards) {
        snd_ctl_close(ctls[ncards]);
    }
}


