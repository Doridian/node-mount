#include <sys/mount.h>
#include <node.h>
#include <errno.h>
#include <string.h>
#include <iostream>

namespace {

void Mount(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  if (args.Length() != 5) {
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, "`mount` needs 5 parameters")));
    return;
  }

  v8::String::Utf8Value device(args[0]->ToString(isolate));
  v8::String::Utf8Value path(args[1]->ToString(isolate));
  v8::String::Utf8Value type(args[2]->ToString(isolate));
  int mask = args[3]->ToInt32(isolate)->Value();
  v8::String::Utf8Value data(args[4]->ToString(isolate));

  int mountNum = mount(*device, *path, *type, mask, *data);
  
  bool mountAction = (mountNum == 0) ? true : false;

  args.GetReturnValue().Set(v8::Boolean::New(isolate, mountAction));
}

void Unmount(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1) {
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, "`umount` needs 1 parameter")));
    return;
  }

  v8::String::Utf8Value path(args[0]->ToString());

  bool mountAction = (umount(*path) == 0) ? true : false;

  args.GetReturnValue().Set(v8::Boolean::New(isolate, mountAction));
}

void init (v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "mount", Mount);
  NODE_SET_METHOD(exports, "unmount", Unmount);

  NODE_DEFINE_CONSTANT(exports, MS_BIND);
  NODE_DEFINE_CONSTANT(exports, MS_MOVE);
  
  NODE_DEFINE_CONSTANT(exports, MS_NOATIME);
  NODE_DEFINE_CONSTANT(exports, MS_NODIRATIME);
  NODE_DEFINE_CONSTANT(exports, MS_RELATIME);
  
  NODE_DEFINE_CONSTANT(exports, MS_NODEV);
  NODE_DEFINE_CONSTANT(exports, MS_NOEXEC);
  NODE_DEFINE_CONSTANT(exports, MS_NOSUID);
  
  NODE_DEFINE_CONSTANT(exports, MS_RDONLY);
  NODE_DEFINE_CONSTANT(exports, MS_REMOUNT);
}
NODE_MODULE(mount, init)

}
