#Team Project 2 - Group 12 ARM Code#
------------------------------------

The goal of this project was to build an *MPC* with the [Stellaris Launchpad](http://ti.com/stellaris-launchpad) platform. A part list and PCB schematic will be uploaded when this project is finished.

#Usage#
-------

__Requirements for linux__
    
* [ARM EABI Toolchain Builder](https://github.com/jsnyder/arm-eabi-toolchain)
* or [summon-arm-toolchain](https://github.com/esden/summon-arm-toolchain)
* [lm4tools](https://github.com/utzig/lm4tools)
* [TI's StellarisWare](https://github.com/yuvadm/stellaris)


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
