#include <nan.h>
#include "snap.h++"
#include "sync.h++"

using namespace snapsync;

NAN_METHOD(snapCreate) {
  try {
    // get directory parameter
    if(!info[0]->IsString()) {
      Nan::ThrowError("directory not provided");
    }
    boost::filesystem::path directory(*Nan::Utf8String(info[0]->ToString()));

    // get directory parameter
    if(!info[1]->IsString()) {
      Nan::ThrowError("image not provided");
    }
    boost::filesystem::path image(*Nan::Utf8String(info[1]->ToString()));

    // run operation
    snap::create(directory, image);
  }
  catch(const std::exception& e) {
    Nan::ThrowError(e.what());
  }
  catch(...) {
    Nan::ThrowError("an unknown error occured");
  }
}

NAN_METHOD(snapExtract) {
  try {
    // get directory parameter
    if(!info[0]->IsString()) {
      Nan::ThrowError("image not provided");
    }
    boost::filesystem::path image(*Nan::Utf8String(info[0]->ToString()));

    // get directory parameter
    if(!info[1]->IsString()) {
      Nan::ThrowError("directory not provided");
    }
    boost::filesystem::path directory(*Nan::Utf8String(info[1]->ToString()));

    // run operation
    snap::extract(image, directory);
  }
  catch(const std::exception& e) {
    Nan::ThrowError(e.what());
  }
  catch(...) {
    Nan::ThrowError("an unknown error occured");
  }
}

NAN_METHOD(syncSignature) {
  info.GetReturnValue().Set(Nan::New("hello world").ToLocalChecked());
}

NAN_METHOD(syncDelta) {
  info.GetReturnValue().Set(Nan::New("hello world").ToLocalChecked());
}

NAN_METHOD(syncPatch) {
  info.GetReturnValue().Set(Nan::New("hello world").ToLocalChecked());
}

NAN_MODULE_INIT(init) {
  // snap methods
  auto snap = Nan::New<v8::Object>();

  Nan::Set(
    snap,
    Nan::New("create").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(snapCreate)->GetFunction()
  );

  Nan::Set(
    snap,
    Nan::New("extract").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(snapExtract)->GetFunction()
  );

  Nan::Set(
    target,
    Nan::New("snap").ToLocalChecked(),
    snap
  );

  // sync methods
  auto sync = Nan::New<v8::Object>();

  Nan::Set(
    sync,
    Nan::New("signature").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(syncSignature)->GetFunction()
  );

  Nan::Set(
    sync,
    Nan::New("delta").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(syncDelta)->GetFunction()
  );

  Nan::Set(
    sync,
    Nan::New("patch").ToLocalChecked(),
    Nan::New<v8::FunctionTemplate>(syncPatch)->GetFunction()
  );

  Nan::Set(
    target,
    Nan::New("sync").ToLocalChecked(),
    sync
  );
}

NODE_MODULE(snapsync, init)
