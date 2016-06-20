{
    "targets": [
        {
            "target_name": "alsa",
            "sources": [
                "bindings.cpp",
                "alsa/monitor-bindings.cpp",
                "alsa/monitor.cpp",
                "alsa/utils.cpp"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ],
            "libraries": [
                "-lasound"
            ],
            "cflags": [
                "-Wall",
                "-Wextra",
                "-pedantic",
                "-O3",
            ],
            "cflags_cc": [
                "-std=c++0x",
            ],
        }
    ]
}
