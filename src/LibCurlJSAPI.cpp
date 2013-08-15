/**
 * @module builtin/curl
 * 
 * ### Synopsis
 * SilkJS builtin curl object.
 * 
 * ### Description
 * The builtin/curl object provides an interface to the curl library.
 * 
 * ### Usage
 * var curl = require('builtin/curl');
 * 
 * ### See Also
 * The JavaScriptimplementation of a more robust console object:
 * modules/console.js
 */
#include "SilkJS.h"
#include "LibCurlJSAPI.h"
#include <curl/curl.h>

struct CHANDLE {
    CURL *curl;
    char *memory;
    size_t size;
    char *headers;
    size_t hsize;
    curl_slist *slist;
    curl_httppost *post;
    curl_httppost *last;
};

static inline CHANDLE *XHANDLE (Handle<Value>v) {
    if (v->IsNull()) {
        ThrowException(String::New("Handle is NULL"));
        return NULL;
    }
    CHANDLE *w = (CHANDLE *) JSOPAQUE(v);
    return w;
}

static size_t WriteMemoryCallback (void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct CHANDLE *w = (CHANDLE *) userp;
    w->memory = (char *) realloc(w->memory, w->size + realsize + 1);
    if (w->memory == NULL) {
        return -1;
    }
    memcpy(&w->memory[w->size], contents, realsize);
    w->size += realsize;
    w->memory[w->size] = '\0';
    return realsize;
}

static size_t WriteHeaderCallback (void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *s = (char *)contents;
    if (s[0] == '\n') {
        return realsize;
    }
    struct CHANDLE *w = (CHANDLE *) userp;
    w->headers = (char *) realloc(w->headers, w->hsize + realsize + 1);
    if (w->headers == NULL) {
        return -1;
    }
    memcpy(&w->headers[w->hsize], contents, realsize);
    w->hsize += realsize;
    w->headers[w->hsize] = '\0';
    return realsize;
}

void LibCurlJSAPI::Error (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CURLcode eNumber = (CURLcode) args[0]->IntegerValue();
	args.GetReturnValue().Set(v8::String::New(curl_easy_strerror(eNumber)));
}

void LibCurlJSAPI::Initialize (const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::String::Utf8Value url(args[0]->ToString());
    CURL *curl = curl_easy_init();
    if (!curl) {
        args.GetReturnValue().Set(v8::String::New("Could not initialize CURL library"));
		return;
    }
    struct CHANDLE *w = new CHANDLE;
    w->curl = curl;
    w->memory = (char *) malloc(1);
    w->size = 0;
    w->headers = (char *) malloc(1);
    w->hsize = 0;
    w->slist = NULL;
    w->post = NULL;
    w->last = NULL;
    curl_easy_setopt(curl, CURLOPT_URL, *url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) w);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, WriteHeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *) w);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "SilkJS/1.0");
    args.GetReturnValue().Set(Opaque::New(w));
}

void LibCurlJSAPI::SetMethod (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    v8::String::Utf8Value method(args[1]->ToString());
	if (!lstrcmpi((LPCWSTR) *method, (LPCWSTR) "post")) {
        args.GetReturnValue().Set(v8::Integer::New(curl_easy_setopt(h->curl, CURLOPT_POST, 1)));
    }
    else if (!lstrcmpi((LPCWSTR) *method, (LPCWSTR) "get")) {
        args.GetReturnValue().Set(Integer::New(curl_easy_setopt(h->curl, CURLOPT_HTTPGET, 1)));
    }
    else {
        args.GetReturnValue().Set(v8::Integer::New(curl_easy_setopt(h->curl, CURLOPT_CUSTOMREQUEST, *method)));
    }
}

void LibCurlJSAPI::FollowRedirects (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    long flag = (long) args[0]->IntegerValue();
	args.GetReturnValue().Set(v8::Integer::New(curl_easy_setopt(h->curl, CURLOPT_FOLLOWLOCATION, flag)));
}

void LibCurlJSAPI::SetCookie (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    v8::String::Utf8Value cookie_string(args[1]->ToString());
    args.GetReturnValue().Set(v8::Integer::New(curl_easy_setopt(h->curl, CURLOPT_COOKIE, *cookie_string)));
}

void LibCurlJSAPI::SetHeader (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    v8::String::Utf8Value header_string(args[1]->ToString());
    h->slist = curl_slist_append(h->slist, *header_string);
    args.GetReturnValue().Set(v8::Undefined());
}

void LibCurlJSAPI::AddFormField(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    v8::String::Utf8Value name(args[1]->ToString());
    v8::String::Utf8Value value(args[2]->ToString());
    char *contentType = NULL;
    if (args.Length() > 3) {
        v8::String::Utf8Value type(args[3]->ToString());
        contentType = *type;
    }
    if (contentType) {
         args.GetReturnValue().Set(v8::Integer::New(curl_formadd(&h->post, &h->last, CURLFORM_COPYNAME, *name, CURLFORM_COPYCONTENTS, *value, CURLFORM_CONTENTTYPE, contentType, CURLFORM_END)));
    }
    else {
         args.GetReturnValue().Set(v8::Integer::New(curl_formadd(&h->post, &h->last, CURLFORM_COPYNAME, *name, CURLFORM_COPYCONTENTS, *value, CURLFORM_END)));
    }
}

void LibCurlJSAPI::AddFormFile(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    v8::String::Utf8Value name(args[1]->ToString());
    v8::String::Utf8Value filename(args[2]->ToString());
    char *contentType = NULL;
    if (args.Length() > 3) {
        v8::String::Utf8Value type(args[3]->ToString());
        contentType = *type;
    }
    if (contentType) {
        args.GetReturnValue().Set(v8::Integer::New(curl_formadd(&h->post, &h->last, CURLFORM_COPYNAME, *name, CURLFORM_FILE, *filename, CURLFORM_CONTENTTYPE, contentType, CURLFORM_END)));
    }
    else {
        args.GetReturnValue().Set(Integer::New(curl_formadd(&h->post, &h->last, CURLFORM_COPYNAME, *name, CURLFORM_FILE, *filename, CURLFORM_END)));
    }
}

void LibCurlJSAPI::SetPostFields (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    v8::String::Utf8Value post_fields(args[1]->ToString());
    //    printf("%d %s\n", strlen(*post_fields), *post_fields);
    curl_easy_setopt(h->curl, CURLOPT_POSTFIELDSIZE, strlen(*post_fields));
    args.GetReturnValue().Set(v8::Integer::New(curl_easy_setopt(h->curl, CURLOPT_COPYPOSTFIELDS, *post_fields)));
}

void LibCurlJSAPI::SetTimeout (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    long timeout = args[1]->IntegerValue();
    curl_easy_setopt(h->curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(h->curl, CURLOPT_TIMEOUT, timeout);
    args.GetReturnValue().Set(v8::Undefined());
}

void LibCurlJSAPI::Perform (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    if (h->slist) {
        curl_easy_setopt(h->curl, CURLOPT_HTTPHEADER, h->slist);
    }
    if (h->post) {
        curl_easy_setopt(h->curl, CURLOPT_HTTPPOST, h->post);
    }
    if (args.Length() > 1) {
        curl_easy_setopt(h->curl, CURLOPT_VERBOSE, args[1]->IntegerValue());
    }
    args.GetReturnValue().Set(v8::Integer::New(curl_easy_perform(h->curl)));
}

void LibCurlJSAPI::GetResponseCode (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    long status;
    curl_easy_getinfo(h->curl, CURLINFO_RESPONSE_CODE, &status);
    args.GetReturnValue().Set(v8::Integer::New(status));
}

void LibCurlJSAPI::GetContentType (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    char *contentType;
    curl_easy_getinfo(h->curl, CURLINFO_CONTENT_TYPE, &contentType);
    if (!contentType) {
        contentType = "";
    }
    args.GetReturnValue().Set(v8::String::New(contentType));
}

void LibCurlJSAPI::GetResponseHeaders (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    if (!h->hsize) {
        args.GetReturnValue().Set(v8::String::New(""));
    } else {
		args.GetReturnValue().Set(v8::String::New(h->headers, h->hsize));
	}
}

void LibCurlJSAPI::GetResponseText (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    if (!h->size) {
        args.GetReturnValue().Set(v8::String::New("{}"));
    } else {
		args.GetReturnValue().Set(v8::String::New(h->memory, h->size));
	}
}

void LibCurlJSAPI::Destroy (const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHANDLE *h = XHANDLE(args[0]);
    if (h->slist) {
        curl_slist_free_all(h->slist);
        h->slist = NULL;
    }
    if (h->post) {
        curl_formfree(h->post);
    }
    curl_easy_cleanup(h->curl);
    free(h->memory);
    free(h->headers);
    free(h);
    args.GetReturnValue().Set(v8::Undefined());
}

void LibCurlJSAPI::Register (v8::Handle<v8::ObjectTemplate> global) {

	v8::PropertyAttribute builtInPropAttr = (v8::PropertyAttribute)(v8::DontDelete | v8::ReadOnly);    
	v8::Handle<v8::ObjectTemplate> curlObject = v8::ObjectTemplate::New();	
    curlObject->Set(v8::String::NewSymbol("error"), v8::FunctionTemplate::New(LibCurlJSAPI::Error), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("init"), v8::FunctionTemplate::New(LibCurlJSAPI::Initialize), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("setMethod"), v8::FunctionTemplate::New(LibCurlJSAPI::SetMethod), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("followRedirects"), v8::FunctionTemplate::New(LibCurlJSAPI::FollowRedirects), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("setCookie"), v8::FunctionTemplate::New(LibCurlJSAPI::SetCookie), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("setHeader"), v8::FunctionTemplate::New(LibCurlJSAPI::SetHeader), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("addFormField"), v8::FunctionTemplate::New(LibCurlJSAPI::AddFormField), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("addFormFile"), v8::FunctionTemplate::New(LibCurlJSAPI::AddFormFile), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("setPostFields"), v8::FunctionTemplate::New(LibCurlJSAPI::SetPostFields), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("setTimeout"), v8::FunctionTemplate::New(LibCurlJSAPI::SetTimeout), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("perform"), v8::FunctionTemplate::New(LibCurlJSAPI::Perform), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("getResponseCode"), v8::FunctionTemplate::New(LibCurlJSAPI::GetResponseCode), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("getContentType"), v8::FunctionTemplate::New(LibCurlJSAPI::GetContentType), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("getResponseText"), v8::FunctionTemplate::New(LibCurlJSAPI::GetResponseText), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("getResponseHeaders"), v8::FunctionTemplate::New(LibCurlJSAPI::GetResponseHeaders), builtInPropAttr);
    curlObject->Set(v8::String::NewSymbol("destroy"), v8::FunctionTemplate::New(LibCurlJSAPI::Destroy), builtInPropAttr);
		
    global->Set(v8::String::NewSymbol("curl"), curlObject, builtInPropAttr);

}