#ifndef ION_REQ_WRAP
#define ION_REQ_WRAP

#include "env.h"

namespace ion{
namespace core{
namespace fs{

template<typename T>
class ReqWrap {
public:
  ReqWrap(){
    this->req_ = new T;
    this->env_ = env::Environment::GetCurrent();
  }

  inline ~ReqWrap() {
    delete req_;
  }
  
  T* req(){ return req_; };
  env::Environment *env(){ return env_; };

protected:
  T *req_;
  env::Environment* env_;
};

}}}

#endif // !ION_REQ_WRAP