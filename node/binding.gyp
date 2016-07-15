{
    "targets": [
        {
            "target_name": "snapsync",
            "sources": [
                "../src/node.cc"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ],
            "cflags": [
                "-std=c++11"
            ],
            'cflags!': [ '-fno-exceptions' ],
            'cflags_cc!': [ '-fno-exceptions' ]
        }
    ]
}
