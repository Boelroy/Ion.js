#ifndef ION_FILE_SYSTEM
#define ION_FILE_SYSTEM

#include "ion.h"
#include "ChakraCore.h"
#include "iostream"
#include <uv.h>
#include <vector>
#include <fcntl.h>
#include <env.h>
#include <cstring>
#include <req-wrap.h>

namespace ion{
namespace core{
namespace fs{

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static char getbuf[PATH_MAX + 1];


class FSReqWrap: public ReqWrap<uv_fs_t> {
public:
	FSReqWrap(napi_value callback):
		ReqWrap(),
		callback_(callback) {}
	~FSReqWrap() {
		uv_fs_req_cleanup(req_);
	}
	napi_value callback() {
		return callback_;
	}
private:
	napi_value callback_;
};


#define ion_define_constants(object, constant) ion_define_int(object, #constant, constant);

static uv_loop_t *event_loop;

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


napi_value GetStatObject(uv_stat_t stat, napi_value *js_stat) {
	ion_create_object(js_stat);

	ion_define_int(*js_stat, "dev", stat.st_dev);
	ion_define_int(*js_stat, "mode", stat.st_mode);
	ion_define_int(*js_stat, "nlink", stat.st_nlink);
	ion_define_int(*js_stat, "uid", stat.st_uid);
	ion_define_int(*js_stat, "gid", stat.st_gid);
	ion_define_int(*js_stat, "rdev", stat.st_rdev);
	ion_define_int(*js_stat, "ino", stat.st_ino);
	ion_define_int(*js_stat, "size", stat.st_size);
	ion_define_int(*js_stat, "blksize", stat.st_blksize);
	ion_define_int(*js_stat, "blocks", stat.st_blocks);
	#define X(name, field_name) \
	ion_define_unsigned_long(*js_stat, name, ((unsigned long)(stat.st_##field_name.tv_sec)) * 1e3 + \
																((unsigned long)(stat.st_##field_name.tv_nsec)) / 1e6); \

	X("atime", atim);
	X("ctime", ctim);
	X("birthtime", birthtim);
	#undef X

	return js_stat;
}

void After(uv_fs_t *req) {
	FSReqWrap *fs_req_wrap = static_cast<FSReqWrap*>(req->data);
	napi_value callback = fs_req_wrap->callback();
	JsValueRef argv[6];
	int argc = 1;
	if (req->result >= 0) {
		switch(req->fs_type) {
			case UV_FS_CLOSE:
				break;
			case UV_FS_OPEN:
				JsIntToNumber(req->result, argv + 1);
				argc = 2;
				break;
			case UV_FS_READ:
				break;
			case UV_FS_STAT:
				std::cout << "in fs stat" << std::endl;
				GetStatObject(req->statbuf, argv + 1);
				argc = 2;
				break;
			default:
			perror("unhandle uv ops");
		}
	} else {

	}
	
	JsGetUndefinedValue(argv);
	JsValueRef result;
	FAIL_CHECK1(JsCallFunction(callback, argv, argc, &result));

	delete fs_req_wrap;
}

ION_FUNCTION(Open) {
	napi_value callback = arguments[argumentCount - 1];
	FSReqWrap *reqwrap = new FSReqWrap(callback);
	JsValueRef undefinedValue;
	JsGetUndefinedValue(&undefinedValue);

	reqwrap->req()->data = reqwrap;

	int flags, mode;
  JsNumberToInt(arguments[3], &mode);
	JsNumberToInt(arguments[2], &flags);

	strcpy(getbuf, ion_get_string(arguments[1]).c_str());
	bool result;
	JsEquals(callback, undefinedValue, &result);
	if (!result) {
		uv_fs_open(reqwrap->env()->event_loop(), reqwrap->req(), getbuf, flags, mode, After);
		return undefinedValue;
	}
	int error = uv_fs_open(reqwrap->env()->event_loop(), reqwrap->req(), getbuf, flags, mode, nullptr);
	int fd_ = error;
	if (error == 0) {
		fd_ = reqwrap->req()->result;
	}
	delete reqwrap;
	napi_value fd;
	JsIntToNumber(fd_, &fd);
	return fd;
}

ION_FUNCTION(Close){
	int fd;
	napi_value callback = arguments[argumentCount - 1];
	JsNumberToInt(arguments[1], &fd);
	JsValueRef undefinedValue;
	JsGetUndefinedValue(&undefinedValue);

	FSReqWrap *reqwrap = new FSReqWrap(callback);
	reqwrap->req()->data = reqwrap;

	bool result;
	JsEquals(callback, undefinedValue, &result);
	if (!result) {
		uv_fs_close(reqwrap->env()->event_loop(), reqwrap->req(), fd, After);
	} else {
		uv_fs_close(reqwrap->env()->event_loop(), reqwrap->req(), fd, nullptr);
	}

	return undefinedValue;
}

ION_FUNCTION(Stat) {
	napi_value callback = arguments[argumentCount - 1];
	FSReqWrap *reqwrap = new FSReqWrap(callback);
	reqwrap->req()->data = reqwrap;

	JsValueRef undefinedValue;
	JsGetUndefinedValue(&undefinedValue);

	bool result;
	JsEquals(callback, undefinedValue, &result);

	strcpy(getbuf, ion_get_string(arguments[1]).c_str());

	if (!result) {
		uv_fs_stat(reqwrap->env()->event_loop(), reqwrap->req(), getbuf, After);
		return undefinedValue;
	}
	int error = uv_fs_stat(reqwrap->env()->event_loop(), reqwrap->req(), getbuf, nullptr);

	napi_value stat;
	if (error == 0) {
		 GetStatObject(reqwrap->req()->statbuf, &stat);
	}

	return stat;

}

ION_FUNCTION(Read){
	int fd;
	JsNumberToInt(arguments[1], &fd);

	std::vector<char> chars;
	int64_t offset = 0;
	const size_t kBlockSize = 4096;
	ssize_t numchars;

	do {
		const size_t start = chars.size();
		chars.resize(start + kBlockSize);
		uv_buf_t buf;
		buf.base = &chars[start];
		buf.len = kBlockSize;

		uv_fs_t read_req;
		numchars = uv_fs_read(uv_default_loop(), &read_req, fd, &buf, 1, offset, nullptr);
		offset+=numchars;
	} while(static_cast<size_t>(numchars) == kBlockSize);
	std::string result = std::string(&chars[0]);
	napi_value str;
	JsCreateString(result.data(), result.length(), &str);

	return str;
}

ION_FUNCTION(Write){
	int fd;
	JsNumberToInt(arguments[1], &fd);
	uv_fs_t req;
	int pos;
	JsNumberToInt(arguments[3], &pos);
	std::string content = ion_get_string(arguments[2]);

	uv_buf_t buf = uv_buf_init(const_cast<char*>(content.data()), content.length());
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
	ION_SET_METHOD(fs, "stat", Stat);
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
