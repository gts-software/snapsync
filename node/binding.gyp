{
    'variables' : {
        'arch': "<!(node -e \"console.log(process.arch)\")",
    },
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
            'conditions': [
                [ 'arch=="arm"', {
                    'libraries': [ "-static-libgcc", "-Wl,-static" ] } ]
            ],
            "libraries": [
                "-Wl,-rpath='$$ORIGIN'",
                "-lboost_system", "-lboost_filesystem",
                "-L../../librsync", "-lrsync",
                "-L../../cryptopp", "-lcryptopp",
            ],
            'cflags!': [ '-fno-exceptions', '-fno-rtti' ],
            'cflags_cc!': [ '-fno-exceptions', '-fno-rtti' ]
        },
        {
            "target_name": "snapsync_copy",
            "type":"none",
            "dependencies" : [ "snapsync" ],
            "copies": [
                {
                    'destination': '<(module_root_dir)/target',
                    'files': [
                        '<(PRODUCT_DIR)/snapsync.node'
                    ],
                    'conditions': [
                        [ 'arch=="x64"', {
                            'files': [
                                '<!@(ls -1 /usr/lib/x86_64-linux-gnu/libboost_system*.so*)',
                                '<!@(ls -1 /usr/lib/x86_64-linux-gnu/libboost_filesystem*.so*)',
                            ] } ]
                    ],
                }
            ]
        }
    ]
}
