# (agn)ostic (c)

agnc is a tool for transpiling C source code from and to equivalent languages
utilizing keywords in other languages. The currently supported languages are
Afrikaans (`za`), Spanish (`es`) and toki pona (`tok`).

## Building

```sh
sh build.sh
```

## How to use

```sh
./agnc to_za file.c # generates 'file.za.c'
./agnc from_tok file.tok.c # generates 'file.c'
```
