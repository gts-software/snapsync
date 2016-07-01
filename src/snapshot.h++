#include <boost/filesystem.hpp>

namespace snapshot {

  void create(boost::filesystem::path directory, std::ostream& image);
  void create(boost::filesystem::path directory, boost::filesystem::path image);

  void extract(std::istream& image, boost::filesystem::path directory);
}
