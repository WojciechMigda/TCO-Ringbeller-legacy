This directory is here for your convenience while building kernel.

To build do following:
- position yourself in this directory
- `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-Wall -Werror -march=native" ..` if you want optimized code, or just `cmake ..` if you want to do debugging.
- `make -j`


EXTRA:

* BOOST_ASIO_ENABLE_HANDLER_TRACKING - enable boost asio logs
