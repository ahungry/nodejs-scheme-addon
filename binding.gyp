{
  "variables": {
    "guile_root%": "/usr/include/guile/2.2"
  },
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "hello.cc" ],
      "include_dirs": [
        "<@(guile_root)",
        "<!(node -e \"require('nan')\")"
      ],
      "link_settings": {
        "libraries": [
          "<!(guile-config link)"
        ],
        "ldflags": [
          "<!(guile-config link)"
        ]
      },
      "cflags": [
        "-std=c++11",
        "<!(guile-config compile)"
      ],
      "cflags_cc!": [
        "-fno-rtti",
        "-fno-exceptions"
      ]
    }
  ]
}
