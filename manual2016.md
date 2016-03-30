# 明畠2015-16年度研究成果物の利用方法について

## 成果物の構成

ライブラリと実験用ソースの二つに大分される．

### ライブラリ

- TAKAO+ccstm
  - ライブラリ本体
  - ccstmもここに含まれる
- libnative.so
  - ネイティブライブラリ群の共有ライブラリ
- PJamVM 
  - NVM用に変更を加えられたJVM
- GNU Classpath
  - PJamVMはGNU Classpathのみに対応している
  - そのため，openjdkを用いることができない

### 実験用ソース

- ScalaTest
  - 実際に使用した実験ソース群
- JavaTest
  - 主に動作確認用に使用したソース群


## セットアップ

poweredge7に構築された仮想マシン(akehata-Reserch-subject-image)はすでにこれらのセットアップが行われているため，以下を行う必要はない．
以下は，それ以外のPCで実験を行うための前処理である．

### 1.GNU Classpathのビルド

1. [こちら](http://www.gnu.org/software/classpath/)よりソースをダウンロードし，任意のディレクトリに展開する．

2. 展開先へ移動し，以下を実行
   
   ```bash
   ./configure --disable-gtk-peer --disable-gconf-peer --disable-plugin
   ```

3. エラーが起こらなければビルドしてインストールを行う
   
   ```bash
   make
   sudo make install
   ```

4. configure実行時にエラーが生じた場合はエラーメッセージを参照し，必要なものを別途インストールする
   
   例:antlrがないという旨のエラーが出た場合
   ```bash
   sudo apt-get install antlr
   ```
   
### 2.PJamVMのビルド

1. PJamVMのソースをダウンロードし，そのディレクトリへ移動する．
   
   ソースはmicrosrvの`/home/akehata/`以下にあるほか，githubにもアップロードしている．
   
2. 以下を実行

   ```bash
   ./configure --with-java-runtime-library=gnuclasspath
   ```
   
3. srcディレクトリに移動し，config.hというヘッダファイルをエディタで開き，113行目をコメントアウトする
   
   ```c
   /* interpreter inlining */
   #define INLINING 1 //この行をコメントアウトする
   ```
   
   ```c
   /* interpreter inlining */
   //#define INLINING 1
   ```
   これはインライン展開を無効にするための処理である．
   
4. ビルドする

   ```bash
   make
   sudo make install
   ```
   ビルドされたバイナリは特に指定しなければ`/usr/local/jamvm/bin/`に格納される
   
5. パスを通す

   ```bash:
   export PATH=$PATH:/usr/local/jamvm/bin/
   ```
   
6. 以下のコマンドを実行し，PJamVMが起動すればインストール完了である

   ```bash
   jamvm -h
   ```

### 3.sbtのインストール

1. [公式サイト](http://www.scala-sbt.org/0.13/docs/Installing-sbt-on-Linux.html)の手順に従う

   ```bash
   echo "deb https://dl.bintray.com/sbt/debian /" | sudo tee -a /etc/apt/sources.list.d/sbt.list
   sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 642AC823
   sudo apt-get update
   sudo apt-get install sbt
   ```


### 4.ライブラリの準備

1. microsrvより`takao.jar`と`libnative.so`をコピーする．この二つは`/home/akehata/`下にある．

2. `libnative.so`を任意のディレクトリへ移動させる．その後，`LD_LIBRARY_PATH`に移動先を指定する．
   例えば，ホーム直下に`ldlib`というディレクトリを作成し，そこに移動させる場合は以下のようにする．

   ```bash
   mkdir ~/ldlib
   cp libnative.so ~/ldlib
   export LD_LIBRARY_PATH=$HOME/ldlib
   ```
   
3. `takao.jar`を実行時に`-cp`オプションでクラスパス内に含めれば利用可能となる


## 利用方法

### ライブラリを用いたプログラミング方法
   
   修士論文参照のこと
   
### PJamVMの実行時オプションについて

   Javaの一般的な実行時オプションはほぼすべて使用可能である．
   以下はPJamVM専用のものである．
   
| オプション | 説明 |
| :---: | :---: |
| `-p` | 再実行可能な永続モードで実行する．マップファイルへの名前付けは行われない |
| `-persistence` | 上記と同様 |
| `-persistence:filename` | マップファイルへの名前付けを行う永続モード |
| `-testmode` | 詳細出力モード．gcなどの情報が詳細に表示されるが，実行速度は大幅に悪化する |
   
### Scalaについて

   通常のScalaの起動方法は以下のとおりである(fooは実行するプログラム名)

   ```bash
   scala foo
   ```
   
   これは以下と同義である
   
   ```bash
   java -cp:path-to-scalalibrary foo
   ```
   
   すなわち，このScala起動方法は通常のJavaの起動方法に，Scalaライブラリへのパスを追加したものと同じである．
   そのため，PJamVMを用いてScalaを実行する場合も同様にする．
   
   ```bash
   jamvm -cp:path-to-scalalibrary foo
   ```
   
   なお，コンパイルも同様である．
   一般的にはこれの代わりに**sbt**と呼ばれるビルドツールを用いてコンパイル，およびプログラムの起動を行う．
   
### sbtについて

   sbtはScala，Java向けに作られたビルドツールである
   依存性などのライブラリ管理，テスト，JVMのチューニングのほか，詳細な設定を行うことができる
   本研究のScalaの実験はこれを用いてビルド，実行を行っている．これは，ライブラリ管理とクラスパスの設定を正確に行うためである
   詳細は[公式マニュアル](http://www.scala-sbt.org/0.13/docs/ja/Getting-Started.html)(日本語)の説明が最も易しく，正確なので参照願いたい

#### 本研究のライブラリをsbtで用いるには

   プロジェクトのルートに`lib`ディレクトリを作成し，そこに`ccstm.jar`と`takao.jar`を格納する
   これで利用が可能となる．ただし，PJamVMを使用するには特別な手順が必要となる．
   まず，任意のディレクトリにjamvmのバイナリへのシンボリックリンクを作成する．この際，リンクの名前を**必ずjavaとしなければならない**
   
   ```bash
   mkdir java_jamvm | cd java_jamvm
   ln /usr/local/jamvm/bin/jamvm java
   ```
   
   次にプロジェクト内に`build.sbt`というファイルを作成し，以下のように記述する．
   
   ```scala
   scalaVersion := "2.8.2" //ccstmがこのバージョン以上ののscalaに対応していないため

   name := "ここはなんでもよい"
   
   fork in run := true

   javaHome in run := Some(file("$HOME/java_jamvm")) //ここに先ほどシンボリックリンクを作成した場所を指定
   
   /* ここにはPJamVMの実行時オプションを記述する */
   javaOptions in run ++= Seq(
     "-p",
     "-testmode",
     "-Djava.library.path=$HOME/ldlib/" //libnative.soが格納されているディレクトリを指定
   )

   initialize ~= { _ =>
      System.setProperty("java.library.path","$HOME/ldlib/") //libnative.soが格納されているディレクトリを指定
      println(System.getProperty("java.library.path"))
   }
   ```
   `build.sbt`
   
   これで本研究のライブラリが利用可能となる．
   なお，実験で使用したScalaTestディレクトリにはこれらの準備はすでにすべて行われている．
   
   
#### sbtによるプログラムのコンパイル，実行方法

   sbtコマンドを用いる．これらは対象のプロジェクトのディレクトリ直下で行う
   
- プログラムのコンパイル

   ```bash
   sbt compile
   ```

- プログラムの実行

   これが可能なのはプログラム内にmain関数があるときのみ
   
   ```bash
   sbt run
   ```
   
- プログラムのパッケージ化

   ```bash
   sbt package
   ```
   
   生成されたjarは`target`ディレクトリ以下に格納される
   
- テストの実行

   ```bash
   sbt test
   ```
   


## 実験の実行方法

   実験に用いたソースはそれぞれホーム直下にある`ScalaTest`,`JavaTest`の二つのディレクトリに格納されている
   
### 実験を行う上での注意

   永続モードで実行するとマップファイルとなるHeapimage等が出力される．これらを削除するためのスクリプトが**prm**ホームカレントリの`bin`の中にある．
   以下のように使用する．
   ```bash
   jamvm -p -testmode -cp .:takao.jar foo
   echo 'Remove map File'
   prm
   ```
   
### ScalaTestに関する注意

   一貫性に関する実験(`failturetest`)と速度測定の実験(`speedtest`)が入っている．
   速度測定の実験を行うにはディレクトリへ移動し，

   ```bash
   sbt run
   ```
   
   とすればよい．ただし，一貫性に関する実験は特別な手段が必要になる．以下に手順を示す
   
   1. コンパイル
   
   必ず事前にコンパイルする必要がある．
   
   ```bash
   sbt compile
   ```
   
   2. 別ターミナルの起動
   
   シグナルを送信するための別のターミナルを起動する
   
   3. スクリプトの実行
   
   `failturetest`ディレクトリで`test.sh`を，先ほど立ち上げた別ターミナルで`randomkill.sh`をそれぞれ**同時に**実行する．
   `randomkill.sh`はホームカレントリの`bin`の中にある．
   