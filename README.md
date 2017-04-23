# GBsnake

Simple port of the Snake game written in C for the Nintendo GameBoy.

![Example](http://brovador.github.io/GBsnake/Demo.gif)

Compiled rom is under ```dist``` directory.

# Build (MacOSX)
* Download [gbdk v2.96a](http://www.rpgmaker.it/proflame/gbdk.zip) for MacOSX and the [bgb emulator](http://bgb.bircd.org/) for windows.

* Place both inside the same directory with the folder names ```gbdk``` and ```bgb```:

```
ROOT_DIRECTORY
   /gbdk
   /bgb
```

* Install ```wine``` using brew: 

```
brew install wine
```

* In the ```Makefile``` configure ```$(TOOLS_DIR)``` so it points to ```ROOT_DIRECTORY``` (which contains gbdk and bgb)
* Call ```make clean && make run```

