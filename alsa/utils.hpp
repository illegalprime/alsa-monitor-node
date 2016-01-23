#ifndef ALSA_UTILS_NODE_HPP
#define ALSA_UTILS_NODE_HPP

#include <alsa/asoundlib.h>

int open_ctl(const char *name, snd_ctl_t **ctlp);
void close_all(snd_ctl_t* ctls[], int ncards);

#endif
