// hello.cc
// https://nodejs.org/api/addons.html#addons_c_addons
#include <node.h>
#include <nan.h>

#include <string>
#include <iostream>
#include <libguile.h>

namespace demo {

  using v8::Context;
  using v8::Exception;
  using v8::Function;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::NewStringType;
  using v8::Null;
  using v8::Number;
  using v8::Object;
  using v8::String;
  using v8::Value;

  static SCM
  my_sum (SCM a, SCM b)
  {
    const double x = scm_to_double (a);
    const double y = scm_to_double (b);

    return scm_from_double (x + y);
  }

  static Nan::CopyablePersistentTraits<v8::Function>::CopyablePersistent _cb;
  // Isolate* isolatePtr;
  // Local<Context> contextPtr;
  // Local<Function> cbPtr;

  // https://stackoverflow.com/questions/38455816/calling-javascript-function-from-c-addon#43174441
  static SCM
  my_fn (SCM name, SCM s1)
  {
    char* namen = scm_to_stringn (name, NULL, "ascii", SCM_FAILED_CONVERSION_ESCAPE_SEQUENCE);
    char* cs1 = scm_to_stringn (s1, NULL, "ascii", SCM_FAILED_CONVERSION_ESCAPE_SEQUENCE);

    std::cout << "Call my-fn as: " << namen << cs1 << std::endl;

    // Isolate* isolate = Isolate::GetCurrent ();// = argPtr.GetIsolate ();
    // Local<Context> context = isolate->GetCurrentContext ();

    // const unsigned argc = 1;
    // Local<Value> argv[argc] =
    //   {
    //    String::NewFromUtf8(isolate,
    //                        "LOL",
    //                        NewStringType::kNormal).ToLocalChecked () };
    // cbPtr->Call (context, Null (isolate), argc, argv);

    // Deprecated note, bla
    Nan::MakeCallback (Nan::GetCurrentContext ()->Global (), Nan::New(_cb), 0, 0);
    _cb.Reset ();

    return scm_from_utf8_string ("Good");
  }

  static void*
  guile_eval (void* data)
  {
    // https://www.gnu.org/software/guile/manual/html_node/Fly-Evaluation.html
    // const char* eval = ((char*) data);
    const char* eval = *((char**) data);

    std::cout << "Evaluation target: " << eval << std::endl;

    scm_c_define_gsubr ("my-fn", 1, 5, 0, (void*) &my_fn);
    scm_c_define_gsubr ("my-sum", 2, 0, 0, (void*) &my_sum);

    return scm_c_eval_string (eval);
  }

  // RegisterCallback(const FunctionCallbackInfo<Value>& args)
  static void
  RegisterCallback(const Nan::FunctionCallbackInfo<v8::Value>& info)
  {
    _cb = Nan::Persistent<v8::Function>(info[0].As<v8::Function>());
    // Isolate* isolate = args.GetIsolate ();
    // Local<Context> context = isolate->GetCurrentContext ();
    // Local<Function> cb = Local<Function>::Cast (args[0]);
    // const unsigned argc = 1;
    // Local<Value> argv[argc] =
    //   {
    //    String::NewFromUtf8(isolate,
    //                        "hello world",
    //                        NewStringType::kNormal).ToLocalChecked () };
    // May want to just shove this in a lambda for later
    // https://en.cppreference.com/w/cpp/language/lambda
    // cb->Call (context, Null (isolate), argc, argv).ToLocalChecked ();
  }

  void
  RunCallback(const Nan::FunctionCallbackInfo<Value>& args)
  {
    Isolate* isolate = args.GetIsolate ();
    Local<Context> context = isolate->GetCurrentContext ();
    Local<Function> cb = Local<Function>::Cast (args[0]);
    const unsigned argc = 1;
    Local<Value> argv[argc] =
      {
       String::NewFromUtf8(isolate,
                           "hello world",
                           NewStringType::kNormal).ToLocalChecked () };
    cb->Call (context, Null (isolate), argc, argv).ToLocalChecked ();
  }

  void
  Eval (const Nan::FunctionCallbackInfo<Value>& args)
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
  Method (const Nan::FunctionCallbackInfo<Value>& args)
  {
    Isolate* isolate = args.GetIsolate();
    args.GetReturnValue ()
      .Set (String::NewFromUtf8
           (isolate, "world", NewStringType::kNormal).ToLocalChecked ());
  }

  void
  Add (const Nan::FunctionCallbackInfo<Value>& args)
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

  // void
  // Initialize (Local<Object> exports)
  // {
  //   NODE_SET_METHOD (exports, "hello", Method);
  //   NODE_SET_METHOD (exports, "add", Add);
  //   NODE_SET_METHOD (exports, "scm_eval", Eval);
  //   NODE_SET_METHOD (exports, "cb", RunCallback);
  //   NODE_SET_METHOD (exports, "reg", RegisterCallback);
  // }

  static void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module)
  {
    Nan::SetMethod (exports, "hello", Method);
    Nan::SetMethod (exports, "add", Add);
    Nan::SetMethod (exports, "scm_eval", Eval);
    Nan::SetMethod (exports, "cb", RunCallback);
    Nan::SetMethod (exports, "reg", RegisterCallback);
  }

  NODE_MODULE (addon, Init)
  // NODE_MODULE (NODE_GYP_MODULE_NAME, Initialize)

}  // namespace demo
