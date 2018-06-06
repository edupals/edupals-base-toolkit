

libedupals_base=shared_library('edupals-base', ['system.cpp','network.cpp','cmd.cpp','process.cpp'],
    cpp_args:'-std=c++1z',
    include_directories: headers,
    link_args:'-lstdc++fs'
    )
    
libedupals_testing=executable('testing',['testing.cpp'],
    cpp_args:'-std=c++1z',
    include_directories: headers,
    link_with: libedupals_base
    )

