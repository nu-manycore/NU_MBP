```
This repository is git large file system for git-hub.
So write how to checkout git lfs. 

i) if not install git-lfs, please install git-lfs as follows
  case 1: Ubuntu18.04
    > sudo apt-get update
    > sudo apt-get install git-lfs

  case 2: Windows 10
    Please access https://git-lfs.github.com/
    Click [Download v2.xx.yy(Windows)]
    Install git-lfs-windows-v2.xx.yy.exe

ii) make working directory and checkout NU_MBP
    > mkdir NU_MBP
    > cd NU_MBP
    > git init
    > git lfs install
    > git remote add origin https://github.com/nu-manycore/NU_MBP.git
    > git fetch
    > git merge origin/master

iii ) after checkout NU_MBP
    > git fetch
    > git merge origin/master

```
