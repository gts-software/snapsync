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
                "<!(node -e \"require('nan')\")"
            ],
            "cflags": [ "-std=c++11" ],
            'cflags!': [ '-fno-exceptions' ],
            'cflags_cc!': [ '-fno-exceptions' ]
        }
    ]
}
