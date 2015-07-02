/************************************************************************//*
 *
 *   Script File: file.cpp
 *
 *   Description:
 *
 *   File handler class
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

#include "filesystem.hpp"
#include "os.hpp"
#include "time.hpp"

namespace zl
{
	namespace fs
	{

		bool File::open()
		{
			os::fstream_open(stream_, filename_, openmode_);
			if (this->is_open()) return true;
			return false;
		}

		void File::try_open(int retryTime, int retryInterval)
		{
			while (retryTime > 0 && this->is_open() != true)
			{
				this->open();
				time::sleep(retryInterval);
				--retryTime;
			}

			auto stategood = stream_.good();
			auto statebad = stream_.bad();
			auto statefail = stream_.fail();
			auto staterd = stream_.rdstate();
		}

		std::string File::endl()
		{
			return os::endl();
		}

		std::size_t get_file_size(std::string filename) 
		{
			std::ifstream fs;
			os::ifstream_open(fs, filename, std::ios::in);
			std::streampos currPos = fs.tellg();
			fs.seekg(0, fs.end);
			std::size_t size = static_cast<std::size_t>(fs.tellg());
			fs.seekg(currPos);
			return size;
		}
	} //namespace fs
} // namespace zl