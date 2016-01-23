#include <unistd.h>
#include <node.h>
#include <string.h>
#include <v8.h>

#include "monitor.hpp"

using namespace v8;

// Carry data between threads
struct MonitorChannel {
    // libuv control
    uv_work_t request;
    // javascript callback
    Persistent<Function> callback;

    // Configuration data for monitoring alsa cards
    char* card_name;
};

// called by libuv worker in thread pool
static void monitor_async(uv_work_t* request) {
    MonitorChannel* channel = static_cast<MonitorChannel*>(request->data);
    monitor_native(channel->card_name);
}

static void monitor_async_after(uv_work_t* request, int status) {
    MonitorChannel* channel = static_cast<MonitorChannel*>(request->data);

    // Execute callback notifying that something has changed
    channel->callback->Call(Context::GetCurrent()->Global(), 0, NULL);

    // This monitor loops forever, so send it back to poll some more
    uv_queue_work(uv_default_loop(), &channel->request, monitor_async, monitor_async_after);

    // TODO: How to cleanup stuff?
}

Handle<Value> Monitor(const Arguments& args) {
    // create communication channel
    MonitorChannel* channel = new MonitorChannel;

    // get callback from function and assign it
    Handle<Function> callback = Handle<Function>::Cast(args[0]);
    channel->callback = Persistent<Function>::New(callback);

    // attatch channel to uv data
    channel->request.data = channel;

    // default is to watch all cards
    channel->card_name = NULL;

    // enqueue some work
    uv_queue_work(uv_default_loop(), &channel->request, monitor_async, monitor_async_after);

    // return nothing
    return Undefined();
}

void monitor_init(Handle<Object> exports) {
    exports->Set(String::NewSymbol("monitor"), FunctionTemplate::New(Monitor)->GetFunction());
}

