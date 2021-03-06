# whlib

##  0 简介

* 这是一个基于Eigen写的点云库；
* 支持obj格式点云文件的读写；

## 1 目录结构

* whilb
  * bin
    * mac
    * win
  * deps
    * boost
    * eigen
    * glm
  * include
    * algorithm
    * basic
    * utils
  * makefile
  * src
  * temp
  * test
  * README.md

### bin

存放可执行文件目录，包含mac和win两个目录。

### deps 

存放所依赖的第三方库源代码。

### include

whlib的头文件目录。

### makefile_temp 

存放临时的makefile文件。

### model

存放测试模型文件。

### src

存放whlib源代码的目录。

### temp 

存放临时文件。

### test

存放测试相关的文件。

## 其他

* 命名规则

  * 所有类使用大驼峰命名法，例如PointCloud；

  * 所有文件（包括头文件）命名使用下划线命名法，例如point_cloud（除某种算法的缩写使用大写，例如LOP）；

  * 所有成员变量（包括成员方法使用）命名使用小驼峰命名法，例如createUnorderedEdges();

    

