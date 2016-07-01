
#include <librsync.h>
#include <mhash.h>

namespace snapsync { namespace sync {

  void signature() {

    mhash_init(MHASH_MD5);

    auto job = rs_sig_begin(RS_DEFAULT_BLOCK_LEN, 0, RS_BLAKE2_SIG_MAGIC);
    rs_job_free(job);
  }

  void delta() {
  }

  void patch() {
  }

} }