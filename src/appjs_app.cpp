#include <node.h>
#include "appjs.h"
#include "appjs_app.h"
#include "appjs_window.h"
#include "includes/util.h"

namespace appjs {

using namespace v8;

App::App() {};
App::~App() {};

bool App::initialized_ = false;
Persistent<Function> App::constructor;

void App::Init () {

  CREATE_NODE_CONSTRUCTOR("App");
  DEFINE_PROTOTYPE_METHOD("on",On);
  DEFINE_PROTOTYPE_METHOD("createWindow",CreateWindow);
  
  END_CONSTRUCTOR();
}

Handle<Value> App::New(const Arguments& args) {
  HandleScope scope;

  App* obj = new App();
 
  obj->Wrap(args.This());

  return scope.Close(args.This());
}

Handle<Value> App::NewInstance(const Arguments& args) {
  HandleScope scope;

  const unsigned argc = 0;
  Handle<Value> argv[argc] = NULL;
  Local<Object> instance = constructor->NewInstance(argc,argv);

  // get the events.EventEmitter constructor
  Local<Object> global = Context::GetCurrent()->Global();
  Handle<Object> process = global->Get(String::NewSymbol("process"))->ToObject();
  Local<Function> emitterConstructor = Local<Function>::Cast(process->Get(String::NewSymbol("EventEmitter")));

  // create process.AppjsEmitter object
  Handle<Object> AppjsEmitter = emitterConstructor->NewInstance();
  process->Set(String::NewSymbol("AppjsEmitter"),AppjsEmitter);

  return scope.Close(instance);
}

Handle<Value> App::CreateWindow(const Arguments& args) {

  HandleScope scope;

  return scope.Close(Window::NewInstance(args));
}

Handle<Value> App::On(const Arguments& args) {
  HandleScope scope;

  Local<Object> global = Context::GetCurrent()->Global();
  Local<Object> process = global->Get(String::NewSymbol("process"))->ToObject();
  Local<Object> Emitter = Local<Object>::Cast(process->Get(String::NewSymbol("AppjsEmitter")));
  Local<Function> On = Local<Function>::Cast(Emitter->Get(String::NewSymbol("on")));

  const unsigned argc = 2;
  Handle<Value> argv[argc] = { args[0] , args[1] };

  On->Call(Emitter,argc,argv);

  return scope.Close(args.This());
}

} /* appjs */
