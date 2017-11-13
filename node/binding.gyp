{
    'variables' : {
        'arch': "<!(node -e \"console.log(process.arch)\")",
    },
    "targets": [
        {
            "target_name": "<(module_name)",
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
                "-Wl,-rpath='$$ORIGIN'",
                "-lboost_system", "-lboost_filesystem",
                "-L../../librsync", "-lrsync",
                "-L../../cryptopp", "-lcryptopp",
            ],
            'cflags!': [ '-fno-exceptions', '-fno-rtti' ],
            'cflags_cc!': [ '-fno-exceptions', '-fno-rtti' ],
        },
        {
            "target_name": "action_after_build",
            "type":"none",
            "dependencies" : [ "<(module_name)" ],
            "copies": [
                {
                    'destination': '<(module_path)',
                    'files': [
                        '<(PRODUCT_DIR)/<(module_name).node',
                        '<!@(ls -1 /usr/lib/libboost_system*.so*)',
                        '<!@(ls -1 /usr/lib/libboost_filesystem*.so*)',
                    ]
                }
            ]
        }
    ]
}
