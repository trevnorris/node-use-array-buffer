#ifndef _NODE_BASICS_H_
#define _NODE_BASICS_H_

#include <stdio.h>  // fprintf, fflush
#include <stdlib.h> // exit

#define CHECK_ARG(args, arg, msg)                                             \
  if (!(arg)) {                                                               \
    args.GetReturnValue().Set(                                                \
        v8::ThrowException(                                                   \
          v8::Exception::TypeError(                                           \
            String::NewFromUtf8(Isolate::GetCurrent(), msg))));               \
    return;                                                                   \
  }                                                                           \
  do { } while (0)


#define SET_RETURN(args, ret)                                                 \
  do { args.GetReturnValue().Set(ret); } while (0)


#define SETUP_SCOPE                                                           \
  Isolate* isolate = Isolate::GetCurrent();                                   \
  HandleScope scope(isolate);                                                 \
  do { } while (0)

namespace basics {

using v8::Isolate;
using v8::Local;
using v8::Persistent;


static void FatalError(const char* location, const char* message) {
  if (location)
    fprintf(stderr, "FATAL ERROR: %s %s\n", location, message);
  else
    fprintf(stderr, "FATAL ERROR: %s\n", message);
  fflush(stderr);
  exit(5);
}


template<class T>
inline Local<T> PersistentToLocal(Isolate* isolate,
                                  const Persistent<T>& p_obj) {
  if (p_obj.IsWeak())
    return Local<T>::New(isolate, p_obj);
  else
    return *reinterpret_cast<Local<T>*>(
        const_cast<Persistent<T>*>(&p_obj));
}


} // namespace basics

#endif
