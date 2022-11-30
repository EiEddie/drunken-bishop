# Drunken Bishop: 醉酒主教

👀 哈希指纹可视化算法, 就像 OpenSSH 那样!

```
> ./bishop
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
Usage: bishop [OPTION] [hex]

  -h, --help     print help info
      --version  print version info
  -q, --quiet    don't echo hex input
      --story    read the story of Bishop Peter
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
