#   -target amd64-linux-none-gnueabi \

clang++ \
  -shared -Ofast -flto  \
  Untitled-1.cpp \
  -I ~/ossia/score/3rdparty/avendish/examples \
  -I ~/ossia/score/3rdparty/avendish/include \
  -I ~/ossia/score/3rdparty/libossia/src/ \
  -I $PWD \
  -std=c++20 \
  -isystem ~/ossia/score/3rdparty/libossia/3rdparty/unordered_dense/include \
  -isystem ~/ossia/score/3rdparty/libossia/3rdparty/SmallFunction/smallfun/include/ \
  -isystem ~/ossia/score/3rdparty/libossia/3rdparty/span/include \
  -isystem ~/ossia/score/3rdparty/libossia/3rdparty/nano-signal-slot/include/ \
  -isystem ~/ossia/score/3rdparty/libossia/3rdparty/rnd/include \
  -ferror-limit=3 \
  -DOSSIA_CALLBACK_CONTAINER_MUTEX=std::mutex \
  -fno-plt -fno-ident \
  -Bsymbolic \
  -fno-exceptions \
  -fno-rtti \
  -fno-unwind-tables \
  -fvisibility=internal \
  -fno-stack-protector  \
   -o x.so  \
  -fno-pic \
  -s \
   -fPIC
