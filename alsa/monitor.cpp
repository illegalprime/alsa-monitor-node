#include "utils.hpp"
#include "constants.hpp"
#include <stdio.h>
#include <alsa/asoundlib.h>

int monitor_native(char *name) {
    snd_ctl_t *ctls[MAX_CARDS];
    int ncards = 0;
    int i, err = 0;

    if (!name) {
        int card = -1;
        while (snd_card_next(&card) >= 0 && card >= 0) {
            char cardname[16];
            if (ncards >= MAX_CARDS) {
                fprintf(stderr, "alsactl: too many cards\n");
                close_all(ctls, ncards);
                return -E2BIG;
            }
            sprintf(cardname, "hw:%d", card);
            err = open_ctl(cardname, &ctls[ncards]);
            if (err < 0) {
                close_all(ctls, ncards);
                return err;
            }
            ncards++;
        }
    } else {
        err = open_ctl(name, &ctls[0]);
        if (err < 0) {
            close_all(ctls, ncards);
            return err;
        }
        ncards++;
    }

    for (;ncards > 0;) {
        pollfd* fds = new pollfd[ncards];

        for (i = 0; i < ncards; i++) {
            snd_ctl_poll_descriptors(ctls[i], &fds[i], 1);
        }

        err = poll(fds, ncards, -1);
        if (err <= 0) {
            err = 0;
            break;
        }

        for (i = 0; i < ncards; i++) {
            unsigned short revents;
            snd_ctl_poll_descriptors_revents(ctls[i], &fds[i], 1, &revents);
            if (revents & POLLIN) {
                snd_ctl_event_t *event;
                snd_ctl_event_alloca(&event);

                if (snd_ctl_read(ctls[i], event) < 0) {
                    continue;
                }
                if (snd_ctl_event_get_type(event) != SND_CTL_EVENT_ELEM) {
                    continue;
                }

                unsigned int mask = snd_ctl_event_elem_get_mask(event);
                if (mask & SND_CTL_EVENT_MASK_VALUE) {
                    close_all(ctls, ncards);
                    return 1;
                }
            }
        }
    }

    close_all(ctls, ncards);
    return 0;
}
