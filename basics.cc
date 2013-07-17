#include <stdlib.h> // malloc
#include <node.h>
#include <v8.h>

#include "basics.h"

#define UPPER_LIMIT 4096

namespace basics {

using v8::ArrayBuffer;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Handle;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void FreeCallback(Isolate* isolate, Persistent<ArrayBuffer>* p_obj, void* arg);

void MakeArrayBuffer(const FunctionCallbackInfo<Value>& args) {
  SETUP_SCOPE;

  CHECK_ARG(args, args[0]->IsNumber(), "first argument should be a number");

  Local<ArrayBuffer> ab;
  uint32_t size = args[0]->Uint32Value();

  if (size > UPPER_LIMIT) {
    void* data = size > 0 ? malloc(size) : NULL;
    if (data == NULL && size > 0)
      return FatalError("basics::Run()", "Out of Memory");
    ab = ArrayBuffer::New(data, size);
    Persistent<ArrayBuffer> p_obj(isolate, ab);
    p_obj.MakeWeak(data, FreeCallback);
    p_obj.MarkIndependent();
    isolate->AdjustAmountOfExternalAllocatedMemory(size);
  } else {
    ab = ArrayBuffer::New(size);
  }

  SET_RETURN(args, ab);
}


void FreeCallback(Isolate* isolate,
                  Persistent<ArrayBuffer>* p_obj,
                  void* data) {
  HandleScope scope(Isolate::GetCurrent());
  Local<ArrayBuffer> obj = PersistentToLocal<ArrayBuffer>(isolate, *p_obj);
  int len = obj->GetIndexedPropertiesExternalArrayDataLength();
  if (data != NULL && len > 0) {
    isolate->AdjustAmountOfExternalAllocatedMemory(-len);
    free(data);
  }
  p_obj->Dispose();
}


void Initialize(Handle<Object> target) {
  HandleScope scope(Isolate::GetCurrent());

  target->Set(String::New("MakeArrayBuffer"),
      FunctionTemplate::New(MakeArrayBuffer)->GetFunction());
}


} // namespace basics

NODE_MODULE(basics, basics::Initialize)
