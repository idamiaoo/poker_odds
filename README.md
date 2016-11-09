# 德州扑克胜率计算服务
    只支持linux环境运行    
## 前置条件
1. 安装[protobuf3](https://github.com/google/protobuf)
2. 安装[grpc](https://github.com/grpc/grpc/tree/master/src/cpp)
3. 安装boost库 sudo apt-get install libboost-all-dev cmake subversion
4. 下载[pokerstove](https://github.com/ohholly/pokerstove)

## 编译准备
1. 修改Makefile,将CPPFLAGS的pokerstove头文件路径更改为自己下载文件所在的目录

## 编译
```
make 
```

## 数据格式
```
message Request{
    uint32 game = 1;//游戏编号
    map<int32, string> pokers = 2;//key为0,1,2,3 ,其中0为board牌,无board牌填"", 1,2,3...为玩家手牌
}


message Reply{
    uint32 game = 1;
    map<int32, double> winpots  = 2; 每个玩家胜利次数分配
    map<int32, int32> wintype  = 3; 每种牌型胜利次数分配
}
```