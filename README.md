# libfiom

A set of function overrides to track file input/output calls of binaries, bundled in a shared library to preload it.

## Build

Simply run `make build`. The resulting library can be found in the directory `out/`

## Run and use

After a successful build, the library can be used like this:
`LD_PRELOAD=./out/libfoam cat /etc/hostname`

## Configuration

Configuration can currently only be done "on-the-fly", by setting environment variables before execution.

| Config name | Values | Description |
| --- | --- | --- |
| `LIBFIOM_LOGSTDOUT` | `TRUE`, `FALSE` | If set to true, logs to `stdout` |

