#include "sync.h++"
#include <librsync.h>

namespace snapsync { namespace sync {

  void signature(std::istream& base, std::ostream& signature) {

      auto job = rs_sig_begin(RS_DEFAULT_BLOCK_LEN, 0, RS_BLAKE2_SIG_MAGIC);
      rs_job_free(job);
  }

  void signature(boost::filesystem::path basePath, boost::filesystem::path signaturePath) {
  }

  void delta(std::istream& signature, std::istream& target, std::ostream& patch) {
  }

  void delta(boost::filesystem::path signature, boost::filesystem::path target, boost::filesystem::path patch) {
  }

  void patch(std::istream& patch, std::iostream& base) {
  }

  void patch(boost::filesystem::path patch, boost::filesystem::path base) {
  }

} }
