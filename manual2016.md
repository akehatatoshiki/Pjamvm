# ����2015-16�N�x�������ʕ��̗��p���@�ɂ���

## ���ʕ��̍\��

���C�u�����Ǝ����p�\�[�X�̓�ɑ啪�����D

### ���C�u����

- TAKAO+ccstm
  - ���C�u�����{��
  - ccstm�������Ɋ܂܂��
- libnative.so
  - �l�C�e�B�u���C�u�����Q�̋��L���C�u����
- PJamVM 
  - NVM�p�ɕύX��������ꂽJVM
- GNU Classpath
  - PJamVM��GNU Classpath�݂̂ɑΉ����Ă���
  - ���̂��߁Copenjdk��p���邱�Ƃ��ł��Ȃ�

### �����p�\�[�X

- ScalaTest
  - ���ۂɎg�p���������\�[�X�Q
- JavaTest
  - ��ɓ���m�F�p�Ɏg�p�����\�[�X�Q


## �Z�b�g�A�b�v

poweredge7�ɍ\�z���ꂽ���z�}�V��(akehata-Reserch-subject-image)�͂��łɂ����̃Z�b�g�A�b�v���s���Ă��邽�߁C�ȉ����s���K�v�͂Ȃ��D
�ȉ��́C����ȊO��PC�Ŏ������s�����߂̑O�����ł���D

### 1.GNU Classpath�̃r���h

1. [������](http://www.gnu.org/software/classpath/)���\�[�X���_�E�����[�h���C�C�ӂ̃f�B���N�g���ɓW�J����D

2. �W�J��ֈړ����C�ȉ������s
   
   ```bash
   ./configure --disable-gtk-peer --disable-gconf-peer --disable-plugin
   ```

3. �G���[���N����Ȃ���΃r���h���ăC���X�g�[�����s��
   
   ```bash
   make
   sudo make install
   ```

4. configure���s���ɃG���[���������ꍇ�̓G���[���b�Z�[�W���Q�Ƃ��C�K�v�Ȃ��̂�ʓr�C���X�g�[������
   
   ��:antlr���Ȃ��Ƃ����|�̃G���[���o���ꍇ
   ```bash
   sudo apt-get install antlr
   ```
   
### 2.PJamVM�̃r���h

1. PJamVM�̃\�[�X���_�E�����[�h���C���̃f�B���N�g���ֈړ�����D
   
   �\�[�X��microsrv��`/home/akehata/`�ȉ��ɂ���ق��Cgithub�ɂ��A�b�v���[�h���Ă���D
   
2. �ȉ������s

   ```bash
   ./configure --with-java-runtime-library=gnuclasspath
   ```
   
3. src�f�B���N�g���Ɉړ����Cconfig.h�Ƃ����w�b�_�t�@�C�����G�f�B�^�ŊJ���C113�s�ڂ��R�����g�A�E�g����
   
   ```c
   /* interpreter inlining */
   #define INLINING 1 //���̍s���R�����g�A�E�g����
   ```
   
   ```c
   /* interpreter inlining */
   //#define INLINING 1
   ```
   ����̓C�����C���W�J�𖳌��ɂ��邽�߂̏����ł���D
   
4. �r���h����

   ```bash
   make
   sudo make install
   ```
   �r���h���ꂽ�o�C�i���͓��Ɏw�肵�Ȃ����`/usr/local/jamvm/bin/`�Ɋi�[�����
   
5. �p�X��ʂ�

   ```bash:
   export PATH=$PATH:/usr/local/jamvm/bin/
   ```
   
6. �ȉ��̃R�}���h�����s���CPJamVM���N������΃C���X�g�[�������ł���

   ```bash
   jamvm -h
   ```

### 3.sbt�̃C���X�g�[��

1. [�����T�C�g](http://www.scala-sbt.org/0.13/docs/Installing-sbt-on-Linux.html)�̎菇�ɏ]��

   ```bash
   echo "deb https://dl.bintray.com/sbt/debian /" | sudo tee -a /etc/apt/sources.list.d/sbt.list
   sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 642AC823
   sudo apt-get update
   sudo apt-get install sbt
   ```


### 4.���C�u�����̏���

1. microsrv���`takao.jar`��`libnative.so`���R�s�[����D���̓��`/home/akehata/`���ɂ���D

2. `libnative.so`��C�ӂ̃f�B���N�g���ֈړ�������D���̌�C`LD_LIBRARY_PATH`�Ɉړ�����w�肷��D
   �Ⴆ�΁C�z�[��������`ldlib`�Ƃ����f�B���N�g�����쐬���C�����Ɉړ�������ꍇ�͈ȉ��̂悤�ɂ���D

   ```bash
   mkdir ~/ldlib
   cp libnative.so ~/ldlib
   export LD_LIBRARY_PATH=$HOME/ldlib
   ```
   
3. `takao.jar`�����s����`-cp`�I�v�V�����ŃN���X�p�X���Ɋ܂߂�Η��p�\�ƂȂ�


## ���p���@

### ���C�u������p�����v���O���~���O���@
   
   �C�m�_���Q�Ƃ̂���
   
### PJamVM�̎��s���I�v�V�����ɂ���

   Java�̈�ʓI�Ȏ��s���I�v�V�����͂قڂ��ׂĎg�p�\�ł���D
   �ȉ���PJamVM��p�̂��̂ł���D
   
| �I�v�V���� | ���� |
| :---: | :---: |
| `-p` | �Ď��s�\�ȉi�����[�h�Ŏ��s����D�}�b�v�t�@�C���ւ̖��O�t���͍s���Ȃ� |
| `-persistence` | ��L�Ɠ��l |
| `-persistence:filename` | �}�b�v�t�@�C���ւ̖��O�t�����s���i�����[�h |
| `-testmode` | �ڍ׏o�̓��[�h�Dgc�Ȃǂ̏�񂪏ڍׂɕ\������邪�C���s���x�͑啝�Ɉ������� |
   
### Scala�ɂ���

   �ʏ��Scala�̋N�����@�͈ȉ��̂Ƃ���ł���(foo�͎��s����v���O������)

   ```bash
   scala foo
   ```
   
   ����͈ȉ��Ɠ��`�ł���
   
   ```bash
   java -cp:path-to-scalalibrary foo
   ```
   
   ���Ȃ킿�C����Scala�N�����@�͒ʏ��Java�̋N�����@�ɁCScala���C�u�����ւ̃p�X��ǉ��������̂Ɠ����ł���D
   ���̂��߁CPJamVM��p����Scala�����s����ꍇ�����l�ɂ���D
   
   ```bash
   jamvm -cp:path-to-scalalibrary foo
   ```
   
   �Ȃ��C�R���p�C�������l�ł���D
   ��ʓI�ɂ͂���̑����**sbt**�ƌĂ΂��r���h�c�[����p���ăR���p�C���C����уv���O�����̋N�����s���D
   
### sbt�ɂ���

   sbt��Scala�CJava�����ɍ��ꂽ�r���h�c�[���ł���
   �ˑ����Ȃǂ̃��C�u�����Ǘ��C�e�X�g�CJVM�̃`���[�j���O�̂ق��C�ڍׂȐݒ���s�����Ƃ��ł���
   �{������Scala�̎����͂����p���ăr���h�C���s���s���Ă���D����́C���C�u�����Ǘ��ƃN���X�p�X�̐ݒ�𐳊m�ɍs�����߂ł���
   �ڍׂ�[�����}�j���A��](http://www.scala-sbt.org/0.13/docs/ja/Getting-Started.html)(���{��)�̐������ł��Ղ����C���m�Ȃ̂ŎQ�Ɗ肢����

#### �{�����̃��C�u������sbt�ŗp����ɂ�

   �v���W�F�N�g�̃��[�g��`lib`�f�B���N�g�����쐬���C������`ccstm.jar`��`takao.jar`���i�[����
   ����ŗ��p���\�ƂȂ�D�������CPJamVM���g�p����ɂ͓��ʂȎ菇���K�v�ƂȂ�D
   �܂��C�C�ӂ̃f�B���N�g����jamvm�̃o�C�i���ւ̃V���{���b�N�����N���쐬����D���̍ہC�����N�̖��O��**�K��java�Ƃ��Ȃ���΂Ȃ�Ȃ�**
   
   ```bash
   mkdir java_jamvm | cd java_jamvm
   ln /usr/local/jamvm/bin/jamvm java
   ```
   
   ���Ƀv���W�F�N�g����`build.sbt`�Ƃ����t�@�C�����쐬���C�ȉ��̂悤�ɋL�q����D
   
   ```scala
   scalaVersion := "2.8.2" //ccstm�����̃o�[�W�����ȏ�̂�scala�ɑΉ����Ă��Ȃ�����

   name := "�����͂Ȃ�ł��悢"
   
   fork in run := true

   javaHome in run := Some(file("$HOME/java_jamvm")) //�����ɐ�قǃV���{���b�N�����N���쐬�����ꏊ���w��
   
   /* �����ɂ�PJamVM�̎��s���I�v�V�������L�q���� */
   javaOptions in run ++= Seq(
     "-p",
     "-testmode",
     "-Djava.library.path=$HOME/ldlib/" //libnative.so���i�[����Ă���f�B���N�g�����w��
   )

   initialize ~= { _ =>
      System.setProperty("java.library.path","$HOME/ldlib/") //libnative.so���i�[����Ă���f�B���N�g�����w��
      println(System.getProperty("java.library.path"))
   }
   ```
   `build.sbt`
   
   ����Ŗ{�����̃��C�u���������p�\�ƂȂ�D
   �Ȃ��C�����Ŏg�p����ScalaTest�f�B���N�g���ɂ͂����̏����͂��łɂ��ׂčs���Ă���D
   
   
#### sbt�ɂ��v���O�����̃R���p�C���C���s���@

   sbt�R�}���h��p����D�����͑Ώۂ̃v���W�F�N�g�̃f�B���N�g�������ōs��
   
- �v���O�����̃R���p�C��

   ```bash
   sbt compile
   ```

- �v���O�����̎��s

   ���ꂪ�\�Ȃ̂̓v���O��������main�֐�������Ƃ��̂�
   
   ```bash
   sbt run
   ```
   
- �v���O�����̃p�b�P�[�W��

   ```bash
   sbt package
   ```
   
   �������ꂽjar��`target`�f�B���N�g���ȉ��Ɋi�[�����
   
- �e�X�g�̎��s

   ```bash
   sbt test
   ```
   


## �����̎��s���@

   �����ɗp�����\�[�X�͂��ꂼ��z�[�������ɂ���`ScalaTest`,`JavaTest`�̓�̃f�B���N�g���Ɋi�[����Ă���
   
### �������s����ł̒���

   �i�����[�h�Ŏ��s����ƃ}�b�v�t�@�C���ƂȂ�Heapimage�����o�͂����D�������폜���邽�߂̃X�N���v�g��**prm**�z�[���J�����g����`bin`�̒��ɂ���D
   �ȉ��̂悤�Ɏg�p����D
   ```bash
   jamvm -p -testmode -cp .:takao.jar foo
   echo 'Remove map File'
   prm
   ```
   
### ScalaTest�Ɋւ��钍��

   ��ѐ��Ɋւ������(`failturetest`)�Ƒ��x����̎���(`speedtest`)�������Ă���D
   ���x����̎������s���ɂ̓f�B���N�g���ֈړ����C

   ```bash
   sbt run
   ```
   
   �Ƃ���΂悢�D�������C��ѐ��Ɋւ�������͓��ʂȎ�i���K�v�ɂȂ�D�ȉ��Ɏ菇������
   
   1. �R���p�C��
   
   �K�����O�ɃR���p�C������K�v������D
   
   ```bash
   sbt compile
   ```
   
   2. �ʃ^�[�~�i���̋N��
   
   �V�O�i���𑗐M���邽�߂̕ʂ̃^�[�~�i�����N������
   
   3. �X�N���v�g�̎��s
   
   `failturetest`�f�B���N�g����`test.sh`���C��قǗ����グ���ʃ^�[�~�i����`randomkill.sh`�����ꂼ��**������**���s����D
   `randomkill.sh`�̓z�[���J�����g����`bin`�̒��ɂ���D
   