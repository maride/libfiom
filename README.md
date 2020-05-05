# libfiom

A set of function overrides to track file input/output calls of binaries, bundled in a shared library to preload it.

## Build

Simply run `make build`. This will build the library as a 64bit and 32bit shared object. The resulting library can be found in the directory `out/`.

If you have trouble compiling the 32bit target on a 64bit machine (missing `bits/libc-header-start.h` and others), you probably need to install 32bit headers (see also [askubuntu](https://askubuntu.com/a/106092))

## Run and use

After a successful build, the library can be used like this:
`LD_PRELOAD=./out/libfoam cat /etc/hostname`

## Examples

### Cats don't read

To prohibit `cat` to read, execute it like this:

`LD_PRELOAD=./out/libfiom-64.so LIBFIOM_LOOSE_FAKEREAD=TRUE cat /etc/hostname`

The output should be empty.

## Configuration

Configuration can currently only be done "on-the-fly", by setting environment variables before execution.

| Config name | Values | Description |
| --- | --- | --- |
| `LIBFIOM_LOGSTDOUT` | `TRUE`, `FALSE` | If set to true, logs to `stdout` |
| `LIBFIOM_FULL_FAKEWRITE` | `TRUE`, `FALSE` | If set to true, simulates a successful `write()` call without writing. *This includes `stdout`* |
| `LIBFIOM_LOOSE_FAKEWRITE` | `TRUE`, `FALSE` | If set to true, simulates a successful `write()` call without writing. *This excludes `stdout`*. Chances are high that you want this kind of `write()` behaviour instead of `FULL_FAKEWRITE`, `FULL_NOWRITE` or `LOOSE_NOWRITE`. |
| `LIBFIOM_FULL_NOWRITE` | `TRUE`, `FALSE` | If set to true, simulates a failed `write()` call. *This includes `stdout`* |
| `LIBFIOM_LOOSE_NOWRITE` | `TRUE`, `FALSE` | If set to true, simulates a failed `write()` call. *This excludes `stdout`* |
| `LIBFIOM_FULL_FAKEREAD` | `TRUE`, `FALSE` | If set to true, simulates a successful `read()` call without reading. *This includes `stdin`* |
| `LIBFIOM_LOOSE_FAKEREAD` | `TRUE`, `FALSE` | If set to true, simulates a successful `read()` call without reading. *This excludes `stdin`*. Chances are high that you want this kind of `read()` behaviour instead of `FULL_FAKEREAD`, `FULL_NOREAD` or `LOOSE_NOREAD`. |
| `LIBFIOM_FULL_NOREAD` | `TRUE`, `FALSE` | If set to true, simulates a failed `read()` call. *This includes `stdin`* |
| `LIBFIOM_LOOSE_NOREAD` | `TRUE`, `FALSE` | If set to true, simulates a failed `read()` call. *This excludes `stdin`* |
| `LIBFIOM_LIMIT_SCOPE_TO` | `<string>` | If set, limits all activity to the named executable, and let calls from other executables pass through without any modification. |
