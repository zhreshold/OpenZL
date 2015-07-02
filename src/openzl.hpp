/***********************************************************************//*

		i#f,.,##f                                        ##KKKKKE###  .K##L.        
	   G#W     K#K                                       #       ##    .##          
	  G##       ##                                      .W      W#t     ##          
	  ##         ##                                     ..    :###      ##          
	 i#E         G#j   :t :ff:       tLi      t  .f:          ###,      ##          
	 D#f         t#K  W#K ####     i#G###: .L#D L###t         ##G       ##          
	 ##i         ;## j##DjfG##G   t#   D## ft#LEE,W##        ;##        ##          
	 ##;         :##  G#W    j#L  #     ##   EG    ##t      K#f         ##          
	 ##t         :##  G#K     #K  #W####KK   EG    ##t     j##          ##          
	 K#j         ;##  G#K     ##  #          EG    ##t    .###          ##          
	 f#D         t#L  G#K     ##  #.         EG    ##t    ###i          ##          
	  ##,        #K   G#K     #f  ##         EG    ##t   ,##        E   ##         j
	  ##K       j#    G#K     #.  ##G    :   EG    ##t   ##,       fj   ##        j 
	  :##j     ,##    G##    ;D   ###E  i#   ED    ##j  E#L        #:   ##       j  
		 f#WEW#f      G#EELD#E     :####t  jW##Kf ####,#############  E###########  
		  .iti        G#K i;         .,                                             
					  G#K                                                           
					  G#K                                                           
					  D#W                                                           
					 i###D                                                          
					 ,::::                                                          

***************************************************************************/

/* Doxygen main page */
/*!	 \mainpage OpenZL Main Page
#	 OpenZL - Open EZ Library

### OpenZL is a light weighted portable library packed with fundamental functions to deal with Computer Vision projects.


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




#ifndef _OPENZL_OPENZL_HPP_
#define _OPENZL_OPENZL_HPP_

#include "common.hpp"
#include "time.hpp"
#include "filesystem.hpp"
#include "os.hpp"

// Macro overloading hack
//#define EXPAND(X) X    // for MSVC10 compatibility
//#define PP_NARG(...) EXPAND( PP_NARG_(__VA_ARGS__, PP_RSEQ_N()) )
//#define PP_NARG_(...) EXPAND( PP_ARG_N(__VA_ARGS__) )
//#define PP_ARG_N(_1, _2, _3, N, ...) N
//#define PP_RSEQ_N() 3, 2, 1, 0


// using stringstream hack to convert chain input to std::string
//#define TO_STRING( x ) (dynamic_cast< std::ostringstream & >( std::ostringstream() << std::dec << x).str())


//#if (!defined(NDEBUG))
//#define DEBUGLN(level, msg) TRACELN(level, msg)
//#else
//#define DEBUGLN(msg)
//#endif
//#define TRACELN(level, msg) do {zl::Logger::get_default_logger().traceln(__FILE__, __LINE__, __func__, level, TO_STRING(msg));} while(0)
//
//#define PRINTLN_1(msg) do {zl::Logger::get_default_logger().println(TO_STRING(msg));} while (0)
//#define PRINTLN_2(logger, msg) do {logger.println(TO_STRING(msg));} while (0)
//
//#define LOG_2(level, msg) do {zl::Logger::get_default_logger().logln(level, TO_STRING(msg));} while (0)
//#define LOG_3(logger, level, msg) do {logger.logln(level, TO_STRING(msg));} while (0)
//
//// macro selection by number of arguments
//#define PRINTLN_(N) PRINTLN_##N
//#define PRINTLN_EVAL(N) PRINTLN_(N)
//#define PRINTLN(...) EXPAND( PRINTLN_EVAL(EXPAND( PP_NARG(__VA_ARGS__) ))(__VA_ARGS__) )
//#define LOG_(N) LOG_##N
//#define LOG_EVAL(N) LOG_(N)
//#define LOGGER(...) EXPAND( LOG_EVAL(EXPAND( PP_NARG(__VA_ARGS__) ))(__VA_ARGS__) )


namespace zl
{
	using namespace time;
	using namespace fs;

} // namespace zl

#endif //END _OPENZL_OPENZL_HPP_


