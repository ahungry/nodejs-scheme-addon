#include <nan.h>

static Nan::CopyablePersistentTraits<v8::Function>::CopyablePersistent _cb;

static void SetCallback(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  _cb = Nan::Persistent<v8::Function>(info[0].As<v8::Function>());
}

static void RunCallback(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), Nan::New(_cb), 0, 0);
  _cb.Reset();
}

void RunThisCallback(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Function> cb = info[0].As<v8::Function>();
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, 0, 0);
}

static void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  Nan::SetMethod(exports, "setCallback", SetCallback);
  Nan::SetMethod(exports, "call", RunCallback);
  Nan::SetMethod(exports, "callThis", RunThisCallback);
}

NODE_MODULE(addon, Init)
