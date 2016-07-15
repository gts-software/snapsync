{
    "targets": [
        {
            "target_name": "snapsync",
            "sources": [
                "../src/node.cc",
                "../src/snap_create.c++",
                "../src/snap_extract.c++",
                "../src/sync_signature.c++",
                "../src/sync_delta.c++",
                "../src/sync_patch.c++",
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "../librsync/src"
            ],
            "libraries": [
                "-lboost_system", "-lboost_filesystem",
                "-L../../librsync", "-lrsync",
                "-L../../cryptopp", "-lcryptopp",
            ],
            "cflags": [ "-std=c++11" ],
            'cflags!': [ '-fno-exceptions' ],
            'cflags_cc!': [ '-fno-exceptions' ]
        }
    ]
}
