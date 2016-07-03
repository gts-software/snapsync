#include <boost/filesystem.hpp>
#include <istream>
#include <ostream>

namespace snapsync { namespace sync {

  void signature(std::istream& base, std::ostream& signature);
  void signature(boost::filesystem::path basePath, boost::filesystem::path signaturePath);

  void delta(std::istream& signature, std::istream& target, std::ostream& patch);
  void delta(boost::filesystem::path signature, boost::filesystem::path target, boost::filesystem::path patch);

  void patch(std::istream& patch, std::iostream& base);
  void patch(boost::filesystem::path patch, boost::filesystem::path base);

} }
