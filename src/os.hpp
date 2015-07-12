/************************************************************************//*
 *
 *   Script File: os.hpp
 *
 *   Description:
 *
 *   OS specific implementations
 *
 *
 *   Author: Joshua Zhang
 *   Date since: June-2015
 *
 *   Copyright (c) <2015> <JOSHUA Z. ZHANG>
 * 
 *	 Open source according to MIT License.
 *	 
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 *   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 *   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 *   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
***************************************************************************/


#ifndef _OPENZL_OS_HPP_
#define _OPENZL_OS_HPP_

#include <ctime>
#include <string>
#include <fstream>
#include <vector>

namespace zl
{
	namespace os
	{
		std::size_t thread_id();
		std::tm localtime(std::time_t t);
		std::tm gmtime(std::time_t t);

		/*!
		 * \fn	std::wstring utf8_to_wstring(std::string &u8str);
		 *
		 * \brief	UTF 8 to wstring.
		 * 			
		 * \note	This is actually only useful in windows.
		 *
		 * \param [in,out]	u8str	The 8str.
		 *
		 * \return	A std::wstring.
		 */
		std::wstring utf8_to_wstring(std::string &u8str);

		std::string wstring_to_utf8(std::wstring &wstr);

		bool path_exists(std::string &path, bool considerFile = false);

		
		void fstream_open(std::fstream &stream, std::string &filename, std::ios::openmode openmode);
		void ifstream_open(std::ifstream &stream, std::string &filename, std::ios::openmode openmode);

		/*!
		 * \fn	std::string endl();
		 *
		 * \brief	Gets the OS dependent line end characters.
		 *
		 * \return	A std::string.
		 */
		std::string endl();

		std::string get_current_working_directory();

		std::string get_absolute_path(std::string reletivePath);

		std::vector<std::string> path_split(std::string path);

		std::string path_join(std::vector<std::string> elems);

		std::string path_split_filename(std::string path);

		std::string path_split_directory(std::string path);

		std::string path_split_basename(std::string path);

		std::string path_split_extension(std::string path);

		bool create_directory(std::string path);

		bool create_directory_recursive(std::string path);

	} // namespace os


} // namespace zl

#endif //END _OPENZL_OS_HPP_
