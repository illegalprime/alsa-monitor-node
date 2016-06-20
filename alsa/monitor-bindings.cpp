#include <unistd.h>
#include <string.h>
#include <nan.h>

#include "monitor.hpp"

using namespace v8;

// Carry data between threads
struct MonitorChannel {
    // libuv control
    uv_work_t request;
    // javascript callback
    Nan::Callback* callback;

    // Configuration data for monitoring alsa cards
    char* card_name;
};

// called by libuv worker in thread pool
static void monitor_async(uv_work_t* request) {
    MonitorChannel* channel = static_cast<MonitorChannel*>(request->data);
    monitor_native(channel->card_name);
}

static void monitor_async_after(uv_work_t* request, int status) {
    Nan::HandleScope scope;

    MonitorChannel* channel = static_cast<MonitorChannel*>(request->data);

    // Execute callback notifying that something has changed
    channel->callback->Call(0, NULL);

    // This monitor loops forever, so send it back to poll some more
    uv_queue_work(
        uv_default_loop(),
        &channel->request,
        monitor_async,
        monitor_async_after
    );

    // TODO: How to cleanup stuff?
}

void Monitor(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    // create communication channel
    MonitorChannel* channel = new MonitorChannel;

    // get callback from function and assign it
    channel->callback = new Nan::Callback(info[0].As<Function>());

    // attatch channel to uv data
    channel->request.data = channel;

    // default is to watch all cards
    channel->card_name = NULL;

    // enqueue some work
    uv_queue_work(
        uv_default_loop(),
        &channel->request,
        monitor_async,
        monitor_async_after
    );

}

void monitor_init(v8::Local<v8::Object> exports) {
    exports->Set(
        Nan::New("monitor").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(Monitor)->GetFunction()
    );
}
