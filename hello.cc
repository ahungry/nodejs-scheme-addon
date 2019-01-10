// hello.cc
// https://nodejs.org/api/addons.html#addons_c_addons
#include <node.h>

#include <string>
#include <iostream>
#include <libguile.h>

namespace demo {

  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::NewStringType;
  using v8::Object;
  using v8::String;
  using v8::Value;
  using v8::Exception;
  using v8::Number;

  static SCM
  my_sum (SCM a, SCM b)
  {
    const double x = scm_to_double (a);
    const double y = scm_to_double (b);

    return scm_from_double (x + y);
  }

  static void*
  guile_eval (void* data)
  {
    // https://www.gnu.org/software/guile/manual/html_node/Fly-Evaluation.html
    // const char* eval = ((char*) data);
    const char* eval = *((char**) data);

    std::cout << "Evaluation target: " << eval << std::endl;

    // scm_c_define_gsubr ("my-fn", 1, 5, 0, (void*) &my_fn);
    scm_c_define_gsubr ("my-sum", 2, 0, 0, (void*) &my_sum);

    return scm_c_eval_string (eval);
  }

  void
  Eval (const FunctionCallbackInfo<Value>& args)
  {
    Isolate* isolate = args.GetIsolate();

    if (args.Length () < 1)
      {
        // Throw an Error that is passed back to JavaScript
        isolate->ThrowException
          (Exception::TypeError
           (String::NewFromUtf8
            (isolate,
             "Wrong number of arguments",
             NewStringType::kNormal).ToLocalChecked ()));
        return;
      }

    // Check the argument types
    if (!args[0]->IsString ())
      {
        isolate->ThrowException
          (Exception::TypeError
           (String::NewFromUtf8
            (isolate,
             "Wrong argument type - expected a string.",
             NewStringType::kNormal).ToLocalChecked ()));

        return;
      }

    // https://nodeaddons.com/
    // wrap the string with v8's string type
    v8::String::Utf8Value val(args[0]->ToString());

    // use it as a standard C++ string
    std::string str (*val);

    std::cout << "Received input: " << str << std::endl;

    // Perform the operation
    // const char* eval = "(number->string (+ 1 2 3))";
    const char* eval = str.c_str ();

    std::cout << "About to eval: " << eval << std::endl;

    SCM g = (SCM) scm_with_guile (&guile_eval, &eval);
    char* result = scm_to_stringn (g, NULL, "ascii", SCM_FAILED_CONVERSION_ESCAPE_SEQUENCE);

    // double value = args[0].As<Number> ()->Value () + args[1].As<Number> ()->Value ();
    // Local<Number> num = Number::New (isolate, value);

    // Set the return value (using the passed in
    // FunctionCallbackInfo<Value>&)
    // args.GetReturnValue ().Set (num);

    args.GetReturnValue ()
      .Set (String::NewFromUtf8
           (isolate, result, NewStringType::kNormal).ToLocalChecked ());
  }

  void
  Method (const FunctionCallbackInfo<Value>& args)
  {
    Isolate* isolate = args.GetIsolate();
    args.GetReturnValue ()
      .Set (String::NewFromUtf8
           (isolate, "world", NewStringType::kNormal).ToLocalChecked ());
  }

  void
  Add (const FunctionCallbackInfo<Value>& args)
  {
    Isolate* isolate = args.GetIsolate();

    if (args.Length () < 2)
      {
        // Throw an Error that is passed back to JavaScript
        isolate->ThrowException
          (Exception::TypeError
           (String::NewFromUtf8
            (isolate,
             "Wrong number of arguments",
             NewStringType::kNormal).ToLocalChecked ()));
        return;
      }

    // Check the argument types
    if (!args[0]->IsNumber () || !args[1]->IsNumber ())
      {
        isolate->ThrowException
          (Exception::TypeError
           (String::NewFromUtf8
            (isolate,
             "Wrong arguments",
             NewStringType::kNormal).ToLocalChecked ()));

        return;
      }

    // Perform the operation
    double value = args[0].As<Number> ()->Value () + args[1].As<Number> ()->Value ();
    Local<Number> num = Number::New (isolate, value);

    // Set the return value (using the passed in
    // FunctionCallbackInfo<Value>&)
    args.GetReturnValue ().Set (num);
  }

  void
  Initialize (Local<Object> exports)
  {
    NODE_SET_METHOD (exports, "hello", Method);
    NODE_SET_METHOD (exports, "add", Add);
    NODE_SET_METHOD (exports, "eval", Eval);
  }

  NODE_MODULE (NODE_GYP_MODULE_NAME, Initialize)

}  // namespace demo
