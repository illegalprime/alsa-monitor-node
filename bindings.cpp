#include "alsa/monitor-bindings.hpp"
#include <node.h>
#include <v8.h>

using namespace v8;

static void init(Handle<Object> exports) {
    monitor_init(exports);
}

NODE_MODULE(alsa, init)
