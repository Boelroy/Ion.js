#ifndef ION_FILE_SYSTEM
#define ION_FILE_SYSTEM

#include "ion.h"
#include <uv.h>
#include <fcntl.h>

namespace ion{
namespace core{
namespace fs{
    
#define ion_define_constants(object, constant) ion_define_int(object, #constant, constant);

	void DefineConstants(napi_value fs) {
		napi_value constants;
		ion_create_object(&fs);
		
		ion_define_constants(fs, O_RDONLY);
		ion_define_constants(fs, O_WRONLY);
		ion_define_constants(fs, O_RDWR);
		ion_define_constants(fs, O_CREAT);
		ion_define_constants(fs, O_EXCL);
#ifdef O_ONCTTY
		ion_define_constants(fs, O_ONCTTY);
#endif
		ion_define_constants(fs, O_TRUNC);
		ion_define_constants(fs, O_APPEND);
#ifdef O_DIRETORY
		ion_define_constants(fs, O_DIRECTORY);
#endif
#ifdef O_NOATIME
		ion_define_constants(fs, O_NOATIME);
#endif
#ifdef O_SYNC
		ion_define_constants(fs, O_SYNC);
#endif
#ifdef O_SYMLINK
		ion_define_constants(fs, O_SYMLINK);
#endif
#ifdef O_NONBLOCK
		ion_define_constants(fs, O_NONBLOCK);
#endif
	}

	void DefineMethod(napi_value fs) {

	}

	napi_value Open(napi_value callle, bool isConstructCall, napi_value *argument, unsigned short* argumentCount, void *callbackState) {
		
		return JS_INVALID_REFERENCE;
	}

	napi_value Close(napi_value callle, bool isConstructCall, napi_value *argument, unsigned short* argumentCount, void *callbackState) {
		return JS_INVALID_REFERENCE;
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