#include <sys/mount.h>
#include <v8.h>
#include <node.h>
#include <errno.h>
#include <string.h>
#include <iostream>

v8::Handle<v8::Value> Mount(const v8::Arguments &args) {
  v8::HandleScope scope;

  if (args.Length() != 5) {
    // TODO: make it raise a proper exception
    return v8::ThrowException(v8::String::New("`mount` needs 5 parameters"));
  }

  v8::String::Utf8Value device(args[0]->ToString());
  v8::String::Utf8Value path(args[1]->ToString());
  v8::String::Utf8Value type(args[2]->ToString());
  int mask = args[3]->ToInt32()->Value();
  v8::String::Utf8Value data(args[4]->ToString());

  int mountNum = mount(*device, *path, *type, mask, *data);
  
  bool mountAction = (mountNum == 0) ? true : false;

  v8::Local<v8::Value> mounted = v8::Local<v8::Value>::New(v8::Boolean::New(mountAction));

  return scope.Close(mounted);
}

v8::Handle<v8::Value> Unmount(const v8::Arguments &args) {
  v8::HandleScope scope;

  if (args.Length() != 1) {
    return v8::ThrowException(v8::String::New("`umount` needs 1 parameter"));
  }

  v8::String::Utf8Value path(args[0]->ToString());

  bool mountAction = (umount(*path) == 0) ? true : false;

  v8::Local<v8::Value> mounted = v8::Local<v8::Value>::New(v8::Boolean::New(mountAction));

  return scope.Close(mounted);
}

#define EXPORT_CONST(__NAME__) exports->Set(v8::String::NewSymbol(#__NAME__), v8::Int32::New(__NAME__), v8::ReadOnly)

void init (v8::Handle<v8::Object> exports, v8::Handle<v8::Object> module) {
  exports->Set(v8::String::NewSymbol("mount"), v8::FunctionTemplate::New(Mount)->GetFunction());
  exports->Set(v8::String::NewSymbol("unmount"), v8::FunctionTemplate::New(Unmount)->GetFunction());
  
  EXPORT_CONST(MS_BIND);
  EXPORT_CONST(MS_MOVE);
  
  EXPORT_CONST(MS_NOATIME);
  EXPORT_CONST(MS_NODIRATIME);
  EXPORT_CONST(MS_RELATIME);
  
  EXPORT_CONST(MS_NODEV);
  EXPORT_CONST(MS_NOEXEC);
  EXPORT_CONST(MS_NOSUID);
  
  EXPORT_CONST(MS_RDONLY);
  EXPORT_CONST(MS_REMOUNT);
}
NODE_MODULE(mount, init)

