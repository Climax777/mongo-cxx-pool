mongo-cxx-pool
==============

A simple connection pool for mongo's C++ client driver. This is strongly based on the old ScopedDBConnection pool the driver used to provide.

This pool can handle authentication also. Please note that `authSource` and `authMechanism` must be specified.

This library requires a C++-11 compatible compiler.
