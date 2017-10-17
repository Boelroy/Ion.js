#ifndef ION_FILE_SYSTEM
#define ION_FILE_SYSTEM

#include "ion.h"
#include "ChakraCore.h"
#include "iostream"
#include <uv.h>
#include <fcntl.h>

namespace ion{
namespace core{
namespace fs{
    
#define ion_define_constants(object, constant) ion_define_int(object, #constant, constant);

void DefineConstants(napi_value fs) {
	napi_value constants;
	ion_create_object(&constants);
	
	ion_define_constants(constants, O_RDONLY);
	ion_define_constants(constants, O_WRONLY);
	ion_define_constants(constants, O_RDWR);
	ion_define_constants(constants, O_CREAT);
	ion_define_constants(constants, O_EXCL);
#ifdef O_ONCTTY
	ion_define_constants(constants, O_ONCTTY);
#endif
	ion_define_constants(constants, O_TRUNC);
	ion_define_constants(constants, O_APPEND);
#ifdef O_DIRETORY
	ion_define_constants(constants, O_DIRECTORY);
#endif
#ifdef O_NOATIME
	ion_define_constants(constants, O_NOATIME);
#endif
#ifdef O_SYNC
	ion_define_constants(constants, O_SYNC);
#endif
#ifdef O_SYMLINK
	ion_define_constants(constants, O_SYMLINK);
#endif
#ifdef O_NONBLOCK
	ion_define_constants(constants, O_NONBLOCK);
#endif

	ion_define(fs, "constants", constants);
}

void
AfterOpen(uv_fs_t *req) {
	napi_value callback = static_cast<napi_value>(req->data);

	JsValueRef argv[2];

  JsGetUndefinedValue(argv);
	JsIntToNumber(req->result, argv + 1);
	JsValueRef result;

  FAIL_CHECK1(JsCallFunction(callback, argv, 2, &result));
}

napi_value 
Open(napi_value callee, bool isConstructCall, napi_value *arguments, unsigned short argumentCount, void *callbackState){
	uv_fs_t req;
	req.data = arguments[argumentCount - 1];

	int flags, mode;
  JsNumberToInt(arguments[3], &mode);
	JsNumberToInt(arguments[2], &flags);
	
	std::string path = ion_get_string(arguments[1]);

	uv_fs_open(uv_default_loop(), &req, path.data(), flags, mode, AfterOpen);
	JsValueRef undefinedValue;
  JsGetUndefinedValue(&undefinedValue);
	return undefinedValue;
}

napi_value
Close(napi_value callle, bool isConstructCall, napi_value *arguments, unsigned short argumentCount, void *callbackState) {
	JsValueRef undefinedValue;
  JsGetUndefinedValue(&undefinedValue);
	return undefinedValue;
}

napi_value
Read(napi_value callle, bool isConstructCall, napi_value *arguments, unsigned short argumentCount, void *callbackState) {
	int fd;
	std::string src;
	uv_fs_t req;

	void* base = malloc(4096);
	if (base == nullptr) {
		perror("can not malloc");
		exit(0);
	}
	JsNumberToInt(arguments[1], &fd);
	
	uv_buf_t buf = uv_buf_init(static_cast<char*>(base), 4096);
	uv_fs_read(uv_default_loop(), &req, fd, &buf, 1, 0, nullptr);
	while(req.result > 0) {
		src += std::string(static_cast<char*>(buf.base), req.result);
		uv_fs_read(uv_default_loop(), &req, fd, &buf, 1, src.length(), nullptr);
	}
	napi_value str;
	JsCreateString(src.c_str(), src.length(), &str);
	return str;
}

napi_value
Write(napi_value callle, bool isConstructCall, napi_value *arguments, unsigned short argumentCount, void *callbackState) {
	int fd;
	JsNumberToInt(arguments[1], &fd);
	uv_fs_t req;
	int pos;
	JsNumberToInt(arguments[3], &pos);
	std::string content = ion_get_string(arguments[2]);

	uv_buf_t buf = uv_buf_init(const_cast<char*>(content.c_str()), content.length());
	uv_fs_write(uv_default_loop(), &req, fd, &buf, 1, pos, nullptr);
	JsValueRef undefinedValue;
  JsGetUndefinedValue(&undefinedValue);
	return undefinedValue;
}

void DefineMethod(napi_value fs) {
	ION_SET_METHOD(fs, "open", Open);
	ION_SET_METHOD(fs, "close", Close);
	ION_SET_METHOD(fs, "read", Read);
	ION_SET_METHOD(fs, "write", Write);
}
void Init(napi_value env) {
	napi_value fs;
	ion_create_object(&fs);

	DefineConstants(fs);
	DefineMethod(fs);

	ion_define(env, "fs", fs);
}

}}}

#endif