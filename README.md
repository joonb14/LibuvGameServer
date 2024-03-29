# Libuv 설치

[https://github.com/libuv/libuv](https://github.com/libuv/libuv)

WSL에서 설치했습니다 <br>
https://learn.microsoft.com/ko-kr/windows/wsl/install <br>
<br>
WSL에서 이 프로젝트를 실행하기위해선 아래 package들을 설치해야합니다
```bash
$ sudo apt-get update
$ sudo apt-get install autotools-dev
$ sudo apt-get install automake
$ sudo apt-get install libtool
$ sudo apt install make
$ sudo apt-get install build-essential gdb
```
WSL package 설치가 끝나면 이제 libuv를 설치합니다
```bash
git clone https://github.com/libuv/libuv.git
cd libuv
sh autogen.sh
./configure --disable-dependency-tracking
make
make check
sudo make install
```
- make check는 skip 하셔도됩니다.(해보실 분은 root 권한이면 fail하기 때문에 꼭 user 권한으로 실행하세요)
- make install은 꼭 sudo로 root 권한으로 실행해야합니다.(/usr/local/lib과 /usr/local/include에 파일들을 추가해주기 때문에 root 권한 필요)
    - 이렇게 해야 /usr/local/include에 uv.h가 추가되고 /usr/local/lib/libuv.a 가 생성됩니다!
    - 이 두가지 파일이 없으면 저희 프로젝트를 make할 수 없습니다
    - 여기까지가 설치 완료입니다

# How To Test

Ubuntu의 하나의 터미널에서 Makefile을 이용하여 make한 다음 Server 실행합니다.
make시 obj 폴더가 없으면 error가 발생하기때문에 처음 한번은 obj directory 생성 필요합니다.

```bash
$ git clone https://github.com/joonb14/LibuvGameServer.git
$ cd LibuvGameServer
$ mkdir obj
$ make
$ ./MetaGameServer
```
![serverStart](https://user-images.githubusercontent.com/30307587/156481341-81e649a5-805f-4e9f-a229-56c62c0ef514.png)

Windows에서 Unity Client를 실행해야합니다.<br>
아래 프로젝트에서 Client 폴더를 Unity에서 엽니다<br>
https://github.com/joonb14/LibuvGameServerClients<br>
Scene에 SampleScene을 실행합니다<br>
