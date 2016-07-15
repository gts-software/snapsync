{
    "targets": [
        {
            "target_name": "snapsync",
            "defines": [
                "BUILD_NODE_MODULE",
            ],
            "sources": [
                "../src/node.cc",
                "../src/snap_create.cc",
                "../src/snap_extract.cc",
                "../src/sync_signature.cc",
                "../src/sync_delta.cc",
                "../src/sync_patch.cc",
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "../librsync/src",
                "../"
            ],
            "libraries": [
                "-lboost_system", "-lboost_filesystem",
                "-L../../librsync", "-lrsync",
                "-L../../cryptopp", "-lcryptopp",
            ],
            "cflags": [ "-std=c++11" ],
            'cflags!': [ '-fno-exceptions', '-fno-rtti' ],
            'cflags_cc!': [ '-fno-exceptions', '-fno-rtti' ]
        }
    ]
}
