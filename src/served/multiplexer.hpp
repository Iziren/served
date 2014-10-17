/*
 * Copyright (C) 2014 MediaSift Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SERVED_MULTIPLEXER_HPP
#define SERVED_MULTIPLEXER_HPP

#include <map>
#include <tuple>
#include <vector>
#include <functional>
#include <served/methods.hpp>
#include <served/request.hpp>
#include <served/response.hpp>
#include <served/mux/segment_matcher.hpp>

namespace served {

typedef std::function<void(response &, const request &)> served_req_handler;
typedef std::function<void(response &, request &)>       served_plugin_req_handler;
typedef std::vector<served_plugin_req_handler>           plugin_handler_list;

class multiplexer
{
public:
	//  -----  constructors  -----

	multiplexer();

	// Provides a base path that is listened for but ignored when selecting path
	// handlers.
	// For example: with a base path "/base" and handler registered at "/foo/bar"
	// the handler will be called for a request at "/base/foo/bar".
	multiplexer(const std::string & base_path);

	//  -----  plugin injection  -----

	void use_plugin(served_plugin_req_handler plugin);

	//  -----  http request handlers  -----

	void get (const std::string & path, served_req_handler handler);
	void head(const std::string & path, served_req_handler handler);
	void post(const std::string & path, served_req_handler handler);
	void put (const std::string & path, served_req_handler handler);

	//  -----  server control  -----
	//  TODO: move to server class?

	void listen(const std::string & address, const std::string & port);
	void stop();

private:
	//  -----  path parsing/compiling  -----

	typedef std::vector<served::mux::segment_matcher_ptr>          path_compiled_segments;

	typedef std::tuple<path_compiled_segments, served_req_handler> path_handler_candidate;
	typedef std::vector<path_handler_candidate>                    path_handler_candidates;
	typedef std::map<served::method, path_handler_candidates>      path_method_handler_candidates;

	path_compiled_segments get_segments(const std::string & path);

private:
	const std::string _base_path;

	path_method_handler_candidates _method_handler_candidates;
	plugin_handler_list            _plugin_handlers;
};

} // served

#endif // SERVED_MULTIPLEXER_HPP
