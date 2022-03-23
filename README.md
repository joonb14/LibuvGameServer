# Libuv 설치

[https://github.com/libuv/libuv](https://github.com/libuv/libuv)

저는 Ubuntu 18.04에서 설치했습니다

```bash
$ git clone https://github.com/libuv/libuv.git
$ cd libuv
$ sh autogen.sh
$ ./configure
$ make
$ make check
$ make install
```

- sh autogen.sh을 하기 위해서 여러가지를 설치해야합니다
    - (그때 안적어놔서 기억이 잘안납니다... 죄송합니다...)
    - sudo apt install ————— 하는게 4개정도됐던거같습니다(찾으시면 추가해주세요)
        - sudo apt-get install automake
        - sudo apt-get install libtool
        - sudo apt-get install m4
- make check는 skip 하셔도됩니다.(해보실 분은 root 권한이면 fail하기 때문에 꼭 user 권한으로 실행하세요)
- make install은 꼭 sudo로 root 권한으로 실행해야합니다.(/usr/local/lib과 /usr/local/include에 파일들을 추가해주기 때문에 root 권한 필요)
    - 이렇게 해야 /usr/local/include에 uv.h가 추가되고 /usr/local/lib/libuv.a 가 생성됩니다!
    - 이 두가지 파일이 없으면 저희 프로젝트를 make할 수 없습니다
    - 여기까지가 설치 완료입니다

# How To Test

먼저 clang과 clang-tools를 설치해야합니다 (scan-build 사용을 위해서)

```bash
$ sudo apt install clang
$ sudo apt install clang-tools
```

Ubuntu의 하나의 터미널에서 Makefile을 이용하여 make한 다음 Server 실행합니다.
make시 obj 폴더가 없으면 error가 발생하기때문에 처음 한번은 obj directory 생성 필요합니다.

```bash
$ scan-build make
$ ./MetaGameServer
```
![serverStart](https://user-images.githubusercontent.com/30307587/156481341-81e649a5-805f-4e9f-a229-56c62c0ef514.png)

Windows에서 Unity Client를 실행해야합니다.
아래 프로젝트에서 Client 폴더를 Unity에서 엽니다(꼭 devel-jb 브랜치를 받아야합니다)
http://192.168.155.24:32080/mv2/mv2-2/csharpgameserver/-/tree/devel-jb
NetworkManager.cs 파일을 더블클릭해서 아래 부분을 수정합니다.
![NetworkManager](https://user-images.githubusercontent.com/30307587/158985763-5606ca04-cbce-4c47-8a1c-4e32d9b22c7a.png)
개인 Linux 개발환경의 수동 ip와 포트 7000번으로 수정합니다.

![Demo Video Link](https://s3.us-west-2.amazonaws.com/secure.notion-static.com/1148af25-b915-4669-a41d-98377a7cd693/%EB%85%B9%ED%99%94_2022_03_17_16_06_26_338.mp4?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Content-Sha256=UNSIGNED-PAYLOAD&X-Amz-Credential=AKIAT73L2G45EIPT3X45%2F20220318%2Fus-west-2%2Fs3%2Faws4_request&X-Amz-Date=20220318T102544Z&X-Amz-Expires=86400&X-Amz-Signature=fa3405ccfbea04c6caf63e40dfa48bddb61d3c056a121acc26f30777e8a4fbaf&X-Amz-SignedHeaders=host&response-content-disposition=filename%20%3D%22%25EB%2585%25B9%25ED%2599%2594_2022_03_17_16_06_26_338.mp4%22&x-id=GetObject)

# TODO List

- [x]  필요없는 파일들 함수들 덜어내기
- [x]  Coding Convention에 맞게 수정
- [x]  C 파일들 C++로 수정
- [x]  여러 .cpp 파일을 하나의 실행 파일로 make할 수 있는 Makefile 생성
- [x]  LibuvCore로 uv 함수들 main function에서 꺼내기
- [x]  현재 GameSession의 기능은 ClientPacketHandler로 이동? -> 보류
- [x]  GameSession과 Session은 접속중인 Client와 Server의 정보를 갖고 있도록 변경 -> Service가 Server의 정보를, Session이 Client의 정보를 저장
- [x]  Session(GameSession)을 ServerService에서 갖고있도록 수정
- [x]  gdb 디버거 사용법 조사, static code analysis(scan-build) 사용하도록 Makefile 수정
- [x]  memory leak 해결 -> scan-build로 static analysis, top 명령어를 이용해 Memory 확인(track)하는 memwatch.sh 작성
- [x]  Unity C# Client에서 MetaGameServer와 Packet을 주고 받는 기본 기능 구현
- [ ]  다중 Client 환경 Test용 DummyClient 구현
- [ ]  현재 BYTE* 에 data를 집어넣어 uv_write에 요청보내고 바로 BYTE* 를 delete하는데 추후에 이 방식은 문제가 생길 것으로 예상되므로 변경
- [ ]  EM → Thread 여러개 사용할 방법 조사
- [ ]  현재 프로젝트는 ServerService 자체를 하나의 Room 처럼 보고 동작하는데 
- [ ]  uv_read_start가 Async IO가 맞는지 조사