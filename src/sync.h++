#include <boost/filesystem.hpp>
#include <istream>
#include <ostream>

namespace snapsync { namespace sync {

  class InvalidFileException : public std::runtime_error {
  public:
    inline InvalidFileException() : std::runtime_error("invalid file") { }
  };

  void signature(std::istream& base, std::ostream& signature, size_t blockLength = 0, size_t sumLength = 0);
  void signature(boost::filesystem::path basePath, boost::filesystem::path signaturePath, size_t blockLength = 0, size_t sumLength = 0);

  void delta(std::istream& signature, std::istream& target, std::ostream& patch);
  void delta(boost::filesystem::path signaturePath, boost::filesystem::path targetPath, boost::filesystem::path patchPath);

  void patch(std::istream& patch, std::istream& base, std::ostream& target);
  void patch(boost::filesystem::path patchPath, boost::filesystem::path basePath, boost::filesystem::path targetPath);

} }
