#include "ngtk_hsv.h"
#include <node.h>

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


/**
 * Sets the current colour of the component
 *
 * Parameters:
 *   h - The hue component.
 *   s - The saturation component.
 *   v - The value component.
 */
Handle<Value> HSV::SetColor (const Arguments &args) {
  HandleScope scope;

  GtkWidget *hsv = HSV::Data(args.This());
  double h = args[0]->NumberValue();
  double s = args[1]->NumberValue();
  double v = args[2]->NumberValue();

  gtk_hsv_set_color(GTK_HSV(hsv), h, s, v);

  return args.This();
}

/**
 * Gets the currently selected colour of the widget
 *
 * Returns:
 * An array with three values: the current hue, saturation and value selected;
 * in that order
 */
Handle<Value> HSV::GetColor (const Arguments &args) {
  HandleScope scope;

  GtkWidget *hsv = HSV::Data(args.This());
  double h = 0;
  double s = 0;
  double v = 0;

  gtk_hsv_get_color(GTK_HSV(hsv), &h, &s, &v);
  v8::Handle<v8::Array> c = v8::Array::New(3);
  c->Set(0, v8::Number::New(h));
  c->Set(1, v8::Number::New(s));
  c->Set(2, v8::Number::New(v));
  return scope.Close(c);
}

/**
 * Converts a value from RGB to HSV. All colour components should be in the range 0-1.
 *
 * Parameters:
 *   r - The red component.
 *   g - The green component.
 *   b - The blue component.
 *
 * Returns:
 * An array with three values: the hue, saturation and value; in that order
 */
Handle<Value> RgbToHsv (const Arguments &args) {
  HandleScope scope;

  double r = args[0]->NumberValue();
  double g = args[1]->NumberValue();
  double b = args[2]->NumberValue();
  double h, s, v;

  gtk_rgb_to_hsv(r, g, b, &h, &s, &v);

  v8::Handle<v8::Array> c = v8::Array::New(3);
  c->Set(0, v8::Number::New(h));
  c->Set(1, v8::Number::New(s));
  c->Set(2, v8::Number::New(v));
  return scope.Close(c);
}

/**
 * Converts a value from HSV to RGB. All colour components should be in the range 0-1.
 *
 * Parameters:
 *   h - The hue component.
 *   s - The saturation component.
 *   v - The value component.
 *
 * Returns:
 * An array with three values: the red, green and blue components; in that order
 */
Handle<Value> HsvToRgb (const Arguments &args) {
  HandleScope scope;

  double h = args[0]->NumberValue();
  double s = args[1]->NumberValue();
  double v = args[2]->NumberValue();
  double r, g, b;

  gtk_hsv_to_rgb(h, s, v, &r, &g, &b);

  v8::Handle<v8::Array> c = v8::Array::New(3);
  c->Set(0, v8::Number::New(r));
  c->Set(1, v8::Number::New(g));
  c->Set(2, v8::Number::New(b));
  return scope.Close(c);
}

/**
 * Sets up the HSV class in JavaScript
 */
void HSV::SetPrototypeMethods (Handle<FunctionTemplate> constructor_template) {
  HandleScope scope;

  Widget::SetPrototypeMethods(constructor_template);

  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "setColor",       HSV::SetColor);
  NGTK_SET_PROTOTYPE_METHOD(constructor_template, "getColor",       HSV::GetColor);

  NGTK_SET_METHOD(constructor_template, "hsvToRgb", HsvToRgb);
  NGTK_SET_METHOD(constructor_template, "rgbToHsv", RgbToHsv);
}

void HSV::SignalMove (GtkHSV *hsv, GtkDirectionType dir, gpointer callback_ptr) {
  Persistent<Function> *callback = reinterpret_cast<Persistent<Function>*>(callback_ptr);
  TryCatch try_catch;

  Local<Value> argv[1] = { v8::Number::New(dir) };

  Local<Value> ret = (*callback)->Call(Context::GetCurrent()->Global(), 1, argv);

  if (try_catch.HasCaught()) {
    //FatalException(try_catch);
  }
}

void HSV::RegisterCallbacks (std::vector<SignalCallback> *callbacks) {
  Widget::RegisterCallbacks(callbacks);
  (*callbacks).push_back(SignalCallback("move",    G_CALLBACK(HSV::SignalMove)));
  (*callbacks).push_back(SignalCallback("changed", G_CALLBACK(Widget::SignalBare)));
}

void HSV::Initialize (Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> t = FunctionTemplate::New(HSV::New);
  constructor_template = Persistent<FunctionTemplate>::New(t);
  constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
  constructor_template->SetClassName(String::NewSymbol("HSV"));

  HSV::SetPrototypeMethods(constructor_template);
  HSV::RegisterCallbacks(callbacks);

  target->Set(String::NewSymbol("Hsv"), constructor_template->GetFunction());
}

} // namespace ngtk

