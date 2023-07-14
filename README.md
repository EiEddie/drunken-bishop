# Drunken Bishop: 醉酒主教

👀 哈希指纹可视化算法, 就像 OpenSSH 那样!

```
> bishop
fingerprint of fc94b0c1e5b0987c5843997697ee9fb7:
+-----------------+
|       .=o.  .   |
|     . *+*. o    |
|      =.*..o     |
|       o + ..    |
|        S o.     |
|         o  .    |
|          .  . . |
|              o .|
|               E.|
+-----------------+
```

## 使用

```
Usage: bishop [OPTION] [data]

  -h, --help         print help info
      --version      print version info
  -q, --quiet        don't echo hex input
  -i, --in <file>    use file, if '-' use stdin
      --story        read the story of Bishop Peter
  -t, --type <type>  specify the input type of data or file
                       hex   - HEX string (default)
                       bytes - bytes stream
```

## 例子

### 获取一段十六进制字符串的指纹

> e.g. `fc94b0c1e5b0987c5843997697ee9fb7`

```
> bishop fc94b0c1e5b0987c5843997697ee9fb7
fingerprint of fc94b0c1e5b0987c5843997697ee9fb7:
+-----------------+
|       .=o.  .   |
|     . *+*. o    |
|      =.*..o     |
|       o + ..    |
|        S o.     |
|         o  .    |
|          .  . . |
|              o .|
|               E.|
+-----------------+
```

### 获取 `base64` 编码的指纹

> e.g. `AMeItYIXNWOp2Qc91TR1iyFWutrVgUfLKCJ3B8/U/QM`

```
> cat data.base64
AMeItYIXNWOp2Qc91TR1iyFWutrVgUfLKCJ3B8/U/QM

> cat data.base64 | base64 -d 2>/dev/null | bishop -t bytes -i -
fingerprint of 00c788b582173563a9d9073dd534758b2156badad58147cb28227707cfd4fd03:
+-----------------+
|  .+B=...o*o=.o. |
| ..o+*+  ..O E.o.|
|. o+..o.o + B B .|
| .o.. .+ o + o o.|
|     .  S . . . .|
|         o .     |
|        . .      |
|                 |
|                 |
+-----------------+
```

## 编译

```shell
> git clone 'https://github.com/EiEddie/drunken-bishop.git' && cd drunken-bishop
> make
> cd bin && ./dishop --version
```

要编译 `debug` 版, 使用

```shell
> make DEBUG=debug
```

## 参考

- [The drunken bishop: An analysis of the OpenSSH fingerprint visualization algorithm
  ](http://dirk-loss.de/sshvis/drunken_bishop.pdf)
