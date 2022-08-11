
# QuickCD - qcd

This command allows you to save directories under an alias,so you can cd to it quicker. Linux Only

## Installation

**First install [xdotool](https://github.com/jordansissel/xdotool)**

```bash
git clone https://github.com/amorcomputrum/QuickCD
cd QuickCD
git submodule init

mkdir build
cd build
cmake ../.
make
sudo mv qcd /bin
```

## Usage/Examples

```
[~]$ qcd set /usr/bin ubin
[~]$ qcd ubin
cd /usr/bin
[/usr/bin]$ pwd
/usr/bin
[/usr/bin]$
```


## Acknowledgements

 - [cpp-sqlitelib](https://github.com/yhirose/cpp-sqlitelib)
 - [xdotool](https://github.com/jordansissel/xdotool)

