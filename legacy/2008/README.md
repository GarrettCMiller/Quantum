# Quantum: 2008 source snapshot

This directory preserves source and shader files from a smaller Quantum archive
dated around 2008. It is included to show an earlier stage of the project; it
is not established to be a complete, independently buildable Visual Studio
project. The active later source remains in the repository's `Quantum/`
directory.

`States/3BodyOrbit.pfx` is a 55-byte Quantum saved configuration recovered from
the separate, larger archive. Static inspection of the older file layout is
consistent with three particles, gravity enabled, and random seed 7. It is not
a trajectory recording, and it has not been loaded to verify the result. The
later program changed its binary serialization layout, so compatibility must
be checked before loading or converting it.

This specific `.pfx` file is Quantum data, not a signing certificate. The
repository's `.gitignore` excludes `*.pfx`, so this exact file is deliberately
tracked; that ignore rule still protects other untracked files with the same
extension. Do not treat every `.pfx` as safe to publish.
