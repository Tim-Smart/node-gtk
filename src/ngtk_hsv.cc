#include "ngtk_hsv.h"

namespace ngtk {

using namespace v8;

Persistent<FunctionTemplate> HSV::constructor_template;

// Check whether is an instance.
bool HSV::HasInstance (Handle<Value> val) {
  HandleScope scope;

  if (val->IsObject()) {
    Local<Object> obj = val->ToObject();

    if (constructor_template->HasInstance(obj)) {
      return true;
    }
  }

  return false;
}

// Public constructor
HSV* HSV::New (void) {
  HandleScope scope;

  Local<Object> dialog = constructor_template->GetFunction()->NewInstance();

  return ObjectWrap::Unwrap<HSV>(dialog);
}

// ECMAScript constructor.
Handle<Value> HSV::New (const Arguments &args) {
  HandleScope scope;

  HSV *hsv = new HSV();
  hsv->Wrap(args.This());

  return args.This();
}

HSV::HSV (void) {
  widget_ = gtk_hsv_new();
}

// gtk_hsv_set_color()
Handle<Value> HSV::SetColor (const Arguments &args) {
  HandleScope scope;

  GtkWidget *hsv = HSV::Data(args.This());
  double r = args[0]->NumberValue();
  double g = args[1]->NumberValue();
  double b = args[2]->NumberValue();

  gtk_hsv_set_color(GTK_HSV(hsv), r, g, b);

  return args.This();
}

// gtk_hsv_set_color()
Handle<Value> HSV::GetColor (const Arguments &args) {
  HandleScope scope;

  GtkWidget *hsv = HSV::Data(args.This());
  double r = 0;
  double g = 0;
  double b = 0;

  gtk_hsv_get_color(GTK_HSV(hsv), &r, &g, &b);
  v8::Handle<v8::Array> c = v8::Array::New(3);
  c->Set(0, v8::Number::New(r));
  c->Set(1, v8::Number::New(g));
  c->Set(2, v8::Number::New(b));
  return scope.Close(c);
}

// Export.
void HSV::SetPrototypeMethods (Handle<FunctionTemplate> constructor_template) {
  HandleScope scope;

  Widget::SetPrototypeMethods(constructor_template);

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setColor",       HSV::SetColor);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getColor",       HSV::GetColor);
}

void HSV::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(HSV::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("HSV"));

  HSV::SetPrototypeMethods(constructor_template);

  target->Set(String::NewSymbol("HSV"), constructor_template->GetFunction());
}

} // namespace ngtk

