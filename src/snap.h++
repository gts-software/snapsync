#include <boost/filesystem.hpp>

namespace snapsync { namespace snap {

  class InvalidFileException : public std::exception { };

  void create(boost::filesystem::path directory, std::ostream& image);
  void create(boost::filesystem::path directory, boost::filesystem::path image);

  void extract(std::istream& image, boost::filesystem::path directory);
  void extract(boost::filesystem::path image, boost::filesystem::path directory);
} }
