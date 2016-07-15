#include <nan.h>
#include "snap.h++"
#include "sync.h++"

using namespace snapsync;

NAN_METHOD(snapCreate) {
  try {
    // get directory parameter
    if(info[0]->IsUndefined()) {
      Nan::ThrowError("directory not provided");
    }
    boost::filesystem::path directory(*Nan::Utf8String(info[0]->ToString()));

    // get image parameter
    if(info[1]->IsUndefined()) {
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
    // get image parameter
    if(info[0]->IsUndefined()) {
      Nan::ThrowError("image not provided");
    }
    boost::filesystem::path image(*Nan::Utf8String(info[0]->ToString()));

    // get directory parameter
    if(info[1]->IsUndefined()) {
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
  try {
    // get basePath parameter
    if(info[0]->IsUndefined()) {
      Nan::ThrowError("basePath not provided");
    }
    boost::filesystem::path basePath(*Nan::Utf8String(info[0]->ToString()));

    // get signaturePath parameter
    if(info[1]->IsUndefined()) {
      Nan::ThrowError("signaturePath not provided");
    }
    boost::filesystem::path signaturePath(*Nan::Utf8String(info[1]->ToString()));

    // get blockLength parameter
    size_t blockLength = 0;
    if(!info[2]->IsUndefined()) {
      blockLength = Nan::To<uint32_t>(info[2]).FromMaybe(0);
    }

    // get blockLength parameter
    size_t sumLength = 0;
    if(!info[3]->IsUndefined()) {
      sumLength = Nan::To<uint32_t>(info[3]).FromMaybe(0);
    }

    // run operation
    sync::signature(basePath, signaturePath, blockLength, sumLength);
  }
  catch(const std::exception& e) {
    Nan::ThrowError(e.what());
  }
  catch(...) {
    Nan::ThrowError("an unknown error occured");
  }
}

NAN_METHOD(syncDelta) {
  try {
    // get signaturePath parameter
    if(info[0]->IsUndefined()) {
      Nan::ThrowError("signaturePath not provided");
    }
    boost::filesystem::path signaturePath(*Nan::Utf8String(info[0]->ToString()));

    // get targetPath parameter
    if(info[1]->IsUndefined()) {
      Nan::ThrowError("targetPath not provided");
    }
    boost::filesystem::path targetPath(*Nan::Utf8String(info[1]->ToString()));

    // get patchPath parameter
    if(info[2]->IsUndefined()) {
      Nan::ThrowError("patchPath not provided");
    }
    boost::filesystem::path patchPath(*Nan::Utf8String(info[2]->ToString()));

    // run operation
    sync::delta(signaturePath, targetPath, patchPath);
  }
  catch(const std::exception& e) {
    Nan::ThrowError(e.what());
  }
  catch(...) {
    Nan::ThrowError("an unknown error occured");
  }
}

NAN_METHOD(syncPatch) {
  try {
    // get patchPath parameter
    if(info[0]->IsUndefined()) {
      Nan::ThrowError("patchPath not provided");
    }
    boost::filesystem::path patchPath(*Nan::Utf8String(info[0]->ToString()));

    // get basePath parameter
    if(info[1]->IsUndefined()) {
      Nan::ThrowError("basePath not provided");
    }
    boost::filesystem::path basePath(*Nan::Utf8String(info[1]->ToString()));

    // get targetPath parameter
    if(info[2]->IsUndefined()) {
      Nan::ThrowError("targetPath not provided");
    }
    boost::filesystem::path targetPath(*Nan::Utf8String(info[2]->ToString()));

    // run operation
    sync::patch(patchPath, basePath, targetPath);
  }
  catch(const std::exception& e) {
    Nan::ThrowError(e.what());
  }
  catch(...) {
    Nan::ThrowError("an unknown error occured");
  }
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
