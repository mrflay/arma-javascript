/*
	Copyright (C) 2013 Joel Andersson

	This file is adapted from SilkJS.

	This file is part of "JavaScript for ARMA" project. 

	JavaScript for ARMA is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "Common.h"

class LibCurlJSAPI {

public:

	static void Register (v8::Handle<v8::ObjectTemplate> global);

protected:

	/**
	 * @function curl.error
	 * 
	 * ### Synopsis
	 * 
	 * var msg = curl.error(code);
	 * 
	 * Returns a string describing the CURLcode error code passed in the argument code.
	 * 
	 * @param {int} code - CURL error code.
	 * @return {string} msg - string describing the error.
	 */
	static void Error (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.init
	 * 
	 * ### Synopsis
	 * 
	 * var handle = curl.init(url);
	 * 
	 * Initialize a curl connection to the specified URL.  By default, redirects will be followed.  The returned handle should be considered opaque by JavaScript code.  The actual request is not performed until the curl.perform() method is called on the handle.
	 * 
	 * You must call curl.destroy() on the returned handle to free the resources allocated for the CURL request.
	 * 
	 * @param {string} url - the URL for the connection.
	 * @return {object} handle - opaque handle
	 */
	static void Initialize (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.setMethod
	 * 
	 * var status = curl.setMethod(handle, method);
	 * 
	 * Sets the method for the CURL request to either GET or POST.
	 * 
	 * @param {object} handle - CURL handle.
	 * @param {string} method - may be either "GET" or "POST".
	 * @return {int} status - 0 for success, otherwise an error code.
	 */
	static void SetMethod (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.followRedirects
	 * 
	 * ### Synopsis
	 * 
	 * var status = curl.followRedirects(handle, flag);
	 * 
	 * Sets the "follow redirect" protocol for the specified handle to the value of flag.
	 * 
	 * @param {object} handle - CURL handle.
	 * @param {int} flag - 1 to follow redirects, 0 to not follow redirects.
	 * @return {int} status - 0 for success, otherwise an error code.
	 */
	static void FollowRedirects (const v8::FunctionCallbackInfo<v8::Value>& args);


	/**
	 * @function curl.setCookie
	 * 
	 * ### Synopsis
	 * 
	 * var status = curl.setCookie(handle, cookie_string);
	 * 
	 * Sets the cookie (header) for the CURL request.
	 * 
	 * The format of the string should be NAME=CONTENTS, where NAME is the cookie name and CONTENTS is what the cookie should contain.
	 * 
	 * If you need to set multiple cookies, you need to set them all using a single option and thus you need to concatenate them all in one single string. Set multiple cookies in one string like this: "name1=content1; name2=content2;" etc.
	 *
	 * This option sets the cookie header explicitly in the outgoing request(s). If multiple requests are done due to authentication, followed redirections or similar, they will all get this cookie passed on.
	 * 
	 * @param {object} handle - CURL handle.
	 * @param {string} cookie_string - see comments above for the format.
	 * @return {int} status - 0 for success, otherwise an error code.
	 */
	static void SetCookie (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.setHeader
	 * 
	 * ### Synopsis
	 * 
	 * var status = curl.setHeader(handle, header_string);
	 * 
	 * Sets the specified header for the CURL request.
	 * 
	 * The format of the string should be NAME=CONTENTS, where NAME is the header name and CONTENTS is the value of the header.
	 * 
	 * This option sets the header explicitly in the outgoing request(s). 
	 * 
	 * @param {object} handle - CURL handle.
	 * @param {string} header_string - see comments above for the format.
	 * @return {int} status - 0 for success, otherwise an error code.
	 */
	static void SetHeader (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.addFormField
	 * 
	 * ### Synopsis
	 * 
	 * var status = curl.addFormField(handle, name, value);
	 * var status = curl.addFormField(handle, name, value, contentType);
	 * 
	 * Add a section to a multipart/formdata HTTP POST.
	 * 
	 * ### Description
	 * 
	 * curl.addFormField() is used to append sections when building a multipart/formdata HTTP POST.  Append one section at a time until you've added all the sections you want included.
	 * 
	 * If you call this function, or addPostFile(), at least once, a POST will automatically occur when you call curl.perform().
	 * 
	 * Each part of a multipart/formdata post consists of at least a NAME and a CONTENTS part.  The second form of the call to this function allows you to set a content-type header for the part.
	 * 
	 * If you want to perform a file upload, you may add that part by calling curl.addPostFile().
	 * 
	 * @param {object} handle - CURL handle
	 * @param {string} name - field name
	 * @param {string} value - field value
	 * @param {string} contentType - optional content-type for the field value.
	 * @return {int} status - 0 for success, otherwise an error code.
	 */
	static void AddFormField(const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.addFormFile
	 * 
	 * ### Synopsis
	 * 
	 * var status = curl.addFormFile(name, filename);
	 * var status = curl.addFormFile(name, filename, contentType);
	 * 
	 * Add a file upload section to a multipart/formdata HTTP POST.
	 * 
	 * ### Description
	 * 
	 * curl.addFormFile() is used to append file upload sections when building a multipart/formdata HTTP POST.
	 * 
	 * If you call this function, or addPostField(), at least once, a POST will automatically occur when you call curl.perform().
	 * 
	 * The second form of the call to this function allows you to set a content-type header for the part.
	 * 
	 * @param {object} handle - CURL handle
	 * @param {string} name - field name
	 * @param {string} filename - path to existing file to upload
	 * @param {string} contentType - optional content-type for the field value.
	 * @return {int} status - 0 for success, otherwise an error code.
	 */
	static void AddFormFile(const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.setPostFields
	 * 
	 * ### Synopsis
	 * 
	 * var status = curl.setPostFields(handle, post_fields);
	 * 
	 * Set the post fields for the CURL request.
	 * 
	 * The post_fields string should be the full data to post in a HTTP POST operation. You must make sure that the data is formatted the way you want the server to receive it. libcurl will not convert or encode it for you. Most web servers will assume this data to be url-encoded.
	 * 
	 * The pointed data are NOT copied by the library: as a consequence, they must be preserved by the calling application until the transfer finishes.
	 * This POST is a normal application/x-www-form-urlencoded kind (and libcurl will set that Content-Type by default when this option is used), which is the most commonly used one by HTML forms.
	 * 
	 * @param {object} handle - CURL handle.
	 * @param {string} cookie_string - see comments above for the format.
	 * @return {int} status - 0 for success, otherwise an error code.
	 */
	static void SetPostFields (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.setTimeout
	 *
	 * ### Synopsis
	 *
	 * curl.setTimeout(handle, seconds);
	 * 
	 * Set timeout for CURL operations.  Normally they do not timeout, which can cause a hang to occur.
	 * @param {object} handle - CURL handle
	 * @param {int} seconds - timeout in seconds, or 0 for infinite.
	 */
	static void SetTimeout (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.perform
	 * 
	 * ###Synopsis
	 * 
	 * var status = curl.perform(handle);
	 * var status = curl.perform(handle, verbose);
	 * 
	 * Perform the CURL request.
	 * 
	 * This function is called after the curl.init() and all the method calls to set options are made, and will perform the transfer as described in the options. It must be called with the same handle as input as the curl.init() call returned.
	 * 
	 * @param {object} handle - CURL handle
	 * @param {int} verbose - set to > 0 to have cURL library print debugging info to console
	 * @return {int} status - 0 for success, otherwise an error code.
	 */
	static void Perform (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.getResponseCode
	 * 
	 * ### Synopsis
	 * 
	 * var code = curl.getResponseCode(handle);
	 *  
	 * Get the response code (e.g. 200 for OK, 404 for not found, etc.) for a completed CURL request.
	 * 
	 * @param {object} handle - CURL handle
	 * @return {int} status - 200 for OK, etc.
	 */
	static void GetResponseCode (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.getContentType
	 * 
	 * ### Synopsis
	 * 
	 * var contentType = curl.getContentType(handle);
	 * 
	 * Gets the content-type: header value for a completed response.  This is the MIME type of the response.
	 * 
	 * @param {object} handle - CURL handle
	 * @return {string} contentType - MIME string / content-type
	 */
	static void GetContentType (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @funciton curl.getResponseHeaders
	 *
	 * ### Synopsis
	 *
	 * var responseHeaders = curl.getResponseHeaders(handle);
	 *
	 * Gets the response headers of the completed CURL response.
	 *
	 * The response headers are returned in the form of a string; each header line is terminated with a newline.
	 *
	 * @param {object} handle - CURL handle
	 * @return {string} responseHeaders - the response headers of the completed CURL request.
	 */
	static void GetResponseHeaders (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.getResponseText
	 * 
	 * ### Synopsis
	 * 
	 * var responseText = curl.getResponseText(handle);
	 * 
	 * Gets the responseText of the completed CURL response.  This does not include any headers.
	 * 
	 * @param {object} handle - CURL handle
	 * @return {string} responseText - the response text of the completed CURL request.
	 */
	static void GetResponseText (const v8::FunctionCallbackInfo<v8::Value>& args);

	/**
	 * @function curl.destroy(handle);
	 * 
	 * ### Synopsis
	 * 
	 * Frees memory allocated by curl.init(), including the handle.  The handle will no longer be valid.
	 * 
	 * @param {object} handle - CURL handle
	 */
	static void Destroy (const v8::FunctionCallbackInfo<v8::Value>& args);
};