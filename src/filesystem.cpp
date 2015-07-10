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

		bool FileEditor::open(bool truncateOrNot)
		{
			if (this->is_open()) return true;
			std::ios::openmode mode = std::ios::in | std::ios::out | std::ios::binary;
			if (truncateOrNot)
			{
				mode |= std::ios::trunc;
			}
			else
			{
				mode |= std::ios::app;
			}

			// try to register this file to avoid multiply access to the same file
			bool success = true;
			//bool success = detail::FileEditorRegistry::instance().try_insert(filename_);
			// fail means someone is editing this file, just return
			if (!success)
			{
				//throw IOException("File: " + filename_ + " already opened by other.");
				return false;
			}

			os::fstream_open(stream_, filename_, mode);
			if (this->is_open()) return true;
			return false;
		}

		void FileEditor::close()
		{
			stream_.close();
			// unregister this file
			//detail::FileEditorRegistry::instance().erase(filename_);
		}

		bool FileEditor::try_open(int retryTime, int retryInterval, bool truncateOrNot)
		{
			while (retryTime > 0 && (!this->open(truncateOrNot)))
			{
				time::sleep(retryInterval);
				--retryTime;
			}
			return this->is_open();
		}

		bool FileReader::open()
		{
			if (this->is_open()) return true;
			this->check_valid();
			os::ifstream_open(istream_, filename_, std::ios::in|std::ios::binary);
			if (this->is_open()) return true;
			return false;
		}

		bool FileReader::try_open(int retryTime, int retryInterval)
		{
			while (retryTime > 0 && (!this->open()))
			{
				time::sleep(retryInterval);
				--retryTime;
			}
			return this->is_open();
		}

		bool is_occupied(std::string &filename)
		{
			std::string fn = os::get_absolute_path(filename);
			return detail::FileEditorRegistry::instance().contains(fn);
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