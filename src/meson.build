

libedupals_base=shared_library('edupals-base', ['system.cpp','filesystem.cpp','cmd.cpp'],
    cpp_args:'-std=c++11',
    include_directories: headers
    )
    
libedupals_testing=executable('testing',['testing.cpp'],
    cpp_args:'-std=c++11',
    include_directories: headers,
    link_with: libedupals_base
    )

