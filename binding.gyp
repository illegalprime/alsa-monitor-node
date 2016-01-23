{
    "targets": [
        {
            "target_name": "alsa",
            "sources": [
                "bindings.cpp",
                "alsa/monitor-bindings.cpp",
                "alsa/monitor.cpp"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ]
        }
    ]
}
