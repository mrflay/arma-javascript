/* 
 * File:   silk.h
 * Author: mschwartz
 *
 * Created on July 3, 2011, 8:11 AM
 */

#ifndef SILK_H
#define SILK_H

#include <v8/v8.h>

//#include <string>

using namespace v8;
//using namespace std;

//extern string Base64Encode(unsigned char const* bytes_to_encode, unsigned long in_len);
//extern string Base64Decode(const char *encodedString);
//extern int decode_base64(unsigned char *dest, const char *src);

//extern Persistent<ObjectTemplate> globalObject;
//extern Persistent<ObjectTemplate> builtinObject;

//extern void init_global_object();

#define JSVAL v8::Handle<v8::Value>
#define JSOBJ Handle<Object>
#define JSOBJT Handle<ObjectTemplate>
#define JSARRAY Handle<Array>
#define JSARGS const Arguments&

static inline char JSCHAR(Handle<Value>v) {
    String::Utf8Value str(v->ToString());
    return (*str)[0];
}
static inline void *JSEXTERN(Handle<Value>v) {
    Handle<External>wrap = Handle<External>::Cast(v);
    return (void *)wrap->Value();
}


class Opaque {
public:
    static Handle<Object>New(void *p) {
#if 1
        Handle<ObjectTemplate>t = ObjectTemplate::New();
        t->SetInternalFieldCount(1);
        Local<Object>o = t->NewInstance();
        o->SetAlignedPointerInInternalField(0, p);
        return o;
#else        
        Handle<ObjectTemplate>t = ObjectTemplate::New();
        t->SetInternalFieldCount(1);
        Local<Object>o = t->NewInstance();
        o->SetInternalField(0, External::New(p));
        return o;
#endif
    }
};
static inline void *JSOPAQUE(Handle<Value>v) {
#if 1
    return v->ToObject()->GetAlignedPointerFromInternalField(0);
#else
    Local<External>wrap = Local<External>::Cast(v->ToObject()->GetInternalField(0));
    return wrap->Value();
#endif
}

#if 0
#define BUFFER_STRING
#undef BUFFER_STRING

#ifdef BUFFER_STRING
typedef struct {
public:
    std::string s;
public:
    void reset() { s.clear(); }
    long length() { return s.size(); }
    unsigned char *data() { return (unsigned char *)s.c_str(); }
} Buffer;
#else
typedef struct  {
    unsigned char *mem;
    long pos;
    long size;
public:
    void reset() { pos = 0; mem[0] = '\0'; }
    long length() { return pos; }
    unsigned char *data() { return mem; }
} Buffer;
#endif

class InputStream {
protected:
    unsigned char buffer[4096];
    ssize_t size;
    ssize_t pos;
    int fd;
protected:
    ssize_t FillBuffer() {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        switch (select(fd+1, &fds, NULL, NULL, &timeout)) {
            case -1:
                perror("select");
                return -1;
            case 0:
//              printf("Read timed out\n");
                return -1;
        }
        size = read(fd, buffer, 4096);
        if (size > 0) {
            pos = 0;
        }
//      if (size != 4096) {
//          printf("FillBuffer %ld\n", size);
//      }
        return size;
    }
public:
    InputStream(int sock) {
        fd = sock;
        size = pos = 0;
    }
    ~InputStream() {
        
    }
public:
    int Read() {
        if (pos >= size && FillBuffer() < 1) {
            return -1;
        }
        return buffer[pos++];
    }
    long Read(unsigned char *buf, ssize_t count) {
        ssize_t n;
        for (n=0; n<count; n++) {
            int c = Read();
            if (c == -1) {
                return n > 0 ? n : -1;
            }
            *buf++ = c;
        }
        return n;
    }
};

// MD5

/* typedef a 32 bit type */
typedef unsigned long int UINT4;

/* Data structure for MD5 (Message Digest) computation */
typedef struct {
  UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
  UINT4 buf[4];                                    /* scratch buffer */
  unsigned char in[64];                              /* input buffer */
  unsigned char digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;

extern void MD5Init (MD5_CTX *mdContext);
extern void MD5Update (MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen);
extern void MD5Final (MD5_CTX *mdContext);
extern void MD5Digest(MD5_CTX *mdContext, char *str);  // str must be 33 char buffer
#endif

#endif  /* SILK_H */

