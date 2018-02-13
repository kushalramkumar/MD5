# MD5

The MD5 algorithm is a widely used hash function producing a 128-bit hash value designed by Ronald Rivest in 1991 to replace MD4. The design is described in [RFC1321](https://www.ietf.org/rfc/rfc1321.txt).

## Getting Started

### Prerequisites

It's just one C file. Compile it and run in an environment of choice!

```
I've used Microsoft Visual Studio 12 as the IDE for development.
```

### Installing

The project uses CMake to create an MSVC solution file. It works just as well with MinGW or Unix Makefiles.

```
cmake . -Bbuild -G"Visual Studio 12"
```

The solution file is created in "build/", load it, compile and run!

## Built With

* [Miscrosoft Visual Studio 12](https://www.visualstudio.com/) - IDE


## Authors

* **Kushal Ramkumar** - [kushalramkumar](https://github.com/kushalramkumar)

## Acknowledgments

* The implementation is based on [RFC1321](https://www.ietf.org/rfc/rfc1321.txt)
