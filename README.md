#Team Project 2 - Group 12 ARM Code#
------------------------------------
The goal of this project was to build a *MPC* with the [Stellaris Launchpad](http://ti.com/stellaris-launchpad) platform. A proper parts list and PCB schematic will in time.

#Usage#
-------
__Requirements for linux__
    
* [ARM GCC Toolchain](https://launchpad.net/gcc-arm-embedded)
* [lm4tools](https://github.com/utzig/lm4tools)
* [TI's StellarisWare](https://github.com/yuvadm/stellaris)

__Toolchain Installation__
The *ARM GCC Toolchain* provided above is precompiled, simply unpack it and add it to your path.

    cd ~/bin
    tar -xvf gcc-arm-none-eabi...tar.bz2
    echo "export PATH=$PATH:~/bin/gcc-arm-none-eabi/bin" >> ~/.zshrc

*lm4tools* provides the flash programmer.
    
    git clone https://github.com/utzig/lm4tools
    cd lm4tools/lm4flash
    make
    chmod +x lm4flash
    sudo cp lm4flash /usr/bin

*StellarisWare* includes all the libraies and utilities provided by TI.

    git clone https://github.com/yuvadm/stellaris
    cd stellaris/boards/ek-lm4f120xl/project0
    ...
    make
    lm4flash

__Building with linux__  

    $ cd project
    $ make
    $ make flash

__Cloning with Stellaris Submodule__  
The *StellarisWare* library was included as a submodule. This module may not be needed if the depencies are already met. 

    $ git clone https://github.com/ichpuchtli/tp2_arm.git
    $ cd tp2_arm
    $ git submodule init
    $ git submodule update


__Checking out different branches__   
A separate CSS branch may be created with frequent merges onto the master branch.

    $ git checkout ccsdev
    ...
    $ git commit -m 'Debug/Fix timing issue with CCS's In-System debug'


__Pushing changes to the github remote__  

    $ vi tp2_arm.c
    ...
    $ git commit
    $ git push origin "branch* 

__Pulling changes from the github remote__  

    $ git reset
    $ git pull origin *branch*


#Licence#
---------
The submodule *stellaris* is licenced under its original terms. The code in this repo however is licenced under GPL/3.0 or later.

#Authors#
----------
Samuel Macpherson & Jake Verry
