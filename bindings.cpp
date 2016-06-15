#include "alsa/monitor-bindings.hpp"
#include <nan.h>

using namespace v8;

static void init(v8::Local<v8::Object> exports) {
    monitor_init(exports);
}

NODE_MODULE(alsa, init)
