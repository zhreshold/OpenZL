/************************************************************************//*
 *
 *   Script File: format.hpp
 *
 *   Description:
 *
 *   String formats
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

#ifndef _OPENZL_FORMAT_HPP_
#define _OPENZL_FORMAT_HPP_

#include <string>
#include <locale>
#include <codecvt>

namespace zl
{
	namespace consts
	{
		static const std::string kFormatSpecifierPlaceHolder = std::string("{}");
	}

	namespace fmt
	{
		namespace detail
		{
			template<class Facet>
			struct DeletableFacet : Facet
			{
				template<class ...Args>
				DeletableFacet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
				~DeletableFacet() {}
			};
		} // namespace detail

		bool str_equals(const char* s1, const char* s2);

		void replace_first_with_escape(std::string &str, const std::string &replaceWhat, const std::string &replaceWith);
		
		void replace_all_with_escape(std::string &str, const std::string &replaceWhat, const std::string &replaceWith);

		inline std::u16string utf8_to_utf16(std::string &u8str)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt;
			return cvt.from_bytes(u8str);
		}

		inline std::string utf16_to_utf8(std::u16string &u16str)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> cvt;
			return cvt.to_bytes(u16str);
		}

		inline std::u32string utf8_to_utf32(std::string &u8str)
		{
			std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
			return cvt.from_bytes(u8str);
		}

		inline std::string utf32_to_utf8(std::u32string &u32str)
		{
			std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
			return cvt.to_bytes(u32str);
		}

		template<typename Arg>
		inline void format_string(std::string &fmt, const Arg &last)
		{
			replace_first_with_escape(fmt, consts::kFormatSpecifierPlaceHolder, std::to_string(last));
		}

		template<typename Arg, typename... Args>
		inline void format_string(std::string &fmt, const Arg& current, const Args&... more)
		{
			replace_first_with_escape(fmt, consts::kFormatSpecifierPlaceHolder, std::to_string(current));
			format_string(fmt, more...);
		}

	} // namespace format

} // namespace zl

#endif //END _OPENZL_FORMAT_HPP_

