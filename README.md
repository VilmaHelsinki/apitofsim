# Code for simulating cluster fragmentation

## Using Meson to compile the Python extension

[meson-python](https://mesonbuild.com/meson-python/) is used to build the Python extension.

You need disable build isolation to install this as a development/editable package which will recompile at import:

Using [uv](https://docs.astral.sh/uv/):

```bash
uv sync
uv pip install --no-build-isolation -e .
```

During development you can also run the following to get compiler errors on import, and add debug symbols:

```bash
uv pip install --no-build-isolation --config-settings=editable-verbose=true -Csetup-args=-Dbuildtype=debug -Cbuild-dir=build-dbg -e .
```

## Using Meson to compile the executables

```bash
meson setup --buildtype release build
meson compile -C build
```

The binaries are then in `build/src`. You add this directory to your PATH or symlink to them.

This will also create a compilation database that `clangd` can use.

## Old instructions (without Meson)

First make the executables with

```bash
cd source
make
```

Then run the simulation with the provided input and data using `run.sh`:

```bash
./run.sh inputs/example
```

You can also run each step manually with:

```bash
./bin/skimmer_win.x < inputs/example/config.in
./bin/densityandrate_win.x < inputs/example/config.in
./bin/apitof_pinhole.x < inputs/example/config.in
```

Outputs are generated in [work/out](./work/out/) directory.
