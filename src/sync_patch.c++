#include "sync.h++"
#include <librsync.h>

using namespace std;

namespace snapsync { namespace sync {

  void patch(std::istream& patch, std::iostream& base) {

    // set exceptions flags
    auto patchOldExceptions = patch.exceptions();
    patch.exceptions(istream::failbit | istream::badbit);

    auto baseOldExceptions = base.exceptions();
    base.exceptions(iostream::failbit | iostream::badbit);

    // reset exceptions flags
    patch.exceptions(patchOldExceptions);
    base.exceptions(baseOldExceptions);
  }

  void patch(boost::filesystem::path patch, boost::filesystem::path base) {
  }

} }
