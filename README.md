# httpc
(REALLY) Manimalistic http client.

## Goal
Trying to create the smallest + most portable linux HTTP client of all time.
using nothing but the std-unix C libraries.

## Updates
### 12 dec 2023 -
as it is for now, the program can parse URLs + send GET reuqests to any address with the HTTP protocol.

## Usage
```bash
./httpc -h # to show usage
./httpc [URL]
./httpc http://localhost:8080/lol.html # get /lol.html from localhost:8080
```

## Compilition
I have created a bash script that can build the program (`build.sh`). to use it run:
```bash
CC=[compiler] ./build.sh [outfile] # default out file name is `httpc`
CC=gcc ./build.sh # compile using gcc to outfile called `httpc`
CC=musl-gcc ./build.sh client # compile using musl to outfile called `httpc` 
```
The script is compiling the program staticly, and then strips it.
#### note.
I recommend using the `musl-gcc` program to compile - smaller binaries + no static compilation problems.
