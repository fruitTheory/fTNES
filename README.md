# fTNES - NES Emulator

Emulation of the Nintendo Entertainment System, written in C  
Currently a 6502 microprocessor but PPU is on the way

## Features

- Full emulation of the legal CPU opcodes
- Emulation of primary system components I/O
- Support for NROM games, non memory-mapped

### Prerequisites

- A C 64-bit compiler (ex. GCC, Clang, MSVC)
- Potentially other dependencies (SDL)
- Basic make and lib linking knowledge

### Building

```mingw32-make``` ```make```

### Running

```
./ftnes path_to_rom
```

## Roadmap

- ~~Implement primary opcodes~~
- Timings and Interrupts
- Debug functions
- Build on top, NES components 
- Refine components
- Release version v0.01
- Streamline build

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change

## License

Licensed under the MIT License - [LICENSE.md](LICENSE.md) 

## Acknowledgments

- nesdev.org - obelisk's guide - Andrew Jacobs
- as65 - assembler by Andrew Jacobs
- masswerk.at - 6502 instruction set reference
- ca65 - Assembler by Ullrich von Bassewitz, Greg King
- hex editor - hexed.it ♡
- NES Documentation - Patrick Diskin 2004 
- mcs6500 family programming manual

