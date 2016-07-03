#include "sync.h++"
#include <librsync.h>

using namespace std;

namespace snapsync { namespace sync {

  void delta(std::istream& signature, std::istream& target, std::ostream& patch) {

      // set exceptions flags
      auto signatureOldExceptions = signature.exceptions();
      signature.exceptions(istream::failbit | istream::badbit);

      auto targetOldExceptions = target.exceptions();
      target.exceptions(istream::failbit | istream::badbit);

      auto patchOldExceptions = patch.exceptions();
      patch.exceptions(ostream::failbit | ostream::badbit);

      // reset exceptions flags
      signature.exceptions(signatureOldExceptions);
      target.exceptions(targetOldExceptions);
      patch.exceptions(patchOldExceptions);

  }

  void delta(boost::filesystem::path signature, boost::filesystem::path target, boost::filesystem::path patch) {
  }

} }
