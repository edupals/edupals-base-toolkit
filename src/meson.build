

libedupals_system=shared_library('edupals-system', ['system.cpp'],
    cpp_args:'-std=c++11',
    include_directories: headers
    )
    
libedupals_testing=executable('testing',['testing.cpp'],
    cpp_args:'-std=c++11',
    include_directories: headers,
    link_with: libedupals_system
    )

