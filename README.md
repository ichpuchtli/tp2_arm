#Team Project 2 - Group 12 ARM Code#
------------------------------------

The goal of this project was to build an *MPC* with the [Stellaris Launchpad](http://ti.com/stellaris-launchpad) platform. A part list and PCB schematic will be uploaded when this project is finished.

#Usage#
-------

##Requirements for linux##
    
* [ARM EABI Toolchain Builder](https://github.com/jsnyder/arm-eabi-toolchain)
* or [summonarm-toolchain](https://github.com/esden/summon-arm-toolchain)
* [lm4tools](https://github.com/utzig/lm4tools)
* [TI's StellarisWare](https://github.com/yuvadm/stellaris)


##Building with linux##

    $ cd project
    $ make
    $ make flash


##Cloning with Stellaris Submodule##
The StellarisWare library was included as a submodule. The libraries should not be changed or may not be needed if the depencies are already met. 

    $ git clone https://github.com/ichpuchtli/tp2_arm.git
    $ cd tp2_arm
    $ git submodule init
    $ git submodule update


##Checking out CSS branch##

A separate CSS branch may be created with frequent merges onto the master branch

    $ git checkout ccsdev
    ...
    $ git commit -m 'Debug/Fix timing issue with CCS'


##Pushing changes to the github remote##

    $ vi tp2_arm.c
    ...
    $ git commit
    $ git push origin *branch* 

##Pulling changes from github remote##

    git reset
    git pull origin *branch*


#Licence#
---------
The submodule *stellaris* is licenced under its original terms. The code in this repo however is licenced under GPL/3.0 or later.

#Authors#
----------
Samuel Macpherson & Jake Verry
