/**
 * @module builtin/fs
 * 
 * ### Synopsis
 * SilkJS builtin fs object.
 * 
 * ### Description
 * The builtin/fs object provides constants and methods to directly access the underlying operating system's file system functions.  
 * 
 * ### Usage
 * var fs = require('builtin/fs');
 * 
 * ### See Also
 * Operating system man pages
 * 
 */

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>

#include <windows.h>
#include <shlobj.h>
#include <crtdefs.h>
#include <basetsd.h>

#include "Common.h"
#include "SilkJS.h"
#include "Filesystem.h"

#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISREG
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/**
 * @function fs.error
 * 
 * ### Synopsis
 * Returns string version of last OS error.
 * 
 * ### Usage:
 * var message = fs.error();
 * 
 * @return {string} message - error message.
 */
static void fs_error (const v8::FunctionCallbackInfo<v8::Value>& args) {
     //args.GetReturnValue().Set(String::New(strerror(errno)));
}

/**
 * @function fs.chdir
 * 
 * ### Synopsis
 * 
 * Change current working directory.
 * 
 * ### Usage:
 * var success = fs.chdir(dirPath);
 * 
 * @param {string} dirPath path in filesystem to set directory to.
 * @return {int} success - 0 on success, or -1 if error occurred.
 */
static void fs_chdir (const v8::FunctionCallbackInfo<v8::Value>& args) {
    String::Utf8Value dir(args[0]->ToString());
    args.GetReturnValue().Set(Integer::New(_chdir(*dir)));
}


static void fs_homedir (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szPath))) {	
		v8::Handle<String>s = v8::String::New((uint16_t*)szPath, wcslen(szPath));
		args.GetReturnValue().Set(s);
		return;
	}
	args.GetReturnValue().Set(Undefined());
}


static void fs_profiledir (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szPath))) {	
		v8::Handle<String>s = v8::String::New((uint16_t*)szPath, wcslen(szPath));
		args.GetReturnValue().Set(s);
		return;
	}
	args.GetReturnValue().Set(Undefined());
}

static void fs_missiondir (const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Handle<String>s = v8::String::New(("\\Users\\joel\\Documents\\Arma 3 - Other Profiles\\mrflay\\missions\\practice_range.Stratis"));
	args.GetReturnValue().Set(s);
}

/**
 * @function fs.getcwd
 * 
 * ### Synopsis
 * 
 * var path = fs.getcwd();
 * 
 * Get current working directory.
 * 
 * @return {string} current working directory path or null if error occurred.
 */
static void fs_getcwd (const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    char *cwd = _getcwd(NULL, 0);
    Handle<String>s = String::New(cwd);
    delete[] cwd;
    args.GetReturnValue().Set(s);
}

/**
 * @function fs.open
 * 
 * ### Synopsis
 * var fd = (filename, flags, mode);
 * 
 * @param {string} filename - name of file to open.
 * @param {int} flags - flags passed to open(2).
 * @param {int} mode - file permissions for file.
 * @return {int} fd - OS file handle, or -1 if error occurred.
 */
static void fs_open(const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    String::Utf8Value filename(args[0]->ToString());
    int flags = args[1]->IntegerValue();
    int mode = args[2]->IntegerValue();
    args.GetReturnValue().Set(Integer::New(_open(*filename, flags, mode)));
}

/**
 * @function fs.close
 * 
 * ### Synopsis
 * 
 * var success = fs.close(fd);
 * 
 * Close an already open file descriptor.
 * 
 * @param {int} fd - the file descriptor to close.
 * @return {int} success - 0 on success, or -1 if error occurred.
 * 
 */
static void fs_close (const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    int fd = args[0]->IntegerValue();
    args.GetReturnValue().Set(Integer::New(_close(fd)));
}

static JSOBJ format_stat (struct stat &buf) 
{
    JSOBJ o = Object::New();
    o->Set(String::New("dev"), Integer::New(buf.st_dev));
    o->Set(String::New("ino"), Integer::New(buf.st_ino));
    o->Set(String::New("mode"), Integer::New(buf.st_mode));
    o->Set(String::New("nlink"), Integer::New(buf.st_nlink));
    o->Set(String::New("uid"), Integer::New(buf.st_uid));
    o->Set(String::New("gid"), Integer::New(buf.st_gid));
    o->Set(String::New("rdev"), Integer::New(buf.st_rdev));
    o->Set(String::New("size"), Integer::New(buf.st_size));
#if 0
    o->Set(String::New("blksize"), Integer::New(buf.st_blksize));
    o->Set(String::New("blocks"), Integer::New(buf.st_blocks));
#endif
    o->Set(String::New("atime"), Integer::New(buf.st_atime));
    o->Set(String::New("mtime"), Integer::New(buf.st_mtime));
    o->Set(String::New("ctime"), Integer::New(buf.st_ctime));
    return o;
}

/**
 * @function fs.stat
 * 
 * ### Synopsis
 * 
 * var o = fs.stat(path);
 * 
 * Get a structure describing a file's status.
 * 
 * ### Status structure
 * The object returned by this function has the following members:
 * dev: ID of device containing the file.
 * ino: inode number.
 * mode: file permissions/protection.
 * nlink: number of hard links.
 * uid - user ID of owner.
 * gid: group ID of owner.
 * rdev: device ID (if special file)
 * size: total size of file in bytes.
 * blksize: block size for file system I/O.
 * blocks: number of 512B blocks allocated.
 * atime: timestamp of last access.
 * mtime: timestamp of last modification.
 * ctime: timestamp of last status change.
 * 
 * @param {string} path - path to file to get status for.
 * @return {object} o - status structure described above, or null if error occurred.
 * 
 * ### Notes
 * It is a bit more expensive to call this function if you are only interested in one of the fields.  This is because the entire result status object must be constructed.  SilkJS provides faster convenience methods to obtain the size, type, etc., of a file path.
 */
static void fs_stat (const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    String::Utf8Value path(args[0]->ToString());
    struct stat buf;
    if (stat(*path, &buf) == -1) {
        args.GetReturnValue().Set(Null());
		return;
    }
    args.GetReturnValue().Set(format_stat(buf));
}


/**
 * @function fs.lstat
 * 
 * ### Synopsis
 * var o = fs.fstat(fd);
 * 
 * This function is identical to fs.stat() except that the file get status for is specified by a file descriptor.
 * 
 * @param {int} fd - file descriptor of an open file to get status for.
 * @return {object} o - status structure described for fs.stat(), or null if error occurred.
 */
static void fs_fstat (const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    int fd = args[0]->IntegerValue();
    struct stat buf;
    if (fstat(fd, &buf) == -1) {
        args.GetReturnValue().Set(False());
		return;
    }
    args.GetReturnValue().Set(format_stat(buf));
}

/**
 * function fs.exists
 * 
 * ### Synopsis
 * 
 * var doesExist = fs.exists(path);
 * 
 * This function tests to see if the specified path exists in the file system.
 * 
 * Calling this function is faster than calling fs.stat() to determine if a file or directory exists.
 * 
 * @param {string} path - path of file
 * @return {boolean} doesExist - true if file exists, false if not.
 */
static void fs_exists (const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    String::Utf8Value path(args[0]->ToString());

    struct stat buf;
    if (stat(*path, &buf)) {
        args.GetReturnValue().Set(False());
		return;
    }
    args.GetReturnValue().Set(True());
}

/**
 * function fs.isFile
 * 
 * ### Synopsis
 * 
 * var is_a_file = fs.isFile(path);
 * 
 * This function tests to see if the specified path exists and is a regular file in the file system.
 * 
 * Calling this function is faster than calling fs.stat() to determine if a path is a file.
 * 
 * @param {string} path - path of file
 * @return {boolean} is_a_file - true if path exists and is a regular file, false if not.
 */
static void fs_isfile (const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    String::Utf8Value path(args[0]->ToString());

    struct stat buf;
    if (stat(*path, &buf)) {
        args.GetReturnValue().Set(False());
		return;
    }
    if (S_ISREG(buf.st_mode)) {
        args.GetReturnValue().Set(True());
		return;
    }
    else {
        args.GetReturnValue().Set(False());
		return;
    }
}

/**
 * function fs.isDir
 * 
 * ### Synopsis
 * 
 * var is_a_directory = fs.isDir(path);
 * 
 * This function tests to see if the specified path exists and is a directory in the file system.
 * 
 * Calling this function is faster than calling fs.stat() to determine if a path is a directory.
 * 
 * @param {string} path - path of file
 * @return {boolean} is_a_directory - true if path exists and is a directory, false if not.
 */
static void fs_isdir (const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    String::Utf8Value path(args[0]->ToString());

    struct stat buf;
    if (stat(*path, &buf)) {
        args.GetReturnValue().Set(False());
		return;
    }
    if (S_ISDIR(buf.st_mode)) {
        args.GetReturnValue().Set(True());
		return;
    }
    else {
		args.GetReturnValue().Set(False());
        return;
    }
}

/**
 * function fs.fileSize
 * 
 * ### Synopsis
 * 
 * var size = fs.fileSize(path);
 * 
 * This function returns the size of a file in bytes.
 * 
 * Calling this function is faster than calling fs.stat() to get the size of a file.
 * 
 * @param {string} path - path of file
 * @return {int} size - size of file in bytes, or false if an error occurred.
 */
static void fs_filesize (const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    String::Utf8Value path(args[0]->ToString());

    struct stat buf;
    if (stat(*path, &buf)) {
        args.GetReturnValue().Set(False());
		return;
    }
    args.GetReturnValue().Set(Integer::New(buf.st_size));
}

/**
 * function fs.fileModified
 * 
 * ### Synopsis
 * 
 * var timestamp = fs.fileModified(path);
 * 
 * This function returns the timestamp when item in the file system with the given path was last modified.
 * 
 * Calling this function is faster than calling fs.stat() to get the modification time of a file.
 * 
 * @param {string} path - path of file
 * @return {int} timestamp - modification time as timestamp, or false if an error occurred.
 */
static void fs_mtime (const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    String::Utf8Value path(args[0]->ToString());

    struct stat buf;
    if (stat(*path, &buf)) {
        args.GetReturnValue().Set(False());
		return;
    }
    args.GetReturnValue().Set(Integer::New(buf.st_mtime));
}

/**
 * @function fs.realpath
 * 
 * ### Synopsis
 * 
 * var real_path = fs.realpath(path);
 * 
 * This function returns the canonicalized absolute path name of the specified path.
 * 
 * All symbolic links, extra "/" characters, and references to "/./" and "/../" are resolved in the returned path.  Both absolute and relative paths are resolved.  All components of the input path must exist when this function is called.
 * 
 * @param {string} path - path to resolve to absolute path.
 * @return {string} real_path - absolute path, or false if an error occurred.
 */
static void fs_realpath (const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    String::Utf8Value relpath(args[0]->ToString());
    char *abspath = _fullpath(NULL, *relpath, 0);
    if (!abspath) {
        args.GetReturnValue().Set(False());
		return;
    }
    Handle<String>s = String::New(abspath);
    free(abspath);
    args.GetReturnValue().Set(s);
}

/**
 * @function fs.readDir
 * 
 * ### Synopsis
 * 
 * var filenames = fs.readDir(path);
 * 
 * This function returns an array of file (or directory) names in the specified path (directory).  
 * 
 * Only the file names are returned, no other information.  The special entries "." and ".." are not returned.
 * 
 * @param {string} path - absolute or relative path to a directory to get the contents of.
 * @return {array} filenames - array of {string} filenames, or null if the directory oculd not be opened.
 */
#if 0
static JSVAL fs_readdir (JSARGS args) {
    String::Utf8Value path(args[0]->ToString());
    DIR *d = opendir(*path);
    if (!d) {
        return Null();
    }
    Handle<Array>a = Array::New();
    int ndx = 0;
    while (struct dirent * e = readdir(d)) {
        if (strcmp(e->d_name, ".") && strcmp(e->d_name, "..")) {
            a->Set(ndx++, String::New(e->d_name));
        }
    }
    closedir(d);
    return a;
}
#endif

/**
 * @function fs.readFile
 * 
 * ### Synopsis
 * 
 * var contents = fs.readFile(filename);
 * 
 * This function reads the entire contents of the specified file into a string.
 * 
 * @param {string} filename - name of file to read.
 * @return {string} contents - content of the file, or null if an error occurred.
 * 
 * ### Notes
 * This function reads 1024 bytes from the file at a time.  This is not optimal for big files, but minimizes the amount of memory used by the process.
 */
static void fs_readfile (const v8::FunctionCallbackInfo<v8::Value>& args) 
{
    String::Utf8Value path(args[0]->ToString());
    int fd = _open(*path, O_RDONLY);
    if (fd == -1) {
        args.GetReturnValue().Set(Null());
		return;
    }
	
	// TODO: file locking on windows
    
    _lseek(fd, 0, 0);
    std::string s;
    char buf[1024];
    SSIZE_T count;
    while ((count = _read(fd, buf, 1024))) {
        s = s.append(buf, count);
    }
    
    _close(fd);
    Handle<String>ret = String::New(s.c_str(), s.size());
    args.GetReturnValue().Set(ret);
}

void FileSystem::Register (v8::Handle<v8::ObjectTemplate> global) {
    
	Handle<ObjectTemplate>fs = ObjectTemplate::New();

    /**
     * @constant fs.O_RDONLY - open for reading only
     * @constant fs.O_WRONLY - open for writing only
     * @constant fs.O_RDWR - open for reading and writing
     * @constant fs.O_CREAT - create file if it does not exist
     * @constant fs.O_TRUNC - truncate file
     */
    fs->Set(String::New("O_RDONLY"), Integer::New(O_RDONLY));
    fs->Set(String::New("O_WRONLY"), Integer::New(O_WRONLY));
    fs->Set(String::New("O_RDWR"), Integer::New(O_RDWR));
    fs->Set(String::New("O_CREAT"), Integer::New(O_CREAT));
    fs->Set(String::New("O_TRUNC"), Integer::New(O_TRUNC));


    /**
     * @constant fs.S_IFMT - bit mask for the file type bit fields
     * @constant fs_S_IFSOCK - socket
     * @constant fs.S_IFLNK - symbolic link
     * @constant fs.S_IFREG - regular file
     * @constant fs.S_IFBLK - block device
     * @constant fs.S_IFHCR - character device
     * @constant fs.S_IFIFO - FIFO
     * @constant fs.S_ISUID - set UID bit
     * @constant fs.S_ISGID - set group id bit
     * @constant fs.S_ISVTX - sticky bit
     * @constant fs.S_IRMXU - mask for file owner permissions
     * @constant fs.S_IRUSR - owner has read permission
     * @constant fs.S_IWUSR - owner has write permission
     * @constant fs.S_IXUSR - owner has execute permission
     * @constant fs.S_IRMXG - mask for group permissions
     * @constant fs.S_IRGRP - group has read permission
     * @constant fs.S_IWGRP - group has write permission
     * @constant fs.S_IXGRP - group has execute permission
     * @constant fs.S_IRMXO - mask for others (not in group) permissions
     * @constant fs.S_IROTH - others have read permission
     * @constant fs.S_IWOTH - others have write permission
     * @constant fs.S_IXOTH - others have execute permission
     */
    fs->Set(String::New("S_IFMT"), Integer::New(S_IFMT));
    fs->Set(String::New("S_IFREG"), Integer::New(S_IFREG));
    fs->Set(String::New("S_IFDIR"), Integer::New(S_IFDIR));
    fs->Set(String::New("S_IFCHR"), Integer::New(S_IFCHR));

    fs->Set(String::New("error"), FunctionTemplate::New(fs_error));
    fs->Set(String::New("chdir"), FunctionTemplate::New(fs_chdir)); // remove?
    fs->Set(String::New("getcwd"), FunctionTemplate::New(fs_getcwd));
	fs->Set(String::New("homedir"), FunctionTemplate::New(fs_homedir));
	fs->Set(String::New("missiondir"), FunctionTemplate::New(fs_missiondir));
    fs->Set(String::New("open"), FunctionTemplate::New(fs_open));
    fs->Set(String::New("close"), FunctionTemplate::New(fs_close));
    fs->Set(String::New("stat"), FunctionTemplate::New(fs_stat));
    fs->Set(String::New("fstat"), FunctionTemplate::New(fs_fstat));
    fs->Set(String::New("realpath"), FunctionTemplate::New(fs_realpath));

    fs->Set(String::New("readFile"), FunctionTemplate::New(fs_readfile));

    fs->Set(String::New("exists"), FunctionTemplate::New(fs_exists));
    fs->Set(String::New("isFile"), FunctionTemplate::New(fs_isfile));
    fs->Set(String::New("isDir"), FunctionTemplate::New(fs_isdir));
    fs->Set(String::New("fileSize"), FunctionTemplate::New(fs_filesize));
    fs->Set(String::New("fileModified"), FunctionTemplate::New(fs_mtime));

    global->Set(String::New("fs"), fs);
}
