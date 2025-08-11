# Code for simulating cluster fragmentation in Mass Spectrometer

## Example

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
./bin/skimmer_win.x < inputs/example
./bin/densityandrate_win.x < inputs/example
./bin/apitof_pinhole.x < inputs/example
```

Outputs are generated in [work/out](./work/out/) directory.
