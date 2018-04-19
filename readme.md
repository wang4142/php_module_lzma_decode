模块说明
===
本模块仅包含一个函数,函数原型如下
```php
string lzmadec( string )
```

## 函数作用说明
将使用lzma压缩的字符串,解压缩成原来的字符串

> 压缩后的字节流,已转化成以字符方式表达的16进制数

## 示例
```php
<?php
    print lzmadec( "5d0000080036000000000000000018e884062349398809b492109d9e102331fd033e3e3c224ca9bbe30d23c69021d6af84579a47f67f7cfffeb65500" ) ;
    print "\n" ;
?>
```
### 输出
```
1111 1231 1234 3456 4554 5472 5542 5692 5732 6723 6773
```

安装说明
===

**本模块仅在php7.1.2上安装测试过**

## 安装过程
```
tar xf lzma_decode.tgz
cd lzma_decode

vi makefile
# 将 makefile中的这一行改成正确的php.ini文件的位置
PHP_INI_FILE = /usr/local/php/etc/php.ini

make all
```


## 生成文件

- lzma_dec_str
  - 二进制测试文件
  - 目录 bin/
- liblzmadec.so
  - 共享库,供其他程序调用,将会拷贝到/lib64目录下, 此文件十分重要
  - 目录 build/
- lzmadec.so
  - php lzma 解码模块,将会拷贝到php模块目录下, 此文件十分重要
  - 目录 php_module_lzmadec/modules/

## 被修改文件

- php.ini
  - 自动添加
    - [lzmadec]
    - extension=lzmadec.so


附录: php7模块开发
===

## 进入php源码目录
```shell
cd /usr/local/software/php/php-7.1.2
cd ext/
```

## 添加函数原型
```shell
vi lzmadec.def

string lzmadec( string str )
```

## 创建php扩展模版
```shell
./ext_skel --extname=lzmadec --proto=lzmadec.def

# 进入 lzmadec 目录
cd lzmadec
# 修改config.m4 文件, 将 16 - 18 行前面的注释 dnl 去掉
PHP_ARG_ENABLE(lzmadec, whether to enable lzmadec support,
Make sure that the comment is aligned:
[  --enable-lzmadec           Enable lzmadec support])
```

## 修改 lzmadec.c 完成模块开发,注意要注释掉那个测试函数哦

## 编译安装
```shell
# 生成configure文件
phpize
# 编译
./configure 
make LDFLAGS=-llzmadec     #lzmadec是我之前写好的liblzmadec.so共享库
cp modules/lzmadec.so /path/to/php/extensions
# 在php.ini文件中添加
[lzmadec]
extension=lzmadec.so
```
