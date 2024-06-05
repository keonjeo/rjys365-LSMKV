## Project LSM-KV: KVStore using Log-structured Merge Tree


### 如何编译

#### 清理本地可执行文件及中间生成文件

```bash
make clean
```

#### 编译项目生成可执行文件

```bash
make all
```


### 如何测试

#### 正确性测试

```bash
./correctness -t
```


#### 性能测试

```bash
./benchmark_test
```


#### 持久化测试

```bash
./persistence             ## 准备测试数据
./persistence -t
```





