# FAQ:
---------------------
## How does PyShiftAE work?
- `PyShiftAE` works by exposing a custom python library that can communicate between processes, effectively establishing communication between python and After Effects. 
Anything you could script with extendscript, you can script with `PyShiftAE`.

## Is this a wrapper around Extendscript?
- No, This is not a wrapper around extendscript. `PyShiftAE` using `pybind11` to wrap the after effects c++ SDK, and expose python classes and functions. This had to be written from ground up, which also gives the opportunity to improve on some shortcomings in the extendscript API.

## Can I use other python libraries?
- Yes! In fact, `PyShiftAE` is designed to allow you to! In doing so, we hope to greatly expand the realms of possiblity within After Effects Scripting.