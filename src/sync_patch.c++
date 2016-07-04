#include "sync.h++"
#include <librsync.h>

using namespace std;

namespace snapsync { namespace sync {

  void patch(std::istream& patch, std::istream& base, std::ostream& target) {

    // set exceptions flags
    auto patchOldExceptions = patch.exceptions();
    patch.exceptions(istream::failbit | istream::badbit);

    auto baseOldExceptions = base.exceptions();
    base.exceptions(istream::failbit | istream::badbit);

    auto targetOldExceptions = target.exceptions();
    target.exceptions(ostream::failbit | ostream::badbit);

    // reset exceptions flags
    patch.exceptions(patchOldExceptions);
    base.exceptions(baseOldExceptions);
    target.exceptions(targetOldExceptions);
  }

  void patch(boost::filesystem::path patchPath, boost::filesystem::path basePath, boost::filesystem::path targetPath) {
  }

} }
