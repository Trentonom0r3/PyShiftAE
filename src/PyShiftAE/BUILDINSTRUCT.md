# Instructions for building

(Please Let me Know if you try this out and run into any errors)

Once you clone the repo and open the PyShiftAE.vcxproj in Visual Studio, everything _should_ be linked properly. 

Configurations for both debug and release modes included.

## External Dependencies
- Outside of standard c++ 17 libs, you'll need pybind11 x64-static, and whatever python libs/includes you're using. I used python 3.11. 

  - Or, if you end up helping to contributing by working on other OS, the equivalent.

### Extraneous Instructions

- Change the build output path under Properties->Configuration Properties->General->Output Directory
    - If you're familiar with AE plugin dev, you could use your ENV variable or just use the effect folder path.

- Change vc++ directories, header dirs, and linker settings. 

