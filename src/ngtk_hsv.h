#ifndef NGTK_HSV_H_
#define NGTK_HSV_H_

#include "ngtk_widget.h"

namespace ngtk {

class HSV : public Widget {
public:
  static void   SetPrototypeMethods (v8::Handle<v8::FunctionTemplate> constructor_template);
  static void   Initialize          (v8::Handle<v8::Object> target);
  static HSV*   New                 (void); // public constructor
  static bool   HasInstance         (v8::Handle<v8::Value> val);

private:
  static v8::Persistent<v8::FunctionTemplate> constructor_template;

  static v8::Handle<v8::Value> New         (const v8::Arguments &args);
  static v8::Handle<v8::Value> SetColor    (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetColor    (const v8::Arguments &args);
  /*
  static v8::Handle<v8::Value> SetMetrics  (const v8::Arguments &args);
  static v8::Handle<v8::Value> GetMetrics  (const v8::Arguments &args);
  static v8::Handle<v8::Value> IsAdjusting (const v8::Arguments &args);
  static v8::Handle<v8::Value> ToRGB       (const v8::Arguments &args);
  static v8::Handle<v8::Value> FromRGB     (const v8::Arguments &args);
  */

  HSV(void);
};

} // namespace ngtk

#endif
